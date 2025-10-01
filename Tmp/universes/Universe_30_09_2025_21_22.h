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

	struct LinesGeodesic
	{
		std::vector<LineGeodesic> lines;

		// Example init: 100 short arcs along the equator (clean visual baseline)
		void init()
		{


			

			if(false)
			{
				LineGeodesic L;

				L.samples = 100;

				L.y0.start = 0.0;
				L.x0.start = 0.5;

				L.y1.start = 0.2;
				L.x1.start = 0.5;

				L.radius.start = 0.5f;

				L.turns.start = 0.4f;
				

				// subtle color variation
				L.rgb0 = { 1.0, 1.0, 1.0 };

				L.thickness.start = 0.001f;

				// make static for now (engine can animate u* if desired)
				L.copy_start_to_end();


				lines.emplace_back(std::move(L));
			}

			if(false)
			{

				//for (int i = 0; i < 1; i++)
				//{
				//	LineGeodesic L;

				//	L.samples = 1000;

				//	L.lon0.start = 0.0;
				//	L.lat0.start = 0.0;

				//	L.lon1.start = 0.2;
				//	L.lat1.start = 0.0;

				//	L.radius.start = 0.5f;

				//	L.turns.start = 0.01f;


				//	// subtle color variation
				//	L.rgb0 = { 1.0, 1.0, 1.0 };

				//	L.thickness.start = 0.001f;

				//	// make static for now (engine can animate u* if desired)
				//	L.copy_start_to_end();


				//	lines.emplace_back(std::move(L));
				//}


				for (int i = 0; i < 10; i++)
				{
					LineGeodesic L;
					L.samples = 600;
					L.radius.start = 0.5f;

					L.x0.start = 0.0f;
					L.y0.start = 0.1f * i;


					L.x1.start = 1.0f;
					L.y1.start = 0.1f * i;

					L.thickness = { 0.004f, 0.004f };
					L.rgb0 = { 1,0,0 };

					L.transform_startEnd.start.position.y = 1.0f;

					L.copy_start_to_end();
					lines.emplace_back(std::move(L));
				}

				for (int i = 0; i < 10; i++)
				{
					LineGeodesic L;
					L.samples = 600;
					L.radius.start = 0.5f;

					L.x0.start = 0.1f * i;
					L.y0.start = 0.0f;


					L.x1.start = 0.1f * i;
					L.y1.start = 0.5f;

					L.thickness = { 0.004f, 0.004f };
					L.rgb0 = { 1,1,0 };

					L.transform_startEnd.start.position.y = 1.0f;

					L.copy_start_to_end();
					lines.emplace_back(std::move(L));
				}

				for (int i = 0; i < 10; i++)
				{
					LineGeodesic L;
					L.samples = 600;
					L.radius.start = 0.5f;

					L.x0.start = 0.1f * i;
					L.y0.start = 0.5f;


					L.x1.start = 0.1f * i;
					L.y1.start = 1.0f;

					L.thickness = { 0.004f, 0.004f };
					L.rgb0 = { 1,0,1 };

					L.transform_startEnd.start.position.y = 1.0f;

					L.copy_start_to_end();
					lines.emplace_back(std::move(L));
				}

				for (int j = 0; j < 10; j++)
				{
					for (int i = 0; i < 10; i++)
					{
						LineGeodesic L;
						L.samples = 600;
						L.radius.start = 0.5f;

						L.x0.start = 0.1f * i;
						L.y0.start = 0.1f * j;


						L.x1.start = 0.1f * (i + 1);
						L.y1.start = 0.1f * (j + 1);

						L.thickness = { 0.001f, 0.001f };
						L.rgb0 = { 1,1,1 };

						L.transform_startEnd.start.position.y = 1.0f;

						L.copy_start_to_end();
						lines.emplace_back(std::move(L));
					}
				}
				

				

				

				//LineGeodesic L;
				//L.samples = 600;
				//L.lon0 = { 0.25f, 0.25f }; // pick meridian
				//L.lon1 = L.lon0;
				//L.lat0 = { 0.25f, 0.25f };
				//L.lat1 = L.lat0;
				//L.turns = { 1.0f, 1.0f }; // ring mode
				//L.radius = { 0.5f, 0.5f };
				//L.thickness = { 0.004f, 0.004f };
				//L.copy_start_to_end();
				//lines.emplace_back(L);

				//LineGeodesic L;
				//L.samples = 180;
				//L.lon0 = { 0.95f, 0.95f };
				//L.lat0 = { 0.40f, 0.40f };
				//L.lon1 = { 0.05f, 0.05f };  // across the wrap
				//L.lat1 = { 0.60f, 0.60f };
				//L.turns = { 0.0f, 0.0f };   // geodesic mode
				//L.radius = { 0.5f, 0.5f };
				//L.thickness = { 0.006f, 0.006f };
				//L.rgb0 = { 1,0.3f,0.3f }; L.rgb1 = { 0.3f,0.6f,1 };
				//L.copy_start_to_end();
				//lines.emplace_back(L);


				//LineGeodesic L;
				//L.samples = 800;
				//L.lon0 = { 0.10f, 0.10f }; L.lat0 = { 0.30f, 0.30f };
				//L.lon1 = { 0.65f, 0.65f }; L.lat1 = { 0.80f, 0.80f };
				//L.turns = { 1.0f, 1.0f };  // ring mode ON
				//L.radius = { 0.5f, 0.5f };
				//L.thickness = { 0.005f, 0.005f };
				//L.copy_start_to_end();
				//lines.emplace_back(L);
				
			}
			

			if (true)
			{
				auto draw_sphere = [](std::vector<LineGeodesic>& l, float radius, float x, float y, float z, float thickness)
					{
						for (int i = 0; i < 10; i++)
						{
							LineGeodesic L;
							L.samples = 600;
							L.radius.start = radius;

							L.x0.start = 0.0f;
							L.y0.start = 0.1f * i;


							L.x1.start = 1.0f;
							L.y1.start = 0.1f * i;

							L.thickness = { thickness, thickness };
							L.rgb0 = { 1,0,0 };

							L.transform_startEnd.start.position.x = x;
							L.transform_startEnd.start.position.y = y;
							L.transform_startEnd.start.position.z = z;

							L.copy_start_to_end();
							l.emplace_back(std::move(L));
						}

						for (int i = 0; i < 10; i++)
						{
							LineGeodesic L;
							L.samples = 600;
							L.radius.start = radius;

							L.x0.start = 0.1f * i;
							L.y0.start = 0.0f;


							L.x1.start = 0.1f * i;
							L.y1.start = 0.5f;

							L.thickness = { thickness, thickness };
							L.rgb0 = { 1,1,0 };

							L.transform_startEnd.start.position.x = x;
							L.transform_startEnd.start.position.y = y;
							L.transform_startEnd.start.position.z = z;

							L.copy_start_to_end();
							l.emplace_back(std::move(L));
						}

						for (int i = 0; i < 10; i++)
						{
							LineGeodesic L;
							L.samples = 600;
							L.radius.start = radius;

							L.x0.start = 0.1f * i;
							L.y0.start = 0.5f;


							L.x1.start = 0.1f * i;
							L.y1.start = 1.0f;

							L.thickness = { thickness, thickness };
							L.rgb0 = { 1,0,1 };

							L.transform_startEnd.start.position.x = x;
							L.transform_startEnd.start.position.y = y;
							L.transform_startEnd.start.position.z = z;

							L.copy_start_to_end();
							l.emplace_back(std::move(L));
						}

						for (int j = 0; j < 10; j++)
						{
							for (int i = 0; i < 10; i++)
							{
								LineGeodesic L;
								L.samples = 600;
								L.radius.start = radius;

								L.x0.start = 0.1f * i;
								L.y0.start = 0.1f * j;


								L.x1.start = 0.1f * (i + 1);
								L.y1.start = 0.1f * (j + 1);

								L.thickness = { thickness, thickness };
								L.rgb0 = { 1,1,1 };

								L.transform_startEnd.start.position.x = x;
								L.transform_startEnd.start.position.y = y;
								L.transform_startEnd.start.position.z = z;

								L.copy_start_to_end();
								l.emplace_back(std::move(L));
							}
						}

					};
				
				draw_sphere(lines, 0.025, 0.5, 0.5, 0.5, 0.00025);

				draw_sphere(lines, 0.1, 0.0, 0.0, 0.0, 0.001);
				draw_sphere(lines, 0.1, 0.0, 0.0, 1.0, 0.001);
				draw_sphere(lines, 0.1, 0.0, 1.0, 0.0, 0.001);
				draw_sphere(lines, 0.1, 0.0, 1.0, 1.0, 0.001);
				
				draw_sphere(lines, 0.1, 1.0, 0.0, 0.0, 0.001);
				draw_sphere(lines, 0.1, 1.0, 0.0, 1.0, 0.001);
				draw_sphere(lines, 0.1, 1.0, 1.0, 0.0, 0.001);
				draw_sphere(lines, 0.1, 1.0, 1.0, 1.0, 0.001);
				
			}

			if (true)
			{

				for (int i = 0; i < 1000; i++)
				{
					LineGeodesic& L = add_line();

					L.samples = 1000;

					L.y0.start = Random::generate_random_float_0_to_1();
					L.x0.start = Random::generate_random_float_0_to_1();

					L.y1.start = L.y0.start + Random::generate_random_float_minus_one_to_plus_one() * 0.1;
					L.x1.start = L.x0.start + Random::generate_random_float_minus_one_to_plus_one() * 0.1;

					L.radius.start = 0.5f;
					
					// subtle color variation
					L.rgb0 = { 0.0, 1.0, 0.0 };
					

					L.thickness.start = 0.0001f;


					L.transform_startEnd.start.position.x = 0.5f;
					L.transform_startEnd.start.position.y = 0.5f;
					L.transform_startEnd.start.position.z = 0.5f;

					// make static for now (engine can animate u* if desired)
					L.copy_start_to_end();

					L.rgb1 = { 0.2, 0.7, 4.0 };

					L.radius.end = 1.0f;
				}

			}

			if(false)
			{
				LineGeodesic L;

				L.samples = 1000;

				L.y0.start = 0.10f;
				L.x0.start = 0.30f;

				L.y1.start = 0.65f;
				L.x1.start = 0.80f;

				L.turns.start = 1.0f;

				L.radius.start = 0.5f;

				// subtle color variation
				L.rgb0 = { 0.0, 1.0, 0.0 };

				L.thickness.start = 0.0001f;

				// make static for now (engine can animate u* if desired)
				L.copy_start_to_end();


				lines.emplace_back(std::move(L));
			}

			if(false)
			{
				LineGeodesic L;

				L.samples = 1000;

				L.y0.start = 0.10f;
				L.x0.start = 0.30f;

				L.y1.start = 0.65f;
				L.x1.start = 0.80f;

				L.turns.start = 0.0f;

				L.radius.start = 0.5f;

				// subtle color variation
				L.rgb0 = { 1.0, 1.0, 0.0 };

				L.thickness.start = 0.0001f;

				// make static for now (engine can animate u* if desired)
				L.copy_start_to_end();



				lines.emplace_back(std::move(L));
			}


			if(false)
			{

				// --- Small helpers ----------------------------------------------------------
				auto hsv2rgb = [](float h, float s, float v) {
					h = h - std::floor(h);
					float i = std::floor(h * 6.0f);
					float f = h * 6.0f - i;
					float p = v * (1.0f - s);
					float q = v * (1.0f - f * s);
					float t = v * (1.0f - (1.0f - f) * s);
					int ii = int(i) % 6;
					switch (ii) {
					case 0: return Vec3{ v, t, p };
					case 1: return Vec3{ q, v, p };
					case 2: return Vec3{ p, v, t };
					case 3: return Vec3{ p, q, v };
					case 4: return Vec3{ t, p, v };
					default:return Vec3{ v, p, q };
					}
					};

				// Aim for ~N segments along an arc based on radius, turns and thickness.
				// Keeps rings smooth without crazy instance counts.
				auto ideal_samples_ring = [](float R, float turns, float thick, float seg_mult = 3.0f) {
					const float PI = 3.14159265358979323846f;
					float L = 2.0f * PI * R * std::max(std::abs(turns), 0.05f); // never below a small fraction
					float seg = std::max(thick * seg_mult, 1e-4f);
					return std::max(16, (int)std::ceil(L / seg));
					};



				// Common sphere radius for the showcase
				const float R = 0.5f;

				// 1) EQUATOR RING (lat=0.5). Full 360, animated turns 0.0001 -> 1.0
				{
					LineGeodesic L;
					L.radius = { R, R };
					L.x0 = { 0.5f, 0.5f };
					L.x1 = { 0.5f, 0.5f };
					L.y0 = { 0.00f, 0.00f };
					L.y1 = { 0.00f, 0.00f };
					L.turns = { 0.0001f, 1.0f };          // avoid exactly 0 so ring mode stays on
					L.thickness = { 0.01f, 0.01f };
					L.samples = ideal_samples_ring(R, 1.0f, L.thickness.start);
					L.rgb0 = { 1.0f, 1.0f, 1.0f }; L.rgb1 = L.rgb0;
					lines.emplace_back(L);
				}

				// 2) MERIDIAN RING (constant longitude). 0.0001 -> 1.5 turns (one and a half)
				{
					LineGeodesic L;
					L.radius = { R, R };
					L.y0 = { 0.25f, 0.25f };   // choose which meridian
					L.y1 = { 0.25f, 0.25f };
					L.x0 = { 0.25f, 0.25f };   // phase along the meridian
					L.x1 = { 0.25f, 0.25f };
					L.turns = { 0.0001f, 1.5f };
					L.thickness = { 0.008f, 0.008f };
					L.samples = ideal_samples_ring(R, 1.5f, L.thickness.start);
					L.rgb0 = hsv2rgb(0.58f, 0.6f, 1.0f); L.rgb1 = L.rgb0;
					lines.emplace_back(L);
				}

				// 3) DIAGONAL RING (arbitrary great circle defined by two non-colinear endpoints)
				//    0.0001 -> 1.0 turn to show the "forced ring" using u4.
				{
					LineGeodesic L;
					L.radius = { R, R };
					L.y0 = { 0.10f, 0.10f }; L.x0 = { 0.30f, 0.30f };
					L.y1 = { 0.65f, 0.65f }; L.x1 = { 0.80f, 0.80f };
					L.turns = { 0.0001f, 1.0f };
					L.thickness = { 0.006f, 0.006f };
					L.samples = ideal_samples_ring(R, 1.0f, L.thickness.start);
					L.rgb0 = hsv2rgb(0.10f, 0.7f, 1.0f); L.rgb1 = L.rgb0;
					lines.emplace_back(L);
				}

				// 4) PARTIAL DIAGONAL RING (quarter loop -> three quarters), showing arc length control via u4
				{
					LineGeodesic L;
					L.radius = { R, R };
					L.y0 = { 0.70f, 0.70f }; L.x0 = { 0.20f, 0.20f };
					L.y1 = { 0.20f, 0.20f }; L.x1 = { 0.75f, 0.75f };
					L.turns = { 0.25f, 0.75f };      // quarter  three quarters of the great circle
					L.thickness = { 0.006f, 0.006f };
					L.samples = ideal_samples_ring(R, 0.75f, L.thickness.start);
					L.rgb0 = hsv2rgb(0.85f, 0.7f, 1.0f); L.rgb1 = L.rgb0;
					lines.emplace_back(L);
				}

				// 5) SHORTEST GEODESIC (segment) that crosses the seam: animate endpoints across lon=10
				{
					LineGeodesic L;
					L.radius = { R, R };
					// Start near seam on opposite sides; shader will choose the shortest arc
					L.y0 = { 0.95f, 0.95f };  L.x0 = { 0.40f, 0.70f }; // animate lat to show path change
					L.y1 = { 0.05f, 0.05f };  L.x1 = { 0.60f, 0.30f };
					L.turns = { 0.0f, 0.0f };    // 0 => geodesic mode (no forced ring)
					L.thickness = { 0.012f, 0.006f };   // animate thickness thinner
					L.samples = 180; // static count is fine for single segment
					L.rgb0 = { 1.0f, 0.2f, 0.2f }; L.rgb1 = { 0.2f, 0.6f, 1.0f };
					lines.emplace_back(L);
				}

				// 6) LATITUDE BANDS (stack a few parallels), all spinning with turns
				{
					const int bands = 7;
					for (int i = 0; i < bands; ++i) {
						float t = (i + 0.5f) / float(bands);      // 0..1
						float lat = 0.15f + 0.70f * t;            // avoid poles
						LineGeodesic L;
						L.radius = { R, R };
						L.x0 = { lat, lat };
						L.x1 = { lat, lat };
						L.y0 = { 0.0f, 0.0f };
						L.y1 = { 0.0f, 0.0f };
						L.turns = { 0.0001f, (i % 2 == 0) ? 1.0f : -1.0f }; // alternate directions
						L.thickness = { 0.0045f, 0.0045f };
						L.samples = ideal_samples_ring(R, 1.0f, L.thickness.start);
						L.rgb0 = hsv2rgb(0.08f + 0.10f * i, 0.6f, 0.95f); L.rgb1 = L.rgb0;
						lines.emplace_back(L);
					}
				}

				// 7) MERIDIAN FAN (several great circles rotated by longitude)
				{
					const int fan = 8;
					for (int i = 0; i < fan; ++i) {
						float lon = float(i) / float(fan);
						LineGeodesic L;
						L.radius = { R, R };
						L.y0 = { lon, lon };
						L.y1 = { lon, lon };
						L.x0 = { 0.35f, 0.35f };
						L.x1 = { 0.35f, 0.35f };
						L.turns = { 0.0001f, 1.0f };
						L.thickness = { 0.0035f, 0.0035f };
						L.samples = ideal_samples_ring(R, 1.0f, L.thickness.start);
						L.rgb0 = hsv2rgb(0.55f + 0.05f * i, 0.5f, 0.9f); L.rgb1 = L.rgb0;


						

						lines.emplace_back(L);
					}
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


		void generate(int number)
		{
			engine_flush_frames();
			engine_delete_flush_frames();

			Program program;
			program.le.halfLife = 0.02f * 4.0f;
			program.le.brightness = 0.0f;
			program.le.exposure = 1.0;
			program.le.msaaSamples = 10;

			program.capture.capture = true;
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


			// CAPTURED: { pos: [0.721016, 1.160607, 3.534045] , yaw : -2.879919, pitch : -11.280016, fov : 45.000000 }

			program.camera_end.x = 0.721016;
			program.camera_end.y = 1.160607;
			program.camera_end.z = 3.534045;

			program.camera_end.yaw = -2.879919;
			program.camera_end.pitch = -11.280016;
			program.camera_end.fov = 45.000000;

			// program.camera_end.x = program.camera_start.x;
			// program.camera_end.y = program.camera_start.y;
			// program.camera_end.z = program.camera_start.z;

			// program.camera_end.yaw = program.camera_start.yaw;
			// program.camera_end.pitch = program.camera_start.pitch;
			// program.camera_end.fov = program.camera_start.fov;





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

			if (enable_shader_22_geodesic_line) // lines with t
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