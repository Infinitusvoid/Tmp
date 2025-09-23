// universe_torus.h  drop-in generator for a torus-based vertex shader
#pragma once

#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <ostream>
#include <format>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

// Your RNG helpers (uses your existing Random.h utilities)
#include "../CppCommponents/Random.h"

namespace Universe_ {

    // ======================================================
    // Writer (same style as yours; compact but feature-parity)
    // ======================================================
    namespace Writer_ {

        class Writer {
        public:
            using Vars = std::unordered_map<std::string, std::string>;

            struct ReplaceStats {
                size_t placeholders_found = 0;
                size_t replacements_done = 0;
                std::vector<std::string> missing_placeholders;
                std::vector<std::string> unused_keys;
                bool ok(bool require_any) const {
                    if (!missing_placeholders.empty()) return false;
                    if (require_any && placeholders_found > 0 && replacements_done == 0) return false;
                    return true;
                }
            };

            explicit Writer(std::string indentUnit = "    ") : indentUnit_(std::move(indentUnit)) {}

            void append_raw(const std::string& line) { lines_.push_back(line); }
            void append(const std::string& line) { lines_.push_back(indent_prefix() + line); }
            void line(const std::string& s) { append(s); }

            bool line(const std::string& tmpl, const Vars& vars, ReplaceStats* outStats = nullptr, bool require_any = true) {
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

            void blank(size_t n = 1) { while (n--) lines_.push_back(""); }

            void comment(const std::string& s) { append("// " + s); }

            bool comments(const std::string& tmplMultiline, const Vars& vars, ReplaceStats* outStats = nullptr, bool require_any = true) {
                ReplaceStats agg;
                std::string cur; cur.reserve(tmplMultiline.size());
                auto flush = [&](std::string& s) {
                    ReplaceStats st;
                    std::string replaced = replace_placeholders(s, vars, st);
                    append("// " + replaced);
                    agg.placeholders_found += st.placeholders_found;
                    agg.replacements_done += st.replacements_done;
                    agg.missing_placeholders.insert(agg.missing_placeholders.end(), st.missing_placeholders.begin(), st.missing_placeholders.end());
                    };
                for (size_t i = 0; i < tmplMultiline.size(); ++i) {
                    char c = tmplMultiline[i];
                    if (c == '\r') { if (i + 1 < tmplMultiline.size() && tmplMultiline[i + 1] == '\n') continue; }
                    if (c == '\n') { flush(cur); cur.clear(); }
                    else         cur.push_back(c);
                }
                if (!cur.empty()) flush(cur);
                std::unordered_set<std::string> used;
                collect_used_placeholders(tmplMultiline, used);
                for (const auto& kv : vars) if (!used.count(kv.first)) agg.unused_keys.push_back(kv.first);
                dedupe_sort(agg.missing_placeholders); dedupe_sort(agg.unused_keys);
                if (outStats) *outStats = agg;
                if (!agg.ok(require_any)) {
                    report_replace_issue("comments", tmplMultiline, agg, require_any);
                    return false;
                }
                return true;
            }

            bool lines(const std::string& tmplMultiline, const Vars& vars, ReplaceStats* outStats = nullptr, bool require_any = true) {
                ReplaceStats agg;
                std::string cur; cur.reserve(tmplMultiline.size());
                auto flush = [&](std::string& s) {
                    ReplaceStats st;
                    std::string replaced = replace_placeholders(s, vars, st);
                    append(replaced);
                    agg.placeholders_found += st.placeholders_found;
                    agg.replacements_done += st.replacements_done;
                    agg.missing_placeholders.insert(agg.missing_placeholders.end(), st.missing_placeholders.begin(), st.missing_placeholders.end());
                    };
                for (size_t i = 0; i < tmplMultiline.size(); ++i) {
                    char c = tmplMultiline[i];
                    if (c == '\r') { if (i + 1 < tmplMultiline.size() && tmplMultiline[i + 1] == '\n') continue; }
                    if (c == '\n') { flush(cur); cur.clear(); }
                    else         cur.push_back(c);
                }
                if (!cur.empty()) flush(cur);
                std::unordered_set<std::string> used;
                collect_used_placeholders(tmplMultiline, used);
                for (const auto& kv : vars) if (!used.count(kv.first)) agg.unused_keys.push_back(kv.first);
                dedupe_sort(agg.missing_placeholders); dedupe_sort(agg.unused_keys);
                if (outStats) *outStats = agg;
                if (!agg.ok(require_any)) {
                    report_replace_issue("lines", tmplMultiline, agg, require_any);
                    return false;
                }
                return true;
            }

            void open(const std::string& lineWithBrace = "{") { line(lineWithBrace); ++indentLevel_; }
            void close(const std::string& closingBrace = "}") { if (indentLevel_ > 0) --indentLevel_; line(closingBrace); }

            void print() const { for (auto& l : lines_) std::cout << l << '\n'; }

            void write_to(std::ostream& os) const { for (const auto& l : lines_) os << l << '\n'; }

            void save(const std::filesystem::path& filepath) const {
                namespace fs = std::filesystem;
                if (filepath.has_parent_path()) fs::create_directories(filepath.parent_path());
                std::ofstream out(filepath, std::ios::binary);
                for (const auto& l : lines_) out << l << '\n';
            }

            void clear() { lines_.clear(); indentLevel_ = 0; }

            std::string str() const { std::ostringstream oss; for (auto& l : lines_) oss << l << '\n'; return oss.str(); }

            template<class... Args>
            void linef(std::string_view fmt, Args&&... args) {
                append(std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
            }

        private:
            static std::string replace_placeholders(const std::string& s, const Vars& vars, ReplaceStats& st) {
                std::string out; out.reserve(s.size());
                for (size_t i = 0; i < s.size();) {
                    if (i + 1 < s.size() && s[i] == '$' && s[i + 1] == '{') {
                        size_t end = s.find('}', i + 2);
                        if (end != std::string::npos) {
                            std::string key = s.substr(i + 2, end - (i + 2));
                            ++st.placeholders_found;
                            auto it = vars.find(key);
                            if (it != vars.end()) { out += it->second; ++st.replacements_done; }
                            else { out += "${"; out += key; out += "}"; st.missing_placeholders.push_back(key); }
                            i = end + 1; continue;
                        }
                    }
                    out += s[i++];
                }
                return out;
            }

            static void collect_used_placeholders(const std::string& s, std::unordered_set<std::string>& used) {
                for (size_t i = 0; i < s.size();) {
                    if (i + 1 < s.size() && s[i] == '$' && s[i + 1] == '{') {
                        size_t end = s.find('}', i + 2);
                        if (end != std::string::npos) { used.insert(s.substr(i + 2, end - (i + 2))); i = end + 1; continue; }
                    }
                    ++i;
                }
            }

            static void report_replace_issue(const char* fn, const std::string&, const ReplaceStats& st, bool require_any) {
                std::cerr << "[Writer] " << fn << " replacement check FAILED\n";
                std::cerr << "  placeholders_found: " << st.placeholders_found << "\n";
                std::cerr << "  replacements_done : " << st.replacements_done << "\n";
                if (!st.missing_placeholders.empty()) {
                    std::cerr << "  missing: "; for (auto& k : st.missing_placeholders) std::cerr << k << " "; std::cerr << "\n";
                }
                if (require_any && st.placeholders_found > 0 && st.replacements_done == 0)
                    std::cerr << "  Warning: placeholders present but no replacements happened (require_any=true)\n";
                if (!st.unused_keys.empty()) {
                    std::cerr << "  unused keys: "; for (auto& k : st.unused_keys) std::cerr << k << " "; std::cerr << "\n";
                }
            }

            static void dedupe_sort(std::vector<std::string>& v) {
                std::sort(v.begin(), v.end());
                v.erase(std::unique(v.begin(), v.end()), v.end());
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

    } // namespace Writer_

    // ======================================================
    // Simple randomized wave composer  emits wave_0 / wave_1
    // ======================================================
    struct SimpleWaves {
        struct W {
            float amp = 0.f; int freq = 1; float phase = 0.f; float tmul = 0.f;
            float cr = 0.f, cg = 0.f, cb = 0.f;
        };

        std::vector<W> X0, Y0, X1, Y1;

        static W make_one(int maxF = 120) {
            W w;
            w.amp = Random::generate_random_float_0_to_1();
            w.freq = Random::random_int(1, maxF);
            w.phase = Random::generate_random_float_0_to_1();
            w.tmul = 0.1f * Random::generate_random_float_minus_one_to_plus_one();
            w.cr = 0.2f * Random::generate_random_float_0_to_1();
            w.cg = 0.2f * Random::generate_random_float_0_to_1();
            w.cb = 0.2f * Random::generate_random_float_0_to_1();
            return w;
        }

        static void normalize(std::vector<W>& v) {
            if (v.empty()) return;
            float a = 0, r = 0, g = 0, b = 0;
            for (auto& w : v) { a += w.amp; r += w.cr; g += w.cg; b += w.cb; }
            auto fix = [&](float s) { return (s > 1.f) ? 1.f / s : 1.f; };
            float fa = fix(a), fr = fix(r), fg = fix(g), fb = fix(b);
            for (auto& w : v) { w.amp *= fa; w.cr *= fr; w.cg *= fg; w.cb *= fb; }
        }

        void init(int terms = 10) {
            X0.clear(); Y0.clear(); X1.clear(); Y1.clear();
            for (int i = 0; i < terms; i++) { X0.push_back(make_one()); Y0.push_back(make_one()); }
            for (int i = 0; i < terms; i++) { X1.push_back(make_one()); Y1.push_back(make_one()); }
            normalize(X0); normalize(Y0); normalize(X1); normalize(Y1);
        }

        static void emit_wave(Writer_::Writer& w, const char* name, const std::vector<W>& X, const std::vector<W>& Y) {
            w.linef("vec4 {}(float x, float y, float t)", name);
            w.open("{");
            w.line("float ox=0.0, oy=0.0;");
            w.line("float cr=0.0, cg=0.0, cb=0.0;");
            for (size_t i = 0; i < X.size(); ++i) {
                const auto& q = X[i];
                w.linef("float xo_{} = float({}) * sin(float({})*x*TAU + float({}) + t*float({}));",
                    i, q.amp, q.freq, q.phase, q.tmul);
                w.linef("ox += xo_{};", i);
                w.linef("cr += float({}) * xo_{};", q.cr, i);
                w.linef("cg += float({}) * xo_{};", q.cg, i);
                w.linef("cb += float({}) * xo_{};", q.cb, i);
            }
            for (size_t i = 0; i < Y.size(); ++i) {
                const auto& q = Y[i];
                w.linef("float yo_{} = float({}) * sin(float({})*y*TAU + float({}) + t*float({}));",
                    i, q.amp, q.freq, q.phase, q.tmul);
                w.linef("oy += yo_{};", i);
                w.linef("cr += float({}) * yo_{};", q.cr, i);
                w.linef("cg += float({}) * yo_{};", q.cg, i);
                w.linef("cb += float({}) * yo_{};", q.cb, i);
            }
            w.line("float off = 0.1 * (ox + oy);");
            w.line("return vec4(cr*0.2, cg*0.2, cb*0.2, off);");
            w.close("}");
            w.blank();
        }

        void write(Writer_::Writer& w) {
            emit_wave(w, "wave_0", X0, Y0);
            emit_wave(w, "wave_1", X1, Y1);
        }
    };

    // ======================================================
    // Torus shader generator (saves vertex_10.glsl)
    // ======================================================
    static void generate_shader_torus(const std::filesystem::path& outPath = "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/shaders/vertex_11.glsl")
    {
        Writer_::Writer w;

        // Header / IO
        w.line("#version 450 core");
        w.line("layout(location = 0) in vec3 aPos;");
        w.line("layout(location = 1) in vec2 aTexCoord;");
        w.blank();
        w.comment("outputs to fragment");
        w.line("out vec2 TexCoord;");
        w.line("out vec3 color_vs;");
        w.line("out vec3 vWorldPos;");
        w.line("out vec3 vNormal;");
        w.blank();
        w.comment("uniforms");
        w.line("uniform mat4 model;");
        w.line("uniform mat4 view;");
        w.line("uniform mat4 projection;");
        w.line("uniform ivec3 uGrid;");
        w.line("uniform float uSpacing;");
        w.line("uniform vec3  uOrigin;");
        w.line("uniform vec3  uScaleMin;");
        w.line("uniform vec3  uScaleMax;");
        w.line("uniform float uTime;");
        w.line("uniform float uRotSpeed;");
        w.line("uniform uint  uSeed;");
        w.line("uniform uint  uDrawcallNumber;");
        w.line("uniform vec3  uCameraPos;");
        w.line("uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;");
        w.blank();

        // Helpers
        w.line("const float PI  = 3.1415926535897932384626433832795;");
        w.line("const float TAU = 6.2831853071795864769252867665590;");
        w.blank();
        w.line("float saturate(float x){ return clamp(x,0.0,1.0); }");
        w.blank();

        w.lines(R"GLSL(
uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}
float rand01(inout uint s){ s = pcg_hash(s); return float(s) * (1.0/4294967295.0); }
)GLSL", {});
        w.blank();

        w.lines(R"GLSL(
mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c,     t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c,     t.y * axis.z - s * axis.x,
        t.z * axis.x - s * axis.y, t.z * axis.y + s * axis.x, t.z * axis.z + c
    );
}
vec3 localCubeFaceNormal(vec3 p) {
    vec3 ap = abs(p);
    if (ap.x >= ap.y && ap.x >= ap.z) return vec3(sign(p.x), 0.0, 0.0);
    if (ap.y >= ap.x && ap.y >= ap.z) return vec3(0.0, sign(p.y), 0.0);
    return vec3(0.0, 0.0, sign(p.z));
}
)GLSL", {});
        w.blank();

