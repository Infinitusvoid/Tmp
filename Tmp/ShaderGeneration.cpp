
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <sstream>
#include <algorithm>

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

	namespace fs = std::filesystem;

	// ---------------- Writer ----------------
	class Writer {
	public:
		explicit Writer(std::string indentUnit = "    ") : indentUnit_(std::move(indentUnit)) {}

		// Append a raw line (no indentation added).
		void append_raw(const std::string& line) {
			lines_.push_back(line);
		}

		// Append a line with current indentation.
		void append(const std::string& line) {
			lines_.push_back(indent_prefix() + line);
		}

		// Convenience alias.
		void line(const std::string& line) { append(line); }

		void comment(const std::string& text) { line("// " + text); }

		// Add a blank line.
		void blank() { lines_.push_back(""); }

		// Open / close blocks (handles the braces + indentation nicely).
		void open(const std::string& lineWithOpeningBrace = "{") {
			append(lineWithOpeningBrace);
			++indentLevel_;
		}

		void close(const std::string& closingBrace = "}") {
			if (indentLevel_ > 0) --indentLevel_;
			lines_.push_back(indent_prefix() + closingBrace);
		}

		// Print to stdout.
		void print() const {
			for (const auto& l : lines_) std::cout << l << '\n';
		}

		// Save to file (creates parent directories if needed).
		void save(const fs::path& filepath) const {
			if (filepath.has_parent_path()) {
				std::error_code ec;
				fs::create_directories(filepath.parent_path(), ec);
				if (ec) {
					std::cerr << "[Writer] Failed to create directories: "
						<< filepath.parent_path().string() << " (" << ec.message() << ")\n";
				}
			}
			std::ofstream out(filepath, std::ios::binary);
			if (!out) {
				std::cerr << "[Writer] Failed to open for write: " << filepath.string() << "\n";
				return;
			}
			for (const auto& l : lines_) out << l << '\n';
			std::cout << "[Writer] Saved: " << filepath.string() << "\n";
		}

		// Reset content.
		void clear() { lines_.clear(); indentLevel_ = 0; }

		// Access whole text (if you want to pass to your engine directly).
		std::string str() const {
			std::ostringstream oss;
			for (const auto& l : lines_) oss << l << '\n';
			return oss.str();
		}

	private:
		std::string indent_prefix() const {
			std::string p;
			for (int i = 0; i < indentLevel_; ++i) p += indentUnit_;
			return p;
		}

		std::vector<std::string> lines_;
		int indentLevel_ = 0;
		std::string indentUnit_;
	};


	// -------------- Example generation --------------
	static void generate_vertex_shader(const fs::path& outPath) {
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

	static void generate_fragment_shader(const fs::path& outPath, bool useTint, bool useTimeSine)
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
			const fs::path outDir = fs::path("C:/Users/Cosmos/Desktop/output/tmp/shaders/");
			const fs::path vsPath = outDir / "generated_vertex.glsl";
			const fs::path fsPath = outDir / "generated_fragment.glsl";

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