#include "Scene.h"

#include <iostream>
#include <vector>
#include <functional>
#include <iomanip>
#include <string>
#include <utility>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <optional>
#include <filesystem>
#include <stdexcept>

#include "CppCommponents/Folder.h"
#include "CppCommponents/File.h"



namespace Scene_
{
	// ----------------- Render / output size -----------------

	int Scene::get_width() const { return width; }
	int Scene::get_height() const { return height; }

	// ----------------- Render timing -----------------

	void Scene::set_render_fps(int number) { render_fps = number; }
	int Scene::get_render_fps() const { return render_fps; }
	void Scene::set_number_of_frames(int number) { number_of_frames = number; }
	int Scene::get_number_of_frames() const { return number_of_frames; }
	void Scene::set_render_time_start(float v) { capture_settings.start_time = v; }
	float Scene::get_render_time_start() const { return capture_settings.start_time; }

	// ----------------- Capture toggles -----------------

	void Scene::set_capture(bool v) { capture_settings.capture_frames = v; }
	bool Scene::get_capture() const { return capture_settings.capture_frames; }
	void Scene::set_capture_png(bool v) { capture_settings.capture_png = v; }
	bool Scene::get_capture_png() const { return capture_settings.capture_png; }
	void Scene::set_capture_bmp(bool v) { capture_settings.capture_bmp = v; }
	bool Scene::get_capture_bmp() const { return capture_settings.capture_bmp; }

	// ----------------- LE (post) settings -----------------

	void Scene::set_le_halfLife(float v) { le_settings.halfLife = v; }
	float Scene::get_le_halfLife() const { return le_settings.halfLife; }
	void Scene::set_le_bloomThreshold(float v) { le_settings.bloomThreshold = v; }
	float Scene::get_le_bloomThreshold() const { return le_settings.bloomThreshold; }
	void Scene::set_le_bloomSoftKnee(float v) { le_settings.bloomSoftKnee = v; }
	float Scene::get_le_bloomSoftKnee() const { return le_settings.bloomSoftKnee; }
	void Scene::set_le_bloomStrength(float v) { le_settings.bloomStrength = v; }
	float Scene::get_le_bloomStrength() const { return le_settings.bloomStrength; }
	void Scene::set_le_bloomIterations(int v) { le_settings.bloomIterations = v; }
	int Scene::get_le_bloomIterations() const { return le_settings.bloomIterations; }

	void  Scene::set_le_exposure(float v) { le_settings.exposure = v; }
	float Scene::get_le_exposure() const { return le_settings.exposure; }

	void  Scene::set_le_gamma(float v) { le_settings.gamma = v; }
	float Scene::get_le_gamma() const { return le_settings.gamma; }

	void  Scene::set_le_brightness(float v) { le_settings.brightness = v; }
	float Scene::get_le_brightness() const { return le_settings.brightness; }

	void  Scene::set_le_contrast(float v) { le_settings.contrast = v; }
	float Scene::get_le_contrast() const { return le_settings.contrast; }

	void  Scene::set_le_saturation(float v) { le_settings.saturation = v; }
	float Scene::get_le_saturation() const { return le_settings.saturation; }

	void  Scene::set_le_msaaSamples(int v) { le_settings.msaaSamples = v; }
	int   Scene::get_le_msaaSamples() const { return le_settings.msaaSamples; }


	// ----------------- Camera start -----------------
	void  Scene::set_camera_start_x(float v) { camera_start.camera_x = v; }
	float Scene::get_camera_start_x() const { return camera_start.camera_x; }

	void  Scene::set_camera_start_y(float v) { camera_start.camera_y = v; }
	float Scene::get_camera_start_y() const { return camera_start.camera_y; }

	void  Scene::set_camera_start_z(float v) { camera_start.camera_z = v; }
	float Scene::get_camera_start_z() const { return camera_start.camera_z; }

	void  Scene::set_camera_start_pitch(float v) { camera_start.pitch = v; }
	float Scene::get_camera_start_pitch() const { return camera_start.pitch; }
	
	// yaw
	void  Scene::set_camera_start_yaw(float v) { camera_start.yaw = v; }
	float Scene::get_camera_start_yaw() const { return camera_start.yaw; }

	void  Scene::set_camera_start_fov(float v) { camera_start.fov = v; }
	float Scene::get_camera_start_fov() const { return camera_start.fov; }


	// ----------------- Camera end -----------------
	void  Scene::set_camera_end_x(float v) { camera_end.camera_x = v; }
	float Scene::get_camera_end_x() const { return camera_end.camera_x; }

	void  Scene::set_camera_end_y(float v) { camera_end.camera_y = v; }
	float Scene::get_camera_end_y() const { return camera_end.camera_y; }

	void  Scene::set_camera_end_z(float v) { camera_end.camera_z = v; }
	float Scene::get_camera_end_z() const { return camera_end.camera_z; }

	void  Scene::set_camera_end_pitch(float v) { camera_end.pitch = v; }
	float Scene::get_camera_end_pitch() const { return camera_end.pitch; }

	void  Scene::set_camera_end_yaw(float v) { camera_end.yaw = v; }
	float Scene::get_camera_end_yaw() const { return camera_end.yaw; }

	void  Scene::set_camera_end_fov(float v) { camera_end.fov = v; }
	float Scene::get_camera_end_fov() const { return camera_end.fov; }


	// ----------------- Shaders: container-level -----------------
	size_t Scene::shader_count() const { return shaders.size(); }

	size_t Scene::add_shader(const char* vertex_path, const char* fragment_path) {
		Shader s;
		s.vertex = vertex_path ? vertex_path : "";
		s.fragment = fragment_path ? fragment_path : "";
		shaders.push_back(std::move(s));
		return shaders.size() - 1;
	}

	// overloads for std::string if you prefer
	size_t Scene::add_shader(const std::string& vertex_path, const std::string& fragment_path) {
		return add_shader(vertex_path.c_str(), fragment_path.c_str());
	}

	bool Scene::remove_shader(size_t shader_idx) {
		if (!valid_shader_index(shader_idx)) return false;
		shaders.erase(shaders.begin() + static_cast<std::ptrdiff_t>(shader_idx));
		return true;
	}
	void Scene::clear_shaders() { shaders.clear(); }

	bool Scene::set_shader_vertex_path(size_t shader_idx, const char* path) {
		if (!valid_shader_index(shader_idx)) return false;
		shaders[shader_idx].vertex = path ? path : "";
		return true;
	}

	bool Scene::set_shader_fragment_path(size_t shader_idx, const char* path) {
		if (!valid_shader_index(shader_idx)) return false;
		shaders[shader_idx].fragment = path ? path : "";
		return true;
	}

	std::string Scene::get_shader_vertex_path(size_t shader_idx) const {
		if (!valid_shader_index(shader_idx)) return {};
		return shaders[shader_idx].vertex;
	}

	std::string Scene::get_shader_fragment_path(size_t shader_idx) const {
		if (!valid_shader_index(shader_idx)) return {};
		return shaders[shader_idx].fragment;
	}

	// ----------------- Instances per shader -----------------
	size_t Scene::instance_count(size_t shader_idx) const {
		if (!valid_shader_index(shader_idx)) return 0;
		return shaders[shader_idx].instances.size();
	}

	// ONLY built-ins: create default instance and return its index
	size_t Scene::add_instance(size_t shader_idx) {
		if (!valid_shader_index(shader_idx)) return static_cast<size_t>(kInvalidIndex);
		shaders[shader_idx].instances.emplace_back();            // default-constructed Instance
		return shaders[shader_idx].instances.size() - 1;
	}

	bool Scene::remove_instance(size_t shader_idx, size_t instance_idx) {
		if (!valid_instance_index(shader_idx, instance_idx)) return false;
		auto& v = shaders[shader_idx].instances;
		v.erase(v.begin() + static_cast<std::ptrdiff_t>(instance_idx));
		return true;
	}


	// -------- Instance: groups & drawcalls --------
	bool Scene::set_instance_group_size(size_t s, size_t i, int gx, int gy, int gz) {
		if (!valid_instance_index(s, i)) return false;
		auto& inst = shaders[s].instances[i];
		inst.group_size_x = gx; inst.group_size_y = gy; inst.group_size_z = gz;
		return true;
	}

