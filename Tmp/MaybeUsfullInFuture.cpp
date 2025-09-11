#include <iostream>
#include <array>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <optional>

#include "CppCommponents/File.h"

#include "Scene.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

//
//// Returns true if the line is ignorable as a comment:
//// - empty / whitespace-only
//// - or first non-space chars are "//"
//inline bool is_comment(std::string line)
//{
//    // Strip UTF-8 BOM if present
//    if (line.size() >= 3 &&
//        static_cast<unsigned char>(line[0]) == 0xEF &&
//        static_cast<unsigned char>(line[1]) == 0xBB &&
//        static_cast<unsigned char>(line[2]) == 0xBF) {
//        line.erase(0, 3);
//    }
//
//    // Find first non-whitespace char
//    std::size_t i = 0;
//    for (; i < line.size(); ++i) {
//        unsigned char ch = static_cast<unsigned char>(line[i]);
//        if (!std::isspace(ch)) break;
//    }
//
//    // Whitespace-only  treat as comment/ignore
//    if (i == line.size()) return true;
//
//    // Starts with // after leading whitespace?
//    if (line[i] == '/' && (i + 1) < line.size() && line[i + 1] == '/') return true;
//
//    return false;
//}
//
//inline std::string remove_white_space_before_command(std::string line)
//{
//    // Strip UTF-8 BOM if present
//    if (line.size() >= 3 &&
//        static_cast<unsigned char>(line[0]) == 0xEF &&
//        static_cast<unsigned char>(line[1]) == 0xBB &&
//        static_cast<unsigned char>(line[2]) == 0xBF) {
//        line.erase(0, 3);
//    }
//
//    // Left-trim
//    std::size_t i = 0;
//    for (; i < line.size(); ++i) {
//        unsigned char ch = static_cast<unsigned char>(line[i]);
//        if (!std::isspace(ch)) break;
//    }
//    if (i > 0) line.erase(0, i);
//
//    return line;
//}
//


struct Command
{
    std::string command;
    std::vector<std::string> arguments;
};


//inline std::optional<Command> line_to_command(std::string line)
//{
//    // Ignore comment/blank lines
//    if (is_comment(line)) return std::nullopt;
//
//    // Remove leading whitespace/BOM
//    std::string s = remove_white_space_before_command(line);
//
//    // Strip inline // comments (outside quotes)
//    bool in_string = false;
//    bool esc = false;
//    std::size_t cut = s.size();
//    for (std::size_t i = 0; i < s.size(); ++i) {
//        char c = s[i];
//        if (!in_string) {
//            if (c == '"') { in_string = true; esc = false; continue; }
//            if (c == '/' && (i + 1) < s.size() && s[i + 1] == '/') { cut = i; break; }
//        }
//        else {
//            if (esc) { esc = false; continue; }
//            if (c == '\\') { esc = true; continue; }
//            if (c == '"') { in_string = false; continue; }
//        }
//    }
//    if (cut != s.size()) s.erase(cut);
//
//    // Right-trim trailing whitespace
//    if (!s.empty()) {
//        std::size_t j = s.size();
//        while (j > 0 && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
//        s.erase(j);
//    }
//    if (s.empty()) return std::nullopt;
//
//    // Tokenize (supports quoted strings with escapes)
//    std::vector<std::string> tokens;
//    std::string cur;
//    in_string = false; esc = false;
//
//    std::size_t i = 0;
//    // (Leading spaces already trimmed, but guard anyway)
//    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
//
//    for (; i < s.size(); ++i) {
//        char c = s[i];
//        if (!in_string) {
//            if (std::isspace(static_cast<unsigned char>(c))) {
//                if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
//                // collapse consecutive spaces
//                while ((i + 1) < s.size() && std::isspace(static_cast<unsigned char>(s[i + 1]))) ++i;
//            }
//            else if (c == '"') {
//                in_string = true; esc = false;
//            }
//            else {
//                cur.push_back(c);
//            }
//        }
//        else {
//            if (esc) {
//                switch (c) {
//                case 'n': cur.push_back('\n'); break;
//                case 't': cur.push_back('\t'); break;
//                case 'r': cur.push_back('\r'); break;
//                case '\\': cur.push_back('\\'); break;
//                case '"': cur.push_back('"'); break;
//                default:  cur.push_back(c); break;
//                }
//                esc = false;
//            }
//            else if (c == '\\') {
//                esc = true;
//            }
//            else if (c == '"') {
//                in_string = false;
//            }
//            else {
//                cur.push_back(c);
//            }
//        }
//    }
//    if (!cur.empty() || in_string) {
//        // If quote was unterminated, we still accept what we have.
//        tokens.push_back(cur);
//    }
//
//    if (tokens.empty()) return std::nullopt;
//
//    Command cmd;
//    cmd.command = tokens[0];
//    for (std::size_t k = 1; k < tokens.size(); ++k) cmd.arguments.push_back(tokens[k]);
//    return cmd;
//}

enum class ArgumentTypes
{
    INT,
    UINT,
    FLOAT,
    DOUBLE,
    STRING
};

struct CommandDescription
{
    std::string command_name;
    std::vector<ArgumentTypes> types;
    void* callback; // function_pointer
};


struct Convert
{
    static bool to_int64(const std::string& s, long long& out)
    {
        if (has_edge_whitespace(s)) return false;

        errno = 0;
        char* end = nullptr;
        const char* b = s.c_str();
        long long v = std::strtoll(b, &end, 10);

        if (errno == ERANGE) return false;                   // overflow/underflow
        if (end == b || *end != '\0') return false;          // no digits or trailing junk
        out = v;
        return true;
    }

