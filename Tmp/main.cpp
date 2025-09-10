#include <iostream>
#include <array>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <optional>

#include "CppCommponents/File.h"

#include "Scene.h"



// Returns true if the line is ignorable as a comment:
// - empty / whitespace-only
// - or first non-space chars are "//"
inline bool is_comment(std::string line)
{
    // Strip UTF-8 BOM if present
    if (line.size() >= 3 &&
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
        line.erase(0, 3);
    }

    // Find first non-whitespace char
    std::size_t i = 0;
    for (; i < line.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(line[i]);
        if (!std::isspace(ch)) break;
    }

    // Whitespace-only  treat as comment/ignore
    if (i == line.size()) return true;

    // Starts with // after leading whitespace?
    if (line[i] == '/' && (i + 1) < line.size() && line[i + 1] == '/') return true;

    return false;
}

inline std::string remove_white_space_before_command(std::string line)
{
    // Strip UTF-8 BOM if present
    if (line.size() >= 3 &&
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
        line.erase(0, 3);
    }

    // Left-trim
    std::size_t i = 0;
    for (; i < line.size(); ++i) {
        unsigned char ch = static_cast<unsigned char>(line[i]);
        if (!std::isspace(ch)) break;
    }
    if (i > 0) line.erase(0, i);

    return line;
}



struct Command
{
    std::string command;
    std::vector<std::string> arguments;
};


inline std::optional<Command> line_to_command(std::string line)
{
    // Ignore comment/blank lines
    if (is_comment(line)) return std::nullopt;

    // Remove leading whitespace/BOM
    std::string s = remove_white_space_before_command(line);

    // Strip inline // comments (outside quotes)
    bool in_string = false;
    bool esc = false;
    std::size_t cut = s.size();
    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (!in_string) {
            if (c == '"') { in_string = true; esc = false; continue; }
            if (c == '/' && (i + 1) < s.size() && s[i + 1] == '/') { cut = i; break; }
        }
        else {
            if (esc) { esc = false; continue; }
            if (c == '\\') { esc = true; continue; }
            if (c == '"') { in_string = false; continue; }
        }
    }
    if (cut != s.size()) s.erase(cut);

    // Right-trim trailing whitespace
    if (!s.empty()) {
        std::size_t j = s.size();
        while (j > 0 && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
        s.erase(j);
    }
    if (s.empty()) return std::nullopt;

    // Tokenize (supports quoted strings with escapes)
    std::vector<std::string> tokens;
    std::string cur;
    in_string = false; esc = false;

    std::size_t i = 0;
    // (Leading spaces already trimmed, but guard anyway)
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) ++i;

    for (; i < s.size(); ++i) {
        char c = s[i];
        if (!in_string) {
            if (std::isspace(static_cast<unsigned char>(c))) {
                if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
                // collapse consecutive spaces
                while ((i + 1) < s.size() && std::isspace(static_cast<unsigned char>(s[i + 1]))) ++i;
            }
            else if (c == '"') {
                in_string = true; esc = false;
            }
            else {
                cur.push_back(c);
            }
        }
        else {
            if (esc) {
                switch (c) {
                case 'n': cur.push_back('\n'); break;
                case 't': cur.push_back('\t'); break;
                case 'r': cur.push_back('\r'); break;
                case '\\': cur.push_back('\\'); break;
                case '"': cur.push_back('"'); break;
                default:  cur.push_back(c); break;
                }
                esc = false;
            }
            else if (c == '\\') {
                esc = true;
            }
            else if (c == '"') {
                in_string = false;
            }
            else {
                cur.push_back(c);
            }
        }
    }
    if (!cur.empty() || in_string) {
        // If quote was unterminated, we still accept what we have.
        tokens.push_back(cur);
    }

    if (tokens.empty()) return std::nullopt;

    Command cmd;
    cmd.command = tokens[0];
    for (std::size_t k = 1; k < tokens.size(); ++k) cmd.arguments.push_back(tokens[k]);
    return cmd;
}

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



void process_one_line(std::string line)
{
    
    
    std::optional<Command> command = line_to_command(line);

    if (command.has_value())
    {
        Command& command_safe = command.value();
        
        std::cout << command_safe.command << "\n";

        for (int i = 0; i < command_safe.arguments.size(); i++)
        {
            std::cout << command_safe.arguments[i] << "\n";

            if (command_safe.command == "camera.start.x")
            {
                if (Convert::to_float32(command_safe.arguments[0], g_scene.camera_start.camera_x))
                {
                
                }
                else
                {
                    std::cout << "can't decode argument for camera.start.x\n";
                }
            }

            if (command_safe.command == "camera.start.y")
            {
                if (Convert::to_float32(command_safe.arguments[0], g_scene.camera_start.camera_y))
                {

                }
                else
                {
                    std::cout << "can't decode argument for camera.start.y\n";
                }
            }
        }
    }

    // std::cout << line << "\n";
}





int main()
{
    std::cout << "Tmp\n";

    std::string filepath = "commands.txt";


    

    File::read_file_line_by_line_with_FpCallback(filepath, process_one_line);

    std::cout << "NICE camera.start.x : " << g_scene.camera_start.camera_x << "\n";
    

    return 0;
}