	bool Scene::get_instance_group_size(size_t s, size_t i, int& gx, int& gy, int& gz) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& inst = shaders[s].instances[i];
		gx = inst.group_size_x; gy = inst.group_size_y; gz = inst.group_size_z;
		return true;
	}

	bool Scene::set_instance_drawcalls(size_t s, size_t i, int n) {
		if (!valid_instance_index(s, i)) return false;
		shaders[s].instances[i].number_of_drawcalls = n;
		return true;
	}

	bool Scene::get_instance_drawcalls(size_t s, size_t i, int& n) const {
		if (!valid_instance_index(s, i)) return false;
		n = shaders[s].instances[i].number_of_drawcalls;
		return true;
	}

	// -------- Instance: transforms (start) --------
	bool Scene::set_instance_position_start(size_t s, size_t i, float x, float y, float z) {
		if (!valid_instance_index(s, i)) return false;
		auto& t = shaders[s].instances[i].transform_start;
		t.position_x = x; t.position_y = y; t.position_z = z; return true;
	}

	bool Scene::set_instance_euler_start(size_t s, size_t i, float ex, float ey, float ez) {
		if (!valid_instance_index(s, i)) return false;
		auto& t = shaders[s].instances[i].transform_start;
		t.euler_x = ex; t.euler_y = ey; t.euler_z = ez; return true;
	}

	bool Scene::set_instance_scale_start(size_t s, size_t i, float sx, float sy, float sz) {
		if (!valid_instance_index(s, i)) return false;
		auto& t = shaders[s].instances[i].transform_start;
		t.scale_x = sx; t.scale_y = sy; t.scale_z = sz; return true;
	}

	bool Scene::get_instance_position_start(size_t s, size_t i, float& x, float& y, float& z) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& t = shaders[s].instances[i].transform_start;
		x = t.position_x; y = t.position_y; z = t.position_z; return true;
	}

	bool Scene::get_instance_euler_start(size_t s, size_t i, float& ex, float& ey, float& ez) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& t = shaders[s].instances[i].transform_start;
		ex = t.euler_x; ey = t.euler_y; ez = t.euler_z; return true;
	}

	bool Scene::get_instance_scale_start(size_t s, size_t i, float& sx, float& sy, float& sz) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& t = shaders[s].instances[i].transform_start;
		sx = t.scale_x; sy = t.scale_y; sz = t.scale_z; return true;
	}

	// -------- Instance: transforms (end) --------
	bool Scene::set_instance_position_end(size_t s, size_t i, float x, float y, float z) {
		if (!valid_instance_index(s, i)) return false;
		auto& t = shaders[s].instances[i].transform_end;
		t.position_x = x; t.position_y = y; t.position_z = z; return true;
	}

	bool Scene::set_instance_euler_end(size_t s, size_t i, float ex, float ey, float ez) {
		if (!valid_instance_index(s, i)) return false;
		auto& t = shaders[s].instances[i].transform_end;
		t.euler_x = ex; t.euler_y = ey; t.euler_z = ez; return true;
	}

	bool Scene::set_instance_scale_end(size_t s, size_t i, float sx, float sy, float sz) {
		if (!valid_instance_index(s, i)) return false;
		auto& t = shaders[s].instances[i].transform_end;
		t.scale_x = sx; t.scale_y = sy; t.scale_z = sz; return true;
	}

	bool Scene::get_instance_position_end(size_t s, size_t i, float& x, float& y, float& z) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& t = shaders[s].instances[i].transform_end;
		x = t.position_x; y = t.position_y; z = t.position_z; return true;
	}

	bool Scene::get_instance_euler_end(size_t s, size_t i, float& ex, float& ey, float& ez) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& t = shaders[s].instances[i].transform_end;
		ex = t.euler_x; ey = t.euler_y; ez = t.euler_z; return true;
	}

	bool Scene::get_instance_scale_end(size_t s, size_t i, float& sx, float& sy, float& sz) const {
		if (!valid_instance_index(s, i)) return false;
		const auto& t = shaders[s].instances[i].transform_end;
		sx = t.scale_x; sy = t.scale_y; sz = t.scale_z; return true;
	}

	// -------- Instance: uniforms (start/end, u0..u9 by index) --------
	bool Scene::set_instance_uniform_start(size_t s, size_t i, int u_index, float value) {
		if (!valid_instance_index(s, i)) return false;
		return set_uniform_by_index(shaders[s].instances[i].uniforms_start, u_index, value);
	}

	bool Scene::set_instance_uniform_end(size_t s, size_t i, int u_index, float value) {
		if (!valid_instance_index(s, i)) return false;
		return set_uniform_by_index(shaders[s].instances[i].uniforms_end, u_index, value);
	}

	bool Scene::get_instance_uniform_start(size_t s, size_t i, int u_index, float& out_value) const {
		if (!valid_instance_index(s, i) || u_index < 0 || u_index > 9) return false;
		out_value = get_uniform_by_index(shaders[s].instances[i].uniforms_start, u_index);
		return true;
	}

	bool Scene::get_instance_uniform_end(size_t s, size_t i, int u_index, float& out_value) const {
		if (!valid_instance_index(s, i) || u_index < 0 || u_index > 9) return false;
		out_value = get_uniform_by_index(shaders[s].instances[i].uniforms_end, u_index);
		return true;
	}

	// --- Image/output size: explicit setters/getters ---
	void Scene::set_width(int w) { width = w; }
	void Scene::set_height(int h) { height = h; }
	// int  get_width() const { return width; }
	// int  get_height() const { return height; }

	// Pretty-print the entire scene state
	void Scene::print() const
	{
		// alias cout so you can reuse the existing body that wrote to 'os'
		std::ostream& os = std::cout;

		using std::fixed;
		using std::setprecision;

		auto b = [](bool v) { return v ? "true" : "false"; };
		auto ind = [&](int n) { for (int i = 0; i < n; ++i) os.put(' '); };

		os << fixed << setprecision(6);

		os << "Scene\n";
		ind(2); os << "resolution: " << width << " x " << height << "\n";

		// Render
		ind(2); os << "render:\n";
		ind(4); os << "fps: " << render_fps << "\n";
		ind(4); os << "number_of_frames: " << number_of_frames << "\n";
		ind(4); os << "time:\n";
		ind(6); os << "start: " << capture_settings.start_time << "\n";

		// Capture
		ind(2); os << "capture:\n";
		ind(4); os << "frames: " << b(capture_settings.capture_frames) << "\n";
		ind(4); os << "png:    " << b(capture_settings.capture_png) << "\n";
		ind(4); os << "bmp:    " << b(capture_settings.capture_bmp) << "\n";

		// LE / Post
		ind(2); os << "post (le):\n";
		ind(4); os << "halfLife: " << le_settings.halfLife << "\n";
		ind(4); os << "bloomThreshold: " << le_settings.bloomThreshold << "\n";
		ind(4); os << "bloomSoftKnee: " << le_settings.bloomSoftKnee << "\n";
		ind(4); os << "bloomStrength: " << le_settings.bloomStrength << "\n";
		ind(4); os << "bloomIterations: " << le_settings.bloomIterations << "\n";
		ind(4); os << "exposure: " << le_settings.exposure << "\n";
		ind(4); os << "gamma: " << le_settings.gamma << "\n";
		ind(4); os << "brightness: " << le_settings.brightness << "\n";
		ind(4); os << "contrast: " << le_settings.contrast << "\n";
		ind(4); os << "saturation: " << le_settings.saturation << "\n";
		ind(4); os << "msaaSamples: " << le_settings.msaaSamples << "\n";

		// Cameras
		auto print_cam = [&](const char* label, const Camera& c) {
			ind(2); os << "camera." << label << ":\n";
			ind(4); os << "x: " << c.camera_x << "\n";
			ind(4); os << "y: " << c.camera_y << "\n";
			ind(4); os << "z: " << c.camera_z << "\n";
			ind(4); os << "pitch: " << c.pitch << "\n";
			ind(4); os << "yaw: " << c.yaw << "\n";
			ind(4); os << "fov: " << c.fov << "\n";
			};
		print_cam("start", camera_start);
		print_cam("end", camera_end);

		// Shaders
		ind(2); os << "shaders: " << shaders.size() << "\n";

		for (size_t si = 0; si < shaders.size(); ++si)
		{
			const auto& S = shaders[si];
			ind(4); os << "[" << si << "]:\n";
			ind(6); os << "vertex:   " << S.vertex << "\n";
			ind(6); os << "fragment: " << S.fragment << "\n";
			ind(6); os << "instances: " << S.instances.size() << "\n";

			for (size_t ii = 0; ii < S.instances.size(); ++ii)
			{
				const auto& I = S.instances[ii];
				ind(8); os << "[" << ii << "]:\n";

				// Groups / drawcalls
				ind(10); os << "groups: "
					<< I.group_size_x << " "
					<< I.group_size_y << " "
					<< I.group_size_z << "\n";
				ind(10); os << "drawcalls: " << I.number_of_drawcalls << "\n";

				// Transforms
				auto print_xf = [&](const char* tag, const Transform& T) {
					ind(10); os << tag << ":\n";
					ind(12); os << "pos:   (" << T.position_x << ", "
						<< T.position_y << ", "
						<< T.position_z << ")\n";
					ind(12); os << "euler: (" << T.euler_x << ", "
						<< T.euler_y << ", "
						<< T.euler_z << ")\n";
					ind(12); os << "scale: (" << T.scale_x << ", "
						<< T.scale_y << ", "
						<< T.scale_z << ")\n";
					};
				print_xf("transform_start", I.transform_start);
				print_xf("transform_end", I.transform_end);

				// Uniforms (u0..u9)
				auto print_uniforms = [&](const char* tag, const Instance::Uniforms& U) {
					ind(10); os << tag << ":";
					for (int ui = 0; ui <= 9; ++ui)
					{
						// reuse class helper
						float v = get_uniform_by_index(U, ui);
						os << " u" << ui << "=" << v;
					}
					os << "\n";
					};
				print_uniforms("uniforms_start", I.uniforms_start);
				print_uniforms("uniforms_end", I.uniforms_end);
			}
		}

		os.flush();
	}





	// --------------- Helpers ---------------
	bool Scene::set_uniform_by_index(Instance::Uniforms& u, int idx, float v)
	{
		switch (idx)
		{
		case 0: u.u0 = v; break; case 1: u.u1 = v; break; case 2: u.u2 = v; break;
		case 3: u.u3 = v; break; case 4: u.u4 = v; break; case 5: u.u5 = v; break;
		case 6: u.u6 = v; break; case 7: u.u7 = v; break; case 8: u.u8 = v; break;
		case 9: u.u9 = v; break; default: return false;
		}
		return true;
	}

	float Scene::get_uniform_by_index(const Instance::Uniforms& u, int idx)
	{
		switch (idx)
		{
		case 0: return u.u0; case 1: return u.u1; case 2: return u.u2;
		case 3: return u.u3; case 4: return u.u4; case 5: return u.u5;
		case 6: return u.u6; case 7: return u.u7; case 8: return u.u8;
		case 9: return u.u9; default: return 0.0f;
		}
	}

	bool Scene::valid_shader_index(size_t s) const
	{
		return s < shaders.size();
	}

	bool Scene::valid_instance_index(size_t s, size_t i) const
	{
		if (!valid_shader_index(s)) return false;
		return i < shaders[s].instances.size();
	}

	bool Scene::clear_instances(size_t shader_idx)
	{
		if (!valid_shader_index(shader_idx)) return false;
		shaders[shader_idx].instances.clear();
		return true;
	}

	bool Scene::has_shader(size_t shader_idx) const
	{
		return valid_shader_index(shader_idx);
	}

}




