// win_process.hpp — Windows-only process launcher with descriptive parameters.
// Single-header, C++17+.

#ifndef WIN_PROCESS_HPP
#define WIN_PROCESS_HPP

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <string>
#include <vector>
#include <thread>
#include <utility>
#include <cstdint>
#include <optional>
#include <algorithm>


namespace WinProcess_
{

    // ----------------------------- Result types -----------------------------

    struct ProcessExecutionResult {
        int                      process_exit_code = -1;
        bool                     process_was_terminated_due_to_timeout = false;
        std::string              standard_output_captured_text_utf8;
        std::string              standard_error_captured_text_utf8;
        // If CreateProcessW or other Win32 API fails, this is set:
        std::optional<DWORD>     windows_last_error_code;
        std::string              human_readable_error_message; // best-effort
    };

    // How to handle a standard stream.
    enum class StandardStreamBehavior {
        CaptureToString,     // Pipe to parent; read into result string
        InheritParent,       // Child inherits parent's console
        RedirectToFileTruncate, // Create/overwrite file at given path
        RedirectToFileAppend    // Create/append file at given path
    };

    // Main configuration for launching a process.
    struct ProcessLaunchConfiguration {
        // Required:
        std::string executable_path_utf8;
        std::vector<std::string> argument_list_utf8;

        // Optional execution context:
        std::optional<std::string> working_directory_utf8;

        // Environment control:
        bool inherit_parent_environment_variables = true;
        // Pairs override or add to inherited env; ignored if inherit=false and you pass none.
        std::vector<std::pair<std::string, std::string>> environment_variable_overrides_utf8;

        // Standard stream handling:
        StandardStreamBehavior standard_output_behavior = StandardStreamBehavior::CaptureToString;
        StandardStreamBehavior standard_error_behavior = StandardStreamBehavior::CaptureToString;
        std::optional<std::string> standard_output_file_path_utf8; // used if RedirectToFile*
        std::optional<std::string> standard_error_file_path_utf8;  // used if RedirectToFile*

        // Window/priority:
        bool create_without_window = true; // true => CREATE_NO_WINDOW + SW_HIDE
        DWORD process_priority_class = BELOW_NORMAL_PRIORITY_CLASS; // e.g., NORMAL_PRIORITY_CLASS

        // Timeouts and termination:
        uint32_t overall_timeout_milliseconds = 0; // 0 = infinite
        bool terminate_entire_process_tree_on_timeout = true; // Uses Job Object kill-on-close
        uint32_t milliseconds_to_wait_after_termination_request = 5000;

        // Advanced flags (optional):
        DWORD additional_creation_flags = 0; // e.g., CREATE_NEW_PROCESS_GROUP
    };

    // ----------------------------- UTF helpers ------------------------------

