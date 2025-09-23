// universe_2.h — auto-generated stub
#pragma once




#include <cstdint>
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <ostream>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <vector>
#include <chrono>
#include <thread>

#include "../CppCommponents/Random.h"

// Random generator
namespace Universe_
{
	struct RNG {
		// PCG parameters/state (64-bit state, 64-bit odd increment)
		uint64_t state;
		uint64_t inc;

		// Construct with a seed and an optional "stream" selector (sequence id).
		// Same (seed, stream) => exact same sequence across platforms/compilers.
		explicit RNG(uint64_t seed = 0x853c49e6748fea9bULL,
			uint64_t stream = 0xda3e39cb94b95bdbULL) {
			// Initialization sequence recommended by PCG reference
			state = 0u;
			inc = (stream << 1u) | 1u;      // must be odd
			next_u32();
			state += seed;
			next_u32();
		}

		// Rotate-right for 32-bit
		static inline uint32_t rotr32(uint32_t x, unsigned r) {
			r &= 31u;                                  // keep 0..31
			return (x >> r) | (x << ((32u - r) & 31u)); // no unary minus on unsigned
		}

		/*static inline uint64_t rotr64(uint64_t x, unsigned r) {
			r &= 63u;
			return (x >> r) | (x << ((64u - r) & 63u));
		}*/

		// Core PCG32 step: returns 32 random bits
		inline uint32_t next_u32() {
			uint64_t oldstate = state;
			// LCG state transition
			state = oldstate * 6364136223846793005ULL + inc;
			// Output function (XSH RR): xorshift + rotate
			uint32_t xorshifted = static_cast<uint32_t>(((oldstate >> 18u) ^ oldstate) >> 27u);
			uint32_t rot = static_cast<uint32_t>(oldstate >> 59u);
			return rotr32(xorshifted, rot);
		}

		// 64 random bits (compose two 32-bit outputs)
		inline uint64_t next_u64() {
			return (uint64_t(next_u32()) << 32) | uint64_t(next_u32());
		}

		// Uniform float in [0,1). Uses 24 mantissa bits (float has 24 incl. hidden bit).
		inline float next_float() {
			// Take top 24 unbiased bits and scale by 1/2^24
			constexpr float inv24 = 1.0f / 16777216.0f; // 2^24
			uint32_t r = next_u32() >> 8;               // keep 24 high bits
			return r * inv24;                           // in [0,1)
		}

		// Uniform double in [0,1). Uses 53 mantissa bits.
		inline double next_double() {
			// Take top 53 bits and scale by 1/2^53
			constexpr double inv53 = 1.0 / 9007199254740992.0; // 2^53
			uint64_t r = next_u64() >> 11;                    // keep 53 high bits
			return r * inv53;                                  // in [0,1)
		}

		// Uniform in [a,b) for doubles
		inline double uniform(double a, double b) {
			return a + (b - a) * next_double();
		}

		// Uniform in [a,b) for floats
		inline float uniform(float a, float b) {
			return a + (b - a) * next_float();
		}
	};

}

// Writer
namespace Universe_
{
	// The .h file
	namespace Writer_ {

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

			explicit Writer(std::string indentUnit = "    ");

			// Append primitives
			void append_raw(const std::string& line);
			void append(const std::string& line);
			void line(const std::string& s);

			// Single-line with placeholder replacement
			bool line(const std::string& tmpl, const Vars& vars,
				ReplaceStats* outStats = nullptr, bool require_any = true);

			// Blank line(s)
			void blank(size_t n = 1);

			// Comments
			void comment(const std::string& s); // single-line, no replacement
			bool comment(const std::string& tmpl, const Vars& vars,
				ReplaceStats* outStats = nullptr, bool require_any = true);
			bool comments(const std::string& tmplMultiline, const Vars& vars,
				ReplaceStats* outStats = nullptr, bool require_any = true);

			// Multi-line content (CR/LF safe)
			bool lines(const std::string& tmplMultiline, const Vars& vars,
				ReplaceStats* outStats = nullptr, bool require_any = true);

			// Indentation helpers
			void open(const std::string& lineWithBrace = "{");
			void close(const std::string& closingBrace = "}");

			// Utilities
			void print() const;
			void write_to(std::ostream& os) const;
			void save(const std::filesystem::path& filepath) const;
			void clear();
			std::string str() const;
			size_t size()  const { return lines_.size(); }
			bool   empty() const { return lines_.empty(); }