namespace Scene_
{


	namespace DefaultShaderSourceCode_
	{
		static constexpr char GLSL_Vertex[] = R"glsl(


#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

out vec3 color_vs;

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

float saturate(float x) { return clamp(x, 0.0, 1.0); }
vec2  saturate(vec2  x) { return clamp(x, 0.0, 1.0); }
vec3  saturate(vec3  x) { return clamp(x, 0.0, 1.0); }

float remap(float x, float a, float b, float c, float d) {
    return c + (d - c) * ((x - a) / (b - a));
}

vec3 safeNormalize(vec3 v) {
    float m = max(length(v), 1e-20);
    return v / m;
}

// safe division that preserves sign and avoids blowups
float safeDiv(float num, float den, float eps) {
    float a = abs(den);
    return num / max(a, eps) * sign(den);
}

// sign-preserving power to avoid NaNs with negative bases
float spow(float x, float e) {
    return sign(x) * pow(abs(x), e);
}

// ---------- Quick polar helpers ----------
vec2 polar(float r, float a) { return r * vec2(cos(a), sin(a)); }
vec3 spherical(float r, float theta, float phi) {
    // theta: azimuth [0,2 * pi), phi: polar [0,pi]
    return vec3(
        r * sin(phi) * cos(theta),
        r * cos(phi),
        r * sin(phi) * sin(theta)
    );
}

// theta01: 0..1  -> azimuth 0..2 * pi (around Y axis)
// phi01  : 0..1  -> polar   0..pi  (0 = north pole, 1 = south pole)
vec3 spherical01(float r, float theta01, float phi01) {
    const float PI = 3.14159265358979323846;
    const float TAU = 6.28318530717958647692; // 2*PI

    float theta = theta01 * TAU; // azimuth
    float phi = phi01 * PI;  // polar

    float sphi = sin(phi);
    return vec3(
        r * sphi * cos(theta),
        r * cos(phi),
        r * sphi * sin(theta)
    );
}

// --------- Small hash (PCG-ish) ----------
uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}
float rand01(inout uint s) {
    s = pcg_hash(s);
    return float(s) * (1.0 / 4294967295.0);
}

// Axis-angle -> mat3 (Rodrigues)
mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c, t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c, t.y * axis.z - s * axis.x,
        t.z * axis.x - s * axis.y, t.z * axis.y + s * axis.x, t.z * axis.z + c
    );
}

// Scene Functions












// ---------- Weight selector config ----------
const int N_WEIGHTS = 22;       // number of effects you want to choose from (0..20)
const float PERIOD = 1.0;      // seconds per "scene" (try 4..12)
const float AMP_MIN = 0.35;     // min per-slot amplitude
const float AMP_MAX = 1.00;     // max per-slot amplitude
const float FADE_SHAPE = 1.0;   // 1=linear Hermite-ish; >1 sharper, <1 softer

// Stagger the 4 slots so they don`t all flip at once (keeps scene readable)
const vec4 SLOT_OFFSETS = vec4(0.00, 0.27, 0.54, 0.79);

// Hash -> random float without mutating an in/out state
float rand01u(uint x) {
    x = pcg_hash(x);
    return float(x) * (1.0 / 4294967295.0);
}

// Get a random int in [0, maxExclusive)
int randInt(uint seed, int maxExclusive) {
    return int(uint(pcg_hash(seed)) % uint(maxExclusive));
}

// Pick 4 distinct indices using simple rehash-on-collision
ivec4 pick4Distinct(uint seed, int maxExclusive) {
    int a = randInt(seed + 11u, maxExclusive);

    int b = a;
    uint sb = seed + 23u;
    for (int i = 0; i < 16 && b == a; ++i) { b = randInt(sb, maxExclusive); sb += 97u; }

    int c = a;
    uint sc = seed + 37u;
    for (int i = 0; i < 24 && (c == a || c == b); ++i) { c = randInt(sc, maxExclusive); sc += 97u; }

    int d = a;
    uint sd = seed + 59u;
    for (int i = 0; i < 32 && (d == a || d == b || d == c); ++i) { d = randInt(sd, maxExclusive); sd += 97u; }

    return ivec4(a, b, c, d);
}

// Smooth 0->1 curve you can tune
float ease01(float x) {
    x = clamp(x, 0.0, 1.0);
    // Hermite, then optional shaping
    float h = x * x * (3.0 - 2.0 * x);
    return pow(h, FADE_SHAPE);
}




// HSV -> RGB (fast, branchless)
vec3 hsv2rgb(vec3 c) {
    vec3 p = abs(fract(c.xxx + vec3(0.0, 1.0, 2.0) / 3.0) * 6.0 - 3.0);
    return c.z * mix(vec3(1.0), clamp(p - 1.0, 0.0, 1.0), c.y);
}

// RGB -> HSV (matches hsv2rgb above)
vec3 rgb2hsv(vec3 c) {
    float cMax = max(c.r, max(c.g, c.b));
    float cMin = min(c.r, min(c.g, c.b));
    float d = cMax - cMin;

    float h = 0.0;
    if (d > 1e-6) {
        if (cMax == c.r)      h = mod((c.g - c.b) / d, 6.0);
        else if (cMax == c.g) h = ((c.b - c.r) / d) + 2.0;
        else                  h = ((c.r - c.g) / d) + 4.0;
        h /= 6.0;
        if (h < 0.0) h += 1.0;
    }
    float s = (cMax <= 1e-6) ? 0.0 : d / cMax;
    float v = cMax;
    return vec3(h, s, v);
}



// A simple deterministic palette per effect index
vec3 effectColor(int i) {
    // Golden-ratio hue spacing keeps colors distinct across 21 effects
    float h = fract(0.61803398875 * float(i) + 0.17);
    float s = 0.70;
    float v = 0.90;
    return hsv2rgb(vec3(h, s, v));
}





void main()
{
    int id = gl_InstanceID;

    // Constanst
    const int number_per_dimension = 1000;

    // Well usualls universally usefull Calculations
    id = id + (number_per_dimension * number_per_dimension) * int(uDrawcallNumber);


    uint s_0 = uSeed + uint(id);
    float rnd_x = rand01(s_0);

    uint s_1 = uSeed + uint(id + 42);
    float rnd_y = rand01(s_1);

    float px = rnd_x;
    float py = rnd_y;
    float pz = 0.0;


    // --- SPHERE CALCULATION ---
    float radius = 100.0;
    
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    px = sphere_position.x;
    py = sphere_position.y;
    pz = sphere_position.z;

    



    float scale_cube = 0.01 * 0.7 * 2.0 * 2.0 * 2.0 * 0.4 * 10.0;

    // 312312
    float color_r = 0.2;
    float color_g = 0.2;
    float color_b = 0.2;

    // Prepering data for the next block
    color_vs = vec3(color_r, color_g, color_b);
    vec3 pos = vec3(px, pz, py);
    vec3 scale = vec3(scale_cube, scale_cube, scale_cube);




    // The block that stays the same 
    {
        // Matrices to calculate instance transform

        mat4 T = mat4(1.0);
        mat4 R = mat4(1.0);
        mat4 S = mat4(1.0);



        // Set Transform
        // pos = vec3(id * 2.0, 0.0, 0.0);

        T[3] = vec4(pos, 1.0);

        // Set rotation
        vec3 axis = vec3(0.0, 1.0, 0.0);
        axis = normalize(axis);

        float angle = uTime * 1.0;

        mat3 R3 = axisAngleToMat3(axis, angle);

        R = mat4(
            vec4(R3[0], 0.0),
            vec4(R3[1], 0.0),
            vec4(R3[2], 0.0),
            vec4(0.0, 0.0, 0.0, 1.0)
        );

        // Set Scale
        S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;



        mat4 instanceModel = T * R * S;

        

        gl_Position = projection * view * model * instanceModel * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }


}



)glsl";
















	static constexpr char GLSL_Fragment[] = R"glsl(
#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float uTime;          // seconds
uniform float uRingFreq;      // rings per face radius (e.g. 8.0)
uniform float uSpeed;         // animation speed (e.g. 0.35)
uniform float uGlowWidth;     // ring width, in [0.01..0.25] (e.g. 0.08)
uniform float uGlowStrength;  // HDR emission gain (e.g. 3.0)
uniform float uStripeCount;   // angular stripes (e.g. 12.0)
uniform float uStripeWidth;   // stripe width (e.g. 0.18)
uniform float uBase;          // base albedo scale (e.g. 0.03)


uniform vec3 uCameraPos;
uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;

const float PI = 3.14159265359;

in vec3 color_vs;

// Little color palette (Inigo Quilez style)
vec3 palette(float t) {
    vec3 a = vec3(0.55, 0.45, 0.60);
    vec3 b = vec3(0.45, 0.35, 0.40);
    vec3 c = vec3(1.00, 1.00, 1.00);
    vec3 d = vec3(0.00, 0.33, 0.67);
    return a + b * cos(2.0 * PI * (c * t + d));
}

// Gaussian helper (for soft peaks)
float gauss(float x, float s) {
    return exp(-(x * x) / (2.0 * s * s));
}

