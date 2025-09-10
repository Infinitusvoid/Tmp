#pragma once

#include <string>
#include <functional>

namespace Scene_
{
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

		Uniforms uniforms_start;
		Uniforms uniforms_end;

		Transform transform_start;
		Transform transform_end;

		int group_size_x = 0;
		int group_size_y = 0;
		int group_size_z = 0;
		int number_of_drawcalls = 1;
	};

	struct Shader
	{
		std::string fragment = "";
		std::string vertex = "";

		std::vector<Instance> instances;
	};

	struct Camera
	{
		float camera_x = 0;
		float camera_y = 0;
		float camera_z = 0;
		float fov = 45.0;
		float yew = 0.0;
		float pitch = 0.0;
	};

	struct LE_settings
	{
		float halfLife = 0.7f * 0.02f;
		float bloomThreshold = 1.4f;
		float bloomSoftKnee = 0.7f;
		float bloomStrength = 0.5f * 4.0;
		float bloomIterations = 6;
		float exposure = 0.7f * 2.0f;
		float gamma = 2.2f;
		float brightness = 0.0f;
		float contrast = 1.0f;
		float saturation = 1.0f;

		int msaaSamples = 4;
	};

	struct CapturingSettings
	{
		bool capture_frames = false;
		bool capture_png = true;
		bool cpature_bmp = false;
	};

	struct Callbacks
	{
		std::function<bool(std::string)> f_valid_file = nullptr;
		std::function<bool(std::string filepath_vertex_shader, std::string filepath_fragment_shader)> f_valid_shader;
	};
	
	struct Scene
	{
		int width = 1920 * 2;
		int height = 1080 * 2;

		CapturingSettings capture_settings;

		LE_settings le_settings;

		Camera camera_start;
		Camera camera_end;

		std::vector<Shader> shaders;
	};
	
	struct Report
	{
		std::vector<std::string> warns;
	};

	Scene init_default(); // Create a safe default scene

	// Load from TOML
	// adds warnings to report
	// returns default scene if file missing or parse fails
	Scene load(std::string filepath, Callbacks& callbacks, Report& report); 

	// SAVE to TOML
	// return true on success
	// may emit warnings 
	// (e.g. missing shader file if you provided the checker )
	bool save(const Scene& scene, Callbacks& callbacks, std::string filepath, Report& report);
	
	void print(Scene& scene);
}

