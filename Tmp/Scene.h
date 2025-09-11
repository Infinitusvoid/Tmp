#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <iosfwd>
#include <optional>
#include <iostream>


namespace Scene_
{


	struct Scene
	{
	private:
		struct Transform
		{
			float position_x = 0.0f;
			float position_y = 0.0f;
			float position_z = 0.0f;
			float euler_x = 0.0f;
			float euler_y = 0.0f;
			float euler_z = 0.0f;
			float scale_x = 1.0f;
			float scale_y = 1.0f;
			float scale_z = 1.0f;
		};

		struct Instance
		{
			struct Uniforms
			{
				float u0 = 0.0f;
				float u1 = 0.0f;
				float u2 = 0.0f;
				float u3 = 0.0f;
				float u4 = 0.0f;
				float u5 = 0.0f;
				float u6 = 0.0f;
				float u7 = 0.0f;
				float u8 = 0.0f;
				float u9 = 0.0f;
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
		[[nodiscard]] int get_width() const;
		[[nodiscard]] int get_height() const;

		// ----------------- Render timing -----------------
		void  set_render_fps(int number);
		[[nodiscard]] int   get_render_fps() const;

		void  set_number_of_frames(int number);
		[[nodiscard]] int   get_number_of_frames() const;

		void  set_render_time_start(float v);
		[[nodiscard]] float get_render_time_start() const;

		// ----------------- Capture toggles -----------------
		void set_capture(bool v);
		[[nodiscard]] bool get_capture() const;

		void set_capture_png(bool v);
		[[nodiscard]] bool get_capture_png() const;

		void set_capture_bmp(bool v);
		[[nodiscard]] bool get_capture_bmp() const;


		// ----------------- LE (post) settings -----------------
		void  set_le_halfLife(float v);
		[[nodiscard]] float get_le_halfLife() const;

		void  set_le_bloomThreshold(float v);
		[[nodiscard]] float get_le_bloomThreshold() const;

		void  set_le_bloomSoftKnee(float v);
		[[nodiscard]] float get_le_bloomSoftKnee() const;

		void  set_le_bloomStrength(float v);
		[[nodiscard]] float get_le_bloomStrength() const;

		void  set_le_bloomIterations(int v);
		[[nodiscard]] int   get_le_bloomIterations() const;

		void  set_le_exposure(float v);
		[[nodiscard]] float get_le_exposure() const;

		void  set_le_gamma(float v);
		[[nodiscard]] float get_le_gamma() const;

		void  set_le_brightness(float v);
		[[nodiscard]] float get_le_brightness() const;

		void  set_le_contrast(float v);
		[[nodiscard]] float get_le_contrast() const;

		void  set_le_saturation(float v);
		[[nodiscard]] float get_le_saturation() const;

		void  set_le_msaaSamples(int v);
		[[nodiscard]] int   get_le_msaaSamples() const;

		// ----------------- Camera start -----------------
		void  set_camera_start_x(float v);
		[[nodiscard]] float get_camera_start_x() const;

		void  set_camera_start_y(float v);
		[[nodiscard]] float get_camera_start_y() const;

		void  set_camera_start_z(float v);
		[[nodiscard]] float get_camera_start_z() const;

		void  set_camera_start_pitch(float v);
		[[nodiscard]] float get_camera_start_pitch() const;

		// yaw
		void  set_camera_start_yaw(float v);
		[[nodiscard]] float get_camera_start_yaw() const;

		void  set_camera_start_fov(float v);
		[[nodiscard]] float get_camera_start_fov() const;

		// ----------------- Camera end -----------------
		void  set_camera_end_x(float v);
		[[nodiscard]] float get_camera_end_x() const;

		void  set_camera_end_y(float v);
		[[nodiscard]] float get_camera_end_y() const;

		void  set_camera_end_z(float v);
		[[nodiscard]] float get_camera_end_z() const;

		void  set_camera_end_pitch(float v);
		[[nodiscard]] float get_camera_end_pitch() const;

		void  set_camera_end_yaw(float v);
		[[nodiscard]] float get_camera_end_yaw() const;

		void  set_camera_end_fov(float v);
		[[nodiscard]] float get_camera_end_fov() const;