void main()
{
    // Face-local UV centered at 0
    vec2 uv = TexCoord;
    vec2 p = uv - 0.5;
    float r = length(p) + 1e-6;      // radial distance
    float a = atan(p.y, p.x);        // angle [-pi,pi]

    // ----- Concentric rings (animated) -----
    // Repeating “saw” over radius, then shape into a bright thin ring with gaussian
    float saw = fract(r * uRingFreq - uSpeed * uTime);  // 0..1 over each ring period
    float ring = gauss(saw - 0.5, uGlowWidth);          // thin bright arc

    // ----- Angular stripes (also animated) -----
    // Map angle to 0..1, then to N stripes and shape with gaussian
    float ang01 = (a / (2.0 * PI)) + 0.5;
    float sawAng = fract(ang01 * uStripeCount - 0.2 * uSpeed * uTime);
    float stripes = gauss(sawAng - 0.5, uStripeWidth);

    // ----- Edge halo so cube borders pop a bit -----
    float edge = min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y)); // distance to nearest edge
    float edgeGlow = smoothstep(0.15, 0.0, edge); // 1 at edges, 0 inside

    // Compose an intensity field (keep it in ~0..1 range before emission)
    float glowMask = ring + 0.6 * stripes + 0.4 * edgeGlow;

    // Vary hue with radius + time for a nice gradient
    vec3 hue = palette(r * 1.2 + 0.05 * uTime + ang01 * 0.15);

    // Base (very dim albedo so faces aren’t pitch black)
    vec3 base = hue * uBase;

    // Emissive HDR glow (this is what bloom picks up!)
    vec3 emissive = hue * glowMask * uGlowStrength;

    vec3 color = base + emissive;  // HDR
    FragColor = vec4(color * color_vs, 1.0);
}



)glsl";




















	static constexpr char GLSL_bloom_blur_fs[] = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uTex;
uniform vec2 uTexelSize;   // 1.0/width, 1.0/height
uniform vec2 uDirection;   // (1,0)=horizontal, (0,1)=vertical

void main() {
    // 5-tap Gaussian weights (normalized)
    float w0 = 0.4026199469; // center
    float w1 = 0.2442013420; // ±1
    float w2 = 0.0544886845; // ±2

    vec2 off1 = uDirection * uTexelSize * 1.0;
    vec2 off2 = uDirection * uTexelSize * 2.0;

    vec3 c = texture(uTex, vUV).rgb * w0
        + texture(uTex, vUV + off1).rgb * w1
        + texture(uTex, vUV - off1).rgb * w1
        + texture(uTex, vUV + off2).rgb * w2
        + texture(uTex, vUV - off2).rgb * w2;

    FragColor = vec4(c, 1.0);
}

)glsl";


	static constexpr char GLSL_bloom_bright_fs[] = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uInput;   // HDR input (accumTex)
uniform float uThreshold;   // e.g. 1.0 (if your emissive is ~1–4)
uniform float uSoftKnee;    // 0..1 (e.g. 0.5)
uniform bool  uUseLuma;     // true: use luminance, false: use max channel

vec3 toLumaCoeff() { return vec3(0.2126, 0.7152, 0.0722); }

void main() {
    vec3 c = texture(uInput, vUV).rgb;

    float m = uUseLuma
        ? dot(c, toLumaCoeff())
        : max(max(c.r, c.g), c.b);

    float t = max(uThreshold, 1e-6);
    float k = max(t * uSoftKnee, 1e-6);

    // Soft knee around threshold
    float x = max(m - (t - k), 0.0);
    float y = (x * x) / (4.0 * k + x);  // 0..~m, smooth near t

    // Color-preserving: scale original color by y/m
    float gain = (m > 1e-6) ? (y / m) : 0.0;
    vec3 outc = c * gain;

    FragColor = vec4(outc, 1.0);
}

)glsl";



	static constexpr char GLSL_le_combine_fs[] = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;
uniform sampler2D uPrevAccum;
uniform sampler2D uScene;
uniform float uDecay;    // e.g. 0.97
uniform float uAddGain;  // 1.0 for neutral

void main() {
    vec3 prev = texture(uPrevAccum, vUV).rgb;
    vec3 curr = texture(uScene, vUV).rgb;
    float k = clamp(1.0 - uDecay, 0.0, 1.0);
    vec3 outc = prev * uDecay + curr * (k * uAddGain);
    FragColor = vec4(outc, 1.0);
}



)glsl";



	static constexpr char GLSL_le_fullscreen_vs[] = R"glsl(
#version 450 core
out vec2 vUV;
void main() {
    // Fullscreen triangle covering NDC [-1,1]
    // (0,0), (2,0), (0,2) in clip space trick
    const vec2 pos[3] = vec2[3](
        vec2(-1.0, -1.0),
        vec2(3.0, -1.0),
        vec2(-1.0, 3.0)
        );
    vUV = (pos[gl_VertexID] * 0.5) + 0.5; // map NDC to [0,1]
    gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
}



)glsl";


	static constexpr char GLSL_le_present_fs[] = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uAccum;        // HDR after long exposure
uniform sampler2D uBloom;        // blurred bright regions (can be half-res)
uniform float uBloomStrength;    // 0..2  (e.g. 0.8)

uniform float uExposure;         // HDR exposure before tone map (e.g. 1.2)
uniform float uGamma;            // 2.2

// LDR post FX
uniform float uBrightness;       // add after tone-map  (-1..+1, e.g. 0.0)
uniform float uContrast;         // scale around 0.5    (0..2,   e.g. 1.0)
uniform float uSaturation;       // 0..2 (1=identity)
uniform float uVignetteStrength; // 0..1
uniform float uVignetteStart;    // 0..1 (radius where vignetting starts)

void main() {
    vec3 hdr = texture(uAccum, vUV).rgb;
    vec3 glow = texture(uBloom, vUV).rgb;

    // Add bloom in HDR
    hdr += glow * uBloomStrength;

    // Simple tonemap (exponential)
    vec3 mapped = vec3(1.0) - exp(-hdr * uExposure);

    // LDR post-processing
    // Contrast around 0.5
    mapped = (mapped - 0.5) * uContrast + 0.5;
    // Brightness (add)
    mapped += vec3(uBrightness);
    // Saturation
    float luma = dot(mapped, vec3(0.2126, 0.7152, 0.0722));
    mapped = mix(vec3(luma), mapped, uSaturation);
    // Vignette
    float d = length(vUV - vec2(0.5)) * 1.41421356; // 0..~1 across diagonal
    float v = smoothstep(uVignetteStart, 1.0, d);
    mapped *= (1.0 - uVignetteStrength * v);

    // Gamma
    mapped = pow(max(mapped, 0.0), vec3(1.0 / max(uGamma, 1e-6)));
    FragColor = vec4(clamp(mapped, 0.0, 1.0), 1.0);
}



)glsl";



	}





}



namespace Scene_
{

