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

namespace Vibe_01_10_2025_11_21_
{
	/*
	What are vibes rough explanation the concept is evoliving

	Well the idea of vibes is to capture things that you would copy paste from project to project and instead reference that
	Well the key thing is that once the vibe is created you don't edit the vibe you just use the vibe
	Than you copy paste and create the new vibe
	The project may use multiply vibes but usually you don't use multiply vibes you use only one vibe at the time most of the time so yea


	The vibe is likely created one per week or something like that
	While the universes are created way more often fron many times per day to once a day
	When you find out there is a lot of new code that you copy paste from project to project you create the vibe
	Well yea when you switch how you do thing you create new vibe that maybe does not have some code as well so yea

	*/

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

	struct Transform
	{
		Vec3 position{ 0.0f, 0.0f, 0.0f };
		Vec3 euler{ 0.0f, 0.0f, 0.0f };
		Vec3 scale{ 1.0f, 1.0f, 1.0f };
	};

	struct Transform_StartEnd
	{
		Transform start;
		Transform end;

		void copy_start_to_end()
		{
			end.position = start.position;
			end.euler = start.euler;
			end.scale = start.scale;
		}

		void send(Program::Shader::Instance& I)
		{
			I.set_position_start(start.position.x, start.position.y, start.position.z)
				.set_position_end(end.position.x, end.position.y, end.position.z)
				.set_euler_start(start.euler.x, start.euler.y, start.euler.z)
				.set_euler_end(end.euler.x, end.euler.y, end.euler.z)
				.set_scale_start(start.scale.x, start.scale.y, start.scale.z)
				.set_scale_end(end.scale.x, end.scale.y, end.scale.z);
		}
	};

	struct Spheres_shader_20
	{
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

		std::vector<Sphere> spheres;
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

	struct Lines_shader_21
	{
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

		std::vector<Line> lines;

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

	struct UnitCube_shader_10
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

	struct LinesGeodesic_shader_22
	{

		struct LineGeodesic
		{
			Transform_StartEnd transform_startEnd;

			// Endpoints on the sphere (normalized)
			Float_start_end y0{ 0.0f, 0.0f };  // u0
			Float_start_end x0{ 0.0f, 0.0f };  // u1
			Float_start_end y1{ 0.0f, 0.0f };  // u2
			Float_start_end x1{ 0.0f, 0.0f };  // u3

			// Sphere radius
			Float_start_end radius{ 1.0f, 1.0f }; // u5


			// Color (start -> end)
			Vec3 rgb0{ 1.0f, 1.0f, 1.0f };  // u6, u7, u8
			Vec3 rgb1{ 1.0f, 1.0f, 1.0f };

			// Thickness / cube scale along the arc
			Float_start_end thickness{ 0.01f, 0.01f }; // u9

			// How many cubes to sample along the arc (your instance X count)
			int samples = 100;

			Float_start_end turns{ 0.0f, 0.0f };

			// Send to shader uniforms (u0..u9). u4 is reserved for future use (set to 0).
			void send(Program::Shader::Instance& I)
			{
				I.set_u_start_end(0, y0.start, y0.end);   // u0 = lon0
				I.set_u_start_end(1, x0.start, x0.end);   // u1 = lat0
				I.set_u_start_end(2, y1.start, y1.end);   // u2 = lon1
				I.set_u_start_end(3, x1.start, x1.end);   // u3 = lat1

				I.set_u_start_end(4, turns.start, turns.end);             // u4 = (reserved)
				I.set_u_start_end(5, radius.start, radius.end); // u5 = radius

				I.set_u_start_end(6, rgb0.x, rgb1.x);         // u6 = R
				I.set_u_start_end(7, rgb0.y, rgb1.y);         // u7 = G
				I.set_u_start_end(8, rgb0.z, rgb1.z);         // u8 = B

				I.set_u_start_end(9, thickness.start, thickness.end); // u9 = thickness

				transform_startEnd.send(I);
			}

			// Convenience: copy all "start" to "end" (static state)
			void copy_start_to_end()
			{
				y0.end = y0.start;  x0.end = x0.start;
				y1.end = y1.start;  x1.end = x1.start;

				turns.end = turns.start;

				radius.end = radius.start;
				rgb1 = rgb0;
				thickness.end = thickness.start;

				transform_startEnd.copy_start_to_end();
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
				y0.start = lon01_from_deg(lon0_deg);
				x0.start = lat01_from_deg(lat0_deg);
				y1.start = lon01_from_deg(lon1_deg);
				x1.start = lat01_from_deg(lat1_deg);

				y0.end = y0.start; x0.end = x0.start;
				y1.end = y1.start; x1.end = x1.start;

				radius.start = radius.end = R;
			}
		};

		std::vector<LineGeodesic> lines;

		// Example init: 100 short arcs along the equator (clean visual baseline)


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
							.set_drawcalls(1);

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
		using initfn = void(*)(Scene_::Scene& scene, const int clip_number, const int clip_fps, const int clip_length_seconds, const bool capture, const bool capture_png, const bool capture_bmp);



		void generate(int number, initfn f_init)
		{
			engine_flush_frames();
			engine_delete_flush_frames();

			if (f_init == nullptr)
			{
				std::cout << "No init Function";
				std::abort();
			}

			Scene_::Scene scene = Scene_::Scene();

			// init(scene, number, clip_fps, clip_length_seconds, capture, capture_png, capture_bmp);
			f_init(scene, number, clip_fps, clip_length_seconds, capture, capture_png, capture_bmp);

			// run program
			{
				// scene.print();
				std::string program_name = NameGenerators_::generate_prefix_timestamp_suffix_name();
				save_program(scene, program_name);
				run_program(program_name);

				int i = clip_number;

				if (capture)
				{
					std::string name = "output_" + std::to_string(i);

					if (capture_bmp)
					{
						Video::generate(name, "bmp");
					}
					else if (capture_png)
					{
						Video::generate(name, "png");
					}
				}



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

		bool capture = false;
		bool capture_png = false;
		bool capture_bmp = true;
	};
}