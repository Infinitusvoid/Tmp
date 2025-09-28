// universe_2.h — auto-generated stub
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

#include "../CppCommponents/Random.h"




namespace Universe_
{
	struct XYZ
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	struct Float_start_end
	{
		float start = 0;
		float end = 0;
	};

	struct Sphere
	{
		XYZ start_position = { 0.0, 0.0, 0.0 };
		XYZ end_position = { 0.0, 0.0, 0.0 };

		XYZ start_color = { 0.0, 0.0, 0.0 };
		XYZ end_color = { 0.0, 0.0, 0.0 };

		Float_start_end cube_size = { 0.0001, 0001 };
		Float_start_end radious = {0.0, 0.0};
		Float_start_end x_rnd_min = { 0.0, 0.0 };
		Float_start_end x_rnd_max = { 0.0, 0.0 };
		Float_start_end y_rnd_min = { 0.0, 0.0 };
		Float_start_end y_rnd_max = { 0.0, 0.0 };
		Float_start_end thickness = { 0.0, 0.0 };
		Float_start_end jitter = { 0.0, 0.0 };


	};

	struct Spheres
	{
		std::vector<Sphere> spheres;
		

		void init_0(int number)
		{
			for(int i = 0; i < number; i++)
			{
				Sphere sphere;

				sphere.start_position = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };
				sphere.end_position = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };

				sphere.start_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };
				sphere.end_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };

				sphere.radious = { 0.02f + Random::generate_random_float_0_to_1() * 0.02f, 0.02f + Random::generate_random_float_0_to_1() * 0.02f };
				
				sphere.cube_size = { 0.001, 0.001 };

				sphere.x_rnd_min = { 0.0, 0.0 };
				sphere.x_rnd_max = { 1.0, 1.0 };

				sphere.y_rnd_min = { 0.0, 0.0 };
				sphere.y_rnd_max = { 1.0, 1.0 };

				sphere.thickness = { 0.0, 0.0 };
				
				sphere.jitter.start = 1.0f;
				sphere.jitter.end = 1.0f;
				
				spheres.push_back(std::move(sphere));
			}
		}

		

		
		void init_1(int number)
		{
			for (int i = 0; i < number; i++)
			{
				Sphere sphere;


				float factor_i = (1.0f / float(number)) * i;
				

				sphere.start_position = { factor_i * 10.0f, 0.0f, Random::generate_random_float_minus_one_to_plus_one() * 2.0f};
				sphere.end_position = 
				{
					sphere.start_position.x,
					sphere.start_position.y + 2.0f,
					sphere.start_position.z
				};

				sphere.start_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };
				sphere.end_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };

				sphere.radious = { 0.02f + Random::generate_random_float_0_to_1() * 0.02f, 0.02f + Random::generate_random_float_0_to_1() * 0.02f };

				sphere.cube_size = { 0.01, 0.01 };

				sphere.x_rnd_min = { 0.0, 0.0 };
				sphere.x_rnd_max = { 1.0, 1.0 };

				sphere.y_rnd_min = { 0.0, 0.0 };
				sphere.y_rnd_max = { 1.0, 1.0 };

				sphere.thickness = { 0.1, 0.1 };

				sphere.jitter.start = 1.0f;
				sphere.jitter.end = 1.0f;

				spheres.push_back(std::move(sphere));
			}
		}

		




		


		void draw(Scene_::Scene& scene, int number_of_cube_per_sphere)
		{
			const int sqrt_number_of_cube_per_sphere = int(sqrtf(float(number_of_cube_per_sphere)));

			add_shader(scene, 20, [&](Program::Shader& sh) {


				// Instance 0
				


				for(Sphere& sphere : spheres)
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(sqrt_number_of_cube_per_sphere, sqrt_number_of_cube_per_sphere, 1)
						.set_drawcalls(1)
						.set_position_start(sphere.start_position.x, sphere.start_position.y, sphere.start_position.z)
						.set_position_end(sphere.end_position.x, sphere.end_position.y, sphere.end_position.z)
						.set_euler_start(0.0f, 0.0f, 0.0f)
						.set_euler_end(0.0f, 0.0f, 0.0f)
						.set_scale_start(sphere.radious.start, sphere.radious.start, sphere.radious.start)
						.set_scale_end(sphere.radious.end, sphere.radious.end, sphere.radious.end);


					I.set_u_start_end(0, sphere.start_color.x, sphere.end_color.x); // u0 color_r
					I.set_u_start_end(1, sphere.start_color.y, sphere.end_color.y); // u1 color_g
					I.set_u_start_end(2, sphere.start_color.z, sphere.end_color.z); // u2 color_b

					I.set_u_start_end(3, sphere.cube_size.start, sphere.cube_size.end); // u3 cube_size
					I.set_u_start_end(4, sphere.x_rnd_min.start, sphere.x_rnd_min.end); // u4 rnd_x_min
					I.set_u_start_end(5, sphere.x_rnd_max.start, sphere.x_rnd_max.end); // u5 rnd_x_max

					I.set_u_start_end(6, sphere.y_rnd_min.start, sphere.y_rnd_min.end); // u6 rnd_y_min
					I.set_u_start_end(7, sphere.y_rnd_max.start, sphere.y_rnd_max.end); // u7 rnd_y_max
					I.set_u_start_end(8, sphere.thickness.start, sphere.thickness.end); // u8 thickness

					I.set_u_start_end(9, sphere.jitter.start, sphere.jitter.end); // u9 jitter scale

					//constexpr int kU = 10;
					//for (int u = 0; u < kU; ++u)
					//{
					//	float v_start = 0.1f * static_cast<float>(u);
					//	float v_end = 1.0f - 0.1f * static_cast<float>(u);

					//	v_start = 0.0;
					//	v_end = 0.0;

					//	if (u == 0) // color_r
					//	{
					//		v_start = 0.0;
					//		v_end = 1.0;
					//	}
					//	else if (u == 1) // color_g
					//	{
					//		v_start = 1.0;
					//		v_end = 1.0;
					//	}
					//	else if (u == 2) // color_b
					//	{
					//		v_start = 0.0;
					//		v_end = 1.0;
					//	}
					//	else if (u == 3) // cube_size
					//	{
					//		v_start = 0.0001;
					//		v_end = 0.0001;
					//	}
					//	else if (u == 4) // rnd_x_min
					//	{
					//		v_start = 0.0;
					//		v_end = 0.0;
					//	}
					//	else if (u == 5) // rnd_x_max
					//	{
					//		v_start = 0.5;
					//		v_end = 0.5;
					//	}
					//	else if (u == 6) // rnd_y_min
					//	{
					//		v_start = 0.0;
					//		v_end = 0.0;
					//	}
					//	else if (u == 7) // rnd_y_max
					//	{
					//		v_start = 0.5;
					//		v_end = 0.5;
					//	}
					//	else if (u == 8) // thickness
					//	{
					//		v_start = 0.1;
					//		v_end = 0.1;
					//	}
					//	else if (u == 9) // jitter scale
					//	{
					//		v_start = 0.0;
					//		v_end = 0.0;
					//	}

					//	I.set_u_start_end(u, v_start, v_end);
					//}

				}


			});
		}
	};

	struct Line
	{
		Float_start_end x0 = {0.0f, 0.0f}; // u0
		Float_start_end y0 = { 0.0f, 0.0f }; // u1
		Float_start_end z0 = { 0.0f, 0.0f }; // u2

		Float_start_end x1 = { 0.0f, 0.0f }; // u3
		Float_start_end y1 = { 0.0f, 0.0f }; // u4
		Float_start_end z1 = { 0.0f, 0.0f }; // u5

		XYZ rgb_t0 = {0.0f, 0.0f, 0.0f}; // u6, u7, u8 
		XYZ rgb_t1 = { 0.0f, 0.0f, 0.0f };

		Float_start_end thickness = { 0.0f, 0.0f }; // u9

		void set_x0(float value_at_t0, float value_at_t1)
		{

		}

		void set_y0(float value_at_t0, float value_at_t1)
		{

		}

		void set_z0(float value_at_t0, float value_at_t1)
		{

		}

		void set_x1(float value_at_t0, float value_at_t1)
		{

		}

		void set_y1(float value_at_t0, float value_at_t1)
		{

		}

		void set_z1(float value_at_t0, float value_at_t1)
		{

		}

		void set_rgb_at_t0(float r, float g, float b)
		{

		}

		void set_rgb_at_t1(float r, float g, float b)
		{

		}

		void set_thickness_at_t0(float value)
		{

		}

		void set_thickness_at_t1(float value)
		{

		}


	};

	struct Lines
	{
		std::vector<Line> lines;

		
	};



	struct Clip
	{
		

		void generate()
		{
			engine_flush_frames();
			engine_delete_flush_frames();

			Program program;
			program.le.halfLife = 0.02f * 4.0f;
			program.le.brightness = 0.0f;
			program.le.exposure = 1.0;
			program.le.msaaSamples = 10;

			program.capture.capture = false;
			program.capture.capture_png = false;
			program.capture.capture_bmp = true;

			program.render_display.render_fps = clip_fps;
			program.render_display.number_of_frames = program.render_display.render_fps * clip_length_seconds;
			program.render_display.render_time_start = 0.0;

			// CAPTURED: { pos:[-0.253910,0.518504,0.281625], yaw:103.439995, pitch:-1.920018, fov:45.000000 }
			// CAPTURED: { pos:[-0.253910,0.518504,0.281625], yaw:86.639862, pitch:9.599976, fov:45.000000 }
			program.camera_start.x = -0.253910;
			program.camera_start.y = 0.518504;
			program.camera_start.z = 0.281625;

			program.camera_start.yaw = 86.639862;
			program.camera_start.pitch = 9.599976;
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
			//if(false) //if (enable_shader_20)
			//{
			//	add_shader(scene, 20, [](Program::Shader& sh) {


			//		// Instance 0
			//		{
			//			auto id = sh.create_instance();
			//			auto I = sh.instance(id);
			//			I.set_group_size(1000, 1000, 1)
			//				.set_drawcalls(2)
			//				.set_position_start(0.0f, 0.0f, 0.0f)
			//				.set_position_end(0.0f, 0.0f, 0.0f)
			//				.set_euler_start(0.0f, 0.0f, 0.0f)
			//				.set_euler_end(0.0f, 0.0f, 0.0f)
			//				.set_scale_start(1.0f, 1.0f, 1.0f)
			//				.set_scale_end(1.0f, 1.0f, 1.0f);


			//			constexpr int kU = 10;
			//			for (int u = 0; u < kU; ++u)
			//			{
			//				float v_start = 0.1f * static_cast<float>(u);
			//				float v_end = 1.0f - 0.1f * static_cast<float>(u);

			//				v_start = 0.0;
			//				v_end = 0.0;

			//				if (u == 0) // color_r
			//				{
			//					v_start = 0.0;
			//					v_end = 1.0;
			//				}
			//				else if (u == 1) // color_g
			//				{
			//					v_start = 1.0;
			//					v_end = 1.0;
			//				}
			//				else if (u == 2) // color_b
			//				{
			//					v_start = 0.0;
			//					v_end = 1.0;
			//				}
			//				else if (u == 3) // cube_size
			//				{
			//					v_start = 0.0001;
			//					v_end = 0.0001;
			//				}
			//				else if (u == 4) // rnd_x_min
			//				{
			//					v_start = 0.0;
			//					v_end = 0.0;
			//				}
			//				else if (u == 5) // rnd_x_max
			//				{
			//					v_start = 0.5;
			//					v_end = 0.5;
			//				}
			//				else if (u == 6) // rnd_y_min
			//				{
			//					v_start = 0.0;
			//					v_end = 0.0;
			//				}
			//				else if (u == 7) // rnd_y_max
			//				{
			//					v_start = 0.5;
			//					v_end = 0.5;
			//				}
			//				else if (u == 8) // thickness
			//				{
			//					v_start = 0.1;
			//					v_end = 0.1;
			//				}
			//				else if (u == 9) // jitter scale
			//				{
			//					v_start = 0.0;
			//					v_end = 0.0;
			//				}

			//				I.set_u_start_end(u, v_start, v_end);
			//			}
			//			
			//		}


			//		});


			//}

			if(true)
			{
				Spheres sphere;
				sphere.init_1(10);
				sphere.draw(scene, 1000);

				
				
				
				
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
		const int clip_length_seconds = 4;

		const bool enable_shader_10_unit_cube = true;
		const bool enable_shader_20 = true;

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