    inline std::wstring utf8_to_utf16(const std::string& s) {
        if (s.empty()) return {};
        int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
        std::wstring w(n, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), n);
        return w;
    }

    inline std::string utf16_to_utf8(const std::wstring& w) {
        if (w.empty()) return {};
        int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
        std::string s(n, '\0');
        WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), n, nullptr, nullptr);
        return s;
    }

    // Proper Windows quoting for a single argument (same rules the C runtime uses).
    inline std::wstring quote_argument_for_windows_command_line(const std::wstring& arg) {
        bool needQuotes = arg.empty() || arg.find_first_of(L" \t\"") != std::wstring::npos;
        if (!needQuotes) return arg;
        std::wstring out; out.reserve(arg.size() + 2);
        out.push_back(L'"');
        size_t backslashes = 0;
        for (wchar_t ch : arg) {
            if (ch == L'\\') {
                ++backslashes;
            }
            else if (ch == L'"') {
                out.append(backslashes * 2, L'\\');
                backslashes = 0;
                out.push_back(L'\\');
                out.push_back(L'"');
            }
            else {
                out.append(backslashes, L'\\');
                backslashes = 0;
                out.push_back(ch);
            }
        }
        out.append(backslashes * 2, L'\\');
        out.push_back(L'"');
        return out;
    }

    inline std::wstring build_command_line(const std::wstring& exe,
        const std::vector<std::wstring>& args) {
        std::wstring cmd = quote_argument_for_windows_command_line(exe);
        for (const auto& a : args) {
            cmd.push_back(L' ');
            cmd += quote_argument_for_windows_command_line(a);
        }
        return cmd;
    }

    // ----------------------------- Env builder ------------------------------

    inline std::wstring build_environment_block_utf16(
        bool inherit_parent,
        const std::vector<std::pair<std::string, std::string>>& overrides_utf8)
    {
        // Build a map of UTF-16 "NAME" -> "VALUE"
        // We keep it simple: gather existing (if inherit), then apply overrides.
        std::vector<std::pair<std::wstring, std::wstring>> entries;

        if (inherit_parent) {
            LPWCH block = GetEnvironmentStringsW();
            if (block) {
                // The block is like: L"Var=Val\0Var2=Val2\0\0"
                for (const wchar_t* p = block; *p; ) {
                    const wchar_t* eq = wcschr(p, L'=');
                    if (eq) {
                        std::wstring name(p, eq);
                        std::wstring value(eq + 1);
                        // Skip entries that start with '=' (system drive etc.) if you want,
                        // but we'll keep them; CreateProcessW accepts them.
                        entries.emplace_back(std::move(name), std::move(value));
                    }
                    p += wcslen(p) + 1;
                }
                FreeEnvironmentStringsW(block);
            }
        }

        // Apply overrides: if exists, replace; else add
        for (const auto& kv : overrides_utf8) {
            std::wstring k = utf8_to_utf16(kv.first);
            std::wstring v = utf8_to_utf16(kv.second);
            auto it = std::find_if(entries.begin(), entries.end(),
                [&](const auto& p) { return _wcsicmp(p.first.c_str(), k.c_str()) == 0; });
            if (it != entries.end()) {
                it->second = std::move(v);
            }
            else {
                entries.emplace_back(std::move(k), std::move(v));
            }
        }

        // Build double-null-terminated block, sorted (recommended by docs)
        std::sort(entries.begin(), entries.end(),
            [](auto& a, auto& b) { return _wcsicmp(a.first.c_str(), b.first.c_str()) < 0; });

        std::wstring block;
        for (const auto& e : entries) {
            block += e.first;
            block.push_back(L'=');
            block += e.second;
            block.push_back(L'\0');
        }
        block.push_back(L'\0');
        return block;
    }

    // ----------------------------- I/O helpers ------------------------------

    inline void read_entire_handle_into_string(HANDLE h, std::string& out) {
        constexpr DWORD CHUNK = 64 * 1024;
        std::vector<char> buf(CHUNK);
        for (;;) {
            DWORD n = 0;
            if (!ReadFile(h, buf.data(), CHUNK, &n, nullptr) || n == 0) break;
            out.append(buf.data(), buf.data() + n);
        }
    }

    // Create file for redirection (truncate/append). Returns INVALID_HANDLE_VALUE on error.
    inline HANDLE open_redirection_file(const std::wstring& path, bool append) {
        DWORD disposition = append ? OPEN_ALWAYS : CREATE_ALWAYS;
        HANDLE h = CreateFileW(
            path.c_str(),
            FILE_WRITE_DATA | FILE_APPEND_DATA,
            FILE_SHARE_READ,
            nullptr,
            disposition,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        if (h != INVALID_HANDLE_VALUE && append) {
            // Move to end for append
            SetFilePointer(h, 0, nullptr, FILE_END);
        }
        return h;
    }

    // ----------------------------- Main runner ------------------------------

    inline ProcessExecutionResult launch_process_and_wait(
        const ProcessLaunchConfiguration& cfg)
    {
        ProcessExecutionResult result;

        // Convert inputs to UTF-16
        std::wstring exeW = utf8_to_utf16(cfg.executable_path_utf8);
        std::vector<std::wstring> argsW;
        argsW.reserve(cfg.argument_list_utf8.size());
        for (const auto& a : cfg.argument_list_utf8) argsW.push_back(utf8_to_utf16(a));
        std::wstring cmdline = build_command_line(exeW, argsW);

        std::wstring cwdW;
        if (cfg.working_directory_utf8) cwdW = utf8_to_utf16(*cfg.working_directory_utf8);

        // Environment block (optional)
        std::wstring envBlockW;
        LPVOID envPtr = nullptr;
        if (!cfg.inherit_parent_environment_variables || !cfg.environment_variable_overrides_utf8.empty()) {
            envBlockW = build_environment_block_utf16(
                cfg.inherit_parent_environment_variables,
                cfg.environment_variable_overrides_utf8
            );
            envPtr = (LPVOID)envBlockW.c_str(); // Wide block; requires CREATE_UNICODE_ENVIRONMENT
        }

        // Setup stdio
        SECURITY_ATTRIBUTES sa{};
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = TRUE;

        // Child std handles; initialize to parent's handles.
        HANDLE hStdOutChild = GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE hStdErrChild = GetStdHandle(STD_ERROR_HANDLE);

        // Our read-ends for capture (parent side)
        HANDLE hStdOutReadParent = nullptr;
        HANDLE hStdErrReadParent = nullptr;

        auto need_pipe_for = [](StandardStreamBehavior b) {
            return b == StandardStreamBehavior::CaptureToString;
            };
        auto need_file_for = [](StandardStreamBehavior b) {
            return b == StandardStreamBehavior::RedirectToFileTruncate
                || b == StandardStreamBehavior::RedirectToFileAppend;
            };

        // stdout setup
        if (cfg.standard_output_behavior == StandardStreamBehavior::CaptureToString) {
            HANDLE writeEnd = nullptr;
            if (!CreatePipe(&hStdOutReadParent, &writeEnd, &sa, 0)) {
                result.windows_last_error_code = GetLastError();
                result.human_readable_error_message = "CreatePipe(stdout) failed";
                return result;
            }
            SetHandleInformation(hStdOutReadParent, HANDLE_FLAG_INHERIT, 0); // parent read end not inheritable
            hStdOutChild = writeEnd; // child gets write end
        }
        else if (need_file_for(cfg.standard_output_behavior)) {
            if (!cfg.standard_output_file_path_utf8) {
                result.human_readable_error_message = "standard_output_file_path_utf8 not set for file redirection";
                return result;
            }
            bool append = (cfg.standard_output_behavior == StandardStreamBehavior::RedirectToFileAppend);
            HANDLE fileH = open_redirection_file(utf8_to_utf16(*cfg.standard_output_file_path_utf8), append);
            if (fileH == INVALID_HANDLE_VALUE) {
                result.windows_last_error_code = GetLastError();
                result.human_readable_error_message = "Failed to open stdout redirection file";
                return result;
            }
            hStdOutChild = fileH;
        }

        // stderr setup
        if (cfg.standard_error_behavior == StandardStreamBehavior::CaptureToString) {
            HANDLE writeEnd = nullptr;
            if (!CreatePipe(&hStdErrReadParent, &writeEnd, &sa, 0)) {
                result.windows_last_error_code = GetLastError();
                result.human_readable_error_message = "CreatePipe(stderr) failed";
                if (hStdOutReadParent) CloseHandle(hStdOutReadParent);
                if (cfg.standard_output_behavior == StandardStreamBehavior::CaptureToString && hStdOutChild) CloseHandle(hStdOutChild);
                return result;
            }
            SetHandleInformation(hStdErrReadParent, HANDLE_FLAG_INHERIT, 0);
            hStdErrChild = writeEnd;
        }
        else if (need_file_for(cfg.standard_error_behavior)) {
            if (!cfg.standard_error_file_path_utf8) {
                result.human_readable_error_message = "standard_error_file_path_utf8 not set for file redirection";
                if (hStdOutReadParent) CloseHandle(hStdOutReadParent);
                if (cfg.standard_output_behavior == StandardStreamBehavior::CaptureToString && hStdOutChild) CloseHandle(hStdOutChild);
                return result;
            }
            bool append = (cfg.standard_error_behavior == StandardStreamBehavior::RedirectToFileAppend);
            HANDLE fileH = open_redirection_file(utf8_to_utf16(*cfg.standard_error_file_path_utf8), append);
            if (fileH == INVALID_HANDLE_VALUE) {
                result.windows_last_error_code = GetLastError();
                result.human_readable_error_message = "Failed to open stderr redirection file";
                if (hStdOutReadParent) CloseHandle(hStdOutReadParent);
                if (cfg.standard_output_behavior == StandardStreamBehavior::CaptureToString && hStdOutChild) CloseHandle(hStdOutChild);
                return result;
            }
            hStdErrChild = fileH;
        }

        // STARTUPINFO
        STARTUPINFOW si{};
        si.cb = sizeof(si);
        si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.hStdOutput = hStdOutChild;
        si.hStdError = hStdErrChild;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
        si.wShowWindow = cfg.create_without_window ? SW_HIDE : SW_SHOWDEFAULT;

        PROCESS_INFORMATION pi{};
        DWORD creationFlags = cfg.additional_creation_flags
            | (cfg.create_without_window ? CREATE_NO_WINDOW : 0)
            | (envPtr ? CREATE_UNICODE_ENVIRONMENT : 0);

        // Optional Job Object for kill-on-close (process tree kill)
        HANDLE hJob = nullptr;
        if (cfg.terminate_entire_process_tree_on_timeout) {
            hJob = CreateJobObjectW(nullptr, nullptr);
            if (hJob) {
                JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli{};
                jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
                SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
            }
        }

        // Create mutable command line buffer
        std::wstring cmdlineMutable = cmdline;

        BOOL ok = CreateProcessW(
            exeW.c_str(),                 // lpApplicationName
            cmdlineMutable.data(),        // lpCommandLine (mutable buffer)
            nullptr,                      // lpProcessAttributes
            nullptr,                      // lpThreadAttributes
            TRUE,                         // bInheritHandles (we pass handles)
            creationFlags,                // dwCreationFlags
            envPtr,                       // lpEnvironment (may be null)
            cwdW.empty() ? nullptr : cwdW.c_str(), // lpCurrentDirectory
            &si,
            &pi
        );

        // Parent no longer needs child's std handles (write/file ends)
        auto safe_close = [](HANDLE& h) { if (h && h != INVALID_HANDLE_VALUE) { CloseHandle(h); h = nullptr; } };
        // Note: if behavior was InheritParent we didn't duplicate; nothing to close.

        if (!ok) {
            result.windows_last_error_code = GetLastError();
            result.human_readable_error_message = "CreateProcessW failed";
            safe_close(hStdOutReadParent);
            safe_close(hStdErrReadParent);
            // If we opened any file handles for child, they were assigned to hStdOutChild/hStdErrChild;
            // but CreateProcess failed, so close them here:
            if (need_file_for(cfg.standard_output_behavior)) safe_close(hStdOutChild);
            if (need_file_for(cfg.standard_error_behavior))  safe_close(hStdErrChild);
            if (hJob) CloseHandle(hJob);
            return result;
        }

        // Assign to Job if requested
        if (hJob) {
            AssignProcessToJobObject(hJob, pi.hProcess);
        }

        // We can now close the child's write/file ends (they were duplicated into child).
        if (need_pipe_for(cfg.standard_output_behavior)) safe_close(hStdOutChild);
        if (need_pipe_for(cfg.standard_error_behavior))  safe_close(hStdErrChild);
        if (need_file_for(cfg.standard_output_behavior)) safe_close(hStdOutChild);
        if (need_file_for(cfg.standard_error_behavior))  safe_close(hStdErrChild);

        // Start reader threads if capturing
        std::thread tOut, tErr;
        if (cfg.standard_output_behavior == StandardStreamBehavior::CaptureToString && hStdOutReadParent) {
            tOut = std::thread(read_entire_handle_into_string, hStdOutReadParent,
                std::ref(result.standard_output_captured_text_utf8));
        }
        if (cfg.standard_error_behavior == StandardStreamBehavior::CaptureToString && hStdErrReadParent) {
            tErr = std::thread(read_entire_handle_into_string, hStdErrReadParent,
                std::ref(result.standard_error_captured_text_utf8));
        }

        // Wait (optional timeout)
        DWORD waitMs = cfg.overall_timeout_milliseconds == 0 ? INFINITE : cfg.overall_timeout_milliseconds;
        DWORD waitRc = WaitForSingleObject(pi.hProcess, waitMs);
        if (waitRc == WAIT_TIMEOUT) {
            result.process_was_terminated_due_to_timeout = true;
            if (cfg.terminate_entire_process_tree_on_timeout && hJob) {
                // Kill everything in the job
                TerminateJobObject(hJob, 1);
            }
            else {
                TerminateProcess(pi.hProcess, 1);
            }
            // Best-effort: wait a bit for termination to propagate
            WaitForSingleObject(pi.hProcess, cfg.milliseconds_to_wait_after_termination_request);
        }

        // Exit code even if terminated:
        DWORD code = 0;
        GetExitCodeProcess(pi.hProcess, &code);
        result.process_exit_code = static_cast<int>(code);

        // Close our read ends (signals EOF to readers)
        safe_close(hStdOutReadParent);
        safe_close(hStdErrReadParent);

        if (tOut.joinable()) tOut.join();
        if (tErr.joinable()) tErr.join();

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        if (hJob) CloseHandle(hJob);

        return result;
    }

    // ------------------------- Convenience helpers -------------------------

    inline ProcessExecutionResult execute_and_capture_stdout_stderr_utf8(
        const std::string& executable_path_utf8,
        const std::vector<std::string>& argument_list_utf8,
        uint32_t timeout_ms = 0,
        bool hidden_window = true)
    {
        ProcessLaunchConfiguration cfg;
        cfg.executable_path_utf8 = executable_path_utf8;
        cfg.argument_list_utf8 = argument_list_utf8;
        cfg.overall_timeout_milliseconds = timeout_ms;
        cfg.create_without_window = hidden_window;
        cfg.process_priority_class = NORMAL_PRIORITY_CLASS;
        // Capture both:
        cfg.standard_output_behavior = StandardStreamBehavior::CaptureToString;
        cfg.standard_error_behavior = StandardStreamBehavior::CaptureToString;
        return launch_process_and_wait(cfg);
    }


    int usage_example()
    {
        

        ProcessLaunchConfiguration cfg;
        cfg.executable_path_utf8 = "C:\\Windows\\System32\\ipconfig.exe";
        cfg.argument_list_utf8 = { "/all" };
        cfg.working_directory_utf8 = "C:\\Windows\\System32";
        cfg.inherit_parent_environment_variables = true;
        cfg.environment_variable_overrides_utf8 = { {"MY_FLAG","1"} };

        cfg.standard_output_behavior = StandardStreamBehavior::CaptureToString;
        cfg.standard_error_behavior = StandardStreamBehavior::CaptureToString;

        cfg.create_without_window = true;
        cfg.process_priority_class = NORMAL_PRIORITY_CLASS;
        cfg.overall_timeout_milliseconds = 5000;
        cfg.terminate_entire_process_tree_on_timeout = true;

        auto res = launch_process_and_wait(cfg);

        if (res.windows_last_error_code) {
            std::cerr << "Windows error: " << *res.windows_last_error_code
                << " — " << res.human_readable_error_message << "\n";
            return 1;
        }

        std::cout << "Exit code: " << res.process_exit_code
            << (res.process_was_terminated_due_to_timeout ? " (timed out)\n" : "\n");
        std::cout << "---- STDOUT ----\n" << res.standard_output_captured_text_utf8 << "\n";
        std::cout << "---- STDERR ----\n" << res.standard_error_captured_text_utf8 << "\n";

        return 0;
    }

} // namespace winproc

#endif // WIN_PROCESS_HPP