	static inline std::string trim_copy(std::string s) {
		auto issp = [](unsigned char c) { return std::isspace(c); };
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c) { return !issp(c); }));
		s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c) { return !issp(c); }).base(), s.end());
		return s;
	}

	static inline void strip_comments(std::string& s) {
		bool in_double = false;   // inside "..."
		bool in_single = false;   // inside '...'
		bool escaped = false;   // previous char was a backslash

		for (size_t i = 0; i < s.size(); ++i) {
			const unsigned char uc = static_cast<unsigned char>(s[i]);
			const char c = static_cast<char>(uc);

			if (escaped) {               // treat current char as literal
				escaped = false;
				continue;
			}

			if (c == '\\') {             // start an escape (only affects next char)
				escaped = true;
				continue;
			}

			// toggle quote states (only when not escaped and not in the other quote type)
			if (!in_single && c == '"') { in_double = !in_double; continue; }
			if (!in_double && c == '\'') { in_single = !in_single; continue; }

			// only consider comments when outside quotes
			if (!in_double && !in_single) {
				// Treat '#' as a comment starter only at BOL or after whitespace.
				if (c == '#') {
					if (i == 0 || std::isspace(static_cast<unsigned char>(s[i - 1]))) {
						s.erase(i);
						break;
					}
				}

				// Treat '//' as a comment starter only at BOL or after whitespace.
				if (c == '/' && i + 1 < s.size() && s[i + 1] == '/') {
					if (i == 0 || std::isspace(static_cast<unsigned char>(s[i - 1]))) {
						s.erase(i);
						break;
					}
				}
			}
		}
	}

	static inline bool parse_bool(const std::string& in, bool& out) {
		std::string s = in;
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)std::tolower(c); });
		if (s == "true" || s == "1" || s == "yes" || s == "on") { out = true;  return true; }
		if (s == "false" || s == "0" || s == "no" || s == "off") { out = false; return true; }
		return false;
	}
	static inline bool parse_int(const std::string& in, int& out) { std::istringstream iss(in); iss >> out; return (bool)iss && iss.eof(); }
	static inline bool parse_float(const std::string& in, float& out) { std::istringstream iss(in); iss >> out; return (bool)iss && iss.eof(); }

	std::optional<Scene> load(const std::string& filepath)
	{
		Scene scene;
		scene.clear_shaders();

		std::ifstream in(filepath);
		if (!in) {
			std::cerr << "[Scene::load] ERROR: cannot open file: " << filepath << "\n";
			return std::nullopt;
		}

		std::string line; int lineno = 0;

		while (std::getline(in, line)) {
			++lineno;

			// Strip BOM on first line if present
			if (lineno == 1 && line.size() >= 3 &&
				(unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
				line.erase(0, 3);
			}

			strip_comments(line);
			line = trim_copy(line);
			if (line.empty()) continue;

			// Split into key + rest
			std::string key, rest;
			{
				std::istringstream iss(line);
				iss >> key;
				std::getline(iss, rest);
				if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
				rest = trim_copy(rest);
			}

			// alias: camrea.* -> camera.*
			if (key.rfind("camrea.", 0) == 0) key.replace(0, 7, "camera.");

			auto warn = [&](const char* msg) {
				std::cerr << "[Scene::load] line " << lineno << ": " << msg << "  (" << key << " " << rest << ")\n";
				};

			// ===== render.* =====
			if (key == "render.fps" || key == "render.framerate") {
				int v; if (parse_int(rest, v)) scene.set_render_fps(v); else warn("expected int");
			}
			else if (key == "render.number_of_frames") {
				int v; if (parse_int(rest, v)) scene.set_number_of_frames(v); else warn("expected int");
			}
			else if (key == "render.time.start") {
				float v; if (parse_float(rest, v)) scene.set_render_time_start(v); else warn("expected float");
			}

			// ===== capture.* =====
			else if (key == "capture") {
				bool v; if (parse_bool(rest, v)) scene.set_capture(v); else warn("expected bool");
			}
			else if (key == "capture.png") {
				bool v; if (parse_bool(rest, v)) scene.set_capture_png(v); else warn("expected bool");
			}
			else if (key == "capture.bmp") {
				bool v; if (parse_bool(rest, v)) scene.set_capture_bmp(v); else warn("expected bool");
			}

			// ===== camera.start.* =====
			else if (key.rfind("camera.start.", 0) == 0) {
				std::string prop = key.substr(std::string("camera.start.").size());
				float v; if (!parse_float(rest, v)) { warn("expected float"); continue; }

				if (prop == "x")     scene.set_camera_start_x(v);
				else if (prop == "y")     scene.set_camera_start_y(v);
				else if (prop == "z")     scene.set_camera_start_z(v);
				else if (prop == "pitch") scene.set_camera_start_pitch(v);
				else if (prop == "yaw") scene.set_camera_start_yaw(v);
				else if (prop == "fov")   scene.set_camera_start_fov(v);
				else warn("unknown camera.start.* key");
			}

			// ===== camera.end.* =====
			else if (key.rfind("camera.end.", 0) == 0) {
				std::string prop = key.substr(std::string("camera.end.").size());
				float v; if (!parse_float(rest, v)) { warn("expected float"); continue; }

				if (prop == "x")     scene.set_camera_end_x(v);
				else if (prop == "y")     scene.set_camera_end_y(v);
				else if (prop == "z")     scene.set_camera_end_z(v);
				else if (prop == "pitch") scene.set_camera_end_pitch(v);
				else if (prop == "yaw") scene.set_camera_end_yaw(v);
				else if (prop == "fov")   scene.set_camera_end_fov(v);
				else warn("unknown camera.end.* key");
			}

			// ===== le.* =====
			else if (key.rfind("le.", 0) == 0) {
				std::string prop = key.substr(3);
				if (prop == "msaaSamples") {
					int vi; if (parse_int(rest, vi)) scene.set_le_msaaSamples(vi); else warn("expected int");
				}
				else if (prop == "bloomIterations") {
					int vi; if (parse_int(rest, vi)) scene.set_le_bloomIterations(vi); else warn("expected int");
				}
				else {
					float vf;
					if (!parse_float(rest, vf)) { warn("expected float"); continue; }
					if (prop == "halfLife")       scene.set_le_halfLife(vf);
					else if (prop == "bloomThreshold") scene.set_le_bloomThreshold(vf);
					else if (prop == "bloomSoftKnee")  scene.set_le_bloomSoftKnee(vf);
					else if (prop == "bloomStrength")  scene.set_le_bloomStrength(vf);
					else if (prop == "exposure")       scene.set_le_exposure(vf);
					else if (prop == "gamma")          scene.set_le_gamma(vf);
					else if (prop == "brightness")     scene.set_le_brightness(vf);
					else if (prop == "contrast")       scene.set_le_contrast(vf);
					else if (prop == "saturation")     scene.set_le_saturation(vf);
					else warn("unknown le.* key");
				}
			}

			// ===== shader.add / shader.vertex / shader.fragment =====
			else if (key == "shader.add") {
				std::istringstream ss(rest);
				std::string vtx, frag;
				if (!(ss >> std::quoted(vtx))) { ss.clear(); ss.str(rest); if (!(ss >> vtx)) { warn("usage: shader.add <vertex> <fragment>"); continue; } }
				if (!(ss >> std::quoted(frag))) { if (!(ss >> frag)) { warn("usage: shader.add <vertex> <fragment>"); continue; } }
				scene.add_shader(vtx, frag);
			}
			else if (key == "shader.vertex") {
				std::istringstream ss(rest);
				size_t idx; if (!(ss >> idx)) { warn("usage: shader.vertex <index> <path>"); continue; }
				std::string path;
				if (!(ss >> std::quoted(path))) { if (!(ss >> path)) { warn("usage: shader.vertex <index> <path>"); continue; } }
				if (!scene.set_shader_vertex_path(idx, path.c_str())) warn("invalid shader index");
			}
			else if (key == "shader.fragment") {
				std::istringstream ss(rest);
				size_t idx; if (!(ss >> idx)) { warn("usage: shader.fragment <index> <path>"); continue; }
				std::string path;
				if (!(ss >> std::quoted(path))) { if (!(ss >> path)) { warn("usage: shader.fragment <index> <path>"); continue; } }
				if (!scene.set_shader_fragment_path(idx, path.c_str())) warn("invalid shader index");
			}

			// ===== shader.instance.* =====
			else if (key == "shader.instance.add") {
				std::istringstream ss(rest);
				size_t s; if (!(ss >> s)) { warn("usage: shader.instance.add <shader_idx>"); continue; }
				size_t idx = scene.add_instance(s);
				if (idx == Scene::kInvalidIndex) warn("invalid shader index");
			}
			else if (key == "shader.instance.group") {
				std::istringstream ss(rest);
				size_t s, i; int gx, gy, gz;
				if (!(ss >> s >> i >> gx >> gy >> gz)) { warn("usage: shader.instance.group <shader_idx> <instance_idx> <gx> <gy> <gz>"); continue; }
				if (!scene.set_instance_group_size(s, i, gx, gy, gz)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.drawcalls") {
				std::istringstream ss(rest);
				size_t s, i; int n;
				if (!(ss >> s >> i >> n)) { warn("usage: shader.instance.drawcalls <shader_idx> <instance_idx> <n>"); continue; }
				if (!scene.set_instance_drawcalls(s, i, n)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.start.pos") {
				std::istringstream ss(rest);
				size_t s, i; float x, y, z;
				if (!(ss >> s >> i >> x >> y >> z)) { warn("usage: shader.instance.start.pos <s> <i> <x> <y> <z>"); continue; }
				if (!scene.set_instance_position_start(s, i, x, y, z)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.start.euler") {
				std::istringstream ss(rest);
				size_t s, i; float ex, ey, ez;
				if (!(ss >> s >> i >> ex >> ey >> ez)) { warn("usage: shader.instance.start.euler <s> <i> <ex> <ey> <ez>"); continue; }
				if (!scene.set_instance_euler_start(s, i, ex, ey, ez)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.start.scale") {
				std::istringstream ss(rest);
				size_t s, i; float sx, sy, sz;
				if (!(ss >> s >> i >> sx >> sy >> sz)) { warn("usage: shader.instance.start.scale <s> <i> <sx> <sy> <sz>"); continue; }
				if (!scene.set_instance_scale_start(s, i, sx, sy, sz)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.end.pos") {
				std::istringstream ss(rest);
				size_t s, i; float x, y, z;
				if (!(ss >> s >> i >> x >> y >> z)) { warn("usage: shader.instance.end.pos <s> <i> <x> <y> <z>"); continue; }
				if (!scene.set_instance_position_end(s, i, x, y, z)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.end.euler") {
				std::istringstream ss(rest);
				size_t s, i; float ex, ey, ez;
				if (!(ss >> s >> i >> ex >> ey >> ez)) { warn("usage: shader.instance.end.euler <s> <i> <ex> <ey> <ez>"); continue; }
				if (!scene.set_instance_euler_end(s, i, ex, ey, ez)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.end.scale") {
				std::istringstream ss(rest);
				size_t s, i; float sx, sy, sz;
				if (!(ss >> s >> i >> sx >> sy >> sz)) { warn("usage: shader.instance.end.scale <s> <i> <sx> <sy> <sz>"); continue; }
				if (!scene.set_instance_scale_end(s, i, sx, sy, sz)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.start.u") {
				std::istringstream ss(rest);
				size_t s, i; int u; float v;
				if (!(ss >> s >> i >> u >> v)) { warn("usage: shader.instance.start.u <s> <i> <u_index:0-9> <value>"); continue; }
				if (u < 0 || u > 9) { warn("u_index out of range (0..9)"); continue; }
				if (!scene.set_instance_uniform_start(s, i, u, v)) warn("invalid shader/instance index");
			}
			else if (key == "shader.instance.end.u") {
				std::istringstream ss(rest);
				size_t s, i; int u; float v;
				if (!(ss >> s >> i >> u >> v)) { warn("usage: shader.instance.end.u <s> <i> <u_index:0-9> <value>"); continue; }
				if (u < 0 || u > 9) { warn("u_index out of range (0..9)"); continue; }
				if (!scene.set_instance_uniform_end(s, i, u, v)) warn("invalid shader/instance index");
			}

			// ===== image/render size =====
			else if (key == "render.size" || key == "image.size")
			{
				std::istringstream ss(rest);
				int w, h;
				if (!(ss >> w >> h))
				{
					warn("usage: render.size <width> <height>");
				}
				else
				{
					scene.set_width(w);
					scene.set_height(h);
				}
			}
			else if (key == "image.width" || key == "render.width") {
				int w; if (parse_int(rest, w)) scene.set_width(w); else warn("expected int");
			}
			else if (key == "image.height" || key == "render.height") {
				int h; if (parse_int(rest, h)) scene.set_height(h); else warn("expected int");
			}

			// ===== unknown =====
			else {
				warn("unknown key");
			}
		}

		return scene;
	}

	void save(const Scene& scene, const std::string& filepath)
	{
		std::ofstream out(filepath);
		if (!out) {
			std::cerr << "[Scene::save] ERROR: cannot open file: " << filepath << "\n";
			return;
		}

		auto b = [](bool v) { return v ? "true" : "false"; };

		out << std::fixed << std::setprecision(6);

		// Header / meta
		out << "# =========================\n";
		out << "# Scene config (auto-saved)\n";
		out << "# =========================\n\n";

		// ---- Render ----
		out << "# ---- Render ----\n";
		out << "render.fps " << scene.get_render_fps() << "\n";
		out << "render.number_of_frames " << scene.get_number_of_frames() << "\n";
		out << "render.time.start " << scene.get_render_time_start() << "\n";
		out << "render.size " << scene.get_width() << " " << scene.get_height() << "\n\n";

		// ---- Capture ----
		out << "# ---- Capture ----\n";
		out << "capture " << b(scene.get_capture()) << "\n";
		out << "capture.png " << b(scene.get_capture_png()) << "\n";
		out << "capture.bmp " << b(scene.get_capture_bmp()) << "\n\n";

		// ---- Camera: start ----
		out << "# ---- Camera: start ----\n";
		out << "camera.start.x " << scene.get_camera_start_x() << "\n";
		out << "camera.start.y " << scene.get_camera_start_y() << "\n";
		out << "camera.start.z " << scene.get_camera_start_z() << "\n";
		out << "camera.start.pitch " << scene.get_camera_start_pitch() << "\n";
		out << "camera.start.yaw " << scene.get_camera_start_yaw() << "\n";
		out << "camera.start.fov " << scene.get_camera_start_fov() << "\n\n";

		// ---- Camera: end ----
		out << "# ---- Camera: end ----\n";
		out << "camera.end.x " << scene.get_camera_end_x() << "\n";
		out << "camera.end.y " << scene.get_camera_end_y() << "\n";
		out << "camera.end.z " << scene.get_camera_end_z() << "\n";
		out << "camera.end.pitch " << scene.get_camera_end_pitch() << "\n";
		out << "camera.end.yaw " << scene.get_camera_end_yaw() << "\n";
		out << "camera.end.fov " << scene.get_camera_end_fov() << "\n\n";

		// ---- LE / Post ----
		out << "# ---- LE / Post ----\n";
		out << "le.halfLife " << scene.get_le_halfLife() << "\n";
		out << "le.bloomThreshold " << scene.get_le_bloomThreshold() << "\n";
		out << "le.bloomSoftKnee " << scene.get_le_bloomSoftKnee() << "\n";
		out << "le.bloomStrength " << scene.get_le_bloomStrength() << "\n";
		out << "le.bloomIterations " << scene.get_le_bloomIterations() << "\n";
		out << "le.exposure " << scene.get_le_exposure() << "\n";
		out << "le.gamma " << scene.get_le_gamma() << "\n";
		out << "le.brightness " << scene.get_le_brightness() << "\n";
		out << "le.contrast " << scene.get_le_contrast() << "\n";
		out << "le.saturation " << scene.get_le_saturation() << "\n";
		out << "le.msaaSamples " << scene.get_le_msaaSamples() << "\n\n";

		// =========================
		// Shaders & Instances
		// =========================
		out << "# =========================\n";
		out << "# Shaders\n";
		out << "# =========================\n";
		const size_t sc = scene.shader_count();
		for (size_t s = 0; s < sc; ++s) {
			const std::string vtx = scene.get_shader_vertex_path(s);
			const std::string frag = scene.get_shader_fragment_path(s);
			out << "shader.add " << std::quoted(vtx) << " " << std::quoted(frag) << "\n";

			const size_t ic = scene.instance_count(s);
			for (size_t i = 0; i < ic; ++i) {
				out << "shader.instance.add " << s << "\n";

				// Group sizes
				int gx = 0, gy = 0, gz = 0;
				if (scene.get_instance_group_size(s, i, gx, gy, gz)) {
					out << "shader.instance.group " << s << " " << i << " "
						<< gx << " " << gy << " " << gz << "\n";
				}

				// Drawcalls
				int n = 0;
				if (scene.get_instance_drawcalls(s, i, n)) {
					out << "shader.instance.drawcalls " << s << " " << i << " " << n << "\n";
				}

				// Transforms: start
				float x, y, z, ex, ey, ez, sx, sy, sz;
				if (scene.get_instance_position_start(s, i, x, y, z))
					out << "shader.instance.start.pos " << s << " " << i << " "
					<< x << " " << y << " " << z << "\n";
				if (scene.get_instance_euler_start(s, i, ex, ey, ez))
					out << "shader.instance.start.euler " << s << " " << i << " "
					<< ex << " " << ey << " " << ez << "\n";
				if (scene.get_instance_scale_start(s, i, sx, sy, sz))
					out << "shader.instance.start.scale " << s << " " << i << " "
					<< sx << " " << sy << " " << sz << "\n";

				// Transforms: end
				if (scene.get_instance_position_end(s, i, x, y, z))
					out << "shader.instance.end.pos " << s << " " << i << " "
					<< x << " " << y << " " << z << "\n";
				if (scene.get_instance_euler_end(s, i, ex, ey, ez))
					out << "shader.instance.end.euler " << s << " " << i << " "
					<< ex << " " << ey << " " << ez << "\n";
				if (scene.get_instance_scale_end(s, i, sx, sy, sz))
					out << "shader.instance.end.scale " << s << " " << i << " "
					<< sx << " " << sy << " " << sz << "\n";

				// Uniforms: start
				for (int u = 0; u <= 9; ++u) {
					float uv = 0.0f;
					if (scene.get_instance_uniform_start(s, i, u, uv))
						out << "shader.instance.start.u " << s << " " << i << " " << u << " " << uv << "\n";
				}
				// Uniforms: end
				for (int u = 0; u <= 9; ++u) {
					float uv = 0.0f;
					if (scene.get_instance_uniform_end(s, i, u, uv))
						out << "shader.instance.end.u " << s << " " << i << " " << u << " " << uv << "\n";
				}

				out << "\n";
			}

			if (s + 1 < sc) out << "\n"; // spacer between shaders
		}

		out.flush();
	}
}



namespace Scene_
{

	void unit_test_with_round_trip()
	{
		using Scene_::Scene;

		auto feq = [](float a, float b, float eps = 1e-5f) {
			float d = a - b; if (d < 0) d = -d; return d <= eps;
			};
		int checks = 0, fails = 0;
		auto CHECK = [&](bool ok, const char* what) {
			++checks;
			if (!ok) { ++fails; std::cerr << "[FAIL] " << what << "\n"; }
			};

		// ---------- Build reference scene S ----------
		Scene s;
		
		s.set_width(1234);
		s.set_height(888);
		s.set_render_fps(61);
		s.set_number_of_frames(777);
		s.set_render_time_start(3.141593f);

		s.set_capture(true);
		s.set_capture_png(true);
		s.set_capture_bmp(false);

		s.set_le_halfLife(0.012345f);
		s.set_le_bloomThreshold(1.234567f);
		s.set_le_bloomSoftKnee(0.765432f);
		s.set_le_bloomStrength(3.5f);
		s.set_le_bloomIterations(7);
		s.set_le_exposure(1.111111f);
		s.set_le_gamma(2.222222f);
		s.set_le_brightness(-0.05f);
		s.set_le_contrast(1.05f);
		s.set_le_saturation(0.95f);
		s.set_le_msaaSamples(8);

		// Camera start
		s.set_camera_start_x(1.0f);
		s.set_camera_start_y(2.0f);
		s.set_camera_start_z(3.0f);
		s.set_camera_start_pitch(0.10f);
		s.set_camera_start_yaw(0.20f);
		s.set_camera_start_fov(60.0f);

		// Camera end
		s.set_camera_end_x(4.0f);
		s.set_camera_end_y(5.0f);
		s.set_camera_end_z(6.0f);
		s.set_camera_end_pitch(0.30f);
		s.set_camera_end_yaw(0.40f);
		s.set_camera_end_fov(45.0f);

		// Shaders & instances
		size_t sh0 = s.add_shader("vs0.glsl", "fs0.glsl");
		size_t sh1 = s.add_shader("vs1.glsl", "fs1.glsl");

		size_t i0 = s.add_instance(sh0);
		size_t i1 = s.add_instance(sh0);
		size_t i2 = s.add_instance(sh1);

		// Instance 0 (shader 0) — rich settings
		s.set_instance_group_size(sh0, i0, 8, 9, 10);
		s.set_instance_drawcalls(sh0, i0, 3);
		s.set_instance_position_start(sh0, i0, 1.0f, 2.0f, 3.0f);
		s.set_instance_euler_start(sh0, i0, 0.1f, 0.2f, 0.3f);
		s.set_instance_scale_start(sh0, i0, 1.0f, 2.0f, 3.0f);
		s.set_instance_position_end(sh0, i0, 4.0f, 5.0f, 6.0f);
		s.set_instance_euler_end(sh0, i0, 0.4f, 0.5f, 0.6f);
		s.set_instance_scale_end(sh0, i0, 4.0f, 5.0f, 6.0f);
		for (int u = 0; u <= 9; ++u) {
			s.set_instance_uniform_start(sh0, i0, u, u * 0.5f);
			s.set_instance_uniform_end(sh0, i0, u, u * 1.5f);
		}

		// Instance 1 (shader 0) — minimal but non-default
		s.set_instance_group_size(sh0, i1, 1, 1, 1);
		s.set_instance_drawcalls(sh0, i1, 1);
		s.set_instance_position_start(sh0, i1, -1.0f, 0.0f, 1.0f);
		s.set_instance_position_end(sh0, i1, -2.0f, 0.5f, 2.0f);
		s.set_instance_scale_start(sh0, i1, 0.5f, 0.5f, 0.5f);
		s.set_instance_scale_end(sh0, i1, 2.0f, 2.0f, 2.0f);
		s.set_instance_euler_start(sh0, i1, 0.0f, 0.1f, 0.2f);
		s.set_instance_euler_end(sh0, i1, 0.3f, 0.4f, 0.5f);

		// Instance 2 (shader 1)
		s.set_instance_group_size(sh1, i2, 16, 8, 1);
		s.set_instance_drawcalls(sh1, i2, 2);

		// ---------- Round-trip ----------
		const char* fileA = "roundtrip_A.txt";
		const char* fileB = "roundtrip_B.txt";

		Scene_::save(s, fileA);
		auto loaded = Scene_::load(fileA);
		CHECK(loaded.has_value(), "load(fileA) should succeed");
		if (!loaded) { std::cerr << "[UNIT] ABORT: load failed\n"; return; }
		Scene t = *loaded;

		Scene_::save(t, fileB);

		auto read_all = [](const char* path) {
			std::ifstream fin(path, std::ios::binary);
			std::ostringstream ss;
			ss << fin.rdbuf();
			return ss.str();
			};
		const std::string A = read_all(fileA);
		const std::string B = read_all(fileB);
		CHECK(A == B, "textual save(load(save(S))) should be byte-identical");

		// ---------- Programmatic comparisons ----------
		CHECK(s.get_width() == t.get_width(), "width");
		CHECK(s.get_height() == t.get_height(), "height");
		CHECK(s.get_render_fps() == t.get_render_fps(), "render_fps");
		CHECK(s.get_number_of_frames() == t.get_number_of_frames(), "number_of_frames");
		CHECK(feq(s.get_render_time_start(), t.get_render_time_start()), "render_time_start");

		CHECK(s.get_capture() == t.get_capture(), "capture");
		CHECK(s.get_capture_png() == t.get_capture_png(), "capture_png");
		CHECK(s.get_capture_bmp() == t.get_capture_bmp(), "capture_bmp");

		CHECK(feq(s.get_le_halfLife(), t.get_le_halfLife()), "le.halfLife");
		CHECK(feq(s.get_le_bloomThreshold(), t.get_le_bloomThreshold()), "le.bloomThreshold");
		CHECK(feq(s.get_le_bloomSoftKnee(), t.get_le_bloomSoftKnee()), "le.bloomSoftKnee");
		CHECK(feq(s.get_le_bloomStrength(), t.get_le_bloomStrength()), "le.bloomStrength");
		CHECK(s.get_le_bloomIterations() == t.get_le_bloomIterations(), "le.bloomIterations");
		CHECK(feq(s.get_le_exposure(), t.get_le_exposure()), "le.exposure");
		CHECK(feq(s.get_le_gamma(), t.get_le_gamma()), "le.gamma");
		CHECK(feq(s.get_le_brightness(), t.get_le_brightness()), "le.brightness");
		CHECK(feq(s.get_le_contrast(), t.get_le_contrast()), "le.contrast");
		CHECK(feq(s.get_le_saturation(), t.get_le_saturation()), "le.saturation");
		CHECK(s.get_le_msaaSamples() == t.get_le_msaaSamples(), "le.msaaSamples");

		// Cameras
		CHECK(feq(s.get_camera_start_x(), t.get_camera_start_x()), "cam.start.x");
		CHECK(feq(s.get_camera_start_y(), t.get_camera_start_y()), "cam.start.y");
		CHECK(feq(s.get_camera_start_z(), t.get_camera_start_z()), "cam.start.z");
		CHECK(feq(s.get_camera_start_pitch(), t.get_camera_start_pitch()), "cam.start.pitch");
		CHECK(feq(s.get_camera_start_yaw(), t.get_camera_start_yaw()), "cam.start.yaw");
		CHECK(feq(s.get_camera_start_fov(), t.get_camera_start_fov()), "cam.start.fov");

		CHECK(feq(s.get_camera_end_x(), t.get_camera_end_x()), "cam.end.x");
		CHECK(feq(s.get_camera_end_y(), t.get_camera_end_y()), "cam.end.y");
		CHECK(feq(s.get_camera_end_z(), t.get_camera_end_z()), "cam.end.z");
		CHECK(feq(s.get_camera_end_pitch(), t.get_camera_end_pitch()), "cam.end.pitch");
		CHECK(feq(s.get_camera_end_yaw(), t.get_camera_end_yaw()), "cam.end.yaw");
		CHECK(feq(s.get_camera_end_fov(), t.get_camera_end_fov()), "cam.end.fov");

		// Shaders & instances
		CHECK(s.shader_count() == t.shader_count(), "shader_count");
		for (size_t si = 0; si < s.shader_count(); ++si) {
			CHECK(s.get_shader_vertex_path(si) == t.get_shader_vertex_path(si), "shader.vertex path");
			CHECK(s.get_shader_fragment_path(si) == t.get_shader_fragment_path(si), "shader.fragment path");

			CHECK(s.instance_count(si) == t.instance_count(si), "instance_count");
			for (size_t ii = 0; ii < s.instance_count(si); ++ii) {
				int sgx = 0, sgy = 0, sgz = 0, tgx = 0, tgy = 0, tgz = 0, sn = 0, tn = 0;

				CHECK(s.get_instance_group_size(si, ii, sgx, sgy, sgz), "get_instance_group_size S");
				CHECK(t.get_instance_group_size(si, ii, tgx, tgy, tgz), "get_instance_group_size T");
				CHECK(sgx == tgx && sgy == tgy && sgz == tgz, "group_size equal");

				CHECK(s.get_instance_drawcalls(si, ii, sn), "get_instance_drawcalls S");
				CHECK(t.get_instance_drawcalls(si, ii, tn), "get_instance_drawcalls T");
				CHECK(sn == tn, "drawcalls equal");

				float sx, sy, sz, tx, ty, tz;

				CHECK(s.get_instance_position_start(si, ii, sx, sy, sz), "pos_start S");
				CHECK(t.get_instance_position_start(si, ii, tx, ty, tz), "pos_start T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "pos_start equal");

				CHECK(s.get_instance_euler_start(si, ii, sx, sy, sz), "euler_start S");
				CHECK(t.get_instance_euler_start(si, ii, tx, ty, tz), "euler_start T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "euler_start equal");

				CHECK(s.get_instance_scale_start(si, ii, sx, sy, sz), "scale_start S");
				CHECK(t.get_instance_scale_start(si, ii, tx, ty, tz), "scale_start T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "scale_start equal");

				CHECK(s.get_instance_position_end(si, ii, sx, sy, sz), "pos_end S");
				CHECK(t.get_instance_position_end(si, ii, tx, ty, tz), "pos_end T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "pos_end equal");

				CHECK(s.get_instance_euler_end(si, ii, sx, sy, sz), "euler_end S");
				CHECK(t.get_instance_euler_end(si, ii, tx, ty, tz), "euler_end T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "euler_end equal");

				CHECK(s.get_instance_scale_end(si, ii, sx, sy, sz), "scale_end S");
				CHECK(t.get_instance_scale_end(si, ii, tx, ty, tz), "scale_end T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "scale_end equal");

				for (int u = 0; u <= 9; ++u) {
					float us = 0, ut = 0;
					CHECK(s.get_instance_uniform_start(si, ii, u, us), "uniform_start S");
					CHECK(t.get_instance_uniform_start(si, ii, u, ut), "uniform_start T");
					CHECK(feq(us, ut), "uniform_start equal");

					CHECK(s.get_instance_uniform_end(si, ii, u, us), "uniform_end S");
					CHECK(t.get_instance_uniform_end(si, ii, u, ut), "uniform_end T");
					CHECK(feq(us, ut), "uniform_end equal");
				}
			}
		}

		CHECK(s.has_shader(sh0), "has_shader(sh0)");
		CHECK(s.clear_instances(sh0), "clear_instances(sh0)");
		CHECK(s.instance_count(sh0) == 0, "instances cleared");



		// A couple of negative checks (invalid indices)
		float dummy;
		CHECK(!t.get_instance_uniform_start(999, 0, 0, dummy), "invalid shader index rejected");
		CHECK(!t.get_instance_uniform_start(0, 999, 0, dummy), "invalid instance index rejected");

		std::cout << "[UNIT] round-trip textual equality: " << (A == B ? "true" : "false") << "\n";
		std::cout << "[UNIT] checks=" << checks << " failures=" << fails << "\n";
		std::cout << (fails == 0 ? "[UNIT] PASS\n" : "[UNIT] FAIL\n");
	}
}





namespace Scene_
{
	void scene_init()
	{
		// writing default shaders into tmp
		{
			Folder::create_folder_if_does_not_exist_already("tmp");
			Folder::create_folder_if_does_not_exist_already("tmp/default_shaders");

			// Vertex Shader
			{
				File::writeFileIfNotExists("tmp/default_shaders/vertex_shader.glsl", DefaultShaderSourceCode_::GLSL_Vertex);
			}

			// Fragment shader
			{
				File::writeFileIfNotExists("tmp/default_shaders/fragment_shader.glsl", DefaultShaderSourceCode_::GLSL_Fragment);
			}

			// bloom_blur_fs.glsl
			{
				File::writeFileIfNotExists("tmp/default_shaders/bloom_blur_fs.glsl", DefaultShaderSourceCode_::GLSL_bloom_blur_fs);
			}

			// bloom_bright_fs.glsl
			{
				File::writeFileIfNotExists("tmp/default_shaders/bloom_bright_fs.glsl", DefaultShaderSourceCode_::GLSL_bloom_bright_fs);
			}

			// le_combine_fs.gls
			{
				File::writeFileIfNotExists("tmp/default_shaders/le_combine_fs.glsl", DefaultShaderSourceCode_::GLSL_le_combine_fs);
			}

			// le_fullscreen_vs.glsl
			{
				File::writeFileIfNotExists("tmp/default_shaders/le_fullscreen_vs.glsl", DefaultShaderSourceCode_::GLSL_le_fullscreen_vs);
			}

			// le_present_fs.glsl
			{
				File::writeFileIfNotExists("tmp/default_shaders/le_present_fs.glsl", DefaultShaderSourceCode_::GLSL_le_present_fs);
			}


		}

		// Well here we can be sure we have the valid shader that we can compile if the other ones are not finee


		{
			bool we_have_an_argument = true;

			if (we_have_an_argument)
			{
				//if (File::exists("path_to_commands/command.txt"))
				{
					// we read commands 
					bool commands_error = true;
					if (commands_error)
					{
						// std::cout << "error with commands\n";
						return;
					}
				}
			}
			else
			{
				if (false)// if (File::exists("commands.txt"))
				{
					// we read commands 
					bool commands_error = true;
					if (commands_error)
					{
						// std::cout << "error with commands\n";
						return;
					}
				}
				else
				{
					// we write the commands file
					// with a starting shaders as well

				}
			}
		}

		// well here we are sure we have valid commands 

	}

	Scene load_commands_no_argument()
	{

		const std::string filepath = "commands.txt";

		// 1) Try to load existing file
		if (auto scene = Scene_::load(filepath)) {
			std::cout << "commands.txt loaded\n";
			return *scene; // or just: return *scene;
		}









		std::string commands_base_source = R"DSL_COMMANDS(

// render / display settings
render.width 3840
render.height 2160
render.fps 60
render.number_of_frames 100
render.time.start 0.0

// capture
capture false
capture.png true
capture.bmp false

// camera start
camera.start.x 0.0
camera.start.y 0.0
camera.start.z 0.0
camera.start.pitch 0.0
camera.start.yaw 0.0
camera.start.fov 45.0

// camera end
camera.end.x 0.0
camera.end.y 0.0
camera.end.z 0.0
camera.end.pitch 0.0
camera.end.yaw 0.0
camera.end.fov 45.0

// LE
le.halfLife 0.2
le.bloomThreshold 1.1
le.bloomSoftKnee 0.7
le.bloomStrength 2.0
le.bloomIterations 6
le.exposure 0.3
le.gamma 2.2
le.brightness 0.0
le.contrast 1.0
le.saturation 1.0

le.msaaSamples 4



// Shader 0
shader.add "tmp/default_shaders/vertex_shader.glsl" "tmp/default_shaders/fragment_shader.glsl"

# (Optional) override a path later
# shader.vertex 0 shaders/experiment_vs.glsl
# shader.fragment 0 shaders/experiment_fs.glsl

# =========================
# Instances — Shader 0
# =========================

# Instance 0 on Shader 0
shader.instance.add 0              # -> instance index 0
shader.instance.group 0 0 1000 1000 1

// shader 0
// instance 0
// drawcalls 4
shader.instance.drawcalls 0 0 4

# start transform
shader.instance.start.pos   0 0  0.000000  0.000000  0.000000
shader.instance.start.euler 0 0  0.000000  0.000000  0.000000
shader.instance.start.scale 0 0  1.000000  1.000000  1.000000

# end transform
shader.instance.end.pos     0 0  5.000000  0.000000  0.000000
shader.instance.end.euler   0 0  0.000000  1.570796  0.000000    # 90 deg about Y
shader.instance.end.scale   0 0  1.000000  1.000000  1.000000

# start uniforms (u0..u9)
shader.instance.start.u 0 0 0 0.000000
shader.instance.start.u 0 0 1 0.100000
shader.instance.start.u 0 0 2 0.200000
shader.instance.start.u 0 0 3 0.300000
shader.instance.start.u 0 0 4 0.400000
shader.instance.start.u 0 0 5 0.500000
shader.instance.start.u 0 0 6 0.600000
shader.instance.start.u 0 0 7 0.700000
shader.instance.start.u 0 0 8 0.800000
shader.instance.start.u 0 0 9 0.900000

# end uniforms (u0..u9)
shader.instance.end.u   0 0 0 1.000000
shader.instance.end.u   0 0 1 0.900000
shader.instance.end.u   0 0 2 0.800000
shader.instance.end.u   0 0 3 0.700000
shader.instance.end.u   0 0 4 0.600000
shader.instance.end.u   0 0 5 0.500000
shader.instance.end.u   0 0 6 0.400000
shader.instance.end.u   0 0 7 0.300000
shader.instance.end.u   0 0 8 0.200000
shader.instance.end.u   0 0 9 0.100000


















# -------------------------
# Instance 1 on Shader 0
// shader.instance.add 0              # -> instance index 1
// shader.instance.group 0 1 512 512 4
// shader.instance.drawcalls 0 1 8

# start transform
// shader.instance.start.pos   0 1  -2.000000  0.500000  1.000000
// shader.instance.start.euler 0 1   0.100000  0.200000  0.300000
// shader.instance.start.scale 0 1   0.500000  0.500000  0.500000

# end transform
// shader.instance.end.pos     0 1   2.000000  1.000000 -1.000000
// shader.instance.end.euler   0 1   0.300000  0.200000  0.100000
// shader.instance.end.scale   0 1   1.500000  1.500000  1.500000

# a few uniforms
// shader.instance.start.u 0 1 0 0.250000
// shader.instance.start.u 0 1 1 0.750000
// shader.instance.end.u   0 1 0 0.900000
// shader.instance.end.u   0 1 1 0.100000

# =========================
# Instances — Shader 1
# =========================

# Instance 0 on Shader 1
// shader.instance.add 1              # -> instance index 0
// shader.instance.group 1 0 256 256 1
// shader.instance.drawcalls 1 0 2

# start transform
// shader.instance.start.pos   1 0   0.000000  0.000000  5.000000
// shader.instance.start.euler 1 0   0.000000  0.000000  0.000000
// shader.instance.start.scale 1 0   2.000000  2.000000  2.000000

# end transform
// shader.instance.end.pos     1 0   0.000000  0.000000  5.000000
// shader.instance.end.euler   1 0   0.000000  6.283185  0.000000    # 360 deg about Y
// shader.instance.end.scale   1 0   2.000000  2.000000  2.000000

# full uniform sweep start/end
// shader.instance.start.u 1 0 0 0.000000
// shader.instance.start.u 1 0 1 0.111111
// shader.instance.start.u 1 0 2 0.222222
// shader.instance.start.u 1 0 3 0.333333
// shader.instance.start.u 1 0 4 0.444444
// shader.instance.start.u 1 0 5 0.555555
// shader.instance.start.u 1 0 6 0.666666
// shader.instance.start.u 1 0 7 0.777777
// shader.instance.start.u 1 0 8 0.888888
// shader.instance.start.u 1 0 9 1.000000

// shader.instance.end.u   1 0 0 1.000000
// shader.instance.end.u   1 0 1 0.888888
// shader.instance.end.u   1 0 2 0.777777
// shader.instance.end.u   1 0 3 0.666666
// shader.instance.end.u   1 0 4 0.555555
// shader.instance.end.u   1 0 5 0.444444
// shader.instance.end.u   1 0 6 0.333333
// shader.instance.end.u   1 0 7 0.222222
// shader.instance.end.u   1 0 8 0.111111
// shader.instance.end.u   1 0 9 0.000000

)DSL_COMMANDS";

		// NOTE: make sure File::writeFileIfNotExists returns bool
		File::writeFileIfNotExists(filepath, commands_base_source);


		// 3) Load the freshly written defaults
		if (auto scene2 = Scene_::load(filepath)) {
			std::cout << "commands.txt created with defaults\n";
			return *scene2;
		}

		// 4) Still failing -> surface a clear error
		throw std::runtime_error("Created '" + filepath + "' but failed to parse it.");
	}

	Scene load_with_arguments(const std::string& filepath)
	{
		if (auto scene = Scene_::load(filepath))
		{
			std::cout << filepath << " loaded\n";
			return *scene;            // or: return std::move(*scene);
		}

		throw std::runtime_error(
			"Failed to load/parse scene from '" + filepath + "'."
		);
	}

	Scene load(int argc, char* argv[])
	{
		scene_init();

		if (argc < 2)
		{
			std::cout << "no arguments\n";
			// load commands.txt
			Scene_::Scene scene = load_commands_no_argument();

			// std::cout << "start fov : " << scene.get_camera_start_fov() << "\n";

			return scene;
		}
		else
		{
			std::string firstArg = argv[1];
			// std::cout << "First argument as std::string: " << firstArg << std::endl;
			Scene_::Scene scene = load_with_arguments(firstArg);
			// std::cout << "start fov : " << scene.get_camera_start_fov() << "\n";

			return scene;
		}
	}
}