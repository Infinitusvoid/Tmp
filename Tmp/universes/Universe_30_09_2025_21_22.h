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
	struct Vec2
	{
		float x;
		float y;
	};

	struct Vec3
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	// (Add these functions before your 'struct Lines')

	namespace Vec3_
	{
		inline Vec3 vec_add(const Vec3& a, const Vec3& b) {
			return { a.x + b.x, a.y + b.y, a.z + b.z };
		}

		inline Vec3 vec_scale(const Vec3& v, float s) {
			return { v.x * s, v.y * s, v.z * s };
		}

		inline float vec_length(const Vec3& v) {
			return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		}

		inline Vec3 vec_normalize(const Vec3& v) {
			float len = vec_length(v);
			if (len > 0.0f) {
				return vec_scale(v, 1.0f / len);
			}
			return { 0.0f, 0.0f, 0.0f }; // Return zero vector if length is zero
		}
	}

	struct Float_start_end
	{
		float start = 0;
		float end = 0;
	};

	struct Sphere
	{
		Vec3 start_position = { 0.0, 0.0, 0.0 };
		Vec3 end_position = { 0.0, 0.0, 0.0 };

		Vec3 start_color = { 0.0, 0.0, 0.0 };
		Vec3 end_color = { 0.0, 0.0, 0.0 };

		Float_start_end cube_size = { 0.0001f, 0.0001f };
		Float_start_end radius = { 0.0, 0.0 };
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
			for (int i = 0; i < number; i++)
			{
				Sphere sphere;

				sphere.start_position = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };
				sphere.end_position = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };

				sphere.start_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };
				sphere.end_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };

				sphere.radius = { 0.02f + Random::generate_random_float_0_to_1() * 0.02f, 0.02f + Random::generate_random_float_0_to_1() * 0.02f };

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


				sphere.start_position = { factor_i * 10.0f, 0.0f, Random::generate_random_float_minus_one_to_plus_one() * 2.0f };
				sphere.end_position =
				{
					sphere.start_position.x,
					sphere.start_position.y + 2.0f,
					sphere.start_position.z
				};

				sphere.start_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };
				sphere.end_color = { Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1(), Random::generate_random_float_0_to_1() };

				sphere.radius = { 0.02f + Random::generate_random_float_0_to_1() * 0.02f, 0.02f + Random::generate_random_float_0_to_1() * 0.02f };

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


				for (Sphere& sphere : spheres)
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(sqrt_number_of_cube_per_sphere, sqrt_number_of_cube_per_sphere, 1)
						.set_drawcalls(1)
						.set_position_start(sphere.start_position.x, sphere.start_position.y, sphere.start_position.z)
						.set_position_end(sphere.end_position.x, sphere.end_position.y, sphere.end_position.z)
						.set_euler_start(0.0f, 0.0f, 0.0f)
						.set_euler_end(0.0f, 0.0f, 0.0f)
						.set_scale_start(sphere.radius.start, sphere.radius.start, sphere.radius.start)
						.set_scale_end(sphere.radius.end, sphere.radius.end, sphere.radius.end);


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
				}


				});
		}
	};

	struct Line
	{
		Float_start_end x0 = { 0.0f, 0.0f }; // u0
		Float_start_end y0 = { 0.0f, 0.0f }; // u1
		Float_start_end z0 = { 0.0f, 0.0f }; // u2

		Float_start_end x1 = { 0.0f, 0.0f }; // u3
		Float_start_end y1 = { 0.0f, 0.0f }; // u4
		Float_start_end z1 = { 0.0f, 0.0f }; // u5

		Vec3 rgb_t0 = { 0.0f, 0.0f, 0.0f }; // u6, u7, u8 
		Vec3 rgb_t1 = { 0.0f, 0.0f, 0.0f };

		Float_start_end thickness = { 0.0f, 0.0f }; // u9

		int number_of_cubes = 100;

		void send(Program::Shader::Instance& I)
		{
			I.set_u_start_end(0, x0.start, x0.end); // u0
			I.set_u_start_end(1, y0.start, y0.end); // u1
			I.set_u_start_end(2, z0.start, z0.end); // u2

			I.set_u_start_end(3, x1.start, x1.end); // u3
			I.set_u_start_end(4, y1.start, y1.end); // u4
			I.set_u_start_end(5, z1.start, z1.end); // u5

			I.set_u_start_end(6, rgb_t0.x, rgb_t1.x); // u6
			I.set_u_start_end(7, rgb_t0.y, rgb_t1.y); // u7
			I.set_u_start_end(8, rgb_t0.z, rgb_t1.z); // u8

			I.set_u_start_end(9, thickness.start, thickness.end); // u9

		}

		void copy_start_to_end()
		{
			x0.end = x0.start;
			y0.end = y0.start;
			z0.end = z0.start;

			x1.end = x1.start;
			y1.end = y1.start;
			z1.end = z1.start;

			rgb_t1.x = rgb_t0.x;
			rgb_t1.y = rgb_t0.y;
			rgb_t1.z = rgb_t0.z;

			thickness.end = thickness.start;
		}
	};

	struct Transform
	{
		Vec3 position;
		Vec3 euler;
		Vec3 scale;
	};

	struct Lines
	{
		std::vector<Line> lines;

		void init()
		{
			{
				int number_of_lines = 100;
				const float TAU = 6.2831853071795864769252867665590;
				float step_size = (1.0 / float(number_of_lines)) * TAU;

				for (int i = 0; i < number_of_lines; i++)
				{
					Line& line = add_line();


					line.x0.start = 0.0f;
					line.y0.start = 0.0f;
					line.z0.start = 0.0f;

					line.x1.start = 0.5f * sin(i * step_size);
					line.y1.start = 0.0f;
					line.z1.start = 0.5f * cos(i * step_size);

					line.rgb_t0.x = 0.2 * Random::generate_random_float_0_to_1();
					line.rgb_t0.y = 0.2 * Random::generate_random_float_0_to_1();
					line.rgb_t0.z = 0.2 * Random::generate_random_float_0_to_1();

					line.thickness.start = 0.01 * 0.2;
					line.number_of_cubes = 100;

					line.copy_start_to_end();

					

					line.x1.end = 0.5f * sin(i * step_size);
					line.y1.end = 0.0f;
					line.z1.end = 0.5f * cos(i * step_size);

					// line.y1.end = 1.0;
					// line.z1.end = 1.0;
				}

				

				
			}


		}

		void draw(Scene_::Scene& scene)
		{
			add_shader(scene, 21, [&](Program::Shader& sh) {


				for (int i = 0; i < lines.size(); i++)
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);

					int grup_size_x = lines.at(i).number_of_cubes;
					int grup_size_y = 1;
					int grop_size_z = 1;
					int drawcalls = 1;

					assert(drawcalls == 1); // This shader works only if per each line we only use 1 drawcall

					I.set_group_size(grup_size_x, grup_size_y, grop_size_z)
						.set_drawcalls(drawcalls)
						.set_position_start(0.0f, 0.0f, 0.0f)
						.set_position_end(0.0f, 0.0f, 0.0f)
						.set_euler_start(0.0f, 0.0f, 0.0f)
						.set_euler_end(0.0f, 0.0f, 0.0f)
						.set_scale_start(1.0f, 1.0f, 1.0f)
						.set_scale_end(1.0f, 1.0f, 1.0f);

					lines.at(i).send(I);
				}

				});
		}

		Line& add_line()
		{
			lines.emplace_back(Line());
			return lines.back();
		}
	};

	struct UnitCube
	{
		void init()
		{

		}

		void draw(Scene_::Scene& scene)
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
	};

	struct LineGeodesic
	{
		// Endpoints on the sphere (normalized)
		Float_start_end lon0{ 0.0f, 0.0f };  // u0
		Float_start_end lat0{ 0.0f, 0.0f };  // u1
		Float_start_end lon1{ 0.0f, 0.0f };  // u2
		Float_start_end lat1{ 0.0f, 0.0f };  // u3

		// Sphere radius
		Float_start_end radius{ 1.0f, 1.0f }; // u5
		
		
		// Color (start -> end)
		Vec3 rgb0{ 1.0f, 1.0f, 1.0f };  // u6, u7, u8
		Vec3 rgb1{ 1.0f, 1.0f, 1.0f };

		// Thickness / cube scale along the arc
		Float_start_end thickness{ 0.01f, 0.01f }; // u9

		// How many cubes to sample along the arc (your instance X count)
		int samples = 100;

		// Send to shader uniforms (u0..u9). u4 is reserved for future use (set to 0).
		void send(Program::Shader::Instance& I)
		{
			I.set_u_start_end(0, lon0.start, lon0.end);   // u0 = lon0
			I.set_u_start_end(1, lat0.start, lat0.end);   // u1 = lat0
			I.set_u_start_end(2, lon1.start, lon1.end);   // u2 = lon1
			I.set_u_start_end(3, lat1.start, lat1.end);   // u3 = lat1

			I.set_u_start_end(4, 0.0f, 0.0f);             // u4 = (reserved)
			I.set_u_start_end(5, radius.start, radius.end); // u5 = radius

			I.set_u_start_end(6, rgb0.x, rgb1.x);         // u6 = R
			I.set_u_start_end(7, rgb0.y, rgb1.y);         // u7 = G
			I.set_u_start_end(8, rgb0.z, rgb1.z);         // u8 = B

			I.set_u_start_end(9, thickness.start, thickness.end); // u9 = thickness
		}

		// Convenience: copy all "start" to "end" (static state)
		void copy_start_to_end()
		{
			lon0.end = lon0.start;  lat0.end = lat0.start;
			lon1.end = lon1.start;  lat1.end = lat1.start;

			radius.end = radius.start;
			rgb1 = rgb0;
			thickness.end = thickness.start;
		}

		// Helpers (optional) ------------------------------------------------------

		// Convert degrees to normalized lon/lat.
		// lon_deg is elemnet (-infitiy,+infinity), lat_deg is element [-90,+90].
		static float lon01_from_deg(float lon_deg) {
			// wrap to [0,1)
			float x = lon_deg / 360.0f;
			x = x - std::floor(x); // fract
			return x;
		}
		static float lat01_from_deg(float lat_deg) {
			// 0 at north pole (+90 degress), 0.5 at equator (0 degress), 1 at south (-90 degress)
			float t = (90.0f - std::clamp(lat_deg, -90.0f, 90.0f)) / 180.0f;
			return std::clamp(t, 0.0f, 1.0f);
		}

		// Set both endpoints from degrees (+ radius & color/thickness if you want)
		void set_from_degrees(float lon0_deg, float lat0_deg,
			float lon1_deg, float lat1_deg,
			float R)
		{
			lon0.start = lon01_from_deg(lon0_deg);
			lat0.start = lat01_from_deg(lat0_deg);
			lon1.start = lon01_from_deg(lon1_deg);
			lat1.start = lat01_from_deg(lat1_deg);

			lon0.end = lon0.start; lat0.end = lat0.start;
			lon1.end = lon1.start; lat1.end = lat1.start;

			radius.start = radius.end = R;
		}
	};

	struct LinesGeodesic
	{
		std::vector<LineGeodesic> lines;

		// Example init: 100 short arcs along the equator (clean visual baseline)
		void init()
		{
			
			{
				LineGeodesic L;

				L.samples = 100;

				L.lon0.start = 0.0;
				L.lat0.start = 0.5;

				L.lon1.start = 0.2;
				L.lat1.start = 0.5;

				L.radius.start = 0.5f;

				// subtle color variation
				L.rgb0 = { 1.0, 1.0, 1.0 };

				L.thickness.start = 0.001f;

				// make static for now (engine can animate u* if desired)
				L.copy_start_to_end();


				lines.emplace_back(std::move(L));
			}
			
			{

				for (int i = 0; i < 100; i++)
				{
					LineGeodesic L;

					L.samples = 100;

					L.lon0.start = Random::generate_random_float_0_to_1();
					L.lat0.start = Random::generate_random_float_0_to_1();

					L.lon1.start = L.lon0.start + Random::generate_random_float_minus_one_to_plus_one() * 0.1;
					L.lat1.start = L.lat0.start + Random::generate_random_float_minus_one_to_plus_one() * 0.1;

					L.radius.start = 0.5f;

					// subtle color variation
					L.rgb0 = { 0.0, 1.0, 0.0 };

					L.thickness.start = 0.0001f;

					// make static for now (engine can animate u* if desired)
					L.copy_start_to_end();


					lines.emplace_back(std::move(L));
				}
				
			}


			

			
		}

		void draw(Scene_::Scene& scene)
		{
			const int shader_number = 22;  // your geodesic-line shader

			add_shader(scene, shader_number, [&](Program::Shader& sh)
				{
					for (size_t i = 0; i < lines.size(); ++i)
					{
						auto id = sh.create_instance();
						auto I = sh.instance(id);

						// one drawcall per arc
						I.set_group_size(lines[i].samples, 1, 1)
							.set_drawcalls(1)
							.set_position_start(0.0f, 0.0f, 0.0f)
							.set_position_end(0.0f, 0.0f, 0.0f)
							.set_euler_start(0.0f, 0.0f, 0.0f)
							.set_euler_end(0.0f, 0.0f, 0.0f)
							.set_scale_start(1.0f, 1.0f, 1.0f)
							.set_scale_end(1.0f, 1.0f, 1.0f);

						lines[i].send(I);
					}
				});
		}

		// Utility to append and get a reference
		LineGeodesic& add_line()
		{
			lines.emplace_back(LineGeodesic{});
			return lines.back();
		}
	};

	struct Clip
	{


		void generate(int number)
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

			
			// CAPTURED: { pos: [-0.986564, 1.019589, 2.007937] , yaw : 46.080070, pitch : -17.400019, fov : 45.000000 }
			program.camera_start.x = -0.986564;
			program.camera_start.y = 1.019589;
			program.camera_start.z = 2.007937;

			program.camera_start.yaw = 46.080070;
			program.camera_start.pitch = -17.400019;
			program.camera_start.fov = 45.0;


			program.camera_end.x = program.camera_start.x;
			program.camera_end.y = program.camera_start.y;
			program.camera_end.z = program.camera_start.z;

			program.camera_end.yaw = program.camera_start.yaw;
			program.camera_end.pitch = program.camera_start.pitch;
			program.camera_end.fov = program.camera_start.fov;

			Scene_::Scene scene = Scene_::Scene();
			program.configure(scene);

			if (enable_shader_20_sphere) // sphered
			{
				Spheres sphere;
				sphere.init_1(10);
				sphere.draw(scene, 1000);
			}

			if (enable_shader_21_line) // lines
			{
				Lines lines;
				lines.init();
				lines.draw(scene);
			}

			if(enable_shader_22_geodesic_line) // lines with t
			{
				LinesGeodesic lines_with_t;
				lines_with_t.init();
				lines_with_t.draw(scene);
			}

			if (enable_shader_10_unit_cube)
			{
				UnitCube unit_cube;
				unit_cube.init();
				unit_cube.draw(scene);
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
		const bool enable_shader_20_sphere = false; // sphere
		const bool enable_shader_21_line = false; // line
		const bool enable_shader_22_geodesic_line = true; // line with t

	};


	void generate()
	{
		Clip clip;

		// for(int i = 0; i < 30; i++)
		{
			std::cout << "clip : " << clip.clip_number << "\n";

			clip.generate(0);

			// for (int i = 1; i < 57; i++)
			// {
			// 	clip.generate(i);
			// }
			
		}




		





	}

}




int universe(int argc, char* argv[])
{
	std::cout << "Universe\n";

	Universe_::generate();

	return 0;
}