			// printf-style but type-safe using std::format
			template <class... Args>
			void linef(std::string_view fmt, Args&&... args) {
				append(std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)));
			}

			// RAII indentation scope (alternative to open/close if you don’t need braces)
			class Indent {
			public:
				explicit Indent(Writer& w) : w_(w) { ++w_.indentLevel_; }
				~Indent() { if (w_.indentLevel_ > 0) --w_.indentLevel_; }
				Indent(const Indent&) = delete; Indent& operator=(const Indent&) = delete;
			private:
				Writer& w_;
			};

		private:
			// Core replacement
			static std::string replace_placeholders(const std::string& s, const Vars& vars, ReplaceStats& st);
			static void collect_used_placeholders(const std::string& s, std::unordered_set<std::string>& used);
			static void report_replace_issue(const char* fn, const std::string& src,
				const ReplaceStats& st, bool require_any);
			static void dedupe_sort(std::vector<std::string>& v);

			std::string indent_prefix() const;

			std::vector<std::string> lines_;
			int indentLevel_ = 0;
			std::string indentUnit_;
		};

	} // namespace Writer_

	// The .cpp file
	namespace Writer_ {

		// ctor
		Writer::Writer(std::string indentUnit)
			: indentUnit_(std::move(indentUnit)) {
		}

		// primitives
		void Writer::append_raw(const std::string& line) { lines_.push_back(line); }
		void Writer::append(const std::string& line) { lines_.push_back(indent_prefix() + line); }
		void Writer::line(const std::string& s) { append(s); }

		// single-line with replacement
		bool Writer::line(const std::string& tmpl, const Vars& vars,
			ReplaceStats* outStats, bool require_any)
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

		// blank lines
		void Writer::blank(size_t n) {
			while (n--) lines_.push_back("");
		}

		// comments
		void Writer::comment(const std::string& s) {
			append("// " + s);
		}

		bool Writer::comment(const std::string& tmpl, const Vars& vars,
			ReplaceStats* outStats, bool require_any)
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

		bool Writer::comments(const std::string& tmplMultiline, const Vars& vars,
			ReplaceStats* outStats, bool require_any)
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
				if (c == '\r') { if (i + 1 < tmplMultiline.size() && tmplMultiline[i + 1] == '\n') continue; }
				if (c == '\n') { flush_line(cur); cur.clear(); }
				else { cur.push_back(c); }
			}
			if (!cur.empty()) flush_line(cur);

			std::unordered_set<std::string> used;
			collect_used_placeholders(tmplMultiline, used);
			for (const auto& kv : vars) if (!used.count(kv.first)) agg.unused_keys.push_back(kv.first);

			dedupe_sort(agg.missing_placeholders);
			dedupe_sort(agg.unused_keys);

			if (outStats) *outStats = agg;
			if (!agg.ok(require_any)) {
				report_replace_issue("comments", tmplMultiline, agg, require_any);
				return false;
			}
			return true;
		}

		// multi-line content with replacement
		bool Writer::lines(const std::string& tmplMultiline, const Vars& vars,
			ReplaceStats* outStats, bool require_any)
		{
			ReplaceStats agg;
			std::string cur; cur.reserve(tmplMultiline.size());

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
				if (c == '\r') { if (i + 1 < tmplMultiline.size() && tmplMultiline[i + 1] == '\n') continue; }
				if (c == '\n') { flush_line(cur); cur.clear(); }
				else { cur.push_back(c); }
			}
			if (!cur.empty()) flush_line(cur);

			std::unordered_set<std::string> used;
			collect_used_placeholders(tmplMultiline, used);
			for (const auto& kv : vars) if (!used.count(kv.first)) agg.unused_keys.push_back(kv.first);

			dedupe_sort(agg.missing_placeholders);
			dedupe_sort(agg.unused_keys);

			if (outStats) *outStats = agg;
			if (!agg.ok(require_any)) {
				report_replace_issue("lines", tmplMultiline, agg, require_any);
				return false;
			}
			return true;
		}

		// indentation helpers
		void Writer::open(const std::string& lineWithBrace) { line(lineWithBrace); ++indentLevel_; }
		void Writer::close(const std::string& closingBrace) { if (indentLevel_ > 0) --indentLevel_; line(closingBrace); }

		// io / utils
		void Writer::print() const { write_to(std::cout); }

		void Writer::write_to(std::ostream& os) const {
			for (const auto& l : lines_) os << l << '\n';
		}

		void Writer::save(const std::filesystem::path& filepath) const {
			namespace fs = std::filesystem;
			if (filepath.has_parent_path()) fs::create_directories(filepath.parent_path());
			std::ofstream out(filepath, std::ios::binary);
			for (const auto& l : lines_) out << l << '\n';
		}

		void Writer::clear() {
			lines_.clear();
			indentLevel_ = 0;
		}

		std::string Writer::str() const {
			std::ostringstream oss;
			for (const auto& l : lines_) oss << l << '\n';
			return oss.str();
		}

		// internals
		std::string Writer::replace_placeholders(const std::string& s, const Vars& vars, ReplaceStats& st) {
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
							// Keep visible in output for easier debugging
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

		void Writer::collect_used_placeholders(const std::string& s, std::unordered_set<std::string>& used) {
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

		void Writer::report_replace_issue(const char* fn, const std::string& /*src*/,
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
		}

		void Writer::dedupe_sort(std::vector<std::string>& v) {
			std::sort(v.begin(), v.end());
			v.erase(std::unique(v.begin(), v.end()), v.end());
		}

		std::string Writer::indent_prefix() const {
			std::string p; p.reserve(indentLevel_ * indentUnit_.size());
			for (int i = 0; i < indentLevel_; ++i) p += indentUnit_;
			return p;
		}

	} // namespace Writer_
}

