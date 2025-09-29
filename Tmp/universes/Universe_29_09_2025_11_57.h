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
	struct Vec3
	{
		float x = 0;
		float y = 0;
		float z = 0;
	};

	// (Add these functions before your 'struct Lines')

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


				// Instance 0



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

					line.thickness.start = 0.01;
					line.number_of_cubes = 100;

					line.copy_start_to_end();

					

					line.x1.end = 0.5f * cos(i * step_size);
					line.y1.end = 0.0f;
					line.z1.end = 0.5f * sin(i * step_size);

					// line.y1.end = 1.0;
					// line.z1.end = 1.0;
				}

				

				
			}


		}

		void init_sphere()
		{
			int num_lat_steps = 20; // Like lines of latitude
			int num_lon_steps = 40; // Like lines of longitude
			const float PI = 3.14159265359f;

			for (int i = 0; i < num_lat_steps; i++)
			{
				// Polar angle (from top to bottom)
				float polar_angle = (float(i) / float(num_lat_steps - 1)) * PI;

				for (int j = 0; j < num_lon_steps; j++)
				{
					// Azimuthal angle (around the circle)
					float azimuth_angle = (float(j) / float(num_lon_steps)) * 2.0f * PI;

					Line& line = add_line();

					// All lines start at the center
					line.x0.start = 0.0f;
					line.y0.start = 0.0f;
					line.z0.start = 0.0f;

					// Use spherical coordinates to find the endpoint
					float start_radius = 0.5f;
					line.x1.start = start_radius * sin(polar_angle) * cos(azimuth_angle);
					line.y1.start = start_radius * cos(polar_angle);
					line.z1.start = start_radius * sin(polar_angle) * sin(azimuth_angle);

					// Color based on height (y-coordinate)
					line.rgb_t0.x = 0.5f + 0.5f * line.y1.start; // Red channel for height
					line.rgb_t0.y = 0.2f;
					line.rgb_t0.z = 0.5f - 0.5f * line.y1.start; // Blue channel for inverse height

					line.thickness.start = 0.005f;
					line.number_of_cubes = 50;

					line.copy_start_to_end();

					// --- ANIMATION ---
					// Make the sphere expand and twist
					float end_radius = 0.8f;
					float twist_factor = 1.5f; // Add a twist
					line.x1.end = end_radius * sin(polar_angle) * cos(azimuth_angle * twist_factor);
					line.y1.end = end_radius * cos(polar_angle);
					line.z1.end = end_radius * sin(polar_angle) * sin(azimuth_angle * twist_factor);
				}
			}
		}

		void init_helix()
		{
			int number_of_lines = 200;
			const float TAU = 6.2831853f;
			const float PI = 3.14159265359;
			float step_size = (1.0 / float(number_of_lines)) * TAU;
			int num_twists = 5; // How many full rotations the helix makes

			for (int i = 0; i < number_of_lines; i++)
			{
				Line& line = add_line();

				// The line starts from the central axis and points outwards
				line.x0.start = 0.0f;
				line.y0.start = -1.0f + 2.0f * (float(i) / number_of_lines); // Move up the Y-axis
				line.z0.start = 0.0f;

				float angle = i * step_size * num_twists;
				float radius = 0.4f;

				// Endpoint forms the helix shape
				line.x1.start = radius * cos(angle);
				line.y1.start = line.y0.start; // y is the same as the start point's y
				line.z1.start = radius * sin(angle);

				// Color based on the angle
				line.rgb_t0.x = 0.5f + 0.5f * cos(angle);
				line.rgb_t0.y = 0.5f + 0.5f * sin(angle + PI / 2.0f);
				line.rgb_t0.z = 0.5f + 0.5f * sin(angle);

				line.thickness.start = 0.01f;
				line.number_of_cubes = 30;

				line.copy_start_to_end();

				// --- ANIMATION ---
				// Animate the helix to unwind into a straight vertical line
				line.x1.end = 0.0f;
				line.z1.end = 0.0f;
				// The y-coordinate remains the same
			}
		}

		void init_vortex()
		{
			int number_of_lines = 150;
			const float TAU = 6.2831853f;
			float step_size = (1.0f / float(number_of_lines)) * TAU;
			int num_twists = 4;

			for (int i = 0; i < number_of_lines; i++)
			{
				Line& line = add_line();

				// All lines originate from the center
				line.x0.start = 0.0f;
				line.y0.start = 0.0f;
				line.z0.start = 0.0f;

				float progress = float(i) / number_of_lines;
				float angle = i * step_size * num_twists;

				// Radius decreases as we go down
				float radius = 0.8f * (1.0f - progress);

				// Endpoint calculation
				line.x1.start = radius * cos(angle);
				line.y1.start = 1.0f - 2.0f * progress; // Move from +1 down to -1 on Y-axis
				line.z1.start = radius * sin(angle);

				line.rgb_t0.x = 0.9f;
				line.rgb_t0.y = 0.2f + 0.5f * progress;
				line.rgb_t0.z = 0.1f;

				line.thickness.start = 0.008f;
				line.number_of_cubes = 70;

				line.copy_start_to_end();

				// --- ANIMATION ---
				// Make the vortex spin faster and collapse inwards
				float end_angle = angle * 3.0f; // Spin faster
				line.x1.end = 0.0f; // Collapse radius to zero
				line.y1.end = line.y1.start;
				line.z1.end = 0.0f; // Collapse radius to zero
			}
		}

		void init_torus()
		{
			int num_major_steps = 50; // Steps around the main ring
			int num_minor_steps = 15; // Steps around the tube
			const float TAU = 6.2831853f;

			float major_radius = 0.6f;
			float minor_radius = 0.2f;

			for (int i = 0; i < num_major_steps; i++)
			{
				// Angle around the main ring
				float major_angle = (float(i) / num_major_steps) * TAU;

				// Calculate the center point of the tube slice (the line's start)
				float ring_x = major_radius * cos(major_angle);
				float ring_z = major_radius * sin(major_angle);

				for (int j = 0; j < num_minor_steps; j++)
				{
					// Angle around the tube
					float minor_angle = (float(j) / num_minor_steps) * TAU;

					Line& line = add_line();

					// Start point is on the central ring
					line.x0.start = ring_x;
					line.y0.start = 0.0f;
					line.z0.start = ring_z;

					// End point is on the surface of the torus
					line.x1.start = (major_radius + minor_radius * cos(minor_angle)) * cos(major_angle);
					line.y1.start = minor_radius * sin(minor_angle);
					line.z1.start = (major_radius + minor_radius * cos(minor_angle)) * sin(major_angle);

					line.rgb_t0.x = 0.5f + 0.5f * cos(minor_angle);
					line.rgb_t0.y = 0.5f + 0.5f * sin(major_angle);
					line.rgb_t0.z = 0.8f;

					line.thickness.start = 0.006f;
					line.number_of_cubes = 40;

					line.copy_start_to_end();

					// --- ANIMATION ---
					// Make the lines retract to the center ring
					line.x1.end = line.x0.start;
					line.y1.end = line.y0.start;
					line.z1.end = line.z0.start;
				}
			}
		}

		void init_0000_swirling_vortex()
		{
			lines.clear(); // keep sketches clean on rerun

			const int   number_of_lines = 220;            // more lines = smoother cage
			const float TAU = 6.2831853071795864769f;

			const float ringR = 0.65f;              // ring radius
			const float chordFrac = 0.06f;              // each line spans this fraction of the circle
			const float twistTurns = 1.75f;              // how many full turns by the end
			const float lift = 1.2f;               // how high it rises by the end
			const float baseThick = 0.006f;             // baseline thickness
			const int   cubesPerLine = 200;                // segment count along each line

			const float arcStep = chordFrac * TAU;

			auto hueColor = [&](float h) -> Vec3 {
				// cheap cosine palette (h in [0,1))
				return Vec3{
					0.5f + 0.5f * cosf(TAU * (h + 0.00f)),
					0.5f + 0.5f * cosf(TAU * (h + 0.33f)),
					0.5f + 0.5f * cosf(TAU * (h + 0.66f))
				};
				};

			for (int i = 0; i < number_of_lines; ++i)
			{
				float tNorm = (i + 0.5f) / float(number_of_lines); // 0..1
				float ang = tNorm * TAU;

				// slight per-line radius jitter so it feels alive
				float jitter = 0.03f * Random::generate_random_float_minus_one_to_plus_one();
				float r = ringR * (1.0f + jitter);

				Line& line = add_line();

				// START: short chord on the XY ring (y=0)
				line.x0.start = r * cosf(ang);
				line.y0.start = 0.0f;
				line.z0.start = r * sinf(ang);

				line.x1.start = r * cosf(ang + arcStep);
				line.y1.start = 0.0f;
				line.z1.start = r * sinf(ang + arcStep);

				line.rgb_t0 = hueColor(tNorm); // rainbow around the ring

				// thickness with a little angular modulation
				line.thickness.start = baseThick * (0.6f + 0.4f * (0.5f + 0.5f * sinf(ang * 4.0f)));
				line.number_of_cubes = cubesPerLine;

				// copy start->end, then override end state we want to animate toward
				line.copy_start_to_end();

				// END: twist around + rise up + hue shift + taper
				float twist = twistTurns * TAU;
				float angEnd = ang + twist;
				float yWave = lift * (0.5f + 0.5f * sinf(ang * 3.0f)); // adds a gentle vertical ripple

				line.x0.end = r * cosf(angEnd);
				line.y0.end = yWave;
				line.z0.end = r * sinf(angEnd);

				line.x1.end = r * cosf(angEnd + arcStep);
				line.y1.end = yWave;
				line.z1.end = r * sinf(angEnd + arcStep);

				line.rgb_t1 = hueColor(std::fmod(tNorm + 0.18f, 1.0f)); // slight hue drift over time
				line.thickness.end = baseThick * 0.25f;                 // taper
			}
		}

		// Add this new function inside your 'struct Lines'
		void init_random_walk_filaments()
		{
			lines.clear(); // Start fresh

			const int number_of_filaments = 25;
			const int segments_per_filament = 400;
			const float step_size = 0.015f;
			const float drift_factor = 0.1f; // How strongly it's pushed outwards

			// Outer loop: Create each main filament
			for (int i = 0; i < number_of_filaments; i++)
			{
				// Give each filament a unique base color
				Vec3 base_color = {
					0.5f + 0.5f * Random::generate_random_float_0_to_1(),
					0.5f + 0.5f * Random::generate_random_float_0_to_1(),
					0.5f + 0.5f * Random::generate_random_float_0_to_1()
				};

				// Start each filament near the center
				Vec3 current_pos = {
					0.01f * Random::generate_random_float_minus_one_to_plus_one(),
					0.01f * Random::generate_random_float_minus_one_to_plus_one(),
					0.01f * Random::generate_random_float_minus_one_to_plus_one()
				};

				// Inner loop: Build a filament segment by segment
				for (int j = 0; j < segments_per_filament; j++)
				{
					Vec3 prev_pos = current_pos;

					// 1. Generate a pure random step
					Vec3 random_step = {
						Random::generate_random_float_minus_one_to_plus_one(),
						Random::generate_random_float_minus_one_to_plus_one(),
						Random::generate_random_float_minus_one_to_plus_one()
					};
					random_step = vec_normalize(random_step);

					// 2. Calculate the outward drift vector (bias)
					Vec3 drift_vector = vec_normalize(current_pos);

					// 3. Combine the random step and the drift
					Vec3 combined_step = vec_add(random_step, vec_scale(drift_vector, drift_factor));
					Vec3 final_direction = vec_normalize(combined_step);

					// 4. Update the current position
					current_pos = vec_add(current_pos, vec_scale(final_direction, step_size));

					// Create the line segment for this step
					Line& line = add_line();
					line.x0.start = prev_pos.x;
					line.y0.start = prev_pos.y;
					line.z0.start = prev_pos.z;
					line.x1.start = current_pos.x;
					line.y1.start = current_pos.y;
					line.z1.start = current_pos.z;

					// Style the line: fades out and gets thinner over its length
					float progress = (float)j / (float)segments_per_filament;
					float falloff = 1.0f - progress;

					line.rgb_t0 = vec_scale(base_color, falloff);
					line.thickness.start = 0.01f * falloff + 0.001f;
					line.number_of_cubes = 5; // Segments are small

					// Animation: collapse everything back to the origin
					line.copy_start_to_end();
					line.x0.end = line.y0.end = line.z0.end = 0.0f;
					line.x1.end = line.y1.end = line.z1.end = 0.0f;
				}
			}
		}
		
		// Add this more advanced function inside your 'struct Lines'
		void init_branching_filaments()
		{
			lines.clear();

			const int   number_of_trunks = 7;
			const int   segments_per_trunk = 150;
			const float trunk_step_size = 0.018f;
			const float trunk_drift = 0.15f;

			const float branch_probability = 0.04f; // 4% chance to branch on any trunk segment
			const int   segments_per_branch = 60;
			const float branch_step_size = 0.012f;
			const float branch_drift = 0.3f; // Branches try harder to get away

			// Outer loop: Create each main "trunk"
			for (int i = 0; i < number_of_trunks; i++)
			{
				Vec3 trunk_color = {
					0.5f + 0.5f * Random::generate_random_float_0_to_1(),
					0.5f + 0.5f * Random::generate_random_float_0_to_1(),
					0.5f + 0.5f * Random::generate_random_float_0_to_1()
				};

				Vec3 current_pos = { 0, 0, 0 };

				// Inner loop: Build the trunk, segment by segment
				for (int j = 0; j < segments_per_trunk; j++)
				{
					Vec3 prev_pos = current_pos;

					// --- Build trunk segment (same logic as before) ---
					Vec3 random_step = vec_normalize({
						Random::generate_random_float_minus_one_to_plus_one(),
						Random::generate_random_float_minus_one_to_plus_one(),
						Random::generate_random_float_minus_one_to_plus_one()
						});
					Vec3 drift_vector = vec_normalize(current_pos);
					Vec3 final_direction = vec_normalize(vec_add(random_step, vec_scale(drift_vector, trunk_drift)));
					current_pos = vec_add(current_pos, vec_scale(final_direction, trunk_step_size));

					// Create the trunk line segment
					Line& trunk_line = add_line();
					// ... (Set coordinates from prev_pos to current_pos) ...
					trunk_line.x0.start = prev_pos.x; trunk_line.y0.start = prev_pos.y; trunk_line.z0.start = prev_pos.z;
					trunk_line.x1.start = current_pos.x; trunk_line.y1.start = current_pos.y; trunk_line.z1.start = current_pos.z;

					float progress = (float)j / (float)segments_per_trunk;
					float falloff = 1.0f - progress;
					trunk_line.rgb_t0 = vec_scale(trunk_color, falloff);
					trunk_line.thickness.start = 0.012f * falloff + 0.002f;
					trunk_line.number_of_cubes = 5;
					trunk_line.copy_start_to_end();
					trunk_line.x0.end = trunk_line.y0.end = trunk_line.z0.end = 0.0f;
					trunk_line.x1.end = trunk_line.y1.end = trunk_line.z1.end = 0.0f;

					// --- Branching Logic ---
					if (Random::generate_random_float_0_to_1() < branch_probability && j > 10)
					{
						Vec3 branch_pos = current_pos; // Branch starts from the trunk
						Vec3 branch_color = vec_scale(trunk_color, 0.75f); // Slightly dimmer

						// Innermost loop: Build the new branch
						for (int k = 0; k < segments_per_branch; k++)
						{
							Vec3 prev_branch_pos = branch_pos;
							// ... (Identical random walk logic for the branch) ...
							random_step = vec_normalize({/* ... */ });
							drift_vector = vec_normalize(branch_pos);
							final_direction = vec_normalize(vec_add(random_step, vec_scale(drift_vector, branch_drift)));
							branch_pos = vec_add(branch_pos, vec_scale(final_direction, branch_step_size));

							// Create branch line segment
							Line& branch_line = add_line();
							// ... (Set coordinates, style, and animation for the branch line) ...
							branch_line.x0.start = prev_branch_pos.x; branch_line.y0.start = prev_branch_pos.y; branch_line.z0.start = prev_branch_pos.z;
							branch_line.x1.start = branch_pos.x; branch_line.y1.start = branch_pos.y; branch_line.z1.start = branch_pos.z;

							float branch_progress = (float)k / (float)segments_per_branch;
							float branch_falloff = 1.0f - branch_progress;
							branch_line.rgb_t0 = vec_scale(branch_color, branch_falloff);
							branch_line.thickness.start = 0.008f * branch_falloff + 0.001f; // Thinner
							branch_line.number_of_cubes = 3;
							branch_line.copy_start_to_end();
							branch_line.x0.end = branch_line.y0.end = branch_line.z0.end = 0.0f;
							branch_line.x1.end = branch_line.y1.end = branch_line.z1.end = 0.0f;
						}
					}
				}
			}
		}

		void init_0001_random_walk_bloom()
		{
			lines.clear();

			// ---- Tweakables (fast play knobs) ----
			const int   walkers = 12;      // number of independent walkers
			const int   stepsPerWalk = 160;     // segments per walker (total lines = walkers * stepsPerWalk)
			const float stepLen = 0.045f;  // length of each random step
			const float boundaryR = 1.25f;   // spherical boundary (reflect at edge)
			const float swirlStrength = 2.8f;    // rotation around Y (adds flow)
			const float driftUp = 0.0035f; // gentle upward drift
			const float baseThick = 0.007f;  // baseline thickness
			const int   cubesPerLine = 26;      // resolution along each line
			const float TAU = 6.28318530718f;

			auto length3 = [](const Vec3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); };
			auto normalize3 = [&](const Vec3& v) {
				float L = length3(v);
				if (L <= 1e-8f) return Vec3{ 0,0,0 };
				return Vec3{ v.x / L, v.y / L, v.z / L };
				};
			auto rotateY = [&](const Vec3& v, float ang) {
				float c = std::cos(ang), s = std::sin(ang);
				return Vec3{ c * v.x + s * v.z, v.y, -s * v.x + c * v.z };
				};
			auto hueColor = [&](float h) -> Vec3 { // h in [0,1)
				return Vec3{
					0.5f + 0.5f * std::cos(TAU * (h + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.66f))
				};
				};

			// spawn walkers in a small ball around the origin
			for (int w = 0; w < walkers; ++w)
			{
				Vec3 p = {
					0.15f * Random::generate_random_float_minus_one_to_plus_one(),
					0.15f * Random::generate_random_float_minus_one_to_plus_one(),
					0.15f * Random::generate_random_float_minus_one_to_plus_one()
				};

				// random initial direction
				Vec3 dir = normalize3(Vec3{
					Random::generate_random_float_minus_one_to_plus_one(),
					Random::generate_random_float_minus_one_to_plus_one(),
					Random::generate_random_float_minus_one_to_plus_one()
					});

				for (int s = 0; s < stepsPerWalk; ++s)
				{
					// small random jitter direction
					Vec3 rnd = normalize3(Vec3{
						Random::generate_random_float_minus_one_to_plus_one(),
						Random::generate_random_float_minus_one_to_plus_one(),
						Random::generate_random_float_minus_one_to_plus_one()
						});

					// swirl field around Y + gentle upward drift
					float swirl = swirlStrength * p.y;
					Vec3 biased = rotateY(rnd, swirl);
					biased.y += driftUp;

					// blend with previous direction for smoother paths
					dir = normalize3(Vec3{
						0.65f * dir.x + 0.35f * biased.x,
						0.65f * dir.y + 0.35f * biased.y,
						0.65f * dir.z + 0.35f * biased.z
						});

					Vec3 p0 = p;
					Vec3 p1 = Vec3{ p.x + dir.x * stepLen, p.y + dir.y * stepLen, p.z + dir.z * stepLen };

					// reflect from spherical boundary
					if (length3(p1) > boundaryR) {
						dir = Vec3{ -dir.x, -dir.y, -dir.z };
						p1 = Vec3{ p.x + dir.x * stepLen, p.y + dir.y * stepLen, p.z + dir.z * stepLen };
					}

					// colors along time (rainbow drift) + slight dependency on height
					float tNorm = (w + (s / float(stepsPerWalk))) / float(std::max(1, walkers));
					Vec3 c0 = hueColor(std::fmod(tNorm + 0.10f * (0.5f + 0.5f * p0.y), 1.0f));
					Vec3 c1 = hueColor(std::fmod(tNorm + 0.10f * (0.5f + 0.5f * p1.y) + 0.12f, 1.0f));

					// build the line as a segment of the walk
					Line& line = add_line();

					// "grow from midpoint": start collapsed at the segment midpoint, end at true endpoints
					Vec3 mid{ 0.5f * (p0.x + p1.x), 0.5f * (p0.y + p1.y), 0.5f * (p0.z + p1.z) };

					line.x0.start = mid.x; line.y0.start = mid.y; line.z0.start = mid.z;
					line.x1.start = mid.x; line.y1.start = mid.y; line.z1.start = mid.z;

					line.rgb_t0 = c0;                // start color
					line.thickness.start = baseThick * 0.25f;
					line.number_of_cubes = cubesPerLine;

					line.copy_start_to_end();

					// end state: the actual segment
					line.x0.end = p0.x; line.y0.end = p0.y; line.z0.end = p0.z;
					line.x1.end = p1.x; line.y1.end = p1.y; line.z1.end = p1.z;

					line.rgb_t1 = c1;                // end color (slight hue shift)
					// a little breathing taper
					float breathe = 0.8f + 0.2f * std::sin(0.35f * s + 1.7f * w);
					line.thickness.end = baseThick * breathe;

					// advance
					p = p1;
				}
			}
		}


		void init_abstract_cityscape()
		{
			lines.clear();

			// --- City Parameters (tweak these for different results!) ---
			const int   grid_size = 25;       // Creates a grid_size x grid_size city
			const float grid_spacing = 0.15f; // Distance between buildings
			const float min_building_height = 0.2f;
			const float max_building_height = 1.5f;
			const float skyway_probability = 0.05f; // 5% chance for any building to create a skyway

			// We'll store the rooftop positions to connect them later
			std::vector<Vec3> rooftops;

			// --- PHASE 1: Generate Buildings ---
			for (int i = 0; i < grid_size; i++)
			{
				for (int j = 0; j < grid_size; j++)
				{
					// Center the grid around the origin (0,0,0)
					float x_pos = (float(i) - float(grid_size) / 2.0f) * grid_spacing;
					float z_pos = (float(j) - float(grid_size) / 2.0f) * grid_spacing;

					// Add some random jitter to the position to make it less perfect
					x_pos += grid_spacing * 0.2f * Random::generate_random_float_minus_one_to_plus_one();
					z_pos += grid_spacing * 0.2f * Random::generate_random_float_minus_one_to_plus_one();

					// Randomize height
					float height = min_building_height +
						(max_building_height - min_building_height) * pow(Random::generate_random_float_0_to_1(), 2.0f); // pow() biases towards shorter buildings

					Line& building = add_line();

					// A building is a vertical line from y=0 to y=height
					building.x0.start = x_pos;
					building.y0.start = 0.0f;
					building.z0.start = z_pos;
					building.x1.start = x_pos;
					building.y1.start = height;
					building.z1.start = z_pos;

					// Style the building
					float height_norm = (height - min_building_height) / (max_building_height - min_building_height);
					building.rgb_t0.x = 0.2f + 0.8f * height_norm; // More red/purple for taller buildings
					building.rgb_t0.y = 0.1f;
					building.rgb_t0.z = 0.5f + 0.5f * (1.0f - height_norm); // More blue for shorter buildings

					building.thickness.start = 0.005f + 0.01f * height_norm; // Taller buildings are thicker
					building.number_of_cubes = 50;

					// --- ANIMATION: Building grows upwards ---
					building.copy_start_to_end();
					building.y1.start = 0.0f; // Start with zero height
					// The .end values are already set to the final height by copy_start_to_end()

					// Store the rooftop for Phase 2
					rooftops.push_back({ x_pos, height, z_pos });
				}
			}

			// --- PHASE 2: Generate Skyways ---
			if (rooftops.size() < 2) return; // Need at least two buildings to connect

			for (const auto& start_point : rooftops)
			{
				if (Random::generate_random_float_0_to_1() < skyway_probability)
				{
					// Pick a random different rooftop to connect to
					int end_index = (int)(Random::generate_random_float_0_to_1() * rooftops.size());
					const auto& end_point = rooftops[end_index];

					// Don't connect a building to itself
					if (start_point.x == end_point.x && start_point.z == end_point.z) continue;

					Line& skyway = add_line();

					// A skyway is a line between two rooftops
					skyway.x0.start = start_point.x;
					skyway.y0.start = start_point.y;
					skyway.z0.start = start_point.z;
					skyway.x1.start = end_point.x;
					skyway.y1.start = end_point.y;
					skyway.z1.start = end_point.z;

					// Style the skyway to look like an energy beam
					skyway.rgb_t0 = { 1.0f, 1.0f, 0.8f }; // Bright yellowish white
					skyway.thickness.start = 0.003f;
					skyway.number_of_cubes = 100;

					// --- ANIMATION: Skyway "shoots" from start to end ---
					skyway.copy_start_to_end();
					// Make the start state's endpoint equal to its startpoint
					skyway.x1.start = skyway.x0.start;
					skyway.y1.start = skyway.y0.start;
					skyway.z1.start = skyway.z0.start;
				}
			}
		}


		void init_0002_trefoil_weave()
		{
			lines.clear();

			// ======= Knobs (play!) =======
			const int   samples = 200;   // rings along the knot (centerline samples)
			const int   hoopSegs = 28;    // small chords per ring
			const int   longSegs = 24;    // longitudinal connections per ring
			const float tubeR = 0.18f; // base tube radius
			const float radiusWaveAmp = 0.25f; // tube radius modulation
			const int   radiusWaveFreq = 5;     // how many bulges along the knot
			const float helixAdvance = 0.35f; // radians advanced per ring (weave skew)
			const float baseThick = 0.006f;
			const int   cubesHoop = 24;    // resolution of hoop segments
			const int   cubesLong = 36;    // resolution of longitudinal segments
			const float chordFrac = 0.6f;  // fraction of ring step spanned by a chord
			const float TAU = 6.28318530718f;

			auto cos01 = [](float x) { return 0.5f + 0.5f * std::cos(x); };
			auto hueColor = [&](float h)->Vec3 {
				return Vec3{
					0.5f + 0.5f * std::cos(TAU * (h + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.66f))
				};
				};

			auto length3 = [](const Vec3& v) { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); };
			auto normalize3 = [&](const Vec3& v) {
				float L = length3(v);
				if (L < 1e-8f) return Vec3{ 0,0,0 };
				return Vec3{ v.x / L, v.y / L, v.z / L };
				};
			auto cross3 = [](const Vec3& a, const Vec3& b) {
				return Vec3{
					a.y * b.z - a.z * b.y,
					a.z * b.x - a.x * b.z,
					a.x * b.y - a.y * b.x
				};
				};

			// (2,3) torus trefoil centerline (scaled to ~unit scene)
			auto center = [&](float t)->Vec3 {
				// classic torus knot form
				float s = 0.30f; // overall scale
				float x = (2.0f + std::cos(3 * t)) * std::cos(2 * t);
				float y = (2.0f + std::cos(3 * t)) * std::sin(2 * t);
				float z = std::sin(3 * t);
				return Vec3{ s * x, s * y, s * z };
				};

			auto tangent = [&](float t)->Vec3 {
				const float dt = 0.0008f * TAU;
				Vec3 a = center(t - dt), b = center(t + dt);
				return normalize3(Vec3{ b.x - a.x, b.y - a.y, b.z - a.z });
				};

			auto frameNB = [&](float t)->std::pair<Vec3, Vec3> {
				Vec3 T = tangent(t);
				Vec3 up{ 0,1,0 };
				if (std::fabs(T.x * up.x + T.y * up.y + T.z * up.z) > 0.98f) up = Vec3{ 1,0,0 }; // avoid degeneracy
				Vec3 N = normalize3(cross3(up, T));
				Vec3 B = normalize3(cross3(T, N));
				return { N,B };
				};

			// ======= Build rings =======
			for (int i = 0; i < samples; ++i)
			{
				float u = float(i) / float(samples); // 0..1
				float t = u * TAU;
				Vec3  C = center(t);
				auto  NB = frameNB(t);
				Vec3  N = NB.first;
				Vec3  B = NB.second;

				// “breathing” tube radius to avoid uniformity
				float r = tubeR * (0.75f + radiusWaveAmp * cos01(radiusWaveFreq * t));

				// small random phase jitter per ring
				float jitter = 0.12f * Random::generate_random_float_minus_one_to_plus_one();
				float ringPhase = t * 0.5f + jitter; // modest twist of the cross-section along the knot

				// ---------- HOOP CHORDS (around the tube) ----------
				const float dTheta = TAU / float(hoopSegs);
				const float chord = chordFrac * dTheta;

				for (int k = 0; k < hoopSegs; ++k)
				{
					float theta = k * dTheta + ringPhase;

					Vec3 Q0{ C.x + r * (std::cos(theta) * N.x + std::sin(theta) * B.x),
							 C.y + r * (std::cos(theta) * N.y + std::sin(theta) * B.y),
							 C.z + r * (std::cos(theta) * N.z + std::sin(theta) * B.z) };

					Vec3 Q1{ C.x + r * (std::cos(theta + chord) * N.x + std::sin(theta + chord) * B.x),
							 C.y + r * (std::cos(theta + chord) * N.y + std::sin(theta + chord) * B.y),
							 C.z + r * (std::cos(theta + chord) * N.z + std::sin(theta + chord) * B.z) };

					Vec3 mid{ 0.5f * (Q0.x + Q1.x), 0.5f * (Q0.y + Q1.y), 0.5f * (Q0.z + Q1.z) };

					Line& L = add_line();

					// start collapsed at midpoint, expand to chord — nice reveal
					L.x0.start = mid.x; L.y0.start = mid.y; L.z0.start = mid.z;
					L.x1.start = mid.x; L.y1.start = mid.y; L.z1.start = mid.z;

					// color: blend ring position and local angle for a woven vibe
					float hue = std::fmod(u + 0.15f * float(k) / float(hoopSegs), 1.0f);
					L.rgb_t0 = hueColor(hue);
					L.thickness.start = baseThick * 0.25f;
					L.number_of_cubes = cubesHoop;

					L.copy_start_to_end();

					L.x0.end = Q0.x; L.y0.end = Q0.y; L.z0.end = Q0.z;
					L.x1.end = Q1.x; L.y1.end = Q1.y; L.z1.end = Q1.z;

					L.rgb_t1 = hueColor(std::fmod(hue + 0.12f, 1.0f));
					L.thickness.end = baseThick * (0.8f + 0.2f * std::sin(4.0f * theta));
				}

				// ---------- LONGITUDINAL HELICAL LINKS (between rings) ----------
				// Connect this ring to the next with a phase advance (weave)
				int iNext = (i + 1) % samples;
				float u2 = float(iNext) / float(samples);
				float t2 = u2 * TAU;
				Vec3  C2 = center(t2);
				auto  NB2 = frameNB(t2);
				Vec3  N2 = NB2.first, B2 = NB2.second;
				float r2 = tubeR * (0.75f + radiusWaveAmp * cos01(radiusWaveFreq * t2));
				float ringPhase2 = t2 * 0.5f + jitter; // keep same jitter variable for small continuity

				const float dPhi = TAU / float(longSegs);
				for (int m = 0; m < longSegs; ++m)
				{
					float phi = m * dPhi + ringPhase;
					float phi2 = phi + helixAdvance + (ringPhase2 - ringPhase); // keep weave smooth across the seam

					Vec3 P0{ C.x + r * (std::cos(phi) * N.x + std::sin(phi) * B.x),
							 C.y + r * (std::cos(phi) * N.y + std::sin(phi) * B.y),
							 C.z + r * (std::cos(phi) * N.z + std::sin(phi) * B.z) };

					Vec3 P1{ C2.x + r2 * (std::cos(phi2) * N2.x + std::sin(phi2) * B2.x),
							 C2.y + r2 * (std::cos(phi2) * N2.y + std::sin(phi2) * B2.y),
							 C2.z + r2 * (std::cos(phi2) * N2.z + std::sin(phi2) * B2.z) };

					// start close to centerline for reveal
					Line& L = add_line();

					L.x0.start = C.x;  L.y0.start = C.y;  L.z0.start = C.z;
					L.x1.start = C.x;  L.y1.start = C.y;  L.z1.start = C.z;

					float hue = std::fmod(0.5f * u + 0.25f * float(m) / float(longSegs), 1.0f);
					L.rgb_t0 = hueColor(hue);
					L.thickness.start = baseThick * 0.25f;
					L.number_of_cubes = cubesLong;

					L.copy_start_to_end();

					L.x0.end = P0.x; L.y0.end = P0.y; L.z0.end = P0.z;
					L.x1.end = P1.x; L.y1.end = P1.y; L.z1.end = P1.z;

					L.rgb_t1 = hueColor(std::fmod(hue + 0.08f, 1.0f));
					L.thickness.end = baseThick * (0.9f + 0.1f * std::sin(3.0f * phi));
				}
			}
		}

		void init_letter_morph()
		{
			lines.clear();

			// --- Letter Definition System ---
			// We define letters as a series of paired points (p0, p1, p0, p1, ...)
			std::unordered_map<char, std::vector<Vec3>> letter_definitions;
			size_t max_lines_per_letter = 0;

			// Define a few letters in a normalized [-0.5, 0.5] space
			// 'A' (3 lines)
			letter_definitions['A'] = {
				{-0.4f, -0.5f, 0.0f}, { 0.0f,  0.5f, 0.0f}, // Left leg
				{ 0.0f,  0.5f, 0.0f}, { 0.4f, -0.5f, 0.0f}, // Right leg
				{-0.25f, 0.0f, 0.0f}, { 0.25f, 0.0f, 0.0f}  // Crossbar
			};

			// 'X' (2 lines)
			letter_definitions['X'] = {
				{-0.4f, -0.5f, 0.0f}, { 0.4f,  0.5f, 0.0f},
				{ 0.4f, -0.5f, 0.0f}, {-0.4f,  0.5f, 0.0f}
			};

			// 'C' (3 lines, approximating a curve)
			letter_definitions['C'] = {
				{ 0.4f,  0.3f, 0.0f}, {-0.2f,  0.5f, 0.0f},
				{-0.2f,  0.5f, 0.0f}, {-0.4f,  0.0f, 0.0f},
				{-0.4f,  0.0f, 0.0f}, {-0.2f, -0.5f, 0.0f},
				{-0.2f, -0.5f, 0.0f}, { 0.4f, -0.3f, 0.0f}
			};

			// 'O' (4 lines, an octagon)
			letter_definitions['O'] = {
				{ 0.0f,  0.5f, 0.0f}, { 0.4f,  0.3f, 0.0f},
				{ 0.4f,  0.3f, 0.0f}, { 0.4f, -0.3f, 0.0f},
				{ 0.4f, -0.3f, 0.0f}, { 0.0f, -0.5f, 0.0f},
				{ 0.0f, -0.5f, 0.0f}, {-0.4f, -0.3f, 0.0f},
				{-0.4f, -0.3f, 0.0f}, {-0.4f,  0.3f, 0.0f},
				{-0.4f,  0.3f, 0.0f}, { 0.0f,  0.5f, 0.0f}
			};

			// --- Step 1: Find the max number of lines needed ---
			for (const auto& pair : letter_definitions) {
				max_lines_per_letter = std::max(max_lines_per_letter, pair.second.size() / 2);
			}

			// --- Step 2: Pad all letters to have the same number of lines ---
			Vec3 zero_point = { 0.0f, 0.0f, 0.0f };
			for (auto& pair : letter_definitions) {
				while (pair.second.size() / 2 < max_lines_per_letter) {
					pair.second.push_back(zero_point); // Add p0 for the zero-length line
					pair.second.push_back(zero_point); // Add p1 for the zero-length line
				}
			}

			// --- Step 3: Configure the Morph ---
			char start_char = 'C';
			char end_char = 'O';
			float scale = 1.2f; // Overall size of the letters

			Vec3 start_color = { 0.1f, 0.9f, 1.0f }; // Cyan
			Vec3 end_color = { 1.0f, 0.4f, 0.1f }; // Orange

			const auto& start_points = letter_definitions.at(start_char);
			const auto& end_points = letter_definitions.at(end_char);

			// --- Step 4: Create the morphing lines ---
			for (size_t i = 0; i < max_lines_per_letter; ++i)
			{
				Line& line = add_line();

				// Get the points for the i-th line of each letter
				const Vec3& p0_start = start_points[i * 2];
				const Vec3& p1_start = start_points[i * 2 + 1];
				const Vec3& p0_end = end_points[i * 2];
				const Vec3& p1_end = end_points[i * 2 + 1];

				// Set the START state of the animation (the first letter)
				line.x0.start = p0_start.x * scale;
				line.y0.start = p0_start.y * scale;
				line.z0.start = p0_start.z * scale;
				line.x1.start = p1_start.x * scale;
				line.y1.start = p1_start.y * scale;
				line.z1.start = p1_start.z * scale;

				// Set the END state of the animation (the second letter)
				line.x0.end = p0_end.x * scale;
				line.y0.end = p0_end.y * scale;
				line.z0.end = p0_end.z * scale;
				line.x1.end = p1_end.x * scale;
				line.y1.end = p1_end.y * scale;
				line.z1.end = p1_end.z * scale;

				// Set colors to morph as well
				line.rgb_t0 = start_color;
				line.rgb_t1 = end_color;

				// Styling
				line.thickness.start = 0.015f;
				line.thickness.end = 0.01f; // Taper slightly
				line.number_of_cubes = 100;
			}
		}

		// inside struct Lines
		void init_0003_floating_text()
		{
			lines.clear();

			// --- knobs ---
			const char* msg = "FLOATING";   // change me
			const float letterHeight = 0.70f;
			const float letterSpacing = 0.58f; // in letter-heights
			const float baseY = 0.0f;
			const float baseZ = 0.0f;
			const float floatLift = 0.18f;  // how much letters rise by end
			const float windAmp = 0.06f;    // sideways drift at end
			const float baseThick = 0.008f;
			const int   cubesPerStroke = 28;
			const float TAU = 6.28318530718f;

			auto hueColor = [&](float h)->Vec3 {
				return Vec3{
					0.5f + 0.5f * std::cos(TAU * (h + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.66f))
				};
				};

			struct V2 { float x, y; };
			using Stroke = std::pair<V2, V2>;

			auto push_vert = [](std::vector<Stroke>& S, float x, float y0, float y1) {
				S.push_back({ {x,y0},{x,y1} });
				};
			auto push_horz = [](std::vector<Stroke>& S, float y, float x0, float x1) {
				S.push_back({ {x0,y},{x1,y} });
				};
			auto push_diag = [](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) {
				S.push_back({ {x0,y0},{x1,y1} });
				};

			// normalized [0,1] vector strokes for a few letters
			auto strokesFor = [&](char c)->std::vector<Stroke>
				{
					std::vector<Stroke> S;
					const float m = 0.10f;           // margin
					const float l = m, r = 1.0f - m;
					const float b = m, t = 1.0f - m;
					const float mid = 0.5f, midHi = 0.62f, midLo = 0.38f;

					switch (c)
					{
					case 'F':
						push_vert(S, l, b, t);
						push_horz(S, t, l, r);
						push_horz(S, mid, l, 0.75f);
						break;
					case 'L':
						push_vert(S, l, b, t);
						push_horz(S, b, l, r);
						break;
					case 'O':
						push_horz(S, t, l, r);
						push_horz(S, b, l, r);
						push_vert(S, l, b, t);
						push_vert(S, r, b, t);
						break;
					case 'A': {
						float ax = 0.5f, ay = t;
						push_diag(S, l, b, ax, ay);
						push_diag(S, r, b, ax, ay);
						push_horz(S, midHi, l + 0.15f, r - 0.15f);
						break;
					}
					case 'T':
						push_horz(S, t, l, r);
						push_vert(S, mid, b, t);
						break;
					case 'I':
						push_vert(S, mid, b, t);
						push_horz(S, t, l + 0.2f, r - 0.2f);
						push_horz(S, b, l + 0.2f, r - 0.2f);
						break;
					case 'N':
						push_vert(S, l, b, t);
						push_vert(S, r, b, t);
						push_diag(S, l, b, r, t);
						break;
					case 'G':
						// like O, but open bite + inner arm
						push_horz(S, t, l, r);
						push_horz(S, b, l, r);
						push_vert(S, l, b, t);
						push_vert(S, r, mid, t);
						push_horz(S, mid, l + 0.40f, r);      // inner arm
						break;
					case ' ':
						break;
					default:
						// fallback: simple box
						push_horz(S, t, l, r);
						push_horz(S, b, l, r);
						push_vert(S, l, b, t);
						push_vert(S, r, b, t);
						break;
					}
					return S;
				};

			// center text horizontally
			int len = 0; while (msg[len] != '\0') ++len;
			const float totalW = (len - 1) * letterSpacing * letterHeight;

			for (int i = 0; i < len; ++i)
			{
				char c = std::toupper(msg[i]);
				auto strokes = strokesFor(c);

				// per-letter placement & vibe
				float xOffset = -0.5f * totalW + i * (letterSpacing * letterHeight);
				float zJitter = baseZ + 0.02f * Random::generate_random_float_minus_one_to_plus_one();
				float wind = windAmp * (0.5f + 0.5f * std::sin(0.7f * i)); // steady sideways drift at end
				float hue = std::fmod(0.12f * i + 0.02f * (c == ' ' ? 0 : 1), 1.0f);

				Vec3 col0 = hueColor(hue);
				Vec3 col1 = hueColor(std::fmod(hue + 0.10f, 1.0f));

				for (const auto& st : strokes)
				{
					// map normalized glyph to world
					auto mapPt = [&](V2 p)->Vec3 {
						float X = xOffset + (p.x - 0.5f) * letterHeight;
						float Y = baseY + (p.y - 0.5f) * letterHeight;
						return Vec3{ X, Y, zJitter };
						};

					Vec3 P0 = mapPt(st.first);
					Vec3 P1 = mapPt(st.second);
					Vec3 M = { 0.5f * (P0.x + P1.x), 0.5f * (P0.y + P1.y) - 0.22f * letterHeight, 0.5f * (P0.z + P1.z) }; // start collapsed & slightly low

					Line& L = add_line();

					// START collapsed
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;

					L.rgb_t0 = col0;
					L.thickness.start = baseThick * 0.25f;
					L.number_of_cubes = cubesPerStroke;

					L.copy_start_to_end();

					// END at actual stroke positions, with rise + wind
					L.x0.end = P0.x + wind; L.y0.end = P0.y + floatLift; L.z0.end = P0.z;
					L.x1.end = P1.x + wind; L.y1.end = P1.y + floatLift; L.z1.end = P1.z;

					L.rgb_t1 = col1;
					L.thickness.end = baseThick;
				}
			}
		}

		void init_spiral_galaxy()
		{
			int number_of_lines = 200;
			const float TAU = 6.2831853071795864769252867665590f;

			for (int i = 0; i < number_of_lines; i++)
			{
				Line& line = add_line();
				float angle = i * (TAU / number_of_lines);
				float radius = 0.1f + 0.4f * (i / float(number_of_lines));

				// Start at center, end at spiral position
				line.x0.start = 0.0f;
				line.y0.start = 0.0f;
				line.z0.start = 0.0f;

				line.x1.start = radius * sin(angle);
				line.y1.start = 0.0f;
				line.z1.start = radius * cos(angle);

				// End points create spiral arms
				line.x1.end = (radius + 0.1f) * sin(angle + 0.3f);
				line.y1.end = 0.0f;
				line.z1.end = (radius + 0.1f) * cos(angle + 0.3f);

				// Color gradient from center (white) to edges (blue/purple)
				float t = i / float(number_of_lines);
				line.rgb_t0.x = 0.8f * (1.0f - t);
				line.rgb_t0.y = 0.6f * (1.0f - t);
				line.rgb_t0.z = 0.9f;

				line.thickness.start = 0.005f + 0.01f * (1.0f - t);
				line.number_of_cubes = 50;
			}
		}

		void init_wireframe_sphere()
		{
			int rings = 20;
			int sectors = 40;

			const float PI = 3.14159265359f;

			for (int r = 0; r < rings; r++) {
				for (int s = 0; s < sectors; s++) {
					Line& line = add_line();

					// Calculate spherical coordinates
					float theta1 = r * PI / rings;
					float theta2 = (r + 1) * PI / rings;
					float phi = s * 2 * PI / sectors;

					// Latitude lines (horizontal rings)
					line.x0.start = sin(theta1) * cos(phi);
					line.y0.start = cos(theta1);
					line.z0.start = sin(theta1) * sin(phi);

					line.x1.start = sin(theta1) * cos(phi + 2 * PI / sectors);
					line.y1.start = cos(theta1);
					line.z1.start = sin(theta1) * sin(phi + 2 * PI / sectors);

					// Longitude lines (vertical meridians)
					line.x1.end = sin(theta2) * cos(phi);
					line.y1.end = cos(theta2);
					line.z1.end = sin(theta2) * sin(phi);

					line.copy_start_to_end(); // For the horizontal part

					// Rainbow colors based on position
					line.rgb_t0.x = 0.5f + 0.5f * sin(phi);
					line.rgb_t0.y = 0.5f + 0.5f * cos(theta1);
					line.rgb_t0.z = 0.5f + 0.5f * sin(theta1 + phi);

					line.thickness.start = 0.008f;
					line.number_of_cubes = 30;
				}
			}
		}

		void init_particle_fountain()
		{
			int number_of_particles = 150;

			const float M_PI = 3.14159265359f;

			for (int i = 0; i < number_of_particles; i++)
			{
				Line& line = add_line();

				// All particles start at fountain base
				line.x0.start = 0.0f;
				line.y0.start = -0.5f;
				line.z0.start = 0.0f;

				// Random upward trajectory with spread
				float angle = Random::generate_random_float_0_to_1() * 2 * M_PI;
				float spread = 0.3f + 0.7f * Random::generate_random_float_0_to_1();
				float height = 0.8f + 0.4f * Random::generate_random_float_0_to_1();

				line.x1.start = spread * cos(angle);
				line.y1.start = height;
				line.z1.start = spread * sin(angle);

				// End points create falling effect
				line.x1.end = spread * 1.5f * cos(angle + 0.2f);
				line.y1.end = -0.2f;
				line.z1.end = spread * 1.5f * sin(angle + 0.2f);

				// Fire-like colors (orange to yellow)
				float fire_t = Random::generate_random_float_0_to_1();
				line.rgb_t0.x = 0.8f + 0.2f * fire_t;
				line.rgb_t0.y = 0.4f + 0.4f * fire_t;
				line.rgb_t0.z = 0.1f + 0.1f * fire_t;

				line.thickness.start = 0.005f;
				line.number_of_cubes = 80;
			}
		}


		void init_dna_helix()
		{
			int number_of_lines = 120;
			const float TAU = 6.2831853071795864769252867665590f;
			const float M_PI = 3.14159265359f;;

			for (int i = 0; i < number_of_lines; i++)
			{
				Line& line = add_line();
				float t = i / float(number_of_lines);
				float angle = t * TAU * 4.0f; // 4 full rotations
				float height = -0.8f + 1.6f * t;

				// First strand
				line.x0.start = 0.3f * cos(angle);
				line.y0.start = height;
				line.z0.start = 0.3f * sin(angle);

				// Second strand (offset by 180 degrees)
				line.x1.start = 0.3f * cos(angle + M_PI);
				line.y1.start = height;
				line.z1.start = 0.3f * sin(angle + M_PI);

				// Connect to next rung
				float next_angle = (t + 1.0f / number_of_lines) * TAU * 4.0f;
				float next_height = -0.8f + 1.6f * (t + 1.0f / number_of_lines);

				line.x1.end = 0.3f * cos(next_angle + M_PI);
				line.y1.end = next_height;
				line.z1.end = 0.3f * sin(next_angle + M_PI);

				line.copy_start_to_end(); // This might need adjustment

				// Alternating red/blue for base pairs
				if (i % 2 == 0) {
					line.rgb_t0 = { 0.8f, 0.2f, 0.2f }; // Red
				}
				else {
					line.rgb_t0 = { 0.2f, 0.2f, 0.8f }; // Blue
				}

				line.thickness.start = 0.012f;
				line.number_of_cubes = 20;
			}
		}


		void init_0004_letter_matrix_fly()
		{
			lines.clear();

			// ===== knobs =====
			const char* msg = "FLOATING  "; // tiled across the grid (add spaces to vary)
			const int   rows = 10;
			const int   cols = 18;
			const float letterH = 0.22f;   // letter height
			const float spacing = 0.26f;   // cell spacing in world units (relative to letterH)
			const float jitterXY = 0.03f;  // small start jitter
			const float baseZ = 0.0f;

			const float rise = 0.80f;  // how high letters lift by end
			const float scatterR = 1.40f;  // how far they scatter radially
			const float swirlAmp = 1.60f;  // swirl intensity around Y
			const float yawSpin = 1.10f;  // extra per-letter yaw at end (radians)
			const float rollSpin = 0.45f;  // subtle Z roll for fun

			const float baseThick = 0.0075f;
			const int   cubesPerStroke = 24;
			const float TAU = 6.28318530718f;

			// tiny helpers
			auto hueColor = [&](float h)->Vec3 {
				return Vec3{
					0.5f + 0.5f * std::cos(TAU * (h + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.66f))
				};
				};
			auto rotY = [](const Vec3& p, float a)->Vec3 {
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z };
				};
			auto rotZ = [](const Vec3& p, float a)->Vec3 {
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z };
				};

			struct V2 { float x, y; };
			using Stroke = std::pair<V2, V2>;

			auto push_v = [](std::vector<Stroke>& S, float x, float y0, float y1) {
				S.push_back({ {x,y0},{x,y1} });
				};
			auto push_h = [](std::vector<Stroke>& S, float y, float x0, float x1) {
				S.push_back({ {x0,y},{x1,y} });
				};
			auto push_d = [](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) {
				S.push_back({ {x0,y0},{x1,y1} });
				};

			// minimalist vector strokes (normalized [0,1]) for common letters; others = box fallback
			auto strokesFor = [&](char c)->std::vector<Stroke> {
				std::vector<Stroke> S;
				const float m = 0.10f, l = m, r = 1.0f - m, b = m, t = 1.0f - m, mid = 0.5f, hi = 0.62f;
				switch (std::toupper(c))
				{
				case 'F': push_v(S, l, b, t); push_h(S, t, l, r); push_h(S, mid, l, 0.75f); break;
				case 'L': push_v(S, l, b, t); push_h(S, b, l, r); break;
				case 'O': push_h(S, t, l, r); push_h(S, b, l, r); push_v(S, l, b, t); push_v(S, r, b, t); break;
				case 'A': push_d(S, l, b, 0.5f, t); push_d(S, r, b, 0.5f, t); push_h(S, hi, l + 0.15f, r - 0.15f); break;
				case 'T': push_h(S, t, l, r); push_v(S, mid, b, t); break;
				case 'I': push_v(S, mid, b, t); push_h(S, t, l + 0.2f, r - 0.2f); push_h(S, b, l + 0.2f, r - 0.2f); break;
				case 'N': push_v(S, l, b, t); push_v(S, r, b, t); push_d(S, l, b, r, t); break;
				case 'G': push_h(S, t, l, r); push_h(S, b, l, r); push_v(S, l, b, t); push_v(S, r, 0.45f, t); push_h(S, 0.45f, l + 0.40f, r); break;
				case 'X': push_d(S, l, b, r, t); push_d(S, l, t, r, b); break;
				case 'Y': push_d(S, l, t, mid, 0.58f); push_d(S, r, t, mid, 0.58f); push_v(S, mid, b, 0.58f); break;
				case 'H': push_v(S, l, b, t); push_v(S, r, b, t); push_h(S, mid, l, r); break;
				case 'E': push_v(S, l, b, t); push_h(S, t, l, r); push_h(S, mid, l, 0.75f); push_h(S, b, l, r); break;
				case 'S': push_h(S, t, l, r); push_h(S, mid, l, 0.65f); push_h(S, b, l, r); push_v(S, l, mid, b); push_v(S, r, t, mid); break;
				case 'M': push_v(S, l, b, t); push_v(S, r, b, t); push_d(S, l, t, mid, b + 0.25f); push_d(S, r, t, mid, b + 0.25f); break;
				case 'W': push_v(S, l, b, t); push_v(S, r, b, t); push_d(S, l, b, mid, t - 0.25f); push_d(S, r, b, mid, t - 0.25f); break;
				case 'U': push_v(S, l, b, t); push_v(S, r, b, t); push_h(S, b, l, r); break;
				case 'V': push_d(S, l, t, 0.5f, b); push_d(S, r, t, 0.5f, b); break;
				case 'Z': push_h(S, t, l, r); push_h(S, b, l, r); push_d(S, l, b, r, t); break;
				case ' ': break;
				default:  // box fallback
					push_h(S, t, l, r); push_h(S, b, l, r); push_v(S, l, b, t); push_v(S, r, b, t);
				}
				return S;
				};

			// layout
			int msgLen = 0; while (msg[msgLen] != '\0') ++msgLen;
			const float cell = letterH / spacing;       // spacing normalized to height
			const float gridW = (cols - 1) * cell;
			const float gridH = (rows - 1) * cell;

			// build letters
			int idx = 0;
			for (int r = 0; r < rows; ++r)
				for (int c = 0; c < cols; ++c)
				{
					char ch = msg[idx++ % msgLen];
					auto strokes = strokesFor(ch);

					// center grid around origin
					float cx = -0.5f * gridW + c * cell;
					float cy = 0.5f * gridH - r * cell;

					// per-letter jitter & seeds
					float jx = jitterXY * Random::generate_random_float_minus_one_to_plus_one();
					float jy = jitterXY * Random::generate_random_float_minus_one_to_plus_one();
					float jz = 0.02f * Random::generate_random_float_minus_one_to_plus_one();

					// color by grid position
					float hue = std::fmod(0.12f * r + 0.07f * c, 1.0f);
					Vec3 c0 = hueColor(hue);
					Vec3 c1 = hueColor(std::fmod(hue + 0.10f, 1.0f));

					// compute end (flight) transform for this letter
					Vec3 centerStart{ cx + jx, cy + jy, baseZ + jz };

					// radial direction from origin (xz uses cx, z based on c; y handled via rise)
					Vec3 radial{ centerStart.x, 0.0f, 0.35f * (c - (cols - 1) * 0.5f) * cell };
					float rl = std::sqrt(radial.x * radial.x + radial.z * radial.z) + 1e-6f;
					radial.x /= rl; radial.z /= rl;

					float swirl = swirlAmp * (0.35f * r);           // more swirl higher up
					float yaw = yawSpin * (0.3f + 0.7f * (r / (float)rows)); // more spin for upper rows
					float roll = rollSpin * (Random::generate_random_float_minus_one_to_plus_one());

					// END transform: translate (scatter + rise) and rotate (yaw around Y, then roll in-plane)
					auto toEnd = [&](const Vec3& p, const Vec3& letterCenter)->Vec3 {
						// move to local
						Vec3 q{ p.x - letterCenter.x, p.y - letterCenter.y, p.z - letterCenter.z };
						// in-plane roll (Z)
						q = rotZ(q, roll);
						// yaw around Y (gives 3D parallax)
						q = rotY(q, yaw + swirl);
						// translate: rise and radial scatter
						Vec3 t{
							letterCenter.x + q.x + radial.x * scatterR,
							letterCenter.y + q.y + rise,
							letterCenter.z + q.z + radial.z * scatterR
						};
						return t;
						};

					// draw strokes
					for (const auto& st : strokes)
					{
						// map glyph [0,1]^2 to world in this cell
						auto mapPt = [&](V2 p)->Vec3 {
							float X = centerStart.x + (p.x - 0.5f) * letterH;
							float Y = centerStart.y + (p.y - 0.5f) * letterH;
							float Z = centerStart.z;
							return Vec3{ X, Y, Z };
							};

						Vec3 P0 = mapPt(st.first);
						Vec3 P1 = mapPt(st.second);
						Vec3 M = { 0.5f * (P0.x + P1.x), 0.5f * (P0.y + P1.y), 0.5f * (P0.z + P1.z) };

						Line& L = add_line();

						// START: exactly on grid (legible)
						L.x0.start = P0.x; L.y0.start = P0.y; L.z0.start = P0.z;
						L.x1.start = P1.x; L.y1.start = P1.y; L.z1.start = P1.z;

						L.rgb_t0 = c0;
						L.thickness.start = baseThick;
						L.number_of_cubes = cubesPerStroke;

						L.copy_start_to_end();

						// END: flown & spun
						Vec3 Cend = toEnd(M, centerStart); // move midpoint, but we need endpoints transformed about center
						// transform endpoints relative to letterCenter for consistent rotation
						Vec3 E0 = toEnd(P0, centerStart);
						Vec3 E1 = toEnd(P1, centerStart);

						L.x0.end = E0.x; L.y0.end = E0.y; L.z0.end = E0.z;
						L.x1.end = E1.x; L.y1.end = E1.y; L.z1.end = E1.z;

						L.rgb_t1 = c1;
						L.thickness.end = baseThick * 0.75f; // slight taper on fly
					}
				}
		}

		void init_quantum_foam_nebula()
		{
			// Self-contained constants
			const float TAU = 6.283185307179586476925286766559f; // 2 * PI
			const float PI = 3.141592653589793238462643383279f;

			const int total_lines = 1200;
			const int octaves = 4; // Noise complexity layers

			for (int i = 0; i < total_lines; ++i)
			{
				Line& line = add_line();

				// Random base direction on unit sphere (using rejection sampling for uniformity)
				float x, y, z, len;
				do {
					x = Random::generate_random_float_minus_one_to_plus_one();
					y = Random::generate_random_float_minus_one_to_plus_one();
					z = Random::generate_random_float_minus_one_to_plus_one();
					len = x * x + y * y + z * z;
				} while (len > 1.0f || len < 1e-6f);
				len = sqrtf(len);
				x /= len; y /= len; z /= len;

				// Base radius and phase
				float base_radius = 0.3f + 0.5f * Random::generate_random_float_0_to_1();
				float global_phase = TAU * Random::generate_random_float_0_to_1();

				// Multi-octave displacement (procedural turbulence)
				float dx_start = 0.0f, dy_start = 0.0f, dz_start = 0.0f;
				float dx_end = 0.0f, dy_end = 0.0f, dz_end = 0.0f;
				float amplitude = 1.0f;
				float frequency = 1.0f;

				for (int o = 0; o < octaves; ++o)
				{
					float angle_start = frequency * (i * 0.13f + global_phase);
					float angle_end = frequency * (i * 0.17f + global_phase + PI * 0.5f);

					// Use different axes for richer 3D motion
					dx_start += amplitude * sinf(angle_start) * cosf(angle_start * 1.618f);
					dy_start += amplitude * cosf(angle_start * 2.414f);
					dz_start += amplitude * sinf(angle_start * 3.141f + PI / 3.0f);

					dx_end += amplitude * cosf(angle_end) * sinf(angle_end * 1.618f);
					dy_end += amplitude * sinf(angle_end * 2.414f + PI / 4.0f);
					dz_end += amplitude * cosf(angle_end * 3.141f);

					amplitude *= 0.5f;
					frequency *= 2.1f; // Non-integer for non-repeating patterns
				}

				// Normalize displacement magnitude
				float disp_scale = 0.12f;
				dx_start *= disp_scale; dy_start *= disp_scale; dz_start *= disp_scale;
				dx_end *= disp_scale; dy_end *= disp_scale; dz_end *= disp_scale;

				// Start point: origin + radial base + displacement
				line.x0.start = dx_start;
				line.y0.start = dy_start;
				line.z0.start = dz_start;

				// End point: sphere surface + animated displacement
				line.x1.start = base_radius * x + dx_start;
				line.y1.start = base_radius * y + dy_start;
				line.z1.start = base_radius * z + dz_start;

				// Animated end position (pulsing + swirling)
				float pulse = 0.8f + 0.2f * sinf(global_phase + i * 0.01f);
				float swirl_factor = 0.3f + 0.7f * Random::generate_random_float_0_to_1();

				// Rotate around Y-axis over "time" (encoded in phase)
				float cos_swirl = cosf(swirl_factor * global_phase);
				float sin_swirl = sinf(swirl_factor * global_phase);
				float x_rot = x * cos_swirl - z * sin_swirl;
				float z_rot = x * sin_swirl + z * cos_swirl;

				line.x1.end = pulse * base_radius * x_rot + dx_end;
				line.y1.end = pulse * base_radius * y + dy_end;
				line.z1.end = pulse * base_radius * z_rot + dz_end;

				// Color: based on 3D position + harmonic phase (vivid nebula palette)
				float hue = fmodf(global_phase + (x + y + z) * 2.0f, TAU);
				float saturation = 0.7f + 0.3f * Random::generate_random_float_0_to_1();
				float value = 0.4f + 0.5f * (sinf(hue * 3.0f) * 0.5f + 0.5f);

				// Simple HSV  RGB (approximate, but fast and colorful)
				float h = hue / TAU * 6.0f;
				float c = value * saturation;
				float x_col = c * (1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
				float m = value - c;

				float r = 0, g = 0, b = 0;
				if (h < 1.0f) { r = c; g = x_col; }
				else if (h < 2.0f) { r = x_col; g = c; }
				else if (h < 3.0f) { g = c; b = x_col; }
				else if (h < 4.0f) { g = x_col; b = c; }
				else if (h < 5.0f) { r = x_col; b = c; }
				else { r = c; b = x_col; }

				line.rgb_t0.x = r + m;
				line.rgb_t0.y = g + m;
				line.rgb_t0.z = b + m;

				// Thickness varies with depth and energy
				float depth_factor = 0.5f + 0.5f * (y + 1.0f) * 0.5f; // Brighter near "equator"
				line.thickness.start = 0.004f + 0.008f * depth_factor * (1.0f + sinf(global_phase * 2.0f)) * 0.5f;

				line.number_of_cubes = 60 + int(40.0f * Random::generate_random_float_0_to_1());
			}
		}


		// inside struct Lines
		void init_0005_letter_layers_fly_yz_swapped()
		{
			lines.clear();

			// ===== knobs =====
			const char* msg = "MATH  "; // tiled across the grid
			const int   rows = 9;
			const int   cols = 16;
			const int   layers = 5;          // stacked depth layers along Y
			const float layerGap = 0.24f;    // spacing between layers (Y axis)

			const float letterH = 0.20f;     // letter height (now along Z)
			const float spacing = 0.26f;     // grid spacing factor
			const float jitterXZ = 0.02f;    // small start jitter in X/Z
			const float jitterY = 0.02f;    // small start jitter in Y

			const float riseZ = 0.80f;   // how high letters lift by end (Z axis)
			const float scatterR = 1.25f;   // radial scatter in X–Y plane
			const float swirlAmpZ = 1.50f;   // swirl intensity around Z
			const float yawY = 0.90f;   // yaw around Y per letter at end
			const float rollX = 0.35f;   // subtle roll around X

			const float baseThick = 0.0070f;
			const int   cubesPerStroke = 24;
			const float TAU = 6.28318530718f;

			// helpers
			auto hueColor = [&](float h)->Vec3 {
				return Vec3{
					0.5f + 0.5f * std::cos(TAU * (h + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (h + 0.66f))
				};
				};
			auto rotZ = [](const Vec3& p, float a)->Vec3 {
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z };
				};
			auto rotY = [](const Vec3& p, float a)->Vec3 {
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z };
				};
			auto rotX = [](const Vec3& p, float a)->Vec3 {
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ p.x, c * p.y - s * p.z, s * p.y + c * p.z };
				};

			struct V2 { float x, y; };
			using Stroke = std::pair<V2, V2>;

			auto push_v = [](std::vector<Stroke>& S, float x, float y0, float y1) {
				S.push_back({ {x,y0},{x,y1} });
				};
			auto push_h = [](std::vector<Stroke>& S, float y, float x0, float x1) {
				S.push_back({ {x0,y},{x1,y} });
				};
			auto push_d = [](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) {
				S.push_back({ {x0,y0},{x1,y1} });
				};

			// vector strokes (normalized [0,1]); add more as needed
			auto strokesFor = [&](char c)->std::vector<Stroke> {
				std::vector<Stroke> S;
				// uppercase without <cctype>
				char C = (c >= 'a' && c <= 'z') ? char(c - 'a' + 'A') : c;
				const float m = 0.10f, l = m, r = 1.0f - m, b = m, t = 1.0f - m, mid = 0.5f, hi = 0.62f;
				switch (C)
				{
				case 'M': push_v(S, l, b, t); push_v(S, r, b, t); push_d(S, l, t, mid, b + 0.25f); push_d(S, r, t, mid, b + 0.25f); break;
				case 'A': push_d(S, l, b, 0.5f, t); push_d(S, r, b, 0.5f, t); push_h(S, hi, l + 0.15f, r - 0.15f); break;
				case 'T': push_h(S, t, l, r); push_v(S, mid, b, t); break;
				case 'H': push_v(S, l, b, t); push_v(S, r, b, t); push_h(S, mid, l, r); break;
				case 'F': push_v(S, l, b, t); push_h(S, t, l, r); push_h(S, mid, l, 0.75f); break;
				case 'L': push_v(S, l, b, t); push_h(S, b, l, r); break;
				case 'O': push_h(S, t, l, r); push_h(S, b, l, r); push_v(S, l, b, t); push_v(S, r, b, t); break;
				case 'I': push_v(S, mid, b, t); push_h(S, t, l + 0.2f, r - 0.2f); push_h(S, b, l + 0.2f, r - 0.2f); break;
				case 'N': push_v(S, l, b, t); push_v(S, r, b, t); push_d(S, l, b, r, t); break;
				case 'G': push_h(S, t, l, r); push_h(S, b, l, r); push_v(S, l, b, t); push_v(S, r, 0.45f, t); push_h(S, 0.45f, l + 0.40f, r); break;
				case ' ': break;
				default:  // box fallback
					push_h(S, t, l, r); push_h(S, b, l, r); push_v(S, l, b, t); push_v(S, r, b, t);
				}
				return S;
				};

			// grid layout in X–Z plane (Z is vertical)
			int msgLen = 0; while (msg[msgLen] != '\0') ++msgLen;
			const float cell = letterH / spacing;
			const float gridW = (cols - 1) * cell;
			const float gridH = (rows - 1) * cell;

			int idx = 0;
			for (int L = 0; L < layers; ++L)
			{
				float layerY = (L - 0.5f * (layers - 1)) * layerGap;

				for (int r = 0; r < rows; ++r)
					for (int c = 0; c < cols; ++c)
					{
						char ch = msg[idx++ % msgLen];
						auto strokes = strokesFor(ch);

						// center grid around origin in X–Z
						float cx = -0.5f * gridW + c * cell;
						float cz = 0.5f * gridH - r * cell; // Z up

						// per-letter jitter
						float jx = jitterXZ * Random::generate_random_float_minus_one_to_plus_one();
						float jy = jitterY * Random::generate_random_float_minus_one_to_plus_one();
						float jz = jitterXZ * Random::generate_random_float_minus_one_to_plus_one();

						// start center (plane X–Z, layered in Y)
						Vec3 centerStart{ cx + jx, layerY + jy, cz + jz };

						// color by layer/row/col
						float hue = std::fmod(0.09f * L + 0.08f * r + 0.05f * c, 1.0f);
						Vec3 c0 = hueColor(hue);
						Vec3 c1 = hueColor(std::fmod(hue + 0.10f, 1.0f));

						// end transform: swirl around Z, then yaw around Y, roll around X; rise on Z; scatter in X–Y
						float swirl = swirlAmpZ * (0.25f + 0.75f * (r / float(std::max(1, rows - 1))));
						float yaw = yawY * (0.35f + 0.65f * (L / float(std::max(1, layers - 1))));
						float roll = rollX * Random::generate_random_float_minus_one_to_plus_one();

						// radial in X–Y plane
						Vec3 radial{ centerStart.x, centerStart.y, 0.0f };
						float rl = std::sqrt(radial.x * radial.x + radial.y * radial.y) + 1e-6f;
						radial.x /= rl; radial.y /= rl;

						auto toEnd = [&](const Vec3& p, const Vec3& letterCenter)->Vec3 {
							Vec3 q{ p.x - letterCenter.x, p.y - letterCenter.y, p.z - letterCenter.z };
							q = rotZ(q, swirl);
							q = rotY(q, yaw);
							q = rotX(q, roll);
							return Vec3{
								letterCenter.x + q.x + radial.x * scatterR,
								letterCenter.y + q.y + radial.y * scatterR,
								letterCenter.z + q.z + riseZ
							};
							};

						// draw strokes (glyph plane is X–Z; map V2.y to Z)
						for (const auto& st : strokes)
						{
							auto mapPt = [&](V2 p)->Vec3 {
								float X = centerStart.x + (p.x - 0.5f) * letterH;
								float Z = centerStart.z + (p.y - 0.5f) * letterH; // Z is vertical on glyph
								float Y = centerStart.y;                           // layer depth
								return Vec3{ X, Y, Z };
								};

							Vec3 P0 = mapPt(st.first);
							Vec3 P1 = mapPt(st.second);

							Line& Ln = add_line();

							// START: legible matrix in X–Z plane, layered in Y
							Ln.x0.start = P0.x; Ln.y0.start = P0.y; Ln.z0.start = P0.z;
							Ln.x1.start = P1.x; Ln.y1.start = P1.y; Ln.z1.start = P1.z;

							Ln.rgb_t0 = c0;
							Ln.thickness.start = baseThick;
							Ln.number_of_cubes = cubesPerStroke;

							Ln.copy_start_to_end();

							// END: fly & swirl
							Vec3 E0 = toEnd(P0, centerStart);
							Vec3 E1 = toEnd(P1, centerStart);

							Ln.x0.end = E0.x; Ln.y0.end = E0.y; Ln.z0.end = E0.z;
							Ln.x1.end = E1.x; Ln.y1.end = E1.y; Ln.z1.end = E1.z;

							Ln.rgb_t1 = c1;
							Ln.thickness.end = baseThick * 0.75f;
						}
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

			if (enable_shader_20) // sphered
			{
				Spheres sphere;
				sphere.init_1(10);
				sphere.draw(scene, 1000);
			}

			if (enable_shader_21) // lines
			{
				Lines lines;
				// lines.init();
				// lines.init_sphere();
				// lines.init_helix();
				// lines.init_vortex();
				// lines.init_torus();
				// lines.init_0000_swirling_vortex(),
				// lines.init_random_walk_filaments();
				// lines.init_0001_random_walk_bloom();
				// lines.init_abstract_cityscape();
				// lines.init_0002_trefoil_weave();
				// lines.init_letter_morph();
				// lines.init_0003_floating_text();
				// lines.init_spiral_galaxy();
				// lines.init_wireframe_sphere();
				// lines.init_particle_fountain();
				// lines.init_dna_helix();
				// lines.init_0004_letter_matrix_fly();
				// lines.init_quantum_foam_nebula();
				lines.init_0005_letter_layers_fly_yz_swapped();


				lines.draw(scene);
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
		const bool enable_shader_20 = true;

		const bool enable_shader_21 = true;

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
	std::cout << "Universe\n";

	Universe_::generate();

	return 0;
}