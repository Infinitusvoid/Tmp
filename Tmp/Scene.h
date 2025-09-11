#pragma once

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

namespace Scene_
{


	struct Scene
	{
	private:
		struct Transform
		{
			float position_x = 0.0;
			float position_y = 0.0;
			float position_z = 0.0;
			float euler_x = 0.0;
			float euler_y = 0.0;
			float euler_z = 0.0;
			float scale_x = 1.0;
			float scale_y = 1.0;
			float scale_z = 1.0;
		};

		struct Instance
		{
			struct Uniforms
			{
				float u0 = 0.0;
				float u1 = 0.0;
				float u2 = 0.0;
				float u3 = 0.0;
				float u4 = 0.0;
				float u5 = 0.0;
				float u6 = 0.0;
				float u7 = 0.0;
				float u8 = 0.0;
				float u9 = 0.0;
			};

			Uniforms  uniforms_start;
			Uniforms  uniforms_end;

			Transform transform_start;
			Transform transform_end;

			int group_size_x = 1000;
			int group_size_y = 1000;
			int group_size_z = 1;

			int number_of_drawcalls = 4;
		};

		struct Shader
		{
			std::string fragment = "";
			std::string vertex = "";
			std::vector<Instance> instances;
		};

		struct Camera
		{
			float camera_x = 0.0f;
			float camera_y = 0.0f;
			float camera_z = 0.0f;
			float fov = 45.0f;
			float yaw = 0.0f;
			float pitch = 0.0f;
		};

		struct LE_settings
		{
			float halfLife = 0.7f * 0.02f;
			float bloomThreshold = 1.4f;
			float bloomSoftKnee = 0.7f;
			float bloomStrength = 0.5f * 4.0f;
			int   bloomIterations = 6;
			float exposure = 0.7f * 2.0f;
			float gamma = 2.2f;
			float brightness = 0.0f;
			float contrast = 1.0f;
			float saturation = 1.0f;

			int   msaaSamples = 10;
		};

		struct CapturingSettings
		{
			bool capture_frames = false;
			bool capture_png = false;
			bool capture_bmp = false;

			float start_time = 0.0f;
		};

	public:

		static constexpr size_t kInvalidIndex = static_cast<size_t>(-1);

		// ----------------- Render / output size -----------------
		int   get_width() const;
		int   get_height() const;

		// ----------------- Render timing -----------------
		void  set_render_fps(int number);
		int   get_render_fps() const;

		void  set_number_of_frames(int number);
		int   get_number_of_frames() const;

		void  set_render_time_start(float v);
		float get_render_time_start() const;

		// ----------------- Capture toggles -----------------
		void set_capture(bool v);
		bool get_capture() const;

		void set_capture_png(bool v);
		bool get_capture_png() const;

		void set_capture_bmp(bool v);
		bool get_capture_bmp() const;


		// ----------------- LE (post) settings -----------------
		void  set_le_halfLife(float v);
		float get_le_halfLife() const;

		void  set_le_bloomThreshold(float v);
		float get_le_bloomThreshold() const;

		void  set_le_bloomSoftKnee(float v);
		float get_le_bloomSoftKnee() const;

		void  set_le_bloomStrength(float v);
		float get_le_bloomStrength() const;

		void  set_le_bloomIterations(int v);
		int   get_le_bloomIterations() const;

		void  set_le_exposure(float v);
		float get_le_exposure() const;

		void  set_le_gamma(float v);
		float get_le_gamma() const;

		void  set_le_brightness(float v);
		float get_le_brightness() const;

		void  set_le_contrast(float v);
		float get_le_contrast() const;

		void  set_le_saturation(float v);
		float get_le_saturation() const;

		void  set_le_msaaSamples(int v);
		int   get_le_msaaSamples() const;

		// ----------------- Camera start -----------------
		void  set_camera_start_x(float v);
		float get_camera_start_x() const;

		void  set_camera_start_y(float v);
		float get_camera_start_y() const;

		void  set_camera_start_z(float v);
		float get_camera_start_z() const;

		void  set_camera_start_pitch(float v);
		float get_camera_start_pitch() const;

		// yaw
		void  set_camera_start_yaw(float v);
		float get_camera_start_yaw() const;

		void  set_camera_start_fov(float v);
		float get_camera_start_fov() const;

		// ----------------- Camera end -----------------
		void  set_camera_end_x(float v);
		float get_camera_end_x() const;