// Shader generator
namespace Universe_
{

	using namespace std::chrono_literals;





	void generate_shader()
	{


		Writer_::Writer w;

		// Generate Header
		{
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
			w.line("uniform mat4 model;       // can be identity");
			w.line("uniform mat4 view;");
			w.line("uniform mat4 projection;");
			w.line("uniform ivec3 uGrid;      // number of instances along X,Y,Z (instanceCount = X*Y*Z)");
			w.line("uniform float uSpacing;   // distance between grid cells"); // we are not using this
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

			w.comment("// ---------- Constants & tiny helpers ----------");
			w.line("const float PI = 3.1415926535897932384626433832795;");
			w.line("const float TAU = 6.2831853071795864769252867665590;");
			w.blank();

			w.line("float saturate(float x) { return clamp(x, 0.0, 1.0); }");
			w.blank();

			w.lines(R"GLSL(
uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}
)GLSL", {});
			w.blank();

			w.line("float rand01(inout uint s) { s = pcg_hash(s); return float(s) * (1.0 / 4294967295.0); }");
			w.blank();

			w.lines(R"GLSL(
vec3 spherical01(float r, float theta01, float phi01) {
    float theta = theta01 * TAU; // azimuth
    float phi = phi01 * PI;   // polar
    float sphi = sin(phi);
    return vec3(r * sphi * cos(theta), r * cos(phi), r * sphi * sin(theta));
}
)GLSL", {});
			w.blank();

			w.lines(R"GLSL(
mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c, t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c, t.y * axis.z - s * axis.x,
        t.z * axis.x - s * axis.y, t.z * axis.y + s * axis.x, t.z * axis.z + c
    );
}
)GLSL", {});
			w.blank();

			w.lines(R"GLSL(
// Axis-aligned cube face normal from aPos (local space)
vec3 localCubeFaceNormal(vec3 p) {
    vec3 ap = abs(p);
    if (ap.x >= ap.y && ap.x >= ap.z) return vec3(sign(p.x), 0.0, 0.0);
    if (ap.y >= ap.x && ap.y >= ap.z) return vec3(0.0, sign(p.y), 0.0);
    return vec3(0.0, 0.0, sign(p.z));
}
)GLSL", {});
			w.blank();
		}


		// The periodic functions
		{
			w.lines(R"GLSL(
// 0 to 1
float f_periodic_0(float x)
{
    return 2.0 * abs(fract(x + 0.5) - 0.5);
}

// Square Wave 
float f_periodic_1(float x)
{
    return  floor(x) - floor(x - 0.5);
}

// The Bouncing Ball (Parabolic Arches)
float f_periodic_2(float x)
{
    return 4 * fract(x) * (1 - fract(x));
}

float f_periodic_3(float x)
{
    return exp(-30 * ((fract(x + 0.5) - 0.5) * (fract(x + 0.5) - 0.5)));
}

float f_periodic_4(float x)
{
    return abs(0.7 * cos(2 * PI * x) + 0.3 * cos(6 * PI * x)) * (-1.0) + 1.0;
}

float f_periodic_5(float x)
{ 
    return 1.0 - abs(round(10 * fract(x)) / 10 - 0.5) * 2.0;
}

float f_periodic_6(float x)
{
    return sqrt(4 * fract(x) * (1 - fract(x)));
}

float f_periodic_7(float x)
{
    return sin(5 * PI * fract(x)) * (1 - fract(x));
}

// 1) Raised-cosine (Hann) arch — smooth & band-limited-ish
float f_periodic_8(float x)
{
    return 0.5 - 0.5 * cos(TAU * x);               // 0 at integers, 1 at half-integers
}

float f_periodic_9(float x)
{
    return pow(2.0 * abs(fract(x + 0.5) - 0.5), 1.5);
}

float f_periodic_10(float x)
{
    return (abs(1.0 / (1.0 + exp(-6.0 * sin(TAU * x))) - 0.5)) * 2.0 * 2.0 * abs(fract(x + 0.5) - 0.5);
}

float f_periodic_11(float x)
{
    return fract(x) * fract(x) * (3.0 - 2.0 * fract(x)) * 2.0 * abs(fract(x + 0.5) - 0.5) * 1.9;
}

float f_adjust_to_two_pi(float x)
{
    return x * (1.0 / TAU);
}
)GLSL", {});
			w.blank();
		}



		// Write waves
		{
			w.blank();
			w.comment("wave functions");

			class CreateWave_N0
			{
			public:
				bool first = true;

				CreateWave_N0()
				{

				}

				void write(Writer_::Writer& w)
				{
					const int index = first ? 0 : 1;

					w.linef("vec4 wave_{}(float x, float y, float t)", index);
					w.open("{");

					w.blank();
					w.line("float offset_x = 0.0;");
					w.line("float offset_y = 0.0;");
					w.line("float wave_color_r = 0.0;");
					w.line("float wave_color_g = 0.0;");
					w.line("float wave_color_b = 0.0;");

					{
						struct Wave
						{
							float amplitude;
							int frequency;
							float offset;
							float time_multiplier;

							float color_r;
							float color_g;
							float color_b;

							void init(int number_frequencys = 100)
							{
								amplitude = Random::generate_random_float_0_to_1();
								frequency = Random::random_int(1, number_frequencys);
								offset = Random::generate_random_float_0_to_1();
								time_multiplier = 0.1 * Random::generate_random_float_minus_one_to_plus_one();

								color_r = Random::generate_random_float_0_to_1() * 0.2;
								color_g = Random::generate_random_float_0_to_1() * 0.2;
								color_b = Random::generate_random_float_0_to_1() * 0.2;
							}

							static void normalize(std::vector<Wave>& waves)
							{
								int num = waves.size();

								float amplitude_sum = 0.0;
								float color_r_sum = 0.0;
								float color_g_sum = 0.0;
								float color_b_sum = 0.0;

								for (int i = 0; i < num; i++)
								{
									const Wave& wave = waves.at(i);
									amplitude_sum += wave.amplitude;
									color_r_sum += wave.color_r;
									color_g_sum += wave.color_g;
									color_b_sum += wave.color_b;
								}

								float factor_amplitude = 1.0;

								if (amplitude_sum > 1.0)
								{
									factor_amplitude = 1.0 / amplitude_sum;
								}

								float factor_color_r = 1.0;

								if (color_r_sum > 1.0)
								{
									factor_color_r = 1.0 / color_r_sum;
								}

								float factor_color_g = 1.0;

								if (color_g_sum > 1.0)
								{
									factor_color_g = 1.0 / color_g_sum;
								}

								float factor_color_b = 1.0;

								if (color_b_sum > 1.0)
								{
									factor_color_b = 1.0 / color_b_sum;
								}

								for (int i = 0; i < waves.size(); i++)
								{
									Wave& wave = waves.at(i);
									wave.amplitude *= factor_amplitude;
									wave.color_r *= factor_color_r;
									wave.color_g *= factor_color_g;
									wave.color_b *= factor_color_b;
								}
							}

						};

						std::vector<Wave> waves_x;
						std::vector<Wave> waves_y;

						{
							// generate

							for (int i = 0; i < 10; i++)
							{
								{
									Wave wave;


									wave.init(Random::random_int(2, 100));
									waves_x.push_back(std::move(wave));
								}

								{
									Wave wave;
									wave.init(Random::random_int(2, 100));
									waves_y.push_back(std::move(wave));
								}
							}

							// normalize

							Wave::normalize(waves_x);
							Wave::normalize(waves_y);


						}

						// write x
						for (int i = 0; i < waves_x.size(); i++)
						{
							Wave& wave = waves_x.at(i);
							w.linef("float offset_x_{} = float({}) * sin(float({}) * x * TAU + float({}) + t * float({}));", i, wave.amplitude, float(wave.frequency), wave.offset, wave.time_multiplier);
							w.linef("offset_x += offset_x_{};", i);
							w.linef("wave_color_r += float({}) * offset_x_{};", wave.color_r, i);
							w.linef("wave_color_g += float({}) * offset_x_{};", wave.color_g, i);
							w.linef("wave_color_b += float({}) * offset_x_{};", wave.color_b, i);
						}

						// write y
						for (int i = 0; i < waves_y.size(); i++)
						{

							Wave& wave = waves_y.at(i);

							w.linef("float offset_y_{} = float({}) * sin(float({}) * y * TAU + float({}) + t * float({}));", i, wave.amplitude, float(wave.frequency), wave.offset, wave.time_multiplier);
							w.linef("offset_y += offset_y_{};", i);
							w.linef("wave_color_r += float({}) * offset_y_{};", wave.color_r, i);
							w.linef("wave_color_g += float({}) * offset_y_{};", wave.color_g, i);
							w.linef("wave_color_b += float({}) * offset_y_{};", wave.color_b, i);
						}



					}



					w.blank();
					w.line("float offset = offset_x + offset_y;");
					w.line("offset *= 0.1;");


					w.line("float color_r = wave_color_r * 0.2;");
					w.line("float color_g = wave_color_g * 0.2;");
					w.line("float color_b = wave_color_b * 0.2;");


					w.line("");
					w.line("return vec4(color_r, color_g, color_b, offset);");
					w.close("}");
					w.blank();
				}
			};

			{
				CreateWave_N0 wave;
				wave.first = true;
				wave.write(w);
			}

			{
				CreateWave_N0 wave;
				wave.first = false;
				wave.write(w);
			}


		}

		w.line("void main()");
		w.open("{");

		w.line("int id = gl_InstanceID;");
		w.blank();

		w.line("id =  id + (uGrid.x * uGrid.y * uGrid.z) * int(uDrawcallNumber);");
		w.blank();

		w.lines(R"GLSL(
// Per-instance randomness
    uint s0 = uSeed + uint(id + 0);
    uint s1 = uSeed + uint(id + 42);
    uint s2 = uSeed + uint(id + 142);
    float rnd_x = rand01(s0);
    float rnd_y = rand01(s1);
    float rnd_z = rand01(s2);

    // The instancd cube rotation randomization
    uint s0_rot_x = uSeed + uint(id + 2431);
    uint s1_rot_y = uSeed + uint(id + 4412);
    uint s2_rot_y = uSeed + uint(id + 1234);
    uint s3_rot_angle = uSeed + uint(id + 2332);
    float rnd_cube_rotation_x = rand01(s0_rot_x);
    float rnd_cube_rotation_y = rand01(s1_rot_y);
    float rnd_cube_rotation_z = rand01(s2_rot_y);
    float rnd_cube_rotation_angle = rand01(s3_rot_angle);
)GLSL", {});
		w.blank();



		{
			w.line("float f_0 = abs(sin(uTime * 0.7)); //fract(uTime * 1.0);");
			w.line("float f_1 = 1.0 - f_0;");
			w.blank();

			w.blank();
			w.line("vec4 value_wave_0 = wave_0(rnd_x, rnd_y, uTime);");
			w.line("vec4 value_wave_1 = wave_1(rnd_x, rnd_y, uTime);");
			w.line("float w = f_1 * value_wave_0.w + f_0 * value_wave_1.w;");

			w.blank();
			w.line("float color_r = value_wave_0.r * f_1 + value_wave_1.r * f_0;");
			w.line("float color_g = value_wave_0.g * f_1 + value_wave_1.g * f_0;");
			w.line("float color_b = value_wave_0.b * f_1 + value_wave_1.b * f_0;");
		}


		w.blank();

		w.line("float radius = 0.2 + w;");

		w.lines(R"GLSL(
// Sphere
vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
float px = sphere_position.x;
float py = sphere_position.y;
float pz = sphere_position.z;

// Instances Cube Scale
float scale_cube = 0.0007;
vec3  pos = vec3(px, pz, py);
vec3  scale = vec3(scale_cube, scale_cube, scale_cube);


// Whole object rotation
vec3 rotation_axis = vec3(0.0, 1.0, 0.0);
float rotation_angle = uTime; // using uTime will not be wise after we will be interpolating between two values

// Whole object scale
vec3 scale_object = vec3(1.0, 1.0, 1.0);


vec4 new_position = vec4(vec3(pos), 1.0);

    if (true) {

        uint s0_instance_0 = uSeed + uint(uint(u0 * 1000.0f));
        uint s0_instance_1 = uSeed + uint(uint(u0 * 1421.0f));
        float rnd_instance_0 = rand01(s0_instance_0);
        float rnd_instance_1 = rand01(s0_instance_1);

        uint s0_instance_x_scale = uSeed + uint(uint(u0 * 14024.0f));
        uint s0_instance_y_scale = uSeed + uint(uint(u0 * 15214.0f));
        uint s0_instance_z_scale = uSeed + uint(uint(u0 * 14215.0f));
        float rnd_instance_scale_x = rand01(s0_instance_x_scale);
        float rnd_instance_scale_y = rand01(s0_instance_y_scale);
        float rnd_instance_scale_z = rand01(s0_instance_z_scale);

        // Rotation
        // vec3 axis = normalize(vec3(0.0, 1.0, 1.0));
        vec3 axis = normalize(rotation_axis);
        // float angle = uTime;
        float angle = rotation_angle;
        mat3 R3 = axisAngleToMat3(axis, angle);
        mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));

        // Translation
        mat4 T = mat4(1.0);
        vec3 offset = vec3(sin(uTime + rnd_instance_0 * 10.0) * 10.0, sin(uTime + rnd_instance_1 * 0.0) * 10.0, 0.0);
        offset = vec3(0.5, 0.5, 0.5);
        T[3] = vec4(offset, 1.0);



        // Scale
        mat4 S = mat4(1.0);
        S[0][0] = scale_object.x;
        S[1][1] = scale_object.y;
        S[2][2] = scale_object.z;

        new_position = T * R * S * new_position;
    }



    pos = new_position.xyz;
    





    
    // Per-instance tint (kept neutral here)
    color_vs = vec3(color_r, color_g, color_b);

    // Build TRS
    mat4 T = mat4(1.0); T[3] = vec4(pos, 1.0);
    vec3 axis = normalize(vec3(rnd_cube_rotation_x, rnd_cube_rotation_y, rnd_cube_rotation_z));
    float angle = rnd_cube_rotation_angle;//uTime * 0.0;
    mat3 R3 = axisAngleToMat3(axis, angle);
    mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
    mat4 S = mat4(1.0); S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;

    mat4 instanceModel = T * R * S;
    mat4 M = model * instanceModel;

    // World-space position (for lighting)
    vec4 wp = M * vec4(aPos, 1.0);
    vWorldPos = wp.xyz;

    // World-space normal:
    // Fast path (assumes uniform scale): rotate the face normal by model rotation and R3.
    // If you later use non-uniform model scale, switch to normal matrix:
    //   mat3 N = transpose(inverse(mat3(M)));
    //   vNormal = normalize(N * nLocal);
    vec3 nLocal = localCubeFaceNormal(aPos);
    vNormal = normalize(mat3(model) * (R3 * nLocal)); // uniform-scale assumption

    // Clip-space position and UV
    gl_Position = projection * view * wp;
    TexCoord = aTexCoord;


    // World position color

    // float world_x = wp.x;
    // float world_y = wp.y;
    // float world_z = wp.z;
    // color_vs = vec3(sin(world_x * 10.0), sin(world_y * 10.0), sin(world_z * 10.0)) * vec3(0.01, 0.01, 0.01);
)GLSL", {});

		w.close("}");


		w.save("C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/shaders/vertex_9.glsl");


	}

}