        // Torus with controllable twist
        w.lines(R"GLSL(
// Torus with optional twist: u around the hole, v around the tube
// 'twistTurns' adds extra tube rotations as we go around u (integer-ish looks tidy, but any float works).
vec3 torus_twist(float R, float r, float u01, float v01, float twistTurns) {
    float u = u01 * TAU;
    float v = v01 * TAU + twistTurns * u;
    float c = cos(u), s = sin(u);
    float cv = cos(v), sv = sin(v);
    float rr = R + r * cv;
    return vec3(rr * c, r * sv, rr * s);
}
)GLSL", {});
        w.blank();

        // Two randomized wave functions
        {
            static SimpleWaves waves;
            waves.init(10);
            waves.write(w);
        }

        // main()
        w.line("void main()");
        w.open("{");

        // Instance RNG
        w.line("int id = gl_InstanceID;");
        w.line("id = id + (uGrid.x * uGrid.y * uGrid.z) * int(uDrawcallNumber);");
        w.blank();
        w.lines(R"GLSL(
uint s0 = uSeed + uint(id + 0);
uint s1 = uSeed + uint(id + 42);
uint s2 = uSeed + uint(id + 142);
float rnd_x = rand01(s0);
float rnd_y = rand01(s1);
float rnd_z = rand01(s2);

// per-instance random local rotation
uint srx = uSeed + uint(id + 2431);
uint sry = uSeed + uint(id + 4412);
uint srz = uSeed + uint(id + 1234);
uint sra = uSeed + uint(id + 2332);
float rnd_rx = rand01(srx);
float rnd_ry = rand01(sry);
float rnd_rz = rand01(srz);
float rnd_ra = rand01(sra);
)GLSL", {});
        w.blank();

