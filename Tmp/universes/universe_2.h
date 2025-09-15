// universe_2.h — auto-generated stub
#pragma once


#include "Extension_Folder.h"

#include <cstdint>
#include <limits>

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

		static inline uint64_t rotr64(uint64_t x, unsigned r) {
			r &= 63u;
			return (x >> r) | (x << ((64u - r) & 63u));
		}

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

	


	struct CameraRecording
	{
		float x;
		float y;
		float z;
		float yaw;
		float pitch;
		float fov;

		int start_frame;
		int end_frame;
	};

	struct CameraPath
	{
		std::vector<CameraRecording> path;

		void load_last(Program& program)
		{
			CameraRecording recording = path.back();

			program.camera_start.x = recording.x;
			program.camera_start.y = recording.y;
			program.camera_start.z = recording.z;
			program.camera_start.yaw = recording.yaw;
			program.camera_start.pitch = recording.pitch;
			program.camera_start.fov = recording.fov;

			program.render_display.render_time_start = float(recording.end_frame) * (1.0 / 60.0f);
			
		}

		void set_index_for_rendering(int index, Program& program, const CameraPath& camera_path)
		{
			{
				const CameraRecording& start = camera_path.path.at(index);

				program.camera_start.x = start.x;
				program.camera_start.y = start.y;
				program.camera_start.z = start.z;
				program.camera_start.yaw = start.yaw;
				program.camera_start.pitch = start.pitch;
				program.camera_start.fov = start.fov;

				
			}

			{
				const CameraRecording& end = camera_path.path.at(index + 1);

				program.camera_end.x = end.x;
				program.camera_end.y = end.y;
				program.camera_end.z = end.z;
				program.camera_end.yaw = end.yaw;
				program.camera_end.pitch = end.pitch;
				program.camera_end.fov = end.fov;
			}
			
			
			program.render_display.number_of_frames = camera_path.path.at(index + 1).end_frame - camera_path.path.at(index).end_frame;
		}
	};

	
	void build_path(CameraPath& path)
	{
		{
			//CAPTURED: { pos: [73.614182, -21.104589, 42.984325] , yaw : -50.880009, pitch : 20.279987, fov : 45.000000 }

			CameraRecording recording;
			recording.x = 73.614182;
			recording.y = -21.104589;
			recording.z = 42.984325;
			recording.yaw = -50.880009;
			recording.pitch = 20.279987;
			recording.fov = 45.0;

			recording.start_frame = 60 * 4 * 3;
			recording.end_frame = 60 * 4 * 4;

			path.path.push_back(recording);
		}
	}

	void hand_still_camera(CameraPath& path)
	{
		CameraRecording recording = path.path.back();

		const float x = recording.x;
		const float y = recording.y;
		const float z = recording.z;
		
		float start_time = std::max(0, recording.end_frame - 1);

		RNG rng(42u, 7u); // deterministic across runs/platforms

		std::cout << "five floats [0,1):\n";
		for (int i = 0; i < 5; ++i)
			std::cout << rng.next_float() << "\n";

		std::cout << "\nfive doubles [0,1):\n";
		for (int i = 0; i < 5; ++i)
			std::cout << rng.next_double() << "\n";

		std::cout << "\nuniform doubles in [10,20):\n";
		for (int i = 0; i < 5; ++i)
			std::cout << rng.uniform(10.0, 20.0) << "\n";
		

	}
}

void flush_frames()
{
	Extension_Folder_::move_images_to_subfolder("C:/Users/Cosmos/Desktop/output/tmp/frames/", "C:/Users/Cosmos/Desktop/output/tmp/frames/old_frames/");
}

