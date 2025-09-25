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

#include "Universe_25_09_2025_13_56_utils.h"

namespace Universe_
{
	struct Clip
	{
		

		void generate()
		{
			engine_flush_frames();
			engine_delete_flush_frames();

			Program program;
			program.le.halfLife = 0.01f;
			program.le.brightness = 0.0f;
			program.le.exposure = 100.0;
			program.le.msaaSamples = 10;

			program.capture.capture = false;
			program.capture.capture_png = false;
			program.capture.capture_bmp = true;

			program.render_display.render_fps = clip_fps;
			program.render_display.number_of_frames = program.render_display.render_fps * clip_length_seconds;
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

			// shaders
			if (enable_shader_11)
			{
				add_shader(scene, 11, [](Program::Shader& sh) {


					// Instance 0
					{
						auto id = sh.create_instance();
						auto I = sh.instance(id);
						I.set_group_size(1000, 1000, 1)
							.set_drawcalls(400)
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

				int i = clip_number;
				std::string name = "output_" + std::to_string(i);
				Video::generate(name, "bmp");


			}

			{
				std::cout << "engine_flush_frames \n";
				engine_flush_frames();
				engine_delete_flush_frames();
			}

			clip_number += 1;
		}


		int clip_number = 0;


		const int clip_fps = 60;
		const int clip_length_seconds = 120;

		const bool enable_shader_10_unit_cube = false;
		const bool enable_shader_11 = true;

	};


	void generate()
	{
		Clip clip;

		// for(int i = 0; i < 30; i++)
		{
			std::cout << "clip : " << clip.clip_number << "\n";

			clip.generate();
		}
		
		
		
		

		

		

		
	}
	
}


int universe(int argc, char* argv[])
{
	std::cout << "Universe_25_09_2025_13_56\n";
	
	

	Universe_::generate();



	return 0;

}