
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

#include "Writer.h"


namespace ShaderGeneration_
{
	// template based you replace some value 
	// you use or not use template 
	// you compose multiply templates 
	// you capture value and crate functions

	/*
	#version 450 core // done
layout(location = 0) in vec3 aPos; // done
layout(location = 1) in vec2 aTexCoord; // done
// done
out vec2 TexCoord;
out vec3 color_vs;

// NEW: feed the FS
out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 model;       // can be identity
uniform mat4 view;
uniform mat4 projection;

// -------- Controls (few scalars only) --------
uniform ivec3 uGrid;      // number of instances along X,Y,Z (instanceCount = X*Y*Z)
uniform float uSpacing;   // distance between grid cells
uniform vec3  uOrigin;    // base world offset
uniform vec3  uScaleMin;  // min scale per axis
uniform vec3  uScaleMax;  // max scale per axis
uniform float uTime;      // time (seconds)
uniform float uRotSpeed;  // radians/sec
uniform uint  uSeed;      // global random seed

uniform uint uDrawcallNumber;
uniform vec3 uCameraPos;
uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;

// ---------- Constants & tiny helpers ----------
const float PI = 3.1415926535897932384626433832795;
const float TAU = 6.2831853071795864769252867665590;
	*/

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

        void append_raw(const std::string& line) { lines_.push_back(line); }
        void append(const std::string& line) { lines_.push_back(indent_prefix() + line); }
        void line(const std::string& s) { append(s); }

        // Single-line with placeholder replacement
        bool line(const std::string& tmpl, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true)
        {
            ReplaceStats st;
            std::string replaced = replace_placeholders(tmpl, vars, st);
            append(replaced);
            if (outStats) *outStats = st;
            if (!st.ok(require_any)) {
                report_replace_issue("line", tmpl, st, require_any);
                return false;
            }
            return true;
        }

        // Blank line(s)
        void blank(size_t n = 1) {
            while (n--) lines_.push_back("");
        }

        // Simple single-line comment (no replacement)
        void comment(const std::string& s) {
            append("// " + s);
        }

        // Single-line comment with ${PLACEHOLDER} replacement + stats
        bool comment(const std::string& tmpl, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true)
        {
            ReplaceStats st;
            std::string replaced = replace_placeholders(tmpl, vars, st);
            append("// " + replaced);
            if (outStats) *outStats = st;
            if (!st.ok(require_any)) {
                report_replace_issue("comment", tmpl, st, require_any);
                return false;
            }
            return true;
        }

        // Multi-line comment block with ${PLACEHOLDER} replacement + stats (CR/LF safe)
        bool comments(const std::string& tmplMultiline, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true)
        {
            ReplaceStats agg;
            std::string cur; cur.reserve(tmplMultiline.size());

            auto flush_line = [&](std::string& s) {
                ReplaceStats st;
                std::string replaced = replace_placeholders(s, vars, st);
                append("// " + replaced);
                agg.placeholders_found += st.placeholders_found;
                agg.replacements_done += st.replacements_done;
                agg.missing_placeholders.insert(agg.missing_placeholders.end(),
                    st.missing_placeholders.begin(),
                    st.missing_placeholders.end());
                };

            for (size_t i = 0; i < tmplMultiline.size(); ++i) {
                char c = tmplMultiline[i];
                if (c == '\r') {
                    if (i + 1 < tmplMultiline.size() && tmplMultiline[i + 1] == '\n') continue; // swallow CR in CRLF
                }
                if (c == '\n') {
                    flush_line(cur);
                    cur.clear();
                }
                else {
                    cur.push_back(c);
                }
            }
            if (!cur.empty()) flush_line(cur);

            // Compute unused keys across the whole template
            std::unordered_set<std::string> used;
            collect_used_placeholders(tmplMultiline, used);
            for (const auto& kv : vars) if (!used.count(kv.first)) agg.unused_keys.push_back(kv.first);

            if (outStats) *outStats = agg;
            if (!agg.ok(require_any)) {
                report_replace_issue("comments", tmplMultiline, agg, require_any);
                return false;
            }
            return true;
        }