		// ----------------- Shaders: container-level -----------------
		[[nodiscard]] size_t shader_count() const;
		[[nodiscard]] size_t add_shader(const char* vertex_path, const char* fragment_path);
		// overloads for std::string if you prefer
		[[nodiscard]] size_t add_shader(const std::string& vertex_path, const std::string& fragment_path);
		[[nodiscard]] bool remove_shader(size_t shader_idx);
		void clear_shaders();
		[[nodiscard]] bool set_shader_vertex_path(size_t shader_idx, const char* path);
		[[nodiscard]] bool set_shader_fragment_path(size_t shader_idx, const char* path);
		[[nodiscard]] std::string get_shader_vertex_path(size_t shader_idx) const;
		[[nodiscard]] std::string get_shader_fragment_path(size_t shader_idx) const;

		// ----------------- Instances per shader -----------------
		[[nodiscard]] size_t instance_count(size_t shader_idx) const;
		
		// ONLY built-ins: create default instance and return its index
		[[nodiscard]] size_t add_instance(size_t shader_idx);

		[[nodiscard]] bool remove_instance(size_t shader_idx, size_t instance_idx);

		// -------- Instance: groups & drawcalls --------
		[[nodiscard]] bool set_instance_group_size(size_t s, size_t i, int gx, int gy, int gz);
		[[nodiscard]] bool get_instance_group_size(size_t s, size_t i, int& gx, int& gy, int& gz) const;
		[[nodiscard]] bool set_instance_drawcalls(size_t s, size_t i, int n);
		[[nodiscard]] bool get_instance_drawcalls(size_t s, size_t i, int& n) const;

		// -------- Instance: transforms (start) --------
		[[nodiscard]] bool set_instance_position_start(size_t s, size_t i, float x, float y, float z);
		[[nodiscard]] bool set_instance_euler_start(size_t s, size_t i, float ex, float ey, float ez);
		[[nodiscard]] bool set_instance_scale_start(size_t s, size_t i, float sx, float sy, float sz);
		[[nodiscard]] bool get_instance_position_start(size_t s, size_t i, float& x, float& y, float& z) const;
		[[nodiscard]] bool get_instance_euler_start(size_t s, size_t i, float& ex, float& ey, float& ez) const;
		[[nodiscard]] bool get_instance_scale_start(size_t s, size_t i, float& sx, float& sy, float& sz) const;

		// -------- Instance: transforms (end) --------
		[[nodiscard]] bool set_instance_position_end(size_t s, size_t i, float x, float y, float z);
		[[nodiscard]] bool set_instance_euler_end(size_t s, size_t i, float ex, float ey, float ez);
		[[nodiscard]] bool set_instance_scale_end(size_t s, size_t i, float sx, float sy, float sz);
		[[nodiscard]] bool get_instance_position_end(size_t s, size_t i, float& x, float& y, float& z) const;
		[[nodiscard]] bool get_instance_euler_end(size_t s, size_t i, float& ex, float& ey, float& ez) const;
		[[nodiscard]] bool get_instance_scale_end(size_t s, size_t i, float& sx, float& sy, float& sz) const;

		// -------- Instance: uniforms (start/end, u0..u9 by index) --------
		[[nodiscard]] bool set_instance_uniform_start(size_t s, size_t i, int u_index, float value);
		[[nodiscard]] bool set_instance_uniform_end(size_t s, size_t i, int u_index, float value);
		[[nodiscard]] bool get_instance_uniform_start(size_t s, size_t i, int u_index, float& out_value) const;
		[[nodiscard]] bool get_instance_uniform_end(size_t s, size_t i, int u_index, float& out_value) const;

		// ----
		[[nodiscard]] bool clear_instances(size_t shader_idx);
		[[nodiscard]] bool has_shader(size_t shader_idx) const;

		// --- Image/output size: explicit setters/getters ---
		void set_width(int w);
		void set_height(int h);
		void print(std::ostream& os = std::cout) const;



		

	private:
		// --------------- Helpers ---------------
		static bool set_uniform_by_index(Instance::Uniforms& u, int idx, float v);
		static float get_uniform_by_index(const Instance::Uniforms& u, int idx);
		[[nodiscard]] bool valid_shader_index(size_t s) const;
		[[nodiscard]] bool valid_instance_index(size_t s, size_t i) const;



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

	[[nodiscard]] std::optional<Scene> load(const std::string& filepath);
	void save(const Scene& scene, const std::string& filepath);
}


namespace Scene_
{
	void unit_test_with_round_trip();
}


namespace Scene_
{
	Scene_::Scene load(int argc, char* argv[]);
}