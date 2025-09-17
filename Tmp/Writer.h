#pragma once


#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <filesystem>

#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <algorithm>

namespace Writer_
{
    class Writer {
    public:
        using Vars = std::unordered_map<std::string, std::string>;

        struct ReplaceStats {
            size_t placeholders_found = 0;
            size_t replacements_done = 0;
            std::vector<std::string> missing_placeholders; // ${...} with no provided value
            std::vector<std::string> unused_keys;          // provided vars not used
            bool ok(bool require_any) const {
                if (!missing_placeholders.empty()) return false;
                if (require_any && replacements_done == 0 && placeholders_found > 0) return false;
                return true;
            }
        };

        explicit Writer(std::string indentUnit = "    ")
            : indentUnit_(std::move(indentUnit)) {
        }

        void append_raw(const std::string& line);
        void append(const std::string& line);
        void line(const std::string& s);

        // Single-line with placeholder replacement
        bool line(const std::string& tmpl, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true);

        // Blank line(s)
        void blank(size_t n = 1);

        // Simple single-line comment (no replacement)
        void comment(const std::string& s);

        // Single-line comment with ${PLACEHOLDER} replacement + stats
        bool comment(const std::string& tmpl, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true);

        // Multi-line comment block with ${PLACEHOLDER} replacement + stats (CR/LF safe)
        bool comments(const std::string& tmplMultiline, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true);


        // Multi-line (splits on '\n', handles CRLF too)
        bool lines(const std::string& tmplMultiline, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true);

        // (Optional) indentation helpers
        void open(const std::string& lineWithBrace = "{");
        void close(const std::string& closingBrace = "}");

        // Print / save
        void print() const;
        void save(const std::filesystem::path& filepath) const;



    private:
        // Replace ${NAME} with vars.at(NAME). Track stats.
        static std::string replace_placeholders(const std::string& s, const Vars& vars, ReplaceStats& st);

        static void collect_used_placeholders(const std::string& s, std::unordered_set<std::string>& used);

        static void report_replace_issue(const char* fn, const std::string& src,
            const ReplaceStats& st, bool require_any);

        std::string indent_prefix() const;

        std::vector<std::string> lines_;
        int indentLevel_ = 0;
        std::string indentUnit_;
    };
}