        // Multi-line (splits on '\n', handles CRLF too)
        bool lines(const std::string& tmplMultiline, const Vars& vars,
            ReplaceStats* outStats = nullptr, bool require_any = true)
        {
            ReplaceStats agg;
            std::string cur;
            cur.reserve(tmplMultiline.size());

            auto flush_line = [&](std::string& s) {
                ReplaceStats st;
                std::string replaced = replace_placeholders(s, vars, st);
                append(replaced);
                agg.placeholders_found += st.placeholders_found;
                agg.replacements_done += st.replacements_done;
                agg.missing_placeholders.insert(agg.missing_placeholders.end(),
                    st.missing_placeholders.begin(),
                    st.missing_placeholders.end());
                };

            for (size_t i = 0; i < tmplMultiline.size(); ++i) {
                char c = tmplMultiline[i];
                if (c == '\r') {
                    // skip CR, handle CRLF gracefully
                    if (i + 1 < tmplMultiline.size() && tmplMultiline[i + 1] == '\n') continue;
                }
                if (c == '\n') {
                    flush_line(cur);
                    cur.clear();
                }
                else {
                    cur.push_back(c);
                }
            }
            if (!cur.empty()) flush_line(cur);

            // Compute unused keys once from all replaced lines
            {
                // Re-scan template to see what was actually used
                // (We already counted replaced occurrences; to get unused, collect used names.)
                std::unordered_set<std::string> used;
                collect_used_placeholders(tmplMultiline, used);
                for (const auto& kv : vars) {
                    if (!used.count(kv.first)) agg.unused_keys.push_back(kv.first);
                }
            }

            if (outStats) *outStats = agg;
            if (!agg.ok(require_any)) {
                report_replace_issue("lines", tmplMultiline, agg, require_any);
                return false;
            }
            return true;
        }

        // (Optional) indentation helpers
        void open(const std::string& lineWithBrace = "{") { line(lineWithBrace); ++indentLevel_; }
        void close(const std::string& closingBrace = "}") { if (indentLevel_ > 0) --indentLevel_; line(closingBrace); }

        // Print / save
        void print() const { for (const auto& l : lines_) std::cout << l << '\n'; }
        void save(const std::filesystem::path& filepath) const {
            namespace fs = std::filesystem;
            if (filepath.has_parent_path()) fs::create_directories(filepath.parent_path());
            std::ofstream out(filepath, std::ios::binary);
            for (const auto& l : lines_) out << l << '\n';
        }



    private:
        // Replace ${NAME} with vars.at(NAME). Track stats.
        static std::string replace_placeholders(const std::string& s, const Vars& vars, ReplaceStats& st) {
            std::string out; out.reserve(s.size());
            for (size_t i = 0; i < s.size(); ) {
                if (i + 1 < s.size() && s[i] == '$' && s[i + 1] == '{') {
                    size_t end = s.find('}', i + 2);
                    if (end != std::string::npos) {
                        std::string key = s.substr(i + 2, end - (i + 2));
                        ++st.placeholders_found;
                        auto it = vars.find(key);
                        if (it != vars.end()) {
                            out += it->second;
                            ++st.replacements_done;
                        }
                        else {
                            // Keep the original token to make the problem visible in output
                            out += "${"; out += key; out += "}";
                            st.missing_placeholders.push_back(key);
                        }
                        i = end + 1;
                        continue;
                    }
                }
                out += s[i++];
            }
            return out;
        }

        static void collect_used_placeholders(const std::string& s, std::unordered_set<std::string>& used) {
            for (size_t i = 0; i < s.size(); ) {
                if (i + 1 < s.size() && s[i] == '$' && s[i + 1] == '{') {
                    size_t end = s.find('}', i + 2);
                    if (end != std::string::npos) {
                        used.insert(s.substr(i + 2, end - (i + 2)));
                        i = end + 1; continue;
                    }
                }
                ++i;
            }
        }

