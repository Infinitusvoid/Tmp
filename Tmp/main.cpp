#include "CppCommponents/Folder.h"
#include "CppCommponents/File.h"

#include "Utils.h"

#include <assert.h>


#include <iostream>

#include "Scene.h"

const std::string folder_output = "C:/Users/Cosmos/Desktop/output/tmp/";
const std::string folder_output_shaders = folder_output + "shaders/";
const std::string folder_output_frames = folder_output + "frames/";
const std::string folder_output_commands = folder_output + "commands/";


struct ShadersFilepath
{
public:
	// No instances needed; call statically.
	static std::string vertex(int index) {
		return base + "vertex_" + std::to_string(index) + ".glsl";
	}

	static std::string fragment(int index) {
		return base + "fragment_" + std::to_string(index) + ".glsl";
	}

private:
	// Compile-time base folder (note the trailing backslash).
	inline static const std::string base = R"(C:\Users\Cosmos\Documents\GitHub\Tmp\Tmp\)";

};

ShadersFilepath shaders;

struct Video
{
	static int generate()
	{
		FFmpeg_::FfmpegImageToVideo job;
		job.input_dir = folder_output_frames; // "C:/renders/shot01";     // folder with frame_000000.png etc.
		job.input_pattern = "frame_%06d.png";        // change if your naming differs
		job.start_number = 0;
		job.input_fps = 60;
		job.output_fps = 60;
		job.preset = "slow";
		job.crf = 14;
		job.pix_fmt = "yuv420p";
		job.faststart = true;
		job.output_path = folder_output_frames + std::string("output.mp4"); //"C:/renders/shot01/output.mp4";

		return job.run();
	}
};












struct Program
{

	struct RenderDisplay
	{
		int width = 1920 * 2;
		int height = 1080 * 2;
		int render_fps = 60;
		int number_of_frames = 100;
		float render_time_start = 0.0;
	};

	struct Capture
	{
		bool capture = false;
		bool capture_png = true;
		bool capture_bmp = false;
	};

	struct Camera
	{

		void init()
		{
			// CAPTURED: { pos: [0.187691, -0.043775, 0.381654] , yaw : -24.120005, pitch : 5.879999, fov : 45.000000 }

			x = 0.187691;
			y = -0.043775;
			z = 0.381654;
			yaw = -24.120005;
			pitch = 5.879999;
			fov = 45.000000;
		}


		void init2()
		{
			// CAPTURED: { pos: [2.695665, -0.675831, 5.983052] , yaw : -24.120005, pitch : 5.879999, fov : 45.000000 }

			x = 2.695665;
			y = -0.675831;
			z = 5.983052;
			yaw = -24.120005;
			pitch = 5.879999;
			fov = 45.000000;
		}

		void init3()
		{
			// CAPTURED: { pos:[-1.245056,-2.551750,19.370687], yaw:5.759995, pitch:4.920000, fov:45.000000 }

			x = -1.245056;
			y = -2.551750;
			z = 19.370687;
			yaw = 5.759995;
			pitch = 4.920000;
			fov = 45.000000;
		}

		float x = 0;
		float y = 0;
		float z = 0;
		float yaw = 0;
		float pitch = 0;
		float fov = 45;

	};

	struct LE
	{
		float halfLife = 0.2f;
		float bloomThreshold = 1.1f;
		float bloomSoftKnee = 0.7f;
		float bloomStrength = 2.0f;
		int bloomIterations = 6;
		float exposure = 0.3f;
		float gamma = 2.2f;
		float brightness = 0.0f;
		float contrast = 1.0f;
		float saturation = 1.0f;

		int msaaSamples = 4;
	};

	struct Shader
	{
		using ShaderId = std::size_t;
		using InstanceId = std::size_t;

		explicit Shader(Scene_::Scene& scene, int source_index)
			: scene_{ &scene }
			, shader_id_{ scene.add_shader(ShadersFilepath::vertex(source_index),
										   ShadersFilepath::fragment(source_index)) }
		{
			assert(scene_->has_shader(shader_id_));
		}

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&&) = default;
		Shader& operator=(Shader&&) = default;

		// ---- Thin, stateless handle that carries BOTH IDs -------------------------
		class Instance {
		public:
			Instance(Scene_::Scene& scene, ShaderId sh, InstanceId inst)
				: scene_{ &scene }, shader_id_{ sh }, id_{ inst } {
			}

			[[nodiscard]] InstanceId id() const noexcept { return id_; }
			[[nodiscard]] ShaderId   shader_id() const noexcept { return shader_id_; }

			const Instance& set_drawcalls(int num) const
			{
				scene_->set_instance_drawcalls(shader_id_, id_, num);
				return *this;
			}