        // Blend waves (4s crossfade), use for color + thickness
        w.line("float t01 = fract(uTime * (1.0/4.0));");
        w.line("float a0  = 1.0 - t01;");
        w.line("vec4 w0   = wave_0(rnd_x, rnd_y, uTime);");
        w.line("vec4 w1   = wave_1(rnd_x, rnd_y, uTime);");
        w.line("float wOffRaw = a0*w0.w + t01*w1.w;");
        w.line("vec3  wCol    = vec3(a0*w0.r + t01*w1.r, a0*w0.g + t01*w1.g, a0*w0.b + t01*w1.b);");
        w.blank();

        // Parameters mapping
        w.comment("u0: major radius, u1: base minor radius, u2: wave influence, u3: spin speed, u4: twist turns");
        w.line("float R = mix(0.35, 1.10, saturate(u0));");
        w.line("float rBase = mix(0.02, 0.30, saturate(u1));");
        w.line("float r     = max(0.001, rBase + (saturate(u2) * wOffRaw));");
        w.line("float twistTurns = mix(0.0, 2.0, saturate(u4));");
        w.blank();

        // Position on torus (with twist)
        w.line("vec3 p = torus_twist(R, r, rnd_x, rnd_y, twistTurns);");
        w.line("vec3 pos = p.xzy; // keep a familiar vertical feel");
        w.blank();