// Put your universe 2 content here.
// This file is safe to include via universes/universe.h
int universe(int argc, char* argv[])
{
	std::cout << "universe_2\n";
	

	

	Scene_::Scene scene = Scene_::Scene();

	const int section = 7;

	flush_frames();
	



	

	// configuration
	{
		Program program;

		program.le.halfLife = 0.02;
		// program.le.exposure = 4.0;
		program.le.brightness = 0.0;

		program.capture.capture = false;

		Universe_::CameraPath path;
		Universe_::build_path(path);

		path.load_last(program);

		{
			Universe_::RNG rng(42u, 7u); // deterministic across runs/platforms

			std::cout << "five floats [0,1):\n";
			for (int i = 0; i < 5; ++i)
				std::cout << rng.next_float() << "\n";

			std::cout << "\nfive doubles [0,1):\n";
			for (int i = 0; i < 5; ++i)
				std::cout << rng.next_double() << "\n";

			std::cout << "\nuniform doubles in [10,20):\n";
			for (int i = 0; i < 5; ++i)
				std::cout << rng.uniform(10.0, 20.0) << "\n";
		}

		//if(false) // 0
		{
			// CAPTURED: { pos: [-142.341278, 171.578140, 448.985260] , yaw : 80.639992, pitch : 19.440002, fov : 45.000000 }


			// CAPTURED: { pos: [-15.359919, -13.098754, 52.493931] , yaw : 20.400003, pitch : 16.679998, fov : 45.000000 }

			
		}

		// if (true) // 1
		{
			// CAPTURED: { pos: [-142.341278, 171.578140, 448.985260] , yaw : 80.639992, pitch : 19.440002, fov : 45.000000 }
			// CAPTURED: { pos: [56.266338, -12.129290, -12.411207] , yaw : -103.920052, pitch : 22.559982, fov : 45.000000 }

			



			// CAPTURED: { pos: [-15.359919, -13.098754, 52.493931] , yaw : 20.400003, pitch : 16.679998, fov : 45.000000 }

			program.camera_start.x = -15.359919f;
			program.camera_start.y = -13.098754f;
			program.camera_start.z = 52.493931f;
			program.camera_start.yaw = 20.400003f;
			program.camera_start.pitch = 16.679998f;
			program.camera_start.fov = 45.0f;

			program.camera_end.x = -15.359919f;
			program.camera_end.y = -13.098754f;
			program.camera_end.z = 52.493931f;
			program.camera_end.yaw = 20.400003f;
			program.camera_end.pitch = 16.679998f;
			program.camera_end.fov = 45.0f;

			
			program.render_display.number_of_frames = 240;
			program.render_display.render_time_start = ((1.0 / 60.0) * 240.0) * section;
			
		}
		

		// program.render_display.number_of_frames = 60 * 4;
		// program.render_display.render_time_start = 60.0 * 4.0 * float(int(1));
		

		program.configure(scene);
	}



	// shaders
	{
		add_shader(scene, 6, [](Program::Shader& sh) {


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


			// Instance 1
			
			{

				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(1)
					.set_position_start(10.0f, 0.0f, 0.0f)
					.set_position_end(10.0f, 0.0f, 0.0f)
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

				I.set_u_start_end(0, 4.237, 4.237);
			}


			});


		add_shader(scene, 7, [](Program::Shader& sh) {


			// Instance 0
			
			{
				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(1)
					.set_position_start(0.0f, 10.0f, 0.0f)
					.set_position_end(0.0f, 10.0f, 0.0f)
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

				I.set_u_start_end(0, 1.427, 1.427);
			}


			// Instance 1
			
			{

				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(1)
					.set_position_start(10.0f, 10.0f, 10.0f)
					.set_position_end(10.0f, 10.0f, 10.0f)
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

				I.set_u_start_end(0, 5.497, 5.497);

			}

			

			});
	}




	{
		// scene.print();
		std::string program_name = NameGenerators_::generate_prefix_timestamp_suffix_name();
		save_program(scene, program_name);
		run_program(program_name);

		std::string name = "output_" + std::to_string(section);
		// Video::generate(name);
		
		
	}



	return 0;
}