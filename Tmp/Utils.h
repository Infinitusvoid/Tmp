#pragma once

#include <filesystem>
#include <string>
#include <system_error>   // std::error_code
#include <cstdint>        // std::uintmax_t
#include <sstream>
#include <vector>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <cstdlib>


namespace File_
{
    namespace fs = std::filesystem;

    struct CopyResult {
        bool ok = false;
        std::uintmax_t bytes = 0;
        std::string message;
    };

    CopyResult copy_file_safely(const std::string& srcPathStr,
        const std::string& dstPathStr,
        bool overwrite = false)
    {
        CopyResult r{};
        std::error_code ec;

        fs::path src(srcPathStr);
        fs::path dst(dstPathStr);

        // Basic validation
        if (src.empty() || dst.empty()) {
            r.message = "Source or destination path is empty.";
            return r;
        }
        if (fs::equivalent(src, dst, ec)) {
            r.message = "Source and destination refer to the same path.";
            return r;
        }

        // Source must exist and be a regular file
        if (!fs::exists(src)) {
            r.message = "Source file does not exist: " + src.string();
            return r;
        }
        if (!fs::is_regular_file(src)) {
            r.message = "Source is not a regular file: " + src.string();
            return r;
        }

        // If destination is an existing directory, copy "into" it
        if (fs::exists(dst) && fs::is_directory(dst)) {
            dst /= src.filename();
        }

        // Ensure destination directory exists
        if (auto parent = dst.parent_path(); !parent.empty()) {
            fs::create_directories(parent, ec);
            if (ec) {
                r.message = "Failed to create destination directories: " + parent.string() +
                    " (" + ec.message() + ")";
                return r;
            }
        }

        // Choose copy options
        auto options = overwrite ? fs::copy_options::overwrite_existing
            : fs::copy_options::none;

        // Perform copy (copy_file only copies file contents/perm bits)
        bool didCopy = fs::copy_file(src, dst, options, ec);
        if (!didCopy || ec) {
            // If not overwriting, and file exists, explain clearly
            if (!overwrite && fs::exists(dst)) {
                r.message = "Destination already exists (set overwrite=true to replace): " + dst.string();
                return r;
            }
            r.message = "Copy failed: " + (ec ? ec.message() : std::string("unknown error"));
            return r;
        }

        // Success
        r.ok = true;
        r.bytes = fs::file_size(src, ec);
        r.message = "Copied to: " + dst.string();
        return r;
    }
}


namespace FFmpeg_
{
    namespace fs = std::filesystem;

   
    struct FfmpegImageToVideo {
        // --- User-configurable fields ---
        std::string ffmpeg_binary =
#ifdef _WIN32
            "ffmpeg.exe";
#else
            "ffmpeg";
#endif
        std::string input_dir;                  // e.g. "C:/renders/shot01"
        std::string input_pattern = "frame_%06d.png"; // filename pattern inside input_dir
        int         start_number = 0;         // first frame number in the sequence
        int         input_fps = 60;        // -framerate (read rate for images)
        int         output_fps = 60;        // -r (container/encoder output fps)
        std::string vcodec = "libx264"; // encoder
        std::string preset = "slow";    // x264 preset
        int         crf = 14;        // quality (lower is higher quality)
        std::string pix_fmt = "yuv420p"; // broad compatibility
        bool        faststart = true;      // move moov atom to front
        std::string output_path;                // e.g. "C:/renders/shot01/output.mp4"

        // --- Helpers ---
        static std::string quote(const std::string& s) {
            // Quote if needed and escape internal quotes for the shell.
            bool need = s.find_first_of(" \t\"\'()[]{}&;|<>`^") != std::string::npos;
            std::string out = s;
            // Escape internal quotes (very basic) for common shells.
            std::string esc; esc.reserve(out.size() + 8);
            for (char c : out) {
                if (c == '\"') esc += "\\\"";
                else esc += c;
            }
            return need ? std::string("\"") + esc + "\"" : esc;
        }

        static std::string to_generic(const fs::path& p)
        {
            return p.generic_string(); // <-- returns std::string
        }

        bool validate(std::string& err) const {
            if (input_dir.empty()) { err = "input_dir is empty."; return false; }
            if (output_path.empty()) { err = "output_path is empty."; return false; }
            if (!fs::exists(input_dir) || !fs::is_directory(input_dir)) {
                err = "input_dir does not exist or is not a directory: " + input_dir; return false;
            }
            // Optional: warn if the directory seems empty
            bool any = false;
            for (auto it = fs::directory_iterator(input_dir); it != fs::directory_iterator(); ++it) {
                if (it->is_regular_file()) { any = true; break; }
            }
            if (!any) {
                err = "input_dir appears to be empty (no files found): " + input_dir; return false;
            }
            // Ensure parent of output exists (create if needed)
            fs::path outp(output_path);
            if (outp.has_parent_path()) {
                std::error_code ec; fs::create_directories(outp.parent_path(), ec);
                if (ec) {
                    err = "Failed to create parent directory for output: " + outp.parent_path().string();
                    return false;
                }
            }
            return true;
        }

        std::string build_command() const {
            // Compose input pattern as full path: <input_dir>/<input_pattern>
            std::string in_pattern_full = to_generic(fs::path(input_dir) / input_pattern);

            std::vector<std::string> args;
            args.reserve(32);
            args.push_back(quote(ffmpeg_binary));
            args.push_back("-y"); // overwrite output if exists; remove if you prefer safety

            // Input sequence timing & pattern
            args.push_back("-framerate");    args.push_back(std::to_string(input_fps));
            args.push_back("-start_number"); args.push_back(std::to_string(start_number));
            args.push_back("-i");            args.push_back(quote(in_pattern_full));

            // Encoding settings
            args.push_back("-c:v");    args.push_back(vcodec);
            args.push_back("-preset"); args.push_back(preset);
            args.push_back("-crf");    args.push_back(std::to_string(crf));
            args.push_back("-pix_fmt"); args.push_back(pix_fmt);

            // Output fps (container/encoder)
            args.push_back("-r");      args.push_back(std::to_string(output_fps));

            if (faststart) {
                args.push_back("-movflags");
                args.push_back("+faststart");
            }

            // Output path
            args.push_back(quote(to_generic(fs::path(output_path))));

            // Join into a single command string for std::system
            std::ostringstream oss;
            for (size_t i = 0; i < args.size(); ++i) {
                if (i) oss << ' ';
                oss << args[i];
            }
            return oss.str();
        }

        int run() const {
            std::string err;
            if (!validate(err)) {
                std::cerr << "[ffmpeg] Validation failed: " << err << "\n";
                return -1;
            }
            const std::string cmd = build_command();
            std::cout << "[ffmpeg] Running:\n" << cmd << "\n";
            // NOTE: std::system invokes the platform shell (cmd.exe/sh). Ensure ffmpeg is on PATH,
            // or set ffmpeg_binary to an absolute path.
            int rc = std::system(cmd.c_str());
            if (rc != 0) {
                std::cerr << "[ffmpeg] ffmpeg exited with code " << rc << "\n";
            }
            return rc;
        }
    };


}