    static bool to_uint64(const std::string& s, unsigned long long& out) {
        if (has_edge_whitespace(s)) return false;
        // no sign allowed for unsigned
        if (!s.empty() && (s[0] == '+' || s[0] == '-')) return false;

        errno = 0;
        char* end = nullptr;
        const char* b = s.c_str();
        unsigned long long v = std::strtoull(b, &end, 10);

        if (errno == ERANGE) return false;                   // overflow
        if (end == b || *end != '\0') return false;          // no digits or trailing junk
        out = v;
        return true;
    }

    [[nodiscard]] static bool to_float32(const std::string& s, float& out) {
        if (has_edge_whitespace(s)) return false;

        errno = 0;
        char* end = nullptr;
        const char* b = s.c_str();
        float v = std::strtof(b, &end);

        if (end == b || *end != '\0') return false;          // parse failed or trailing junk
        if (!std::isfinite(v)) return false;                 // reject NaN/Inf
        out = v;
        return true;
    }

    static bool to_float64(const std::string& s, double& out) {
        if (has_edge_whitespace(s)) return false;

        errno = 0;
        char* end = nullptr;
        const char* b = s.c_str();
        double v = std::strtod(b, &end);

        if (end == b || *end != '\0') return false;          // parse failed or trailing junk
        if (!std::isfinite(v)) return false;                 // reject NaN/Inf
        out = v;
        return true;
    }

    // Handy for your DSL: accepts true/false (case-insensitive) and 1/0.
    [[nodiscard]] static bool to_bool(const std::string& s, bool& out)
    {
        if (s.empty()) return false;

        // Fast path for 1/0
        if (s.size() == 1) {
            if (s[0] == '1') { out = true;  return true; }
            if (s[0] == '0') { out = false; return true; }
        }

        // Case-insensitive compare for "true"/"false"
        std::string t;
        t.reserve(s.size());
        for (char c : s) t.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));

        if (t == "true") { out = true;  return true; }
        if (t == "false") { out = false; return true; }
        return false;
    }

private:

    // Reject if there is any leading or trailing whitespace.
    // (Internal spaces are already impossible if you tokenized properly.)
    static bool has_edge_whitespace(const std::string& s) {
        if (s.empty()) return true; // empty token is not a number
        // leading
        std::size_t i = 0;
        while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
        if (i != 0) return true;
        // trailing
        std::size_t j = s.size();
        while (j > 0 && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
        if (j != s.size()) return true;
        return false;
    }
};



Scene_::Scene g_scene;







#include <iostream>
#include <string>

#include <sstream>
#include <cctype>

// tiny helpers
static std::string ltrim(std::string s) {
    while (!s.empty() && std::isspace((unsigned char)s.front())) s.erase(s.begin());
    return s;
}
static std::string rtrim(std::string s) {
    while (!s.empty() && std::isspace((unsigned char)s.back())) s.pop_back();
    return s;
}


static std::string trim(std::string s) { return rtrim(ltrim(std::move(s))); }

void read_eval_print_loop_example()
{
    // File::read_file_line_by_line_with_FpCallback(filepath, process_one_line);

    // std::cout << "NICE camera.start.x : " << g_scene.camera_start.camera_x << "\n";

    std::string userInput;

    // Print a welcome message and instructions just once, before the loop starts.
    std::cout << "Welcome to the Echo Looper!" << std::endl;
    std::cout << "Type anything and press Enter. I will echo it back." << std::endl;
    std::cout << "Type 'end' to quit the program." << std::endl;
    std::cout << "----------------------------------" << std::endl;

    // This is the main program loop.
    // while(true) creates a loop that will run forever unless we
    // explicitly tell it to stop from inside.
    while (true) {

        {
            std::cout << "> " << std::flush;
            if (!std::getline(std::cin, userInput)) break;

            userInput = trim(userInput);
            if (userInput.empty() || userInput[0] == '#') continue;

            if (userInput == "end" || userInput == "exit" || userInput == "quit") {
                break;
            }
        }

        std::string cmd;
        std::string args;
        {
            // ---- minimal command parsing: first token = cmd, rest = args ----
            std::istringstream iss(userInput);
            iss >> cmd;
            std::getline(iss, args);
            if (!args.empty() && args[0] == ' ') args.erase(0, 1);
        }

        // 3. PRINT: If the loop hasn't ended, print the response.
        // std::cout << "You entered: " << userInput << std::endl;


        if (cmd == "help")
        {
            std::cout
                << "Commands:\n"
                << "  help            - this help\n"
                << "  ping            - prints 'pong'\n"
                << "  echo <text>     - prints <text>\n"
                << "  add <a> <b>     - prints a+b (integers)\n"
                << "  end|exit|quit   - leave\n";
            // create shader
            // print_shader <int:index>

        }
        else if (cmd == "ping")
        {
            std::cout << "pong\n";
        }
        else if (cmd == "echo")
        {
            std::cout << args << "\n";
        }
        else if (cmd == "echo") {
            std::cout << args << "\n";
        }
        else if (cmd == "add") {
            long long a, b;
            std::istringstream nums(args);
            if (nums >> a >> b) std::cout << (a + b) << "\n";
            else std::cout << "usage: add <a> <b>\n";
        }
        else {
            // fallback: behave exactly like your original echo
            std::cout << "You entered: " << userInput << "\n";
        }

    }

    // This code is outside the loop. It will only run after the `break`
    // statement is executed.
    std::cout << "Goodbye!" << std::endl;
}