namespace Universe_
{
	void generate()
	{
		// engine_flush_frames();

		Program program;
		program.le.halfLife = 0.02f;
		program.le.brightness = 0.0f;
		program.le.exposure = 100.0;
		program.capture.capture = false;
		program.capture.capture_png = false;
		program.capture.capture_bmp = true;

		program.render_display.number_of_frames = 60 * 16;
		program.render_display.render_time_start = 0.0;

		// CAPTURED: { pos:[-0.253910,0.518504,0.281625], yaw:103.439995, pitch:-1.920018, fov:45.000000 }
		program.camera_start.x = -0.253910;
		program.camera_start.y = 0.518504;
		program.camera_start.z = 0.281625;

		program.camera_start.yaw = 103.439995;
		program.camera_start.pitch = -1.920018;
		program.camera_start.fov = 45.0;


		program.camera_end.x = program.camera_start.x;
		program.camera_end.y = program.camera_start.y;
		program.camera_end.z = program.camera_start.z;

		program.camera_end.yaw = program.camera_start.yaw;
		program.camera_end.pitch = program.camera_start.pitch;
		program.camera_end.fov = program.camera_start.fov;

		Scene_::Scene scene = Scene_::Scene();
		program.configure(scene);

		bool enable_shader_9 = true;
		bool enable_shader_10_unit_cube = true;


		// shaders
		if (enable_shader_9)
		{
			add_shader(scene, 9, [](Program::Shader& sh) {


				// Instance 0
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(1000, 1000, 1)
						.set_drawcalls(1)
						.set_position_start(0.0f, 0.0f, 0.0f)
						.set_position_end(0.0f, 0.0f, 0.0f)
						.set_euler_start(0.0f, 0.0f, 0.0f)
						.set_euler_end(0.0f, 0.0f, 0.0f)
						.set_scale_start(1.0f, 1.0f, 1.0f)
						.set_scale_end(1.0f, 1.0f, 1.0f);


					constexpr int kU = 10;
					for (int u = 0; u < kU; ++u)
					{
						float v_start = 0.1f * static_cast<float>(u);
						float v_end = 1.0f - 0.1f * static_cast<float>(u);

						v_start = 0.0;
						v_end = 0.0;

						I.set_u_start_end(u, v_start, v_end);
					}

					I.set_u_start_end(0, 72.29710, 72.29710);
				}


				});


		}

