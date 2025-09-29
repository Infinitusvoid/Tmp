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


		void init_fractal_tree_3d()
		{
			const float M_PI = 3.14159265359f;
			int max_depth = 6;

			auto add_leaf_cluster = [&](float x, float y, float z)
				{
					const float M_PI = 3.14159265359f;

					for (int i = 0; i < 8; i++) {
						Line& leaf = add_line();

						float angle = i * (2.0f * M_PI / 8.0f);
						float leaf_length = 0.05f + 0.02f * sin(i * 2.0f);

						leaf.x0.start = x;
						leaf.y0.start = y;
						leaf.z0.start = z;

						leaf.x1.start = x + leaf_length * sin(angle);
						leaf.y1.start = y + leaf_length * 0.3f;
						leaf.z1.start = z + leaf_length * cos(angle);

						// Vibrant leaf colors
						leaf.rgb_t0.x = 0.1f + 0.3f * sin(i);
						leaf.rgb_t0.y = 0.6f + 0.3f * cos(i * 0.7f);
						leaf.rgb_t0.z = 0.1f + 0.2f * sin(i * 1.3f);

						leaf.thickness.start = 0.008f;
						leaf.number_of_cubes = 8;

						leaf.copy_start_to_end();

						// Animate leaves - gentle floating motion
						leaf.x1.end = x + leaf_length * sin(angle + 0.5f);
						leaf.y1.end = y + leaf_length * 0.5f;
						leaf.z1.end = z + leaf_length * cos(angle + 0.5f);
					}
				};

			std::vector<std::tuple<float, float, float, float, float, float, float, int>> branches;
			// x, y, z, angle_xy, angle_z, length, thickness, depth

			// Start with trunk
			branches.push_back({ 0.0f, -0.8f, 0.0f, 0.0f, 0.0f, 0.4f, 0.03f, 0 });

			for (int depth = 0; depth < max_depth; depth++) {
				int current_size = branches.size();
				for (int i = 0; i < current_size; i++) {
					auto [x, y, z, angle_xy, angle_z, length, thickness, d] = branches[i];
					if (d != depth) continue;

					if (depth < max_depth - 1) {
						// Create 3-5 child branches in 3D space
						int num_children = 3 + (depth % 3);
						for (int j = 0; j < num_children; j++) {
							float child_angle_xy = angle_xy + (M_PI * 0.4f) + (j * 2.0f * M_PI / num_children);
							float child_angle_z = angle_z + (M_PI * 0.3f) * sin(j * 1.5f);
							float child_length = length * (0.6f + 0.1f * sin(depth + j)) * 2.0;
							float child_thickness = thickness * 0.7f;

							branches.push_back({ x, y, z, child_angle_xy, child_angle_z, child_length, child_thickness, depth + 1 });
						}
					}
				}
			}

			// Convert to animated lines with particles
			for (auto [start_x, start_y, start_z, angle_xy, angle_z, length, thickness, depth] : branches) {
				Line& line = add_line();

				// Calculate end point in 3D
				float end_x = start_x + length * sin(angle_xy) * cos(angle_z);
				float end_y = start_y + length * cos(angle_xy);
				float end_z = start_z + length * sin(angle_xy) * sin(angle_z);

				line.x0.start = start_x;
				line.y0.start = start_y;
				line.z0.start = start_z;
				line.x1.start = end_x;
				line.y1.start = end_y;
				line.z1.start = end_z;

				// Dynamic color based on depth and position
				float hue = depth * 0.15f + start_x * 0.5f;
				line.rgb_t0.x = 0.3f + 0.5f * sin(hue);
				line.rgb_t0.y = 0.2f + 0.6f * sin(hue + 2.0f);
				line.rgb_t0.z = 0.1f + 0.4f * sin(hue + 4.0f);

				line.thickness.start = thickness;
				line.number_of_cubes = 15 + depth * 10;

				line.copy_start_to_end();

				// Animate branches - gentle swaying motion
				float sway = 0.2f * sin(depth * 0.8f);
				line.x1.end = end_x + sway * 0.1f;
				line.y1.end = end_y + sway * 0.05f;
				line.z1.end = end_z + sway * 0.1f;

				// Add leaves at the ends of deepest branches
				if (depth == max_depth - 1) {
					add_leaf_cluster(end_x, end_y, end_z);
				}
			}
		}

		
		

		

		
		void init_crystal_fractal_tree()
		{
			const float M_PI = 3.14159265359f;
			int max_depth = 6;


			auto add_crystal_facets = [&](float x, float y, float z, float size, float rotation)
				{
					for (int i = 0; i < 6; i++) { // Hexagonal crystal pattern
						Line& facet = add_line();

						float angle1 = i * (M_PI / 3.0f) + rotation;
						float angle2 = (i + 1) * (M_PI / 3.0f) + rotation;

						facet.x0.start = x + size * 0.2f * sin(angle1);
						facet.y0.start = y;
						facet.z0.start = z + size * 0.2f * cos(angle1);

						facet.x1.start = x + size * 0.2f * sin(angle2);
						facet.y1.start = y;
						facet.z1.start = z + size * 0.2f * cos(angle2);

						// Bright crystal facet colors
						facet.rgb_t0.x = 0.3f + 0.5f * sin(i);
						facet.rgb_t0.y = 0.6f + 0.3f * cos(i * 1.5f);
						facet.rgb_t0.z = 0.9f + 0.1f * sin(i * 2.0f);

						facet.thickness.start = 0.006f;
						facet.number_of_cubes = 8;
						facet.copy_start_to_end();
					}
				};


			std::vector<std::tuple<float, float, float, float, float, int, float>> crystals;
			// x, y, z, angle, length, depth, rotation

			crystals.push_back({ 0.0f, -0.8f, 0.0f, 0.0f, 0.35f, 0, 0.0f });

			for (int depth = 0; depth < max_depth; depth++) {
				int current_size = crystals.size();
				for (int i = 0; i < current_size; i++) {
					auto [x, y, z, angle, length, d, rot] = crystals[i];
					if (d != depth) continue;

					if (depth < max_depth - 1) {
						// Crystal branching - precise geometric patterns
						int branches = 4; // Square/crystal pattern
						for (int j = 0; j < branches; j++) {
							float branch_angle = angle + (j * M_PI / 2.0f) + (M_PI / 8.0f) * sin(depth);
							float branch_length = length * (0.5f + 0.2f * cos(depth + j)) * 2.7;
							float branch_rotation = rot + (M_PI / 4.0f) * (j % 2);

							crystals.push_back({ x, y, z, branch_angle, branch_length, depth + 1, branch_rotation });
						}
					}
				}
			}

			// Create crystal branches with geometric precision
			for (auto [start_x, start_y, start_z, angle, length, depth, rotation] : crystals) {
				// Calculate crystal end point with rotation
				float end_x = start_x + length * sin(angle) * cos(rotation);
				float end_y = start_y + length * cos(angle);
				float end_z = start_z + length * sin(angle) * sin(rotation);

				Line& crystal = add_line();
				crystal.x0.start = start_x;
				crystal.y0.start = start_y;
				crystal.z0.start = start_z;
				crystal.x1.start = end_x;
				crystal.y1.start = end_y;
				crystal.z1.start = end_z;

				// Icy crystal colors
				float crystal_hue = depth * 0.3f + rotation * 2.0f;
				crystal.rgb_t0.x = 0.2f + 0.3f * sin(crystal_hue);
				crystal.rgb_t0.y = 0.4f + 0.4f * sin(crystal_hue + 1.0f);
				crystal.rgb_t0.z = 0.8f + 0.2f * sin(crystal_hue + 2.0f);

				crystal.thickness.start = 0.012f * pow(0.65f, depth);
				crystal.number_of_cubes = 25;

				crystal.copy_start_to_end();

				// Crystal growth animation
				crystal.x1.end = end_x + 0.1f * sin(depth * 5.0f);
				crystal.y1.end = end_y + 0.05f * cos(depth * 5.0f);

				// Add geometric crystal facets at ends
				if (depth > 2) {
					add_crystal_facets(end_x, end_y, end_z, length * 0.3f, rotation);
				}
			}
		}

		
		


		// --- abstract dunes: wavy heightfield drawn as crosshatched line segments
		void init_0006_wavy_dune_landscape()
		{
			lines.clear();

			// ======= knobs (play!) =======
			const int   gridX = 64;              // samples along X (creates ~2*gridX*gridZ segments total)
			const int   gridZ = 42;              // samples along Z
			const float extentX = 2.8f;          // world width (X)
			const float extentZ = 2.0f;          // world depth (Z)
			const float baseY = -0.25f;        // base plane height (Y)
			const float ampY = 0.70f;         // vertical amplitude
			const bool  drawRows = true;         // segments along X direction
			const bool  drawCols = true;         // segments along Z direction
			const int   cubesPerSeg = 18;        // resolution per segment
			const float baseThick = 0.0065f;   // baseline thickness
			const float windX = 0.08f;     // horizontal wind sway (X) at the end
			const float windZ = 0.06f;     // horizontal wind sway (Z) at the end
			const float TAU = 6.28318530718f;

			// If you prefer Z as "up" (Y/Z swapped), set this to true
			const bool  yz_swapped = false;

			auto saturate = [](float v) { return std::clamp(v, 0.0f, 1.0f); };
			auto mixf = [](float a, float b, float t) { return a + (b - a) * t; };
			auto length2 = [](float x, float z) { return std::sqrt(x * x + z * z); };

			// Cosine palette: a + b*cos(2(c*t + d))
			auto palette = [&](float t)->Vec3 {
				// tuned for dune at sunset vibes
				const Vec3 a{ 0.48f, 0.32f, 0.26f };
				const Vec3 b{ 0.52f, 0.42f, 0.40f };
				const Vec3 c{ 1.00f, 1.00f, 1.00f };
				const Vec3 d{ 0.00f, 0.27f, 0.57f };
				float ct = TAU * t;
				return Vec3{
					a.x + b.x * std::cos(ct * c.x + TAU * d.x),
					a.y + b.y * std::cos(ct * c.y + TAU * d.y),
					a.z + b.z * std::cos(ct * c.z + TAU * d.z)
				};
				};

			// Domain-warped, ridgey heightfield in [-1,1] (roughly)
			auto hfield = [&](float x, float z)->float {
				// scale into feature space
				float sx = 1.10f, sz = 1.20f;
				float X = x * sx, Z = z * sz;

				// domain warp (folds)
				float wx = X + 0.35f * std::sin(0.65f * Z) + 0.15f * std::sin(2.15f * Z + 1.1f);
				float wz = Z + 0.35f * std::sin(0.70f * X) + 0.15f * std::sin(1.75f * X + 0.7f);

				// layered waves
				float h = 0.60f * std::sin(1.20f * wx + 0.55f * wz);
				h += 0.35f * std::sin(2.30f * wx - 1.70f * wz);
				h += 0.20f * std::cos(1.90f * wx + 2.50f * wz);
				h += 0.15f * std::sin(0.55f * (wx * wx + wz * wz)); // radial wrinkle

				// ridge shaping (absolute + soft clip)
				float ridge = std::pow(std::fabs(h), 0.75f);
				h = 0.55f * h + 0.65f * ridge;

				// gentle bowl to keep composition centered
				float r = length2(x * 0.4f, z * 0.5f);
				h -= 0.15f * r;

				// clamp-ish
				return std::max(-1.2f, std::min(1.2f, h));
				};

			// approximate gradient magnitude for slope-based thickness/color
			auto slopeMag = [&](float x, float z)->float {
				const float e = 0.015f;
				float hx = hfield(x + e, z) - hfield(x - e, z);
				float hz = hfield(x, z + e) - hfield(x, z - e);
				return std::sqrt(hx * hx + hz * hz) / (2.0f * e); // ~|h|
				};

			// map from (i,j) to world (x,z)
			const float dx = (gridX > 1) ? extentX / float(gridX - 1) : 0.0f;
			const float dz = (gridZ > 1) ? extentZ / float(gridZ - 1) : 0.0f;
			auto Xat = [&](int j) { return -0.5f * extentX + j * dx; };
			auto Zat = [&](int i) { return  0.5f * extentZ - i * dz; };

			// ---- helper to spawn one segment (with start collapsed to plane for reveal) ----
			auto add_seg = [&](float x0, float z0, float x1, float z1)
				{
					float h0 = hfield(x0, z0);
					float h1 = hfield(x1, z1);

					// normalize height for palette
					float t0 = saturate(0.5f + 0.5f * (0.85f * h0));
					float t1 = saturate(0.5f + 0.5f * (0.85f * h1));

					Vec3 c0 = palette(t0);
					Vec3 c1 = palette(t1);

					// slope influences thickness (ridges pop)
					float s0 = std::min(1.0f, slopeMag(x0, z0) * 0.9f);
					float s1 = std::min(1.0f, slopeMag(x1, z1) * 0.9f);
					float thick0 = baseThick * (0.6f + 0.9f * s0);
					float thick1 = baseThick * (0.6f + 0.9f * s1);

					// wind sway at the end
					float wx0 = windX * std::sin(0.7f * z0 + 1.2f * x0);
					float wz0 = windZ * std::cos(0.8f * x0 - 1.0f * z0);
					float wx1 = windX * std::sin(0.7f * z1 + 1.2f * x1);
					float wz1 = windZ * std::cos(0.8f * x1 - 1.0f * z1);

					// Build endpoints in your chosen "up" axis
					Vec3 P0_start, P1_start, P0_end, P1_end;
					if (!yz_swapped)
					{
						// Y is up
						P0_start = { x0, baseY, z0 };
						P1_start = { x1, baseY, z1 };
						P0_end = { x0 + wx0, baseY + ampY * h0, z0 + wz0 };
						P1_end = { x1 + wx1, baseY + ampY * h1, z1 + wz1 };
					}
					else
					{
						// Z is up (Y/Z swapped)
						P0_start = { x0, z0, baseY };
						P1_start = { x1, z1, baseY };
						P0_end = { x0 + wx0, z0 + wz0, baseY + ampY * h0 };
						P1_end = { x1 + wx1, z1 + wz1, baseY + ampY * h1 };
					}

					// start collapsed to the mid at base plane  reveals nicely
					Vec3 M = { 0.5f * (P0_start.x + P1_start.x),
							   0.5f * (P0_start.y + P1_start.y),
							   0.5f * (P0_start.z + P1_start.z) };

					Line& L = add_line();

					// START
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = c0;
					L.thickness.start = baseThick * 0.35f;
					L.number_of_cubes = cubesPerSeg;

					L.copy_start_to_end();

					// END
					L.x0.end = P0_end.x; L.y0.end = P0_end.y; L.z0.end = P0_end.z;
					L.x1.end = P1_end.x; L.y1.end = P1_end.y; L.z1.end = P1_end.z;
					L.rgb_t1 = c1;
					L.thickness.end = 0.5f * (thick0 + thick1);
				};

			// ---- rows: connect along X (constant Z) ----
			if (drawRows)
			{
				for (int i = 0; i < gridZ; ++i)
				{
					float z = Zat(i);
					for (int j = 0; j < gridX - 1; ++j)
					{
						float x0 = Xat(j);
						float x1 = Xat(j + 1);
						add_seg(x0, z, x1, z);
					}
				}
			}

			// ---- cols: connect along Z (constant X) ----
			if (drawCols)
			{
				for (int j = 0; j < gridX; ++j)
				{
					float x = Xat(j);
					for (int i = 0; i < gridZ - 1; ++i)
					{
						float z0 = Zat(i);
						float z1 = Zat(i + 1);
						add_seg(x, z0, x, z1);
					}
				}
			}
		}

		void init_brutalist_monolith()
		{
			// Self-contained constants
			const float TAU = 6.283185307179586476925286766559f;
			const float PI = 3.141592653589793238462643383279f;

			// Building dimensions (in world units)
			const float building_width = 2.0f;
			const float building_depth = 1.2f;
			const int   num_floors = 12;
			const float floor_height = 0.3f;
			const float base_offset_y = -1.5f; // Sink building into ground slightly

			// Grid parameters
			const int cols_x = 6; // Columns along width
			const int cols_z = 4; // Columns along depth
			const float col_width = building_width / (cols_x - 1);
			const float col_depth = building_depth / (cols_z - 1);

			// Window grid (smaller than column grid for inset windows)
			const int win_cols_x = 4;
			const int win_cols_z = 3;
			const float win_pad = 0.12f; // Inset from column lines

			// === 1. FLOOR SLABS (horizontal concrete decks) ===
			for (int floor = 0; floor <= num_floors; ++floor)
			{
				float y = base_offset_y + floor * floor_height;

				// Outer perimeter of each floor
				for (int edge = 0; edge < 4; ++edge)
				{
					Line& line = add_line();
					// Bottom-left, bottom-right, top-right, top-left
					float corners_x[4] = { -building_width * 0.5f,  building_width * 0.5f,  building_width * 0.5f, -building_width * 0.5f };
					float corners_z[4] = { -building_depth * 0.5f, -building_depth * 0.5f,  building_depth * 0.5f,  building_depth * 0.5f };

					line.x0.start = corners_x[edge];
					line.y0.start = y;
					line.z0.start = corners_z[edge];

					line.x1.start = corners_x[(edge + 1) % 4];
					line.y1.start = y;
					line.z1.start = corners_z[(edge + 1) % 4];

					line.copy_start_to_end(); // Static slab

					// Concrete gray with slight random variation (weathering)
					float gray = 0.25f + 0.08f * Random::generate_random_float_0_to_1();
					line.rgb_t0.x = gray;
					line.rgb_t0.y = gray;
					line.rgb_t0.z = gray;

					line.thickness.start = (floor == 0 || floor == num_floors) ? 0.025f : 0.012f; // Thicker base/roof
					line.number_of_cubes = 40;
				}

				// Inner slab texture: cross grid for "poured concrete" look
				if (floor > 0 && floor < num_floors)
				{
					// X-direction ribs
					for (int i = 1; i < cols_x - 1; ++i)
					{
						Line& line = add_line();
						float x = -building_width * 0.5f + i * col_width;
						line.x0.start = x; line.y0.start = y; line.z0.start = -building_depth * 0.5f;
						line.x1.start = x; line.y1.start = y; line.z1.start = building_depth * 0.5f;
						line.copy_start_to_end();
						float gray = 0.22f + 0.06f * Random::generate_random_float_0_to_1();
						line.rgb_t0 = { gray, gray, gray };
						line.thickness.start = 0.006f;
						line.number_of_cubes = 30;
					}
					// Z-direction ribs
					for (int i = 1; i < cols_z - 1; ++i)
					{
						Line& line = add_line();
						float z = -building_depth * 0.5f + i * col_depth;
						line.x0.start = -building_width * 0.5f; line.y0.start = y; line.z0.start = z;
						line.x1.start = building_width * 0.5f; line.y1.start = y; line.z1.start = z;
						line.copy_start_to_end();
						float gray = 0.22f + 0.06f * Random::generate_random_float_0_to_1();
						line.rgb_t0 = { gray, gray, gray };
						line.thickness.start = 0.006f;
						line.number_of_cubes = 30;
					}
				}
			}

			// === 2. STRUCTURAL COLUMNS (vertical load-bearers) ===
			for (int ix = 0; ix < cols_x; ++ix)
			{
				for (int iz = 0; iz < cols_z; ++iz)
				{
					// Corner and edge columns are thicker
					bool is_corner = (ix == 0 || ix == cols_x - 1) && (iz == 0 || iz == cols_z - 1);
					bool is_edge = (ix == 0 || ix == cols_x - 1) || (iz == 0 || iz == cols_z - 1);

					float thickness = is_corner ? 0.035f : (is_edge ? 0.025f : 0.018f);
					int segments = is_corner ? 120 : 80;

					float x = -building_width * 0.5f + ix * col_width;
					float z = -building_depth * 0.5f + iz * col_depth;

					Line& line = add_line();
					line.x0.start = x;
					line.y0.start = base_offset_y;
					line.z0.start = z;

					line.x1.start = x;
					line.y1.start = base_offset_y + num_floors * floor_height;
					line.z1.start = z;

					line.copy_start_to_end();

					// Slightly darker gray for columns (shadow effect)
					float gray = 0.18f + 0.07f * Random::generate_random_float_0_to_1();
					line.rgb_t0 = { gray, gray, gray };
					line.thickness.start = thickness;
					line.number_of_cubes = segments;
				}
			}

			// === 3. WINDOW VOIDS (negative space as lines) ===
			// Brutalist windows are recessed  we draw their inner frames
			for (int floor = 1; floor < num_floors; ++floor)
			{
				float y_bottom = base_offset_y + (floor - 1) * floor_height + 0.04f;
				float y_top = base_offset_y + floor * floor_height - 0.04f;

				for (int wx = 0; wx < win_cols_x; ++wx)
				{
					for (int wz = 0; wz < win_cols_z; ++wz)
					{
						// Skip some windows randomly for asymmetry (brutalist imperfection)
						if (Random::generate_random_float_0_to_1() > 0.85f) continue;

						float win_width = (building_width - 2 * win_pad) / win_cols_x;
						float win_depth = (building_depth - 2 * win_pad) / win_cols_z;

						float x0 = -building_width * 0.5f + win_pad + wx * win_width;
						float x1 = x0 + win_width;
						float z0 = -building_depth * 0.5f + win_pad + wz * win_depth;
						float z1 = z0 + win_depth;

						// Draw 4 edges of window frame (recessed)
						float frame_gray = 0.35f + 0.1f * Random::generate_random_float_0_to_1();
						auto add_window_edge = [&](float x_a, float z_a, float x_b, float z_b, float y_a, float y_b)
							{
								Line& line = add_line();
								line.x0.start = x_a; line.y0.start = y_a; line.z0.start = z_a;
								line.x1.start = x_b; line.y1.start = y_b; line.z1.start = z_b;
								line.copy_start_to_end();
								line.rgb_t0 = { frame_gray, frame_gray, frame_gray };
								line.thickness.start = 0.004f;
								line.number_of_cubes = 25;
							};

						// Vertical edges
						add_window_edge(x0, z0, x0, z0, y_bottom, y_top);
						add_window_edge(x1, z0, x1, z0, y_bottom, y_top);
						add_window_edge(x0, z1, x0, z1, y_bottom, y_top);
						add_window_edge(x1, z1, x1, z1, y_bottom, y_top);

						// Top/bottom edges (only if not adjacent to floor slab)
						if (floor < num_floors - 1)
							add_window_edge(x0, z0, x1, z0, y_top, y_top); // Bottom of upper window
						if (floor > 1)
							add_window_edge(x0, z1, x1, z1, y_bottom, y_bottom); // Top of lower window
					}
				}
			}

			// === 4. STAIR TOWER (asymmetrical brutalist element) ===
			const float stair_width = 0.4f;
			const float stair_depth = 0.3f;
			const float stair_x = building_width * 0.5f + 0.05f; // Protruding from main mass
			const float stair_z = -building_depth * 0.3f;

			// Stair tower outer shell
			float stair_corners_x[4] = { stair_x, stair_x + stair_width, stair_x + stair_width, stair_x };
			float stair_corners_z[4] = { stair_z, stair_z, stair_z + stair_depth, stair_z + stair_depth };

			for (int edge = 0; edge < 4; ++edge)
			{
				Line& line = add_line();
				line.x0.start = stair_corners_x[edge];
				line.y0.start = base_offset_y;
				line.z0.start = stair_corners_z[edge];

				line.x1.start = stair_corners_x[(edge + 1) % 4];
				line.y1.start = base_offset_y + num_floors * floor_height;
				line.z1.start = stair_corners_z[(edge + 1) % 4];

				line.copy_start_to_end();
				float gray = 0.20f + 0.05f * Random::generate_random_float_0_to_1();
				line.rgb_t0 = { gray, gray, gray };
				line.thickness.start = 0.018f;
				line.number_of_cubes = 100;
			}

			// Stair landings (every 3 floors)
			for (int floor = 3; floor <= num_floors; floor += 3)
			{
				float y = base_offset_y + floor * floor_height;
				for (int e = 0; e < 4; ++e)
				{
					Line& line = add_line();
					line.x0.start = stair_corners_x[e];
					line.y0.start = y;
					line.z0.start = stair_corners_z[e];
					line.x1.start = stair_corners_x[(e + 1) % 4];
					line.y1.start = y;
					line.z1.start = stair_corners_z[(e + 1) % 4];
					line.copy_start_to_end();
					line.rgb_t0 = { 0.28f, 0.28f, 0.28f };
					line.thickness.start = 0.01f;
					line.number_of_cubes = 20;
				}
			}

			// === 5. SERVICE CORE / VENTS (industrial details) ===
			// Small protruding boxes on roof
			for (int i = 0; i < 3; ++i)
			{
				float offset_x = (Random::generate_random_float_0_to_1() - 0.5f) * building_width * 0.6f;
				float offset_z = (Random::generate_random_float_0_to_1() - 0.5f) * building_depth * 0.6f;
				float core_w = 0.15f + 0.1f * Random::generate_random_float_0_to_1();
				float core_d = 0.15f + 0.1f * Random::generate_random_float_0_to_1();
				float roof_y = base_offset_y + num_floors * floor_height;

				// Simple box outline
				float cx0 = offset_x - core_w * 0.5f, cx1 = offset_x + core_w * 0.5f;
				float cz0 = offset_z - core_d * 0.5f, cz1 = offset_z + core_d * 0.5f;

				auto add_core_line = [&](float x0, float z0, float x1, float z1)
					{
						Line& line = add_line();
						line.x0.start = x0; line.y0.start = roof_y; line.z0.start = z0;
						line.x1.start = x1; line.y1.start = roof_y + 0.15f; line.z1.start = z1;
						line.copy_start_to_end();
						line.rgb_t0 = { 0.15f, 0.15f, 0.15f };
						line.thickness.start = 0.008f;
						line.number_of_cubes = 30;
					};

				add_core_line(cx0, cz0, cx0, cz0);
				add_core_line(cx1, cz0, cx1, cz0);
				add_core_line(cx0, cz1, cx0, cz1);
				add_core_line(cx1, cz1, cx1, cz1);
			}

			// Final touch: ground plane (concrete plaza)
			{
				const float ground_size = 3.0f;
				float gray = 0.12f + 0.03f * Random::generate_random_float_0_to_1();
				for (int axis = 0; axis < 2; ++axis)
				{
					for (int i = -10; i <= 10; ++i)
					{
						Line& line = add_line();
						if (axis == 0) // X lines
						{
							line.x0.start = -ground_size; line.y0.start = base_offset_y - 0.01f; line.z0.start = i * 0.2f;
							line.x1.start = ground_size; line.y1.start = base_offset_y - 0.01f; line.z1.start = i * 0.2f;
						}
						else // Z lines
						{
							line.x0.start = i * 0.2f; line.y0.start = base_offset_y - 0.01f; line.z0.start = -ground_size;
							line.x1.start = i * 0.2f; line.y1.start = base_offset_y - 0.01f; line.z1.start = ground_size;
						}
						line.copy_start_to_end();
						line.rgb_t0 = { gray, gray, gray };
						line.thickness.start = 0.002f;
						line.number_of_cubes = 50;
					}
				}
			}
		}

		void init_0007_tesseract_warp()
		{
			lines.clear();

			const float baseThick = 0.007f;
			const int   cubesPerEdge = 40;

			// 4D helpers
			struct V4 { float x, y, z, w; };
			auto rot2 = [](float& a, float& b, float ang) {
				float c = std::cos(ang), s = std::sin(ang);
				float A = c * a - s * b;
				float B = s * a + c * b;
				a = A; b = B;
				};
			auto project4 = [](V4 v, float dist)->Vec3 {
				float k = dist / (dist - 0.8f * v.w); // perspective by w
				return Vec3{ k * v.x, k * v.y, k * v.z };
				};

			// Build 16 vertices of a 4D cube at 1 in each coord
			std::vector<V4> verts(16);
			for (int i = 0; i < 16; ++i) {
				verts[i] = V4{
					(i & 1) ? 1.f : -1.f,
					(i & 2) ? 1.f : -1.f,
					(i & 4) ? 1.f : -1.f,
					(i & 8) ? 1.f : -1.f
				};
			}

			// Start/end rotation & scale
			const float s0 = 0.45f, s1 = 0.70f;
			const float dist = 3.2f;

			const float ax0 = 0.10f, ay0 = 0.00f, az0 = 0.05f, aw0 = 0.00f;
			const float ax1 = 0.95f, ay1 = 0.65f, az1 = 0.45f, aw1 = 0.80f;
			// Well rotate in planes: (x,w)=ax, (y,z)=ay, (x,y)=az, (z,w)=aw

			auto pose = [&](bool endPose) { // returns 16 projected 3D points
				std::vector<Vec3> P(16);
				float ax = endPose ? ax1 : ax0;
				float ay = endPose ? ay1 : ay0;
				float az = endPose ? az1 : az0;
				float aw = endPose ? aw1 : aw0;
				float s = endPose ? s1 : s0;

				for (int i = 0; i < 16; ++i) {
					V4 v = verts[i];
					rot2(v.x, v.w, ax);
					rot2(v.y, v.z, ay);
					rot2(v.x, v.y, az);
					rot2(v.z, v.w, aw);
					Vec3 p = project4(v, dist);
					p.x *= s; p.y *= s; p.z *= s;
					P[i] = p;
				}
				return P;
				};

			auto P0 = pose(false);
			auto P1 = pose(true);

			auto add_edge = [&](int a, int b, int dimColor) {
				Line& L = add_line();

				// start/end endpoints
				Vec3 A0 = P0[a], B0 = P0[b];
				Vec3 A1 = P1[a], B1 = P1[b];

				// start: a subtle reveal from the midpoint
				Vec3 M0{ 0.5f * (A0.x + B0.x), 0.5f * (A0.y + B0.y), 0.5f * (A0.z + B0.z) };
				L.x0.start = M0.x; L.y0.start = M0.y; L.z0.start = M0.z;
				L.x1.start = M0.x; L.y1.start = M0.y; L.z1.start = M0.z;

				// simple RGB by dimension (x,y,z,w)  (R,G,B,purple)
				Vec3 cstart{}, cend{};
				switch (dimColor) {
				case 0: cstart = { 0.95f,0.35f,0.35f }; cend = { 1.00f,0.65f,0.50f }; break; // X
				case 1: cstart = { 0.35f,0.95f,0.45f }; cend = { 0.55f,1.00f,0.65f }; break; // Y
				case 2: cstart = { 0.35f,0.55f,0.95f }; cend = { 0.55f,0.75f,1.00f }; break; // Z
				case 3: cstart = { 0.80f,0.45f,0.95f }; cend = { 0.95f,0.65f,1.00f }; break; // W-link
				}
				L.rgb_t0 = cstart;
				L.thickness.start = baseThick * 0.30f;
				L.number_of_cubes = cubesPerEdge;

				L.copy_start_to_end();

				L.x0.end = A1.x; L.y0.end = A1.y; L.z0.end = A1.z;
				L.x1.end = B1.x; L.y1.end = B1.y; L.z1.end = B1.z;
				L.rgb_t1 = cend;
				L.thickness.end = baseThick;
				};

			// Connect edges: for each vertex, flip one bit (one dimension)
			for (int i = 0; i < 16; ++i) {
				for (int d = 0; d < 4; ++d) {
					if (!(i & (1 << d))) {
						int j = i | (1 << d);
						add_edge(i, j, d);
					}
				}
			}
		}

		void init_0008_supershape_ribbons()
		{
			lines.clear();

			const int   nLat = 46;    // latitude bands
			const int   nLon = 68;    // longitude bands
			const int   segs = 68;    // segments per band
			const float R = 0.85f; // overall size
			const int   cubesPerSeg = 22;
			const float baseThick = 0.0065f;
			const float TAU = 6.28318530718f;

			// superformula (a=b=1)
			auto sform = [](float ang, float m, float n1, float n2, float n3) {
				float t = 0.5f * m * ang;
				float c = std::pow(std::fabs(std::cos(t)), n2);
				float s = std::pow(std::fabs(std::sin(t)), n3);
				float d = std::pow(c + s, 1.0f / std::max(1e-6f, n1));
				return (d == 0.0f) ? 0.0f : (1.0f / d);
				};

			// parameter set (pleasant spiky flower sphere)
			const float m1 = 7.0f, n1a = 0.25f, n2a = 1.7f, n3a = 1.7f; // latitude supershape
			const float m2 = 3.0f, n1b = 0.20f, n2b = 0.80f, n3b = 1.6f; // longitude supershape

			auto surf = [&](float theta, float phi) { // theta in [-,], phi in [-/2, /2]
				float r1 = sform(theta, m1, n1a, n2a, n3a);
				float r2 = sform(phi, m2, n1b, n2b, n3b);
				float x = R * r1 * std::cos(theta) * r2 * std::cos(phi);
				float y = R * r1 * std::sin(theta) * r2 * std::cos(phi);
				float z = R * r2 * std::sin(phi);
				return Vec3{ x, y, z };
				};

			auto hue = [&](float t)->Vec3 {
				return Vec3{
					0.5f + 0.5f * std::cos(TAU * (t + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (t + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (t + 0.66f))
				};
				};

			auto add_seg = [&](const Vec3& A_start, const Vec3& B_start,
				const Vec3& A_end, const Vec3& B_end,
				const Vec3& C0, const Vec3& C1, float thickEnd)
				{
					Line& L = add_line();

					// start collapsed at midpoint for a clean reveal
					Vec3 M{ 0.5f * (A_start.x + B_start.x),
							0.5f * (A_start.y + B_start.y),
							0.5f * (A_start.z + B_start.z) };

					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;

					L.rgb_t0 = C0;
					L.thickness.start = baseThick * 0.35f;
					L.number_of_cubes = cubesPerSeg;

					L.copy_start_to_end();

					L.x0.end = A_end.x; L.y0.end = A_end.y; L.z0.end = A_end.z;
					L.x1.end = B_end.x; L.y1.end = B_end.y; L.z1.end = B_end.z;

					L.rgb_t1 = C1;
					L.thickness.end = thickEnd;
				};

			// animate: start at smaller radius, end at full + slight twist
			auto twist = [](Vec3 p, float ang)->Vec3 {
				float c = std::cos(ang), s = std::sin(ang);
				return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z };
				};
			const float twistEnd = 0.35f;

			// ----- latitude ribbons (phi fixed, vary theta) -----
			for (int i = 0; i < nLat; ++i)
			{
				float v0 = (i / float(nLat - 1)) * TAU - (TAU * 0.5f); // map to [-,] for theta span when we use bands look
				float phi = ((i / float(nLat - 1)) - 0.5f) * TAU * 0.5f; // [-/2, /2]
				float colPhase = i / float(nLat);

				Vec3 C0 = hue(0.10f + 0.55f * colPhase);
				Vec3 C1 = hue(0.18f + 0.55f * colPhase);

				for (int j = 0; j < segs; ++j)
				{
					float t0 = -3.14159265f + (j / float(segs)) * TAU;
					float t1 = -3.14159265f + ((j + 1) / float(segs)) * TAU;

					Vec3 A = surf(t0, phi);
					Vec3 B = surf(t1, phi);

					// start small & end with a twist
					Vec3 As = Vec3{ A.x * 0.35f, A.y * 0.35f, A.z * 0.35f };
					Vec3 Bs = Vec3{ B.x * 0.35f, B.y * 0.35f, B.z * 0.35f };
					Vec3 Ae = twist(A, twistEnd);
					Vec3 Be = twist(B, twistEnd);

					float thick = baseThick * (0.75f + 0.25f * std::sin(4.0f * phi + 2.0f * t0));
					add_seg(As, Bs, Ae, Be, C0, C1, thick);
				}
			}

			// ----- longitude ribbons (theta fixed, vary phi) -----
			for (int i = 0; i < nLon; ++i)
			{
				float theta = -3.14159265f + (i / float(nLon)) * TAU;
				float colPhase = i / float(nLon);

				Vec3 C0 = hue(0.60f + 0.40f * colPhase);
				Vec3 C1 = hue(0.68f + 0.40f * colPhase);

				for (int j = 0; j < segs; ++j)
				{
					float p0 = -1.57079633f + (j / float(segs)) * 3.14159265f;      // [-/2, /2]
					float p1 = -1.57079633f + ((j + 1) / float(segs)) * 3.14159265f;

					Vec3 A = surf(theta, p0);
					Vec3 B = surf(theta, p1);

					Vec3 As = Vec3{ A.x * 0.35f, A.y * 0.35f, A.z * 0.35f };
					Vec3 Bs = Vec3{ B.x * 0.35f, B.y * 0.35f, B.z * 0.35f };
					Vec3 Ae = twist(A, -twistEnd);
					Vec3 Be = twist(B, -twistEnd);

					float thick = baseThick * (0.75f + 0.25f * std::sin(3.0f * theta + 2.5f * p0));
					add_seg(As, Bs, Ae, Be, C0, C1, thick);
				}
			}
		}

		// Matrix-like green glyph columns flowing over a supershape surface
		void init_0009_supershape_matrix_rain()
		{
			lines.clear();

			// ===== knobs =====
			const int   nCols = 28;     // how many rain columns around the shape
			const int   glyphsPer = 14;     // glyphs per column
			const float R = 0.85f;  // overall size of the surface
			const float glyphH = 0.06f;  // glyph height in world units
			const float glyphW = 0.045f; // glyph width  in world units
			const float colStep = 2.6f;   // spacing between glyphs along column (multiplies glyphH)
			const float fallDist = 0.65f;  // how far start is offset along fall direction (bigger = longer trails)
			const float twistEnd = 0.35f;  // end twist around the normal for extra motion
			const int   cubesPerStroke = 20;
			const float baseThick = 0.006f;
			const float TAU = 6.28318530718f;

			// --- tiny supershape surf (a=b=1); tune these for different sculptures ---
			auto sform = [](float ang, float m, float n1, float n2, float n3) {
				float t = 0.5f * m * ang;
				float c = std::pow(std::fabs(std::cos(t)), n2);
				float s = std::pow(std::fabs(std::sin(t)), n3);
				float d = std::pow(c + s, 1.0f / std::max(1e-6f, n1));
				return d == 0.0f ? 0.0f : 1.0f / d;
				};
			// flower-ish sphere
			const float m1 = 7.0f, n1a = 0.25f, n2a = 1.7f, n3a = 1.7f; // theta
			const float m2 = 3.0f, n1b = 0.20f, n2b = 0.80f, n3b = 1.6f; // phi
			auto surf = [&](float th, float ph)->Vec3 {
				float r1 = sform(th, m1, n1a, n2a, n3a);
				float r2 = sform(ph, m2, n1b, n2b, n3b);
				float x = R * r1 * std::cos(th) * r2 * std::cos(ph);
				float y = R * r1 * std::sin(th) * r2 * std::cos(ph);
				float z = R * r2 * std::sin(ph);
				return Vec3{ x, y, z };
				};

			auto sub = [](Vec3 a, Vec3 b) { return Vec3{ a.x - b.x, a.y - b.y, a.z - b.z }; };
			auto add = [](Vec3 a, Vec3 b) { return Vec3{ a.x + b.x, a.y + b.y, a.z + b.z }; };
			auto mul = [](Vec3 a, float s) { return Vec3{ a.x * s, a.y * s, a.z * s }; };
			auto dot = [](Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; };
			auto len = [&](Vec3 v) { return std::sqrt(dot(v, v)); };
			auto nrm = [&](Vec3 v) { float L = len(v); return (L < 1e-8f) ? Vec3{ 0,0,0 } : mul(v, 1.0f / L); };
			auto cross = [](Vec3 a, Vec3 b) {
				return Vec3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
				};
			auto rotAround = [&](Vec3 p, Vec3 axis, float ang)->Vec3 {
				// Rodrigues
				Vec3 u = nrm(axis);
				float c = std::cos(ang), s = std::sin(ang);
				return add(add(mul(p, c), mul(cross(u, p), s)), mul(u, dot(u, p) * (1.0f - c)));
				};

			// neon green palette (head bright, tail dim)
			auto green = [&](float t)->Vec3 {
				float g = 0.6f + 0.4f * t;
				float b = 0.25f + 0.25f * t;
				return Vec3{ 0.05f, g, b };
				};

			// minimal glyph strokes (normalized [0,1]  [0,1]); feel free to extend
			struct V2 { float x, y; };
			using Stroke = std::pair<V2, V2>;
			auto S_push_v = [](std::vector<Stroke>& S, float x, float y0, float y1) { S.push_back({ {x,y0},{x,y1} }); };
			auto S_push_h = [](std::vector<Stroke>& S, float y, float x0, float x1) { S.push_back({ {x0,y},{x1,y} }); };
			auto S_push_d = [](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) { S.push_back({ {x0,y0},{x1,y1} }); };

			auto glyph = [&](char c)->std::vector<Stroke> {
				std::vector<Stroke> S;
				const float m = 0.12f, l = m, r = 1.0f - m, b = m, t = 1.0f - m, mid = 0.5f;
				switch (c) {
				case '0': S_push_h(S, t, l, r); S_push_h(S, b, l, r); S_push_v(S, l, b, t); S_push_v(S, r, b, t); break;
				case '1': S_push_v(S, mid, b, t); break;
				case '2': S_push_h(S, t, l, r); S_push_h(S, b, l, r); S_push_d(S, l, b, r, mid); S_push_d(S, l, mid, r, t); break;
				case '3': S_push_h(S, t, l, r); S_push_h(S, mid, l, r); S_push_h(S, b, l, r); S_push_v(S, r, b, t); break;
				case '4': S_push_v(S, l, b, t); S_push_v(S, r, b, t); S_push_h(S, mid, l, r); break;
				case '5': S_push_h(S, t, l, r); S_push_h(S, b, l, r); S_push_v(S, l, b, mid); S_push_v(S, r, mid, t); break;
				case '6': S_push_h(S, t, l, r); S_push_h(S, b, l, r); S_push_v(S, l, b, t); S_push_h(S, mid, l, r); break;
				case '7': S_push_h(S, t, l, r); S_push_d(S, l, b, r, t); break;
				case '8': S_push_h(S, t, l, r); S_push_h(S, mid, l, r); S_push_h(S, b, l, r); S_push_v(S, l, b, t); S_push_v(S, r, b, t); break;
				case '9': S_push_h(S, t, l, r); S_push_h(S, b, l, r); S_push_v(S, r, b, t); S_push_h(S, mid, l, r); break;
				case 'A': S_push_d(S, l, b, mid, t); S_push_d(S, r, b, mid, t); S_push_h(S, mid, l, r); break;
				case 'H': S_push_v(S, l, b, t); S_push_v(S, r, b, t); S_push_h(S, mid, l, r); break;
				case 'K': S_push_v(S, l, b, t); S_push_d(S, l, mid, r, t); S_push_d(S, l, mid, r, b); break;
				case '+': S_push_h(S, mid, l, r); S_push_v(S, mid, b, t); break;
				case '-': S_push_h(S, mid, l, r); break;
				default:  S_push_h(S, t, l, r); S_push_h(S, b, l, r); S_push_v(S, l, b, t); S_push_v(S, r, b, t); break; // box
				}
				return S;
				};
			const char alphabet[] = "00110110011100010110AHK+-";

			// build columns at evenly spaced thetas; glyphs fall along +phi (toward south pole)
			for (int ci = 0; ci < nCols; ++ci)
			{
				float u = (ci + 0.5f) / float(nCols);         // 0..1 around
				float theta = -3.14159265f + u * TAU;         // wrap
				float phiTop = 0.9f * 1.57079633f * 0.65f;   // start near north-mid
				float dPhi = (colStep * glyphH) / R;        // arc step per glyph (approx)

				// column local frame at (theta, phi)
				auto frameAt = [&](float th, float ph) {
					const float eps = 0.0015f;
					Vec3 P = surf(th, ph);
					Vec3 Pt = surf(th + eps, ph);
					Vec3 Pp = surf(th, ph + eps);
					Vec3 Tt = nrm(sub(Pt, P));   // along theta (around)
					Vec3 Tp = nrm(sub(Pp, P));   // along phi   (down/up)
					Vec3 N = nrm(cross(Tt, Tp)); // surface normal
					// re-orthogonalize
					Tp = nrm(cross(N, Tt));
					return std::tuple<Vec3, Vec3, Vec3, Vec3>(P, Tt, Tp, N); // origin, X(around), Y(down), N
					};

				for (int gi = 0; gi < glyphsPer; ++gi)
				{
					// choose glyph
					char ch = alphabet[Random::random_int(0, 999999) % (sizeof(alphabet) - 1)];
					auto strokes = glyph(ch);

					float phEnd = phiTop - gi * dPhi;     // final position along column
					float phStart = phEnd + (fallDist / R); // start higher (falls down)
					auto [Pe, Xt, Yd, Ne] = frameAt(theta, phEnd);
					auto [Ps, Xs, Ys, Ns] = frameAt(theta, phStart);

					// slight end twist around normal (adds motion)
					float twist = twistEnd * (0.25f + 0.75f * (gi / float(glyphsPer)));

					// colors: head brighter
					float head = 1.0f - (gi / float(glyphsPer - 1));
					Vec3 C0 = green(0.30f * head);
					Vec3 C1 = green(0.80f * head);

					// map glyph space [0,1]^2 to 3D using (X,Y) axes at start/end
					auto mapStart = [&](V2 p)->Vec3 {
						Vec3 local = add(mul(Xs, (p.x - 0.5f) * glyphW),
							mul(Ys, (p.y - 0.5f) * glyphH));
						return add(Ps, local);
						};
					auto mapEnd = [&](V2 p)->Vec3 {
						// rotate in-plane around normal, then place
						Vec3 ex = Xt, ey = Yd;
						Vec3 local = add(mul(ex, (p.x - 0.5f) * glyphW),
							mul(ey, (p.y - 0.5f) * glyphH));
						Vec3 spun = rotAround(local, Ne, twist);
						return add(Pe, spun);
						};

					// emit strokes
					for (const auto& st : strokes)
					{
						Vec3 A0 = mapStart(st.first);
						Vec3 B0 = mapStart(st.second);
						Vec3 A1 = mapEnd(st.first);
						Vec3 B1 = mapEnd(st.second);

						// start collapsed to mid for smooth reveal
						Vec3 M0{ 0.5f * (A0.x + B0.x), 0.5f * (A0.y + B0.y), 0.5f * (A0.z + B0.z) };

						Line& L = add_line();
						L.x0.start = M0.x; L.y0.start = M0.y; L.z0.start = M0.z;
						L.x1.start = M0.x; L.y1.start = M0.y; L.z1.start = M0.z;
						L.rgb_t0 = C0;
						L.thickness.start = baseThick * 0.30f;
						L.number_of_cubes = cubesPerStroke;

						L.copy_start_to_end();

						L.x0.end = A1.x; L.y0.end = A1.y; L.z0.end = A1.z;
						L.x1.end = B1.x; L.y1.end = B1.y; L.z1.end = B1.z;
						L.rgb_t1 = C1;
						L.thickness.end = baseThick * (0.65f + 0.35f * head);
					}
				}
			}
		}

		// Add this new function inside your 'struct Lines'

		void init_0011_flip_unique_matrix_orbit()
		{
			lines.clear();
			const float TAU = 6.28318530718f;

			// =========================
			// helpers (Z is UP)
			// =========================
			auto rotZ = [](Vec3 p, float a)->Vec3 { // spin in XY plane (since Z is up)
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z };
				};
			auto rotY = [](Vec3 p, float a)->Vec3 { // yaw around Y (depth axis now)
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z };
				};
			auto rotX = [](Vec3 p, float a)->Vec3 { // pitch around X
				float c = std::cos(a), s = std::sin(a);
				return Vec3{ p.x, c * p.y - s * p.z, s * p.y + c * p.z };
				};
			auto sub = [](Vec3 a, Vec3 b) {return Vec3{ a.x - b.x,a.y - b.y,a.z - b.z }; };
			auto add = [](Vec3 a, Vec3 b) {return Vec3{ a.x + b.x,a.y + b.y,a.z + b.z }; };
			auto mul = [](Vec3 a, float s) {return Vec3{ a.x * s,a.y * s,a.z * s }; };
			auto dot = [](Vec3 a, Vec3 b) {return a.x * b.x + a.y * b.y + a.z * b.z; };
			auto len = [&](Vec3 v) {return std::sqrt(dot(v, v)); };
			auto nrm = [&](Vec3 v) {float L = len(v); return (L < 1e-8f) ? Vec3{ 0,0,0 } : mul(v, 1.0f / L); };
			auto cross = [](Vec3 a, Vec3 b) {return Vec3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; };

			// =========================
			// A) Z-up dunes (unique base)
			// =========================
			{
				const int   gridX = 44, gridY = 36;    // XY grid (Z is height)
				const float extentX = 2.6f, extentY = 2.0f;
				const float baseZ = -0.55f;
				const float ampZ = 0.70f;
				const float windX = 0.06f, windY = 0.05f;
				const float baseThick = 0.0065f;
				const int   cubesPer = 16;
				auto warm = [&](float t)->Vec3 {
					const Vec3 a{ 0.48f,0.32f,0.26f }, b{ 0.52f,0.42f,0.40f };
					const Vec3 c{ 1,1,1 }, d{ 0.00f,0.27f,0.57f };
					float ct = TAU * t;
					return Vec3{
						a.x + b.x * std::cos(ct * c.x + TAU * d.x),
						a.y + b.y * std::cos(ct * c.y + TAU * d.y),
						a.z + b.z * std::cos(ct * c.z + TAU * d.z)
					};
					};
				auto hfield = [&](float x, float y)->float {
					float sx = 1.15f, sy = 1.10f;
					float X = x * sx, Y = y * sy;
					float wx = X + 0.35f * std::sin(0.7f * Y) + 0.15f * std::sin(2.1f * Y + 1.0f);
					float wy = Y + 0.35f * std::sin(0.7f * X) + 0.15f * std::sin(1.8f * X + 0.6f);
					float h = 0.60f * std::sin(1.2f * wx + 0.55f * wy);
					h += 0.35f * std::sin(2.3f * wx - 1.7f * wy);
					h += 0.20f * std::cos(1.9f * wx + 2.5f * wy);
					h += 0.15f * std::sin(0.55f * (wx * wx + wy * wy));
					float ridge = std::pow(std::fabs(h), 0.75f);
					h = 0.55f * h + 0.65f * ridge;
					float r = std::sqrt((x * 0.45f) * (x * 0.45f) + (y * 0.55f) * (y * 0.55f));
					h -= 0.18f * r;
					return std::clamp(h, -1.2f, 1.2f);
					};
				const float dx = (gridX > 1) ? extentX / float(gridX - 1) : 0.0f;
				const float dy = (gridY > 1) ? extentY / float(gridY - 1) : 0.0f;
				auto Xat = [&](int j) {return -0.5f * extentX + j * dx; };
				auto Yat = [&](int i) {return  0.5f * extentY - i * dy; };

				auto add_seg = [&](float x0, float y0, float x1, float y1) {
					float hz0 = hfield(x0, y0), hz1 = hfield(x1, y1);
					Vec3 C0 = warm(0.5f + 0.5f * (0.85f * hz0));
					Vec3 C1 = warm(0.5f + 0.5f * (0.85f * hz1));
					float wx0 = windX * std::sin(0.8f * y0 + 1.1f * x0);
					float wy0 = windY * std::cos(0.7f * x0 - 1.0f * y0);
					float wx1 = windX * std::sin(0.8f * y1 + 1.1f * x1);
					float wy1 = windY * std::cos(0.7f * x1 - 1.0f * y1);

					Vec3 A0{ x0, y0, baseZ }, B0{ x1, y1, baseZ };
					Vec3 A1{ x0 + wx0, y0 + wy0, baseZ + ampZ * hz0 };
					Vec3 B1{ x1 + wx1, y1 + wy1, baseZ + ampZ * hz1 };

					Vec3 M{ 0.5f * (A0.x + B0.x), 0.5f * (A0.y + B0.y), 0.5f * (A0.z + B0.z) };

					Line& L = add_line();
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = C0;
					L.thickness.start = baseThick * 0.35f;
					L.number_of_cubes = cubesPer;
					L.copy_start_to_end();
					L.x0.end = A1.x; L.y0.end = A1.y; L.z0.end = A1.z;
					L.x1.end = B1.x; L.y1.end = B1.y; L.z1.end = B1.z;
					L.rgb_t1 = C1; L.thickness.end = baseThick;
					};

				for (int i = 0; i < gridY; ++i) { float y = Yat(i); for (int j = 0; j < gridX - 1; ++j) add_seg(Xat(j), y, Xat(j + 1), y); }
				for (int j = 0; j < gridX; ++j) { float x = Xat(j); for (int i = 0; i < gridY - 1; ++i) add_seg(x, Yat(i), x, Yat(i + 1)); }
			}

			// =========================
			// B) Supershape core with Matrix rain (Z-up)  truly different flow
			// =========================
			{
				const int   nCols = 26, glyphsPer = 12;
				const float R = 0.78f, glyphH = 0.055f, glyphW = 0.042f;
				const float colStep = 2.5f, fallDist = 0.60f, twistEnd = 0.38f;
				const float baseThick = 0.0060f; const int cubesPer = 18;

				auto sform = [](float ang, float m, float n1, float n2, float n3) {
					float t = 0.5f * m * ang;
					float c = std::pow(std::fabs(std::cos(t)), n2);
					float s = std::pow(std::fabs(std::sin(t)), n3);
					float d = std::pow(c + s, 1.0f / std::max(1e-6f, n1));
					return d == 0.0f ? 0.0f : 1.0f / d;
					};
				// theta around Z, phi from equator to poles (Z up)
				const float m1 = 7.0f, n1a = 0.25f, n2a = 1.7f, n3a = 1.7f;
				const float m2 = 3.0f, n1b = 0.20f, n2b = 0.80f, n3b = 1.6f;
				auto surf = [&](float th, float ph)->Vec3 {
					float r1 = sform(th, m1, n1a, n2a, n3a);
					float r2 = sform(ph, m2, n1b, n2b, n3b);
					float x = R * r1 * std::cos(th) * r2 * std::cos(ph);
					float y = R * r1 * std::sin(th) * r2 * std::cos(ph);
					float z = R * r2 * std::sin(ph); // Z is up
					return Vec3{ x,y,z };
					};

				struct V2 { float x, y; }; using Stroke = std::pair<V2, V2>;
				auto pv = [&](std::vector<Stroke>& S, float x, float y0, float y1) {S.push_back({ {x,y0},{x,y1} }); };
				auto phl = [&](std::vector<Stroke>& S, float y, float x0, float x1) {S.push_back({ {x0,y},{x1,y} }); };
				auto pd = [&](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) {S.push_back({ {x0,y0},{x1,y1} }); };
				auto glyph = [&](char c)->std::vector<Stroke> {
					std::vector<Stroke> S; const float m = 0.12f, l = m, r = 1.0f - m, b = m, t = 1.0f - m, mid = 0.5f;
					switch (c) {
					case '0': phl(S, t, l, r); phl(S, b, l, r); pv(S, l, b, t); pv(S, r, b, t); break;
					case '1': pv(S, mid, b, t); break;
					case '2': phl(S, t, l, r); phl(S, b, l, r); pd(S, l, b, r, mid); pd(S, l, mid, r, t); break;
					case '3': phl(S, t, l, r); phl(S, mid, l, r); phl(S, b, l, r); pv(S, r, b, t); break;
					case '4': pv(S, l, b, t); pv(S, r, b, t); phl(S, mid, l, r); break;
					case '5': phl(S, t, l, r); phl(S, b, l, r); pv(S, l, b, mid); pv(S, r, mid, t); break;
					case '6': phl(S, t, l, r); phl(S, b, l, r); pv(S, l, b, t); phl(S, mid, l, r); break;
					case '7': phl(S, t, l, r); pd(S, l, b, r, t); break;
					case '8': phl(S, t, l, r); phl(S, mid, l, r); phl(S, b, l, r); pv(S, l, b, t); pv(S, r, b, t); break;
					case '9': phl(S, t, l, r); phl(S, b, l, r); pv(S, r, b, t); phl(S, mid, l, r); break;
					case 'A': pd(S, l, b, 0.5f, t); pd(S, r, b, 0.5f, t); phl(S, 0.62f, l + 0.15f, r - 0.15f); break;
					case 'H': pv(S, l, b, t); pv(S, r, b, t); phl(S, 0.5f, l, r); break;
					case 'K': pv(S, l, b, t); pd(S, l, 0.5f, r, t); pd(S, l, 0.5f, r, b); break;
					default:  phl(S, t, l, r); phl(S, b, l, r); pv(S, l, b, t); pv(S, r, b, t); break;
					}
					return S;
					};
				const char alphabet[] = "0123456789AAHKK";

				auto frameAt = [&](float th, float ph) {
					const float eps = 0.0016f;
					Vec3 P = surf(th, ph), Pt = surf(th + eps, ph), Pp = surf(th, ph + eps);
					Vec3 Tt = nrm(sub(Pt, P));   // tangent around Z
					Vec3 Tp = nrm(sub(Pp, P));   // tangent toward pole (+/-Z)
					Vec3 N = nrm(cross(Tt, Tp));
					Tp = nrm(cross(N, Tt));    // re-orthogonalize
					return std::tuple<Vec3, Vec3, Vec3, Vec3>(P, Tt, Tp, N);
					};
				auto neon = [&](float t)->Vec3 { float g = 0.6f + 0.4f * t, b = 0.22f + 0.22f * t; return Vec3{ 0.06f,g,b }; };

				for (int ci = 0; ci < nCols; ++ci) {
					float u = (ci + 0.5f) / float(nCols);
					float theta = -3.14159265f + u * TAU;        // around Z
					float phiTop = 0.9f * 1.57079633f * 0.68f; // start near +Z mid
					float dPhi = (colStep * glyphH) / R;

					for (int gi = 0; gi < glyphsPer; ++gi) {
						char ch = alphabet[Random::random_int(0, 999999) % (sizeof(alphabet) - 1)];
						auto strokes = glyph(ch);

						float phEnd = phiTop - gi * dPhi;      // falling toward -Z
						float phStart = phEnd + (fallDist / R);  // start higher
						auto [Pe, Xt, Yd, Ne] = frameAt(theta, phEnd);
						auto [Ps, Xs, Ys, Ns] = frameAt(theta, phStart);

						float twist = twistEnd * (0.25f + 0.75f * (gi / float(glyphsPer)));
						float head = 1.0f - (gi / float(glyphsPer - 1));
						Vec3  C0 = neon(0.30f * head), C1 = neon(0.90f * head);

						auto mapS = [&](V2 p)->Vec3 {
							Vec3 local = add(mul(Xs, (p.x - 0.5f) * glyphW), mul(Ys, (p.y - 0.5f) * glyphH));
							return add(Ps, local);
							};
						auto rotAround = [&](Vec3 v, Vec3 axis, float a) { Vec3 u = nrm(axis); float c = std::cos(a), s = std::sin(a);
						return add(add(mul(v, c), mul(cross(u, v), s)), mul(u, dot(u, v) * (1.0f - c))); };
						auto mapE = [&](V2 p)->Vec3 {
							Vec3 local = add(mul(Xt, (p.x - 0.5f) * glyphW), mul(Yd, (p.y - 0.5f) * glyphH));
							return add(Pe, rotAround(local, Ne, twist));
							};

						for (const auto& st : strokes) {
							Vec3 A0 = mapS(st.first), B0 = mapS(st.second);
							Vec3 A1 = mapE(st.first), B1 = mapE(st.second);
							Vec3 M{ 0.5f * (A0.x + B0.x), 0.5f * (A0.y + B0.y), 0.5f * (A0.z + B0.z) };

							Line& L = add_line();
							L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
							L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
							L.rgb_t0 = C0; L.thickness.start = baseThick * 0.30f; L.number_of_cubes = cubesPer;
							L.copy_start_to_end();
							L.x0.end = A1.x; L.y0.end = A1.y; L.z0.end = A1.z;
							L.x1.end = B1.x; L.y1.end = B1.y; L.z1.end = B1.z;
							L.rgb_t1 = C1; L.thickness.end = baseThick * (0.65f + 0.35f * head);
						}
					}
				}
			}

			// =========================
			// C) Triple orbital text bands (layers along Z) + hex cage
			// =========================
			{
				const char* text = "  MATRIX  MATH  ";
				int L = 0; while (text[L] != '\0') ++L;

				struct V2 { float x, y; }; using Stroke = std::pair<V2, V2>;
				auto pv = [&](std::vector<Stroke>& S, float x, float y0, float y1) {S.push_back({ {x,y0},{x,y1} }); };
				auto phl = [&](std::vector<Stroke>& S, float y, float x0, float x1) {S.push_back({ {x0,y},{x1,y} }); };
				auto pd = [&](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) {S.push_back({ {x0,y0},{x1,y1} }); };
				auto strokesFor = [&](char c)->std::vector<Stroke> {
					std::vector<Stroke> S; char C = (c >= 'a' && c <= 'z') ? char(c - 'a' + 'A') : c;
					const float m = 0.10f, l = m, r = 1.0f - m, b = m, t = 1.0f - m, mid = 0.5f, hi = 0.62f;
					switch (C) {
					case 'M': pv(S, l, b, t); pv(S, r, b, t); pd(S, l, t, 0.5f, b + 0.25f); pd(S, r, t, 0.5f, b + 0.25f); break;
					case 'A': pd(S, l, b, 0.5f, t); pd(S, r, b, 0.5f, t); phl(S, hi, l + 0.15f, r - 0.15f); break;
					case 'T': phl(S, t, l, r); pv(S, mid, b, t); break;
					case 'R': pv(S, l, b, t); phl(S, t, l, 0.82f); phl(S, 0.56f, l, 0.82f); pd(S, l, 0.56f, 0.86f, b); break;
					case 'I': pv(S, mid, b, t); phl(S, t, l + 0.2f, r - 0.2f); phl(S, b, l + 0.2f, r - 0.2f); break;
					case 'X': pd(S, l, b, r, t); pd(S, l, t, r, b); break;
					case ' ': break;
					default:  phl(S, t, l, r); phl(S, b, l, r); pv(S, l, b, t); pv(S, r, b, t); break;
					}
					return S;
					};

				const int   bands = 3;
				const float ringR[bands] = { 1.55f, 1.85f, 2.15f };
				const float ringZ[bands] = { 0.15f, 0.00f, -0.15f }; // stacked along Z (Z up)
				const float letterH = 0.22f;
				const float baseThick = 0.0075f;
				const int   cubesPer = 26;
				const float yawEnd[bands] = { 0.85f, -0.65f, 1.05f }; // each band spins differently
				const float lift[bands] = { 0.10f,  0.00f,  0.12f };
				const float push[bands] = { 0.18f,  0.24f,  0.30f };

				for (int b = 0; b < bands; ++b)
				{
					for (int i = 0; i < L; ++i)
					{
						auto S = strokesFor(text[i]);
						float u = (i + 0.5f) / float(L);
						float ang = u * TAU;

						// ring basis (XY circle at fixed Z)
						Vec3 pos{ ringR[b] * std::cos(ang), ringR[b] * std::sin(ang), ringZ[b] };
						Vec3 out = nrm(Vec3{ pos.x, pos.y, 0.0f });       // radial in XY
						Vec3 tang{ -out.y, out.x, 0.0f };                 // tangent in XY
						Vec3 up{ 0,0,1 };                                 // Z up

						// cool cyan-magenta scheme, different per band
						Vec3 c0{ 0.45f + 0.35f * std::cos(ang + 0.2f * b), 0.55f, 0.75f + 0.20f * std::sin(ang + 0.4f * b) };
						Vec3 c1{ 0.65f, 0.90f, 1.00f };

						auto mapPt = [&](V2 p)->Vec3 {
							float dx = (p.x - 0.5f) * letterH, dy = (p.y - 0.5f) * letterH;
							return Vec3{
								pos.x + tang.x * dx + up.x * dy,
								pos.y + tang.y * dx + up.y * dy,
								pos.z + tang.z * dx + up.z * dy
							};
							};
						auto toEnd = [&](Vec3 P)->Vec3 {
							// spin around Z + slight lift + outward push
							Vec3 Q{ P.x + out.x * push[b], P.y + out.y * push[b], P.z + lift[b] };
							return rotZ(Q, yawEnd[b]);
							};

						for (const auto& st : S)
						{
							Vec3 A = mapPt(st.first), B = mapPt(st.second);
							Vec3 M{ 0.5f * (A.x + B.x), 0.5f * (A.y + B.y), 0.5f * (A.z + B.z) };
							Line& L = add_line();
							L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
							L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
							L.rgb_t0 = c0; L.thickness.start = baseThick * 0.35f; L.number_of_cubes = cubesPer;
							L.copy_start_to_end();
							Vec3 A1 = toEnd(A), B1 = toEnd(B);
							L.x0.end = A1.x; L.y0.end = A1.y; L.z0.end = A1.z;
							L.x1.end = B1.x; L.y1.end = B1.y; L.z1.end = B1.z;
							L.rgb_t1 = c1; L.thickness.end = baseThick;
						}
					}
				}

				// Minimal hex cage (unique geometric accent), tied to outer band
				{
					const int   hexSides = 6;
					const float r0 = ringR[2] - 0.06f, r1 = ringR[2] + 0.06f;
					const float z0 = ringZ[0] + 0.16f, z1 = ringZ[2] - 0.16f;
					const float thick = 0.009f; const int cubes = 40;

					auto add_edge = [&](Vec3 A, Vec3 B, Vec3 c0, Vec3 c1) {
						Line& L = add_line();
						Vec3 M{ 0.5f * (A.x + B.x),0.5f * (A.y + B.y),0.5f * (A.z + B.z) };
						L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
						L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
						L.rgb_t0 = c0; L.thickness.start = thick * 0.35f; L.number_of_cubes = cubes;
						L.copy_start_to_end();
						L.x0.end = A.x; L.y0.end = A.y; L.z0.end = A.z;
						L.x1.end = B.x; L.y1.end = B.y; L.z1.end = B.z;
						L.rgb_t1 = c1; L.thickness.end = thick;
						};

					// two hex rings (top/bottom) and 6 verticals + 6 diagonals
					std::vector<Vec3> top, bottom;
					for (int i = 0; i < hexSides; ++i) {
						float a = (i / float(hexSides)) * TAU + TAU / 12.0f;
						top.push_back(Vec3{ r0 * std::cos(a), r0 * std::sin(a), z0 });
						bottom.push_back(Vec3{ r1 * std::cos(a), r1 * std::sin(a), z1 });
					}
					for (int i = 0; i < hexSides; ++i) {
						int j = (i + 1) % hexSides;
						add_edge(top[i], top[j], { 0.85f,0.85f,0.95f }, { 0.60f,0.80f,1.00f });
						add_edge(bottom[i], bottom[j], { 0.85f,0.85f,0.95f }, { 0.60f,0.80f,1.00f });
						add_edge(top[i], bottom[i], { 0.80f,0.80f,0.92f }, { 0.55f,0.75f,0.98f });
						add_edge(top[i], bottom[j], { 0.80f,0.80f,0.92f }, { 0.55f,0.75f,0.98f }); // weave diagonal
					}
				}
			}
		}

		// Add this new, self-contained function inside your 'struct Lines'

		void init_geometric_unfolding()
		{
			lines.clear();

			// --- SECTION 1: Self-Contained Helpers (Vector Math Lambdas) ---
			auto vec_add = [](const Vec3& a, const Vec3& b) -> Vec3 { return { a.x + b.x, a.y + b.y, a.z + b.z }; };
			auto vec_scale = [](const Vec3& v, float s) -> Vec3 { return { v.x * s, v.y * s, v.z * s }; };

			// --- SECTION 2: Geometric & Animation Parameters ---
			const float scale = 0.8f;                     // Overall size of the final shape
			const float final_rotation_deg = 120.0f;      // How much the icosahedron rotates as it forms
			Vec3 start_color = { 0.1f, 0.9f, 1.0f };      // Cool cyan for the flat "glyph"
			Vec3 end_color = { 1.0f, 0.85f, 0.4f };       // Warm gold for the finished "jewel"

			// --- SECTION 3: Define the Icosahedron Geometry ---
			// An icosahedron is a 20-faced solid made of equilateral triangles.
			// We first define its 12 vertices and then its 30 edges.

			// Golden ratio constant, crucial for defining the vertices
			const float PHI = 1.61803398875f;

			// The 12 vertices of a perfect icosahedron
			std::vector<Vec3> vertices_3d = {
				{-1,  PHI, 0}, { 1,  PHI, 0}, {-1, -PHI, 0}, { 1, -PHI, 0},
				{0, -1,  PHI}, {0,  1,  PHI}, {0, -1, -PHI}, {0,  1, -PHI},
				{ PHI, 0, -1}, { PHI, 0,  1}, {-PHI, 0, -1}, {-PHI, 0,  1}
			};

			// The START state: A hand-crafted 2D "net" of the icosahedron, laid flat on the XZ plane.
			// This defines where each vertex is before the folding begins.
			std::vector<Vec3> vertices_2d = {
				{0.0f, 0, 1.86f}, {2.0f, 0, 1.86f}, {0.0f, 0, -1.86f}, {2.0f, 0, -1.86f},
				{1.0f, 0, -0.93f}, {1.0f, 0, 0.93f}, {1.0f, 0, -2.79f}, {1.0f, 0, 2.79f},
				{3.0f, 0, -0.93f}, {3.0f, 0, 0.93f}, {-1.0f, 0, -0.93f}, {-1.0f, 0, 0.93f}
			};

			// The 30 edges, defined as pairs of vertex indices
			std::vector<std::pair<int, int>> edges = {
				{0,11}, {0,7}, {0,5}, {0,1}, {1,5}, {1,9}, {1,7}, {2,3}, {2,4}, {2,6},
				{2,10}, {3,4}, {3,9}, {3,8}, {4,11}, {4,9}, {5,11}, {5,9}, {6,7}, {6,8},
				{6,10}, {7,8}, {8,9}, {10,11}, {0,10}, {1,8}, {2,11}, {3,10}, {4,5}, {6,9}
			};

			// --- SECTION 4: Pre-process the Final State (Scaling and Rotation) ---
			const float rot_rad = final_rotation_deg * 3.14159265f / 180.0f;
			const float cos_r = cos(rot_rad);
			const float sin_r = sin(rot_rad);

			for (auto& v : vertices_3d) {
				// Scale it to the desired size
				v = vec_scale(v, scale);
				// Apply a rotation around the Y-axis for the final animation state
				float x_new = v.x * cos_r - v.z * sin_r;
				float z_new = v.x * sin_r + v.z * cos_r;
				v.x = x_new;
				v.z = z_new;
			}
			for (auto& v : vertices_2d) {
				// Scale the start state as well
				v = vec_scale(v, scale);
			}


			// --- SECTION 5: Create the Morphing Lines ---
			// Iterate through each edge and create a line that morphs from the 2D net to the 3D shape.
			for (const auto& edge : edges)
			{
				Line& line = add_line();

				const Vec3& p0_start = vertices_2d[edge.first];
				const Vec3& p1_start = vertices_2d[edge.second];
				const Vec3& p0_end = vertices_3d[edge.first];
				const Vec3& p1_end = vertices_3d[edge.second];

				// Set START state (flat net)
				line.x0.start = p0_start.x; line.y0.start = p0_start.y; line.z0.start = p0_start.z;
				line.x1.start = p1_start.x; line.y1.start = p1_start.y; line.z1.start = p1_start.z;

				// Set END state (closed 3D shape)
				line.x0.end = p0_end.x; line.y0.end = p0_end.y; line.z0.end = p0_end.z;
				line.x1.end = p1_end.x; line.y1.end = p1_end.y; line.z1.end = p1_end.z;

				// Animate color and thickness
				line.rgb_t0 = start_color;
				line.rgb_t1 = end_color;
				line.thickness.start = 0.006f;
				line.thickness.end = 0.008f; // Lines get slightly bolder as the shape forms
				line.number_of_cubes = 100;
			}
		}
		

		void init_hyperbolic_gyroid_lattice()
		{
			lines.clear();

			// Self-contained constants
			const float TAU = 6.283185307179586476925286766559f;
			const float PI = 3.141592653589793238462643383279f;
			const float PHI = 1.6180339887498948482045868343656f; // Golden ratio

			// Gyroid implicit function: sin(x)cos(y) + sin(y)cos(z) + sin(z)cos(x) = 0
			auto gyroid_field = [&](float x, float y, float z) -> float {
				return std::sin(x) * std::cos(y) + std::sin(y) * std::cos(z) + std::sin(z) * std::cos(x);
				};

			// Hyperbolic radial warp (Poincaré ball model)
			auto hyperbolic_warp = [&](Vec3 p) -> Vec3 {
				float r2 = p.x * p.x + p.y * p.y + p.z * p.z;
				if (r2 >= 1.0f) return p; // Clamp to unit ball
				float factor = 2.0f / (1.0f - r2);
				return { p.x * factor, p.y * factor, p.z * factor };
				};

			// Approximate surface normal via finite differences
			auto gyroid_normal = [&](float x, float y, float z) -> Vec3 {
				const float eps = 0.01f;
				float dx = gyroid_field(x + eps, y, z) - gyroid_field(x - eps, y, z);
				float dy = gyroid_field(x, y + eps, z) - gyroid_field(x, y - eps, z);
				float dz = gyroid_field(x, y, z + eps) - gyroid_field(x, y, z - eps);
				return vec_normalize({ dx, dy, dz });
				};

			// Generate base gyroid mesh on a 3D grid
			const int grid_size = 18;
			const float scale = 1.8f;
			const float step = scale / (grid_size - 1);
			const float offset = -scale * 0.5f;

			std::vector<Vec3> vertices;
			std::vector<std::pair<int, int>> edges;

			// Step 1: Sample gyroid isosurface via marching cubes (simplified edge detection)
			for (int ix = 0; ix < grid_size - 1; ++ix)
				for (int iy = 0; iy < grid_size - 1; ++iy)
					for (int iz = 0; iz < grid_size - 1; ++iz)
					{
						Vec3 base = { offset + ix * step, offset + iy * step, offset + iz * step };
						float v000 = gyroid_field(base.x, base.y, base.z);
						float v100 = gyroid_field(base.x + step, base.y, base.z);
						float v010 = gyroid_field(base.x, base.y + step, base.z);
						float v001 = gyroid_field(base.x, base.y, base.z + step);

						// Detect zero-crossings (simplified)
						auto add_edge_if_cross = [&](float a, float b, Vec3 pa, Vec3 pb) {
							if ((a < 0 && b >= 0) || (a >= 0 && b < 0)) {
								float t = std::fabs(a) / (std::fabs(a) + std::fabs(b));
								Vec3 interp = {
									pa.x + t * (pb.x - pa.x),
									pa.y + t * (pb.y - pa.y),
									pa.z + t * (pb.z - pa.z)
								};
								vertices.push_back(interp);
								return (int)vertices.size() - 1;
							}
							return -1;
							};

						int p0 = add_edge_if_cross(v000, v100, base, { base.x + step, base.y, base.z });
						int p1 = add_edge_if_cross(v000, v010, base, { base.x, base.y + step, base.z });
						int p2 = add_edge_if_cross(v000, v001, base, { base.x, base.y, base.z + step });

						if (p0 != -1 && p1 != -1) edges.push_back({ p0, p1 });
						if (p1 != -1 && p2 != -1) edges.push_back({ p1, p2 });
						if (p2 != -1 && p0 != -1) edges.push_back({ p2, p0 });
					}

			// Step 2: Subdivide edges with quasicrystalline logic (golden ratio splits)
			const int subdivisions = 2;
			for (int sub = 0; sub < subdivisions; ++sub)
			{
				std::vector<std::pair<int, int>> new_edges;
				size_t orig_size = edges.size();
				for (size_t i = 0; i < orig_size; ++i)
				{
					auto [a, b] = edges[i];
					Vec3 pa = vertices[a];
					Vec3 pb = vertices[b];
					Vec3 dir = vec_normalize(vec_add(pb, vec_scale(pa, -1.0f)));
					float len = vec_length(vec_add(pb, vec_scale(pa, -1.0f)));

					// Split at golden ratio points
					float t1 = 1.0f / PHI;
					float t2 = 1.0f - t1;

					Vec3 mid1 = vec_add(pa, vec_scale(dir, len * t1));
					Vec3 mid2 = vec_add(pa, vec_scale(dir, len * t2));

					int m1 = (int)vertices.size(); vertices.push_back(mid1);
					int m2 = (int)vertices.size(); vertices.push_back(mid2);

					// Replace edge with 3 segments
					new_edges.push_back({ a, m1 });
					new_edges.push_back({ m1, m2 });
					new_edges.push_back({ m2, b });
				}
				edges = std::move(new_edges);
			}

			// Step 3: Emit animated lines with curvature-based coloring
			for (auto [a, b] : edges)
			{
				Vec3 pa = vertices[a];
				Vec3 pb = vertices[b];

				// Warp into hyperbolic space
				Vec3 pa_hyp = hyperbolic_warp(pa);
				Vec3 pb_hyp = hyperbolic_warp(pb);

				// Compute curvature sign at midpoint
				Vec3 mid = { (pa.x + pb.x) * 0.5f, (pa.y + pb.y) * 0.5f, (pa.z + pb.z) * 0.5f };
				Vec3 normal = gyroid_normal(mid.x, mid.y, mid.z);
				float curvature = normal.x + normal.y + normal.z; // Approximate mean curvature sign

				// Color by curvature
				Vec3 color;
				if (curvature > 0.1f) {
					// Dome (positive curvature)
					color = { 0.9f, 0.3f, 0.2f };
				}
				else if (curvature < -0.1f) {
					// Saddle (negative curvature)
					color = { 0.2f, 0.3f, 0.9f };
				}
				else {
					// Flat (minimal surface)
					color = { 0.3f, 0.9f, 0.3f };
				}

				Line& line = add_line();
				// START: collapsed at origin (clean reveal)
				line.x0.start = 0.0f; line.y0.start = 0.0f; line.z0.start = 0.0f;
				line.x1.start = 0.0f; line.y1.start = 0.0f; line.z1.start = 0.0f;
				line.rgb_t0 = color;
				line.thickness.start = 0.002f;
				line.number_of_cubes = 12;

				// END: hyperbolic gyroid edge
				line.copy_start_to_end();
				line.x0.end = pa_hyp.x; line.y0.end = pa_hyp.y; line.z0.end = pa_hyp.z;
				line.x1.end = pb_hyp.x; line.y1.end = pb_hyp.y; line.z1.end = pb_hyp.z;
				line.rgb_t1 = color;
				line.thickness.end = 0.004f + 0.003f * std::fabs(curvature);
			}

			// Step 4: Add Penrose quasicrystal frame around boundary
			const int penrose_rings = 5;
			const float penrose_radius = 2.2f;
			for (int ring = 0; ring < penrose_rings; ++ring)
			{
				float r = penrose_radius * (ring + 1) / penrose_rings;
				int points = 5 * (ring + 1);
				for (int i = 0; i < points; ++i)
				{
					float angle1 = TAU * i / points;
					float angle2 = TAU * (i + 1) / points;

					// Apply 5-fold quasiperiodic modulation
					float mod1 = 1.0f + 0.3f * std::cos(5.0f * angle1 + ring * 0.4f);
					float mod2 = 1.0f + 0.3f * std::cos(5.0f * angle2 + ring * 0.4f);

					Vec3 p1 = { r * mod1 * std::cos(angle1), 0.0f, r * mod1 * std::sin(angle1) };
					Vec3 p2 = { r * mod2 * std::cos(angle2), 0.0f, r * mod2 * std::sin(angle2) };

					Line& frame = add_line();
					frame.x0.start = p1.x; frame.y0.start = p1.y; frame.z0.start = p1.z;
					frame.x1.start = p2.x; frame.y1.start = p2.y; frame.z1.start = p2.z;
					frame.rgb_t0 = { 0.8f, 0.8f, 0.2f };
					frame.thickness.start = 0.006f;
					frame.number_of_cubes = 20;
					frame.copy_start_to_end();

					// Animate: spiral inward
					frame.x0.end = 0.0f; frame.y0.end = 0.0f; frame.z0.end = 0.0f;
					frame.x1.end = 0.0f; frame.y1.end = 0.0f; frame.z1.end = 0.0f;
				}
			}
		}

		

		void init_0013_sea_and_sky_zup()
		{
			lines.clear();

			const float TAU = 6.28318530718f;

			// --------------------
			// tiny helpers
			// --------------------
			auto clamp01 = [](float v) { return std::max(0.0f, std::min(1.0f, v)); };
			auto mixf = [](float a, float b, float t) { return a + (b - a) * t; };
			auto length2 = [](float x, float y) { return std::sqrt(x * x + y * y); };

			auto add_line_collapsed = [&](Vec3 A, Vec3 B, Vec3 c0, Vec3 c1,
				float thickStart, float thickEnd, int cubes)
				{
					Line& L = add_line();
					// start collapsed at midpoint (nice reveal)
					Vec3 M{ 0.5f * (A.x + B.x), 0.5f * (A.y + B.y), 0.5f * (A.z + B.z) };
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = c0;
					L.thickness.start = thickStart;
					L.number_of_cubes = cubes;
					L.copy_start_to_end();
					L.x0.end = A.x; L.y0.end = A.y; L.z0.end = A.z;
					L.x1.end = B.x; L.y1.end = B.y; L.z1.end = B.z;
					L.rgb_t1 = c1;
					L.thickness.end = thickEnd;
				};

			// =========================================================
			// A) OCEAN  wavy Z-up heightfield in the XY plane (below horizon)
			// =========================================================
			{
				// knobs
				const int   gridX = 56;
				const int   gridY = 42;
				const float extentX = 3.0f;     // world width (X)
				const float extentY = 2.6f;     // world depth (Y)
				const float seaZ = -0.08f;   // base sea level
				const float ampZ0 = 0.18f;    // starting amplitude
				const float ampZ1 = 0.32f;    // ending amplitude (tween up)
				const float windX = 0.06f;    // sideways drift in X at end
				const float windY = 0.04f;    // sideways drift in Y at end
				const int   cubesPerSeg = 18;
				const float baseThick = 0.0065f;

				// "sun" position (affects reflection highlight on water)
				const float sunX = -0.6f;   // left/right
				const float sunY = -0.2f;   // depth
				const float sunZ = 0.28f;  // height above horizon

				// wave shape (two swells + a choppy term), Z-up
				auto waves = [&](float x, float y, float amp)->float {
					float kx = 1.8f, ky = 1.3f;
					float s1 = std::sin(kx * x + 0.6f * y);
					float s2 = std::sin(0.7f * x - 1.9f * y + 1.7f);
					float ch = 0.35f * std::sin(3.4f * x + 2.2f * y);
					// bias amplitude so far distance (larger |y|) looks calmer
					float fade = 0.5f + 0.5f * std::exp(-0.9f * std::fabs(y));
					return amp * fade * (0.55f * s1 + 0.45f * s2 + 0.25f * ch);
					};

				// slope estimate for foam and thickness
				auto slopeMag = [&](float x, float y, float amp)->float {
					const float e = 0.012f;
					float hdx = waves(x + e, y, amp) - waves(x - e, y, amp);
					float hdy = waves(x, y + e, amp) - waves(x, y - e, amp);
					return std::sqrt(hdx * hdx + hdy * hdy) / (2.0f * e);
					};

				// ocean palette (deep -> teal)
				auto ocean = [&](float t)->Vec3 {
					t = clamp01(t);
					float r = mixf(0.04f, 0.10f, t);
					float g = mixf(0.18f, 0.75f, t);
					float b = mixf(0.35f, 0.95f, t);
					return { r, g, b };
					};

				const float dx = (gridX > 1) ? extentX / float(gridX - 1) : 0.0f;
				const float dy = (gridY > 1) ? extentY / float(gridY - 1) : 0.0f;
				auto Xat = [&](int j) { return -0.5f * extentX + j * dx; };
				auto Yat = [&](int i) { return -0.5f * extentY + i * dy; };

				// helper: build one water segment from (x0,y0) to (x1,y1)
				auto add_water_seg = [&](float x0, float y0, float x1, float y1)
					{
						// start/end amplitudes for tween
						float h0s = seaZ + waves(x0, y0, ampZ0);
						float h1s = seaZ + waves(x1, y1, ampZ0);
						float h0e = seaZ + waves(x0 + 0.15f, y0 + 0.10f, ampZ1); // slight phase shift (motion)
						float h1e = seaZ + waves(x1 + 0.15f, y1 + 0.10f, ampZ1);

						// wind drift at the end (subtle)
						float x0e = x0 + windX * std::sin(0.6f * y0 + 0.8f * x0);
						float y0e = y0 + windY * std::cos(0.7f * x0 - 0.9f * y0);
						float x1e = x1 + windX * std::sin(0.6f * y1 + 0.8f * x1);
						float y1e = y1 + windY * std::cos(0.7f * x1 - 0.9f * y1);

						// sun reflection highlight  brightens near a vertical ribbon under the sun
						float sigmaX = 0.22f; // width of highlight band in X
						auto highlight = [&](float x, float y)->float {
							float dxs = (x - sunX);
							float base = std::exp(-0.5f * (dxs * dxs) / (sigmaX * sigmaX));
							// fade with distance in depth and with wave height (sparkle near crests)
							float depthFade = clamp01(1.0f - 0.35f * std::fabs(y - sunY));
							return 0.8f * base * depthFade;
							};

						float foam0 = clamp01(1.5f * slopeMag(x0, y0, ampZ1));
						float foam1 = clamp01(1.5f * slopeMag(x1, y1, ampZ1));
						float hil0 = highlight(x0, y0);
						float hil1 = highlight(x1, y1);

						// color blend: water + foam + reflection
						auto waterCol = [&](float z, float foam, float hil)->Vec3 {
							float deepT = clamp01((z - (seaZ - 0.35f)) / 0.9f);
							Vec3 base = ocean(deepT);
							// foam -> push to near white; reflection -> push to warm gold
							Vec3 foamCol{ 0.92f, 0.96f, 0.98f };
							Vec3 gold{ 1.00f, 0.88f, 0.52f };
							// combine
							base.x = mixf(base.x, foamCol.x, 0.55f * foam);
							base.y = mixf(base.y, foamCol.y, 0.55f * foam);
							base.z = mixf(base.z, foamCol.z, 0.55f * foam);
							base.x = mixf(base.x, gold.x, 0.75f * hil);
							base.y = mixf(base.y, gold.y, 0.75f * hil);
							base.z = mixf(base.z, gold.z, 0.55f * hil);
							return base;
							};

						Vec3 A_start{ x0, y0, h0s }, B_start{ x1, y1, h1s };
						Vec3 A_end{ x0e, y0e, h0e }, B_end{ x1e, y1e, h1e };

						Vec3 c0 = waterCol(0.5f * (h0s + h1s), 0.5f * (foam0 + foam1), 0.5f * (hil0 + hil1));
						Vec3 c1 = waterCol(0.5f * (h0e + h1e), 0.5f * (foam0 + foam1), 0.5f * (hil0 + hil1));

						float thick0 = baseThick * (0.8f + 0.5f * foam0);
						float thick1 = baseThick * (0.8f + 0.5f * foam1);

						add_line_collapsed(A_start, B_start, c0, c1,
							baseThick * 0.35f, 0.5f * (thick0 + thick1), cubesPerSeg);
					};

				// draw rows (constant Y)
				for (int i = 0; i < gridY; ++i) {
					float y = Yat(i);
					for (int j = 0; j < gridX - 1; ++j) {
						add_water_seg(Xat(j), y, Xat(j + 1), y);
					}
				}
				// draw cols (constant X)
				for (int j = 0; j < gridX; ++j) {
					float x = Xat(j);
					for (int i = 0; i < gridY - 1; ++i) {
						add_water_seg(x, Yat(i), x, Yat(i + 1));
					}
				}
			}

			// =========================================================
			// B) SKY  vertical gradient curtains + Sun bloom (above horizon)
			// =========================================================
			{
				const int   skyCols = 54;          // density across X
				const int   skyRows = 10;          // a few bands along Y (depth)
				const float extentX = 3.0f;
				const float extentY = 2.6f;
				const float zHorizon = 0.00f;      // horizon height
				const float zTop = 1.35f;      // sky top
				const float baseThick = 0.0060f;
				const int   cubes = 22;

				// same sun as before (for warm near-horizon tint)
				const float sunX = -0.6f, sunY = -0.2f, sunZ = 0.28f;

				auto skyGrad = [&](float z)->Vec3 {
					// warm near horizon -> cool blue overhead
					float t = clamp01((z - zHorizon) / (zTop - zHorizon));
					Vec3 warm{ 0.98f, 0.78f, 0.55f }; // apricot glow
					Vec3 cool{ 0.24f, 0.52f, 0.95f }; // clear blue
					return {
						mixf(warm.x, cool.x, t),
						mixf(warm.y, cool.y, t),
						mixf(warm.z, cool.z, t)
					};
					};

				auto Xat = [&](int j) { return -0.5f * extentX + j * (extentX / std::max(1, skyCols - 1)); };
				auto Yat = [&](int i) { return -0.5f * extentY + i * (extentY / std::max(1, skyRows - 1)); };

				for (int i = 0; i < skyRows; ++i)
				{
					for (int j = 0; j < skyCols; ++j)
					{
						float x = Xat(j), y = Yat(i);

						// subtle horizontal sway at the end (like heat haze)
						float swayX = 0.04f * std::sin(0.8f * y + 0.6f * x);
						float swayY = 0.03f * std::cos(0.7f * x - 0.9f * y);

						// color tint towards sun horizontally
						float ds = std::exp(-2.0f * ((x - sunX) * (x - sunX) + 0.4f * (y - sunY) * (y - sunY)));
						Vec3 cStart = skyGrad(zHorizon + 0.001f);
						Vec3 cEnd = skyGrad(zTop);
						// warm boost near sun at the end
						cEnd.x = mixf(cEnd.x, 1.00f, 0.35f * ds);
						cEnd.y = mixf(cEnd.y, 0.88f, 0.35f * ds);
						cEnd.z = mixf(cEnd.z, 0.60f, 0.25f * ds);

						Vec3 A{ x, y, zHorizon }, B{ x + swayX, y + swayY, zTop };
						add_line_collapsed(A, B, cStart, cEnd, baseThick * 0.28f, baseThick, cubes);
					}
				}

				// --- Sun disk (ring of chords), blooming from horizon
				{
					const int   rays = 64;
					const float R0 = 0.14f;    // start radius
					const float R1 = 0.22f;    // end radius (bloom)
					const float thick = 0.009f;
					const int   rcubes = 30;

					for (int k = 0; k < rays; ++k)
					{
						float a0 = (k / float(rays)) * TAU;
						float a1 = a0 + (0.75f * TAU / rays); // short chord
						Vec3 S{ sunX + R0 * std::cos(a0), sunY + R0 * std::sin(a0), sunZ };
						Vec3 E{ sunX + R0 * std::cos(a1), sunY + R0 * std::sin(a1), sunZ };

						Vec3 S2{ sunX + R1 * std::cos(a0), sunY + R1 * std::sin(a0), sunZ };
						Vec3 E2{ sunX + R1 * std::cos(a1), sunY + R1 * std::sin(a1), sunZ };

						Vec3 warm0{ 1.00f, 0.88f, 0.58f };
						Vec3 warm1{ 1.00f, 0.95f, 0.85f };
						add_line_collapsed(S, E, warm0, warm1, thick * 0.25f, thick, rcubes);
						add_line_collapsed(S2, E2, warm1, warm0, thick * 0.25f, thick, rcubes);
					}
				}

				// --- Clouds: soft puffs drifting in depth (Y)
				{
					const int clouds = 8;
					for (int c = 0; c < clouds; ++c)
					{
						float cx = -1.4f + 2.8f * Random::generate_random_float_0_to_1();
						float cy = -1.1f + 2.2f * Random::generate_random_float_0_to_1();
						float cz = 0.55f + 0.55f * Random::generate_random_float_0_to_1();
						int   puffs = 5 + (Random::random_int(0, 1000) % 4);
						float baseR = 0.10f + 0.08f * Random::generate_random_float_0_to_1();

						for (int p = 0; p < puffs; ++p)
						{
							float pr = baseR * (0.75f + 0.5f * Random::generate_random_float_0_to_1());
							float px = cx + 0.65f * pr * Random::generate_random_float_minus_one_to_plus_one();
							float py = cy + 0.65f * pr * Random::generate_random_float_minus_one_to_plus_one();
							float pz = cz + 0.06f * Random::generate_random_float_minus_one_to_plus_one();

							int segs = 22;
							for (int s = 0; s < segs; ++s)
							{
								float a0 = (s / float(segs)) * TAU;
								float a1 = ((s + 1) / float(segs)) * TAU;

								Vec3 S{ px + 0.5f * pr * std::cos(a0), py + 0.8f * pr * std::sin(a0), pz };
								Vec3 E{ px + 0.5f * pr * std::cos(a1), py + 0.8f * pr * std::sin(a1), pz };
								// drift in depth by end
								Vec3 S2{ S.x, S.y + 0.12f, S.z + 0.03f };
								Vec3 E2{ E.x, E.y + 0.12f, E.z + 0.03f };

								Vec3 c0{ 0.94f, 0.96f, 0.98f };
								Vec3 c1{ 0.88f, 0.92f, 0.98f };
								add_line_collapsed(S, E, c0, c1, 0.0045f, 0.0075f, 16);
							}
						}
					}
				}
			}

			// =========================================================
			// C) HORIZON accent  a thin bright line where sea meets sky
			// =========================================================
			{
				const int   segs = 80;
				const float extentX = 3.0f;
				const float yNear = -1.3f, yFar = 1.3f; // a gentle bow in depth for curvature
				const float zH = 0.002f; // slightly above zero for clarity
				for (int i = 0; i < segs; ++i)
				{
					float t0 = i / float(segs);
					float t1 = (i + 1) / float(segs);
					float x0 = mixf(-0.5f * extentX, 0.5f * extentX, t0);
					float x1 = mixf(-0.5f * extentX, 0.5f * extentX, t1);
					// shallow curve in Y so horizon reads better in 3D
					float y0 = 0.15f * std::sin(0.7f * t0) * (yFar - yNear) * 0.06f;
					float y1 = 0.15f * std::sin(0.7f * t1) * (yFar - yNear) * 0.06f;

					Vec3 A{ x0, y0, zH }, B{ x1, y1, zH };
					Vec3 c0{ 0.95f, 0.98f, 1.00f }, c1{ 0.85f, 0.95f, 1.00f };
					add_line_collapsed(A, B, c0, c1, 0.0035f, 0.0065f, 26);
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
				// lines.init_0005_letter_layers_fly_yz_swapped();
				// lines.init_fractal_tree_3d();
				// lines.init_crystal_fractal_tree();
				// lines.init_0006_wavy_dune_landscape();
				// lines.init_brutalist_monolith();
				// lines.init_0007_tesseract_warp();
				// lines.init_0008_supershape_ribbons();
				// lines.init_0009_supershape_matrix_rain();
				// lines.init_0011_flip_unique_matrix_orbit();
				// lines.init_geometric_unfolding();
				// lines.init_hyperbolic_gyroid_lattice();



				lines.init_0013_sea_and_sky_zup();






				

				
				

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