        static void report_replace_issue(const char* fn, const std::string& src,
            const ReplaceStats& st, bool require_any)
        {
            std::cerr << "[Writer] " << fn << " replacement check FAILED\n";
            std::cerr << "  placeholders_found: " << st.placeholders_found << "\n";
            std::cerr << "  replacements_done : " << st.replacements_done << "\n";
            if (!st.missing_placeholders.empty()) {
                std::cerr << "  missing: ";
                for (auto& k : st.missing_placeholders) std::cerr << k << " ";
                std::cerr << "\n";
            }
            if (require_any && st.placeholders_found > 0 && st.replacements_done == 0) {
                std::cerr << "  Warning: placeholders present but no replacements happened (require_any=true)\n";
            }
            if (!st.unused_keys.empty()) {
                std::cerr << "  unused keys: ";
                for (auto& k : st.unused_keys) std::cerr << k << " ";
                std::cerr << "\n";
            }
            (void)src; // keep around if you later want to dump context
        }

        std::string indent_prefix() const {
            std::string p; p.reserve(indentLevel_ * indentUnit_.size());
            for (int i = 0; i < indentLevel_; ++i) p += indentUnit_;
            return p;
        }

        std::vector<std::string> lines_;
        int indentLevel_ = 0;
        std::string indentUnit_;
    };

    


	


	// -------------- Example generation --------------
	static void generate_vertex_shader(const std::filesystem::path& outPath) {
		Writer w;


		w.line("#version 450 core");
        w.blank();

		w.comment("Auto-generated vertex shader");
		w.line("layout(location = 0) in vec3 aPos;");
		w.line("layout(location = 1) in vec2 aTexCoord;");
		w.blank();

		w.line("uniform mat4 model;");
		w.line("uniform mat4 view;");
		w.line("uniform mat4 projection;");
		w.blank();



		w.line("out vec2 vTex;");
		w.line("out vec2 TexCoord;");
		w.line("out vec3 color_vs;");
		w.line("out vec3 vWorldPos;");
		w.line("out vec3 vNormal;");
		w.blank();

		w.comment("-------- Controls (few scalars only) --------");

		w.line("uniform ivec3 uGrid;      // number of instances along X,Y,Z (instanceCount = X*Y*Z)");
		w.line("uniform float uSpacing;   // distance between grid cells");
		w.line("uniform vec3  uOrigin;    // base world offset");
		w.line("uniform vec3  uScaleMin;  // min scale per axis");
		w.line("uniform vec3  uScaleMax;  // max scale per axis");
		w.line("uniform float uTime;      // time (seconds)");
		w.line("uniform float uRotSpeed;  // radians/sec");
		w.line("uniform uint  uSeed;      // global random seed");
		w.blank();

		w.line("uniform uint uDrawcallNumber;");
		w.line("uniform vec3 uCameraPos;");
		w.line("uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;"); 
		w.blank();


		

		w.comment("----------Constants & tiny helpers----------");
		w.line("const float PI = 3.1415926535897932384626433832795;");
		w.line("const float TAU = 6.2831853071795864769252867665590;");
		w.blank();

		

		w.line("void main()");
		w.open();
			w.line("vTex = aTexCoord;");
			w.line("gl_Position = projection * view * model * vec4(aPos, 1.0);");
		w.close();

		w.save(outPath);
	}

	static void generate_fragment_shader(const std::filesystem::path& outPath, bool useTint, bool useTimeSine)
	{
		Writer w;
		

		w.line("#version 450 core");
		w.blank();

		w.comment("Auto-generated fragment shader");
		w.line("in vec2 vTex;");
		w.line("out vec4 FragColor;");
		w.blank();

		w.save(outPath);
	}


	void run()
	{
		std::cout << "ShaderGeneration_";

		Writer w;
		w.line("#version 450 core");
		w.line("layout(location = 0) in vec3 aPos;");
		w.line("layout(location = 1) in vec2 aTexCoord;");

		{
			const std::filesystem::path outDir = std::filesystem::path("C:/Users/Cosmos/Desktop/output/tmp/shaders/");
			const std::filesystem::path vsPath = outDir / "generated_vertex.glsl";
			const std::filesystem::path fsPath = outDir / "generated_fragment.glsl";

			// Produce minimal, valid shaders. Flip toggles here to include/exclude sections.
			const bool USE_TINT = true;
			const bool USE_TIME_SINE = true;

			generate_vertex_shader(vsPath);
			generate_fragment_shader(fsPath, USE_TINT, USE_TIME_SINE);

			std::cout << "Generated:\n  " << vsPath.string()
				<< "\n  " << fsPath.string() << "\n";
			std::cout << "ShaderGeneration_ done\n";
		}


	}
}