			// Chainable const setters (mutate Scene via both ids)
			const Instance& set_group_size(int x, int y, int z) const {
				scene_->set_instance_group_size(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_position_start(float x, float y, float z) const {
				scene_->set_instance_position_start(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_position_end(float x, float y, float z) const {
				scene_->set_instance_position_end(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_euler_start(float x, float y, float z) const {
				scene_->set_instance_euler_start(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_euler_end(float x, float y, float z) const {
				scene_->set_instance_euler_end(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_scale_start(float x, float y, float z) const {
				scene_->set_instance_scale_start(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_scale_end(float x, float y, float z) const {
				scene_->set_instance_scale_end(shader_id_, id_, x, y, z);
				return *this;
			}
			const Instance& set_u_start_end(int idx, float u_start, float u_end) const
			{
				// adjust the bound if you expose more than 10 uniforms
				static constexpr int kMaxUniforms = 10;
				assert(idx >= 0 && idx < kMaxUniforms);
				scene_->set_instance_uniform_start(shader_id_, id_, idx, u_start);
				scene_->set_instance_uniform_end(shader_id_, id_, idx, u_end);
				return *this;
			}

		private:
			Scene_::Scene* scene_;
			ShaderId       shader_id_;
			InstanceId     id_;
		};
		// ---------------------------------------------------------------------------

		// Create -> returns the new instance id (sequential, never deleted)
		[[nodiscard]] InstanceId create_instance() {
			return scene_->add_instance(shader_id_);
		}

		// Cheap by-value handle (no lifetime issues)
		[[nodiscard]] Instance instance(InstanceId id) const {
			// Optional: assert(Scene has this (shader_id_, id)) if you expose it
			return Instance(*scene_, shader_id_, id);
		}

		[[nodiscard]] ShaderId id() const noexcept { return shader_id_; }

	private:
		Scene_::Scene* scene_;
		const ShaderId  shader_id_;
	};



	// ---- 

	RenderDisplay render_display;
	Capture capture;

	Camera camera_start;
	Camera camera_end;

	LE le;

	

	void write_commands_using_scene(Scene_::Scene& scene, const std::string& filepath) const
	{
		
		// ----- render / display settings -----
		scene.set_width(render_display.width);
		scene.set_height(render_display.height);
		scene.set_render_fps(render_display.render_fps);
		scene.set_number_of_frames(render_display.number_of_frames);
		scene.set_render_time_start(render_display.render_time_start);

		// ----- capture -----
		scene.set_capture(capture.capture);
		scene.set_capture_png(capture.capture_png);
		scene.set_capture_bmp(capture.capture_bmp);

		// ----- camera start -----
		scene.set_camera_start_x(camera_start.x);
		scene.set_camera_start_y(camera_start.y);
		scene.set_camera_start_z(camera_start.z);
		scene.set_camera_start_pitch(camera_start.pitch);
		scene.set_camera_start_yaw(camera_start.yaw);
		scene.set_camera_start_fov(camera_start.fov);

		// ----- camera end -----
		scene.set_camera_end_x(camera_end.x);
		scene.set_camera_end_y(camera_end.y);
		scene.set_camera_end_z(camera_end.z);
		scene.set_camera_end_pitch(camera_end.pitch);
		scene.set_camera_end_yaw(camera_end.yaw);
		scene.set_camera_end_fov(camera_end.fov);

		// ----- LE settings -----
		scene.set_le_halfLife(le.halfLife);
		scene.set_le_bloomThreshold(le.bloomThreshold);
		scene.set_le_bloomSoftKnee(le.bloomSoftKnee);
		scene.set_le_bloomStrength(le.bloomStrength);
		scene.set_le_bloomIterations(le.bloomIterations);
		scene.set_le_exposure(le.exposure);
		scene.set_le_gamma(le.gamma);
		scene.set_le_brightness(le.brightness);
		scene.set_le_contrast(le.contrast);
		scene.set_le_saturation(le.saturation);
		scene.set_le_msaaSamples(le.msaaSamples);


		// ----- Shader 0 -----
		{
			Program::Shader sh(scene, 1);
			
			auto inst_id = sh.create_instance();
			sh.instance(inst_id)
				.set_group_size(1000, 1000, 1)
				.set_drawcalls(4)
				.set_position_start(0.0f, 0.0f, 0.0f)
				.set_position_end(0.0f, 0.0f, 0.0f)
				.set_euler_start(0.0f, 0.0f, 0.0f)
				.set_euler_end(0.0f, 0.0f, 0.0f)
				.set_scale_start(1.0f, 1.0f, 1.0f)
				.set_scale_end(1.0f, 1.0f, 1.0f);

			// start uniforms (u0..u9) = 0.0, 0.1, ..., 0.9
			constexpr int kU = 10;
			for (int u = 0; u < kU; ++u)
			{
				const float v_start = 0.1f * static_cast<float>(u);
				const float v_end = 1.0f - 0.1f * static_cast<float>(u);
				sh.instance(inst_id).set_u_start_end(u, v_start, v_end);
			}
		}


		// optional: print to verify
		// scene.print();
		Scene_::save(scene, filepath);

	}
};




int main(int argc, char* argv[])
{
	std::cout << "Tmp\n";

	Folder::create_folder_if_does_not_exist_already(folder_output);
	Folder::create_folder_if_does_not_exist_already(folder_output_shaders);
	Folder::create_folder_if_does_not_exist_already(folder_output_frames);
	Folder::create_folder_if_does_not_exist_already(folder_output_commands);

	// write_commands();
	// write_commands_using_scene(camera_start, camera_end);

	Scene_::Scene scene = Scene_::Scene();

	std::string filepath = folder_output_commands + "commands.txt";
	{
		Program program;
		program.write_commands_using_scene(scene, filepath);
	}
	


	if (true)
	{
		// Example: run "C:\Tools\my app\tool.exe" --mode test "C:\path with spaces\file.txt"
		std::string cmd = std::string("LightPainting.exe") + std::string(" ") + folder_output_commands + std::string("commands.txt") + std::string(" ") + std::string(folder_output_frames);
		// R"("LightPainting.exe"";      // quote the exe path
		// " " + folder_output_commands + "commands.txt)";
		// R"("C:\path with spaces\file.txt")"; // quote args with spaces

		int exitCode = std::system(cmd.c_str());
	}


	{
		// Video::generate();
	}


	return 0;
}