		void  set_camera_end_y(float v);
		float get_camera_end_y() const;

		void  set_camera_end_z(float v);
		float get_camera_end_z() const;

		void  set_camera_end_pitch(float v);
		float get_camera_end_pitch() const;

		void  set_camera_end_yaw(float v);
		float get_camera_end_yaw() const;

		void  set_camera_end_fov(float v);
		float get_camera_end_fov() const;


		// ----------------- Shaders: container-level -----------------
		size_t shader_count() const;
		size_t add_shader(const char* vertex_path, const char* fragment_path);
		// overloads for std::string if you prefer
		size_t add_shader(const std::string& vertex_path, const std::string& fragment_path);
		bool remove_shader(size_t shader_idx);
		void clear_shaders();
		bool set_shader_vertex_path(size_t shader_idx, const char* path);
		bool set_shader_fragment_path(size_t shader_idx, const char* path);
		std::string get_shader_vertex_path(size_t shader_idx) const;
		std::string get_shader_fragment_path(size_t shader_idx) const;

		// ----------------- Instances per shader -----------------
		size_t instance_count(size_t shader_idx) const;
		
		// ONLY built-ins: create default instance and return its index
		size_t add_instance(size_t shader_idx);

		bool remove_instance(size_t shader_idx, size_t instance_idx);

		// -------- Instance: groups & drawcalls --------
		bool set_instance_group_size(size_t s, size_t i, int gx, int gy, int gz);
		bool get_instance_group_size(size_t s, size_t i, int& gx, int& gy, int& gz) const;
		bool set_instance_drawcalls(size_t s, size_t i, int n);
		bool get_instance_drawcalls(size_t s, size_t i, int& n) const;

		// -------- Instance: transforms (start) --------
		bool set_instance_position_start(size_t s, size_t i, float x, float y, float z);
		bool set_instance_euler_start(size_t s, size_t i, float ex, float ey, float ez);
		bool set_instance_scale_start(size_t s, size_t i, float sx, float sy, float sz);
		bool get_instance_position_start(size_t s, size_t i, float& x, float& y, float& z) const;
		bool get_instance_euler_start(size_t s, size_t i, float& ex, float& ey, float& ez) const;
		bool get_instance_scale_start(size_t s, size_t i, float& sx, float& sy, float& sz) const;

		// -------- Instance: transforms (end) --------
		bool set_instance_position_end(size_t s, size_t i, float x, float y, float z);
		bool set_instance_euler_end(size_t s, size_t i, float ex, float ey, float ez);
		bool set_instance_scale_end(size_t s, size_t i, float sx, float sy, float sz);
		bool get_instance_position_end(size_t s, size_t i, float& x, float& y, float& z) const;
		bool get_instance_euler_end(size_t s, size_t i, float& ex, float& ey, float& ez) const;
		bool get_instance_scale_end(size_t s, size_t i, float& sx, float& sy, float& sz) const;

		// -------- Instance: uniforms (start/end, u0..u9 by index) --------
		bool set_instance_uniform_start(size_t s, size_t i, int u_index, float value);
		bool set_instance_uniform_end(size_t s, size_t i, int u_index, float value);
		bool get_instance_uniform_start(size_t s, size_t i, int u_index, float& out_value) const;
		bool get_instance_uniform_end(size_t s, size_t i, int u_index, float& out_value) const;

		// --- Image/output size: explicit setters/getters ---
		void set_width(int w);
		void set_height(int h);
		void print(std::ostream& os = std::cout) const;
	private:
		// --------------- Helpers ---------------
		static bool set_uniform_by_index(Instance::Uniforms& u, int idx, float v);
		static float get_uniform_by_index(const Instance::Uniforms& u, int idx);
		bool valid_shader_index(size_t s) const;
		bool valid_instance_index(size_t s, size_t i) const;



	private:
		int width = 1920 * 2;
		int height = 1080 * 2;

		int render_fps = 60;
		int number_of_frames = 100;

		CapturingSettings capture_settings;
		LE_settings       le_settings;

		Camera camera_start;
		Camera camera_end;

		std::vector<Shader> shaders;
	};

}




namespace Scene_ {

	inline std::optional<Scene> load(const std::string& filepath);
	inline void save(const Scene& scene, const std::string& filepath);
}


namespace Scene_
{
	void unit_test_with_round_trip();
}