        // Global rotation (scaled by u3)
        w.line("float spinMul = saturate(u3);");
        w.lines(R"GLSL(
vec3 rotation_axis = vec3(0.0, 1.0, 0.0);
float rotation_angle = sin(uTime * TAU * (1.0/4.0) * spinMul);
mat3 R3g = axisAngleToMat3(normalize(rotation_axis), rotation_angle);
mat4 Rg  = mat4(vec4(R3g[0],0.0), vec4(R3g[1],0.0), vec4(R3g[2],0.0), vec4(0,0,0,1));
mat4 Tg  = mat4(1.0); Tg[3] = vec4(0.5,0.5,0.5,1.0);   // center in world
vec4 P4  = Tg * Rg * vec4(pos, 1.0);
pos = P4.xyz;
)GLSL", {});
        w.blank();

        // Instance cube scale
        w.line("float scale_cube = 0.0001;");
        w.line("vec3  scale      = vec3(scale_cube);");
        w.blank();

        // Color
        w.line("color_vs = wCol;");
        w.blank();

        // Per-instance TRS
        w.lines(R"GLSL(
mat4 Ti = mat4(1.0); Ti[3] = vec4(pos, 1.0);
vec3 ax = normalize(vec3(rnd_rx, rnd_ry, rnd_rz));
float ang = rnd_ra;
mat3 R3i = axisAngleToMat3(ax, ang);
mat4 Ri = mat4(vec4(R3i[0],0.0), vec4(R3i[1],0.0), vec4(R3i[2],0.0), vec4(0,0,0,1));
mat4 Si = mat4(1.0); Si[0][0]=scale.x; Si[1][1]=scale.y; Si[2][2]=scale.z;

mat4 M = model * Ti * Ri * Si;

vec4 wp = M * vec4(aPos, 1.0);
vWorldPos = wp.xyz;

vec3 nLocal = localCubeFaceNormal(aPos);
vNormal = normalize(mat3(model) * (R3i * nLocal)); // assumes uniform model scale

gl_Position = projection * view * wp;
TexCoord = aTexCoord;
)GLSL", {});

        w.close("}"); // main
        w.save(outPath);
    }

    // ======================================================
    // Convenience entrypoints
    // ======================================================

    inline void generate_all() {
        std::cout << "[Universe] Generating torus shader (vertex_10.glsl)\n";
        generate_shader_torus();
        std::cout << "[Universe] Done.\n";
    }

    // Optional: keep parity with your previous entrypoint
    inline int universe(int, char**) {
        std::cout << "universe_torus\n";
        generate_all();
        return 0;
    }

} // namespace Universe_
