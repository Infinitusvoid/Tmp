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
				lines.init_0002_trefoil_weave();


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