		if (enable_shader_10_unit_cube)
		{
			add_shader(scene, 10, [](Program::Shader& sh) {


				// Instance 0
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(1000, 100, 1)
						.set_drawcalls(1)
						.set_position_start(0.0f, 0.0f, 0.0f)
						.set_position_end(0.0f, 0.0f, 0.0f)
						.set_euler_start(0.0f, 0.0f, 0.0f)
						.set_euler_end(0.0f, 0.0f, 0.0f)
						.set_scale_start(1.0f, 1.0f, 1.0f)
						.set_scale_end(1.0f, 1.0f, 1.0f);


					constexpr int kU = 10;
					for (int u = 0; u < kU; ++u)
					{
						float v_start = 0.1f * static_cast<float>(u);
						float v_end = 1.0f - 0.1f * static_cast<float>(u);

						v_start = 0.0;
						v_end = 0.0;

						I.set_u_start_end(u, v_start, v_end);
					}

					I.set_u_start_end(0, 72.29710, 72.29710);
				}


				});
		}

		// run program
		{
			// scene.print();
			std::string program_name = NameGenerators_::generate_prefix_timestamp_suffix_name();
			save_program(scene, program_name);
			run_program(program_name);

			int i = 0;
			std::string name = "output_" + std::to_string(i);
			// Video::generate(name, "bmp");


		}

		{
			// engine_flush_frames();
			// engine_delete_flush_frames();
		}
	}
	
}





int universe(int argc, char* argv[])
{
	std::cout << "universe_2\n";
	
	

	Universe_::generate();



	return 0;

}

// ---------------------------------------
// Goals
// Unit cube voxel way ( we will build worlds ) 
// Try on spheres parameter traveling  ( the spheres may be placed in that kind of world ) 
// Well I can try first building spheres and than go and build the voxels 
// Well yea let's think well there is a need to change the mindset 
// Usually I rush thru the code build something and than 
// Well just le it be like that 
// What we need is to build something with joy and passion not rushing to get something done 
// 
// Well let's think about the sphere
// Well go into code and play a little bit 
// Than we write some more notes 
// But basically they are distances R -> R kind of function and the output has to be peridic 
// ---------------------------------------