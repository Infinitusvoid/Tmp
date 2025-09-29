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

					line.thickness.start = 0.01 * 0.2;
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

					line.thickness.start = 0.005f * 0.2f;
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

		void init_temporal_echo_cascade()
		{
			lines.clear();

			// Self-contained constants
			const float TAU = 6.283185307179586476925286766559f;
			const float PI = 3.141592653589793238462643383279f;

			// --- System Parameters ---
			const int generations = 4; // How many "echo" levels
			const int wave_segments = 120; // Resolution of each spherical wavefront
			const float initial_radius = 0.05f; // Size of the initial pulse
			const float max_radius = 1.2f; // Boundary where waves reverse
			const float expansion_speed = 0.8f; // How fast waves grow
			const float echo_shrink_factor = 0.6f; // How much smaller each echo generation is
			const int sub_wave_count = 6; // How many secondary waves spawn from each wave edge
			const float sub_wave_offset = 0.15f; // Distance from edge for secondary spawn

			// Helper: Generate points on a sphere (for wavefronts)
			auto generate_sphere_points = [&](float radius, int segments) -> std::vector<Vec3> {
				std::vector<Vec3> points;
				points.reserve(segments);
				for (int i = 0; i < segments; ++i) {
					float theta = TAU * i / segments;
					float phi = PI * Random::generate_random_float_0_to_1(); // Uniform distribution on sphere
					float sin_phi = std::sin(phi);
					points.push_back({
						radius * sin_phi * std::cos(theta),
						radius * sin_phi * std::sin(theta),
						radius * std::cos(phi)
						});
				}
				return points;
				};

			// Helper: Color based on wave age/generation (brighter = newer)
			auto age_color = [&](int gen, float progress) -> Vec3 {
				float brightness = std::pow(0.7f, gen); // Each generation is dimmer
				float hue_shift = 0.1f * gen; // Shift color per generation
				float t = 0.5f + 0.5f * std::sin(TAU * (0.3f * progress + hue_shift));
				return {
					brightness * (0.5f + 0.5f * t),
					brightness * (0.5f + 0.5f * std::sin(TAU * (0.33f + 0.3f * progress + hue_shift))),
					brightness * (0.5f + 0.5f * std::sin(TAU * (0.66f + 0.3f * progress + hue_shift)))
				};
				};

			// --- Recursive Cascade Generator ---
			std::function<void(float, int, Vec3)> spawn_generation = [&](float start_time, int gen, Vec3 origin) {
				if (gen >= generations) return;

				// Calculate time scale for this generation
				float gen_scale = std::pow(echo_shrink_factor, gen);
				float gen_initial_radius = initial_radius * gen_scale;
				float gen_max_radius = max_radius * gen_scale;
				float gen_expansion_speed = expansion_speed * gen_scale;

				// --- Primary Wave ---
				auto primary_points = generate_sphere_points(gen_initial_radius, wave_segments);
				for (int i = 0; i < wave_segments; ++i) {
					Vec3 start_pos = origin;
					Vec3 end_pos = primary_points[i];

					Line& primary_line = add_line();
					primary_line.x0.start = start_pos.x;
					primary_line.y0.start = start_pos.y;
					primary_line.z0.start = start_pos.z;
					primary_line.x1.start = start_pos.x;
					primary_line.y1.start = start_pos.y;
					primary_line.z1.start = start_pos.z; // Start collapsed at origin

					// Animate: expand and then contract (echo)
					float mid_time = start_time + 0.5f; // Midpoint is full expansion
					primary_line.copy_start_to_end();
					// Full expansion at mid_time
					primary_line.x0.end = origin.x + (end_pos.x - origin.x) * (gen_max_radius / gen_initial_radius);
					primary_line.y0.end = origin.y + (end_pos.y - origin.y) * (gen_max_radius / gen_initial_radius);
					primary_line.z0.end = origin.z + (end_pos.z - origin.z) * (gen_max_radius / gen_initial_radius);
					primary_line.x1.end = primary_line.x0.end;
					primary_line.y1.end = primary_line.y0.end;
					primary_line.z1.end = primary_line.z0.end;

					// Color: Bright for new generation
					Vec3 color = age_color(gen, 0.0f);
					primary_line.rgb_t0 = color;
					primary_line.rgb_t1 = color;
					primary_line.thickness.start = 0.005f * gen_scale;
					primary_line.thickness.end = 0.002f * gen_scale;
					primary_line.number_of_cubes = 40;
				}

				// --- Secondary Waves Spawned at Wave Edge ---
				if (gen < generations - 1) { // Don't spawn children for the last gen
					float spawn_time = start_time + 0.45f; // Spawn just before the echo
					for (int j = 0; j < sub_wave_count; ++j) {
						// Pick a random point on the primary wave's edge at its full expansion
						int idx = (j * wave_segments) / sub_wave_count;
						Vec3 spawn_origin = {
							origin.x + (primary_points[idx].x - origin.x) * (gen_max_radius / gen_initial_radius),
							origin.y + (primary_points[idx].y - origin.y) * (gen_max_radius / gen_initial_radius),
							origin.z + (primary_points[idx].z - origin.z) * (gen_max_radius / gen_initial_radius)
						};
						// Add small jitter to spawn location
						spawn_origin.x += sub_wave_offset * Random::generate_random_float_minus_one_to_plus_one();
						spawn_origin.y += sub_wave_offset * Random::generate_random_float_minus_one_to_plus_one();
						spawn_origin.z += sub_wave_offset * Random::generate_random_float_minus_one_to_plus_one();

						// Recursively spawn the next generation
						spawn_generation(spawn_time, gen + 1, spawn_origin);
					}
				}
				};

			// --- Start the Cascade ---
			spawn_generation(0.0f, 0, { 0.0f, 0.0f, 0.0f }); // Start at origin, generation 0

			// --- Add a Faint Static Grid for Spatial Reference ---
			const float grid_size = 2.0f;
			const int grid_lines = 10;
			const float step = grid_size / grid_lines;
			Vec3 grid_color = { 0.1f, 0.1f, 0.12f };

			for (int i = 0; i <= grid_lines; ++i) {
				float pos = -grid_size * 0.5f + i * step;
				// XZ plane grid
				{
					Line& line = add_line();
					line.x0.start = -grid_size * 0.5f; line.y0.start = -0.5f; line.z0.start = pos;
					line.x1.start = grid_size * 0.5f; line.y1.start = -0.5f; line.z1.start = pos;
					line.x0.end = line.x0.start; line.y0.end = line.y0.start; line.z0.end = line.z0.start;
					line.x1.end = line.x1.start; line.y1.end = line.y1.start; line.z1.end = line.z1.start;
					line.rgb_t0 = grid_color; line.rgb_t1 = grid_color;
					line.thickness.start = 0.001f; line.thickness.end = 0.001f;
					line.number_of_cubes = 100;
				}
				{
					Line& line = add_line();
					line.x0.start = pos; line.y0.start = -0.5f; line.z0.start = -grid_size * 0.5f;
					line.x1.start = pos; line.y1.start = -0.5f; line.z1.start = grid_size * 0.5f;
					line.x0.end = line.x0.start; line.y0.end = line.y0.start; line.z0.end = line.z0.start;
					line.x1.end = line.x1.start; line.y1.end = line.y1.start; line.z1.end = line.z1.start;
					line.rgb_t0 = grid_color; line.rgb_t1 = grid_color;
					line.thickness.start = 0.001f; line.thickness.end = 0.001f;
					line.number_of_cubes = 100;
				}
			}
		}

		// --- Ocean + Sky + Rising Glyphs (Z is up; Y is depth) ---
		void init_2010_ocean_glyph_overture()
		{
			lines.clear();

			const float TAU = 6.28318530718f;

			auto clamp01 = [](float v) { return std::max(0.0f, std::min(1.0f, v)); };
			auto mixf = [](float a, float b, float t) { return a + (b - a) * t; };

			// helper to emit a line that "reveals" from its midpoint
			auto emit = [&](Vec3 A, Vec3 B, Vec3 c0, Vec3 c1, float t0, float t1, int cubes)
				{
					Line& L = add_line();
					Vec3 M{ 0.5f * (A.x + B.x), 0.5f * (A.y + B.y), 0.5f * (A.z + B.z) };
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = c0; L.thickness.start = t0; L.number_of_cubes = cubes;
					L.copy_start_to_end();
					L.x0.end = A.x; L.y0.end = A.y; L.z0.end = A.z;
					L.x1.end = B.x; L.y1.end = B.y; L.z1.end = B.z;
					L.rgb_t1 = c1; L.thickness.end = t1;
				};

			// ===============================
			// A) OCEAN (Z-up heightfield)
			// ===============================
			{
				const int   GX = 48, GY = 34;
				const float EX = 2.8f, EY = 2.2f;
				const float seaZ = -0.08f, amp0 = 0.16f, amp1 = 0.30f;
				const float windX = 0.05f, windY = 0.04f;
				const float baseTh = 0.0062f;
				const int   cubes = 18;

				// sun for reflection
				const float sunX = -0.58f, sunY = -0.22f, sunZ = 0.26f;

				auto waves = [&](float x, float y, float amp) {
					float s1 = std::sin(1.8f * x + 0.6f * y);
					float s2 = std::sin(0.7f * x - 1.9f * y + 1.7f);
					float ch = 0.35f * std::sin(3.4f * x + 2.2f * y);
					float fade = 0.5f + 0.5f * std::exp(-0.9f * std::fabs(y));
					return amp * fade * (0.55f * s1 + 0.45f * s2 + 0.25f * ch);
					};
				auto slopeMag = [&](float x, float y, float amp) {
					const float e = 0.012f;
					float dx = waves(x + e, y, amp) - waves(x - e, y, amp);
					float dy = waves(x, y + e, amp) - waves(x, y - e, amp);
					return std::sqrt(dx * dx + dy * dy) / (2.0f * e);
					};
				auto oceanCol = [&](float z, float foam, float hil)->Vec3 {
					float t = clamp01((z - (seaZ - 0.35f)) / 0.9f);
					Vec3 base{ mixf(0.04f,0.10f,t), mixf(0.18f,0.75f,t), mixf(0.35f,0.95f,t) };
					Vec3 foamC{ 0.92f,0.96f,0.98f }, gold{ 1.0f,0.88f,0.52f };
					base.x = mixf(base.x, foamC.x, 0.55f * foam);
					base.y = mixf(base.y, foamC.y, 0.55f * foam);
					base.z = mixf(base.z, foamC.z, 0.55f * foam);
					base.x = mixf(base.x, gold.x, 0.75f * hil);
					base.y = mixf(base.y, gold.y, 0.75f * hil);
					base.z = mixf(base.z, gold.z, 0.55f * hil);
					return base;
					};
				auto highlight = [&](float x, float y) {
					float dx = (x - sunX), sy = 0.22f;
					float band = std::exp(-0.5f * (dx * dx) / (sy * sy));
					float depth = clamp01(1.0f - 0.35f * std::fabs(y - sunY));
					return 0.8f * band * depth;
					};

				float dx = (GX > 1) ? EX / float(GX - 1) : 0.0f;
				float dy = (GY > 1) ? EY / float(GY - 1) : 0.0f;
				auto X = [&](int j) {return -0.5f * EX + j * dx; };
				auto Y = [&](int i) {return -0.5f * EY + i * dy; };

				auto addSeg = [&](float x0, float y0, float x1, float y1) {
					float z0s = seaZ + waves(x0, y0, amp0), z1s = seaZ + waves(x1, y1, amp0);
					float z0e = seaZ + waves(x0 + 0.15f, y0 + 0.10f, amp1);
					float z1e = seaZ + waves(x1 + 0.15f, y1 + 0.10f, amp1);
					float x0e = x0 + windX * std::sin(0.6f * y0 + 0.8f * x0);
					float y0e = y0 + windY * std::cos(0.7f * x0 - 0.9f * y0);
					float x1e = x1 + windX * std::sin(0.6f * y1 + 0.8f * x1);
					float y1e = y1 + windY * std::cos(0.7f * x1 - 0.9f * y1);

					float foam0 = clamp01(1.5f * slopeMag(x0, y0, amp1));
					float foam1 = clamp01(1.5f * slopeMag(x1, y1, amp1));
					float hil0 = highlight(x0, y0), hil1 = highlight(x1, y1);

					Vec3 A0{ x0, y0, z0s }, B0{ x1, y1, z1s };
					Vec3 A1{ x0e,y0e,z0e }, B1{ x1e,y1e,z1e };
					Vec3 c0 = oceanCol(0.5f * (z0s + z1s), 0.5f * (foam0 + foam1), 0.5f * (hil0 + hil1));
					Vec3 c1 = oceanCol(0.5f * (z0e + z1e), 0.5f * (foam0 + foam1), 0.5f * (hil0 + hil1));
					float th0 = baseTh * (0.8f + 0.5f * foam0), th1 = baseTh * (0.8f + 0.5f * foam1);

					emit(A0, B0, c0, c1, baseTh * 0.35f, 0.5f * (th0 + th1), cubes);
					};

				for (int i = 0; i < GY; i++) {
					float y = Y(i);
					for (int j = 0; j < GX - 1; j++) addSeg(X(j), y, X(j + 1), y);
				}
				for (int j = 0; j < GX; j++) {
					float x = X(j);
					for (int i = 0; i < GY - 1; i++) addSeg(x, Y(i), x, Y(i + 1));
				}
			}

			// ===============================
			// B) SKY (gradient curtains + sun + a few clouds)
			// ===============================
			{
				const int   C = 52, R = 9;
				const float EX = 2.8f, EY = 2.2f;
				const float zH = 0.0f, zTop = 1.30f;
				const float baseTh = 0.0058f;
				const int   cubes = 20;

				const float sunX = -0.58f, sunY = -0.22f, sunZ = 0.26f;

				auto skyGrad = [&](float z) {
					float t = clamp01((z - zH) / (zTop - zH));
					Vec3 warm{ 0.98f,0.78f,0.55f }, cool{ 0.24f,0.52f,0.95f };
					return Vec3{ mixf(warm.x,cool.x,t), mixf(warm.y,cool.y,t), mixf(warm.z,cool.z,t) };
					};

				auto X = [&](int j) { return -0.5f * EX + j * (EX / std::max(1, C - 1)); };
				auto Y = [&](int i) { return -0.5f * EY + i * (EY / std::max(1, R - 1)); };

				for (int i = 0; i < R; i++) for (int j = 0; j < C; j++) {
					float x = X(j), y = Y(i);
					float sx = 0.04f * std::sin(0.8f * y + 0.6f * x);
					float sy = 0.03f * std::cos(0.7f * x - 0.9f * y);
					float ds = std::exp(-2.0f * ((x - sunX) * (x - sunX) + 0.4f * (y - sunY) * (y - sunY)));
					Vec3 c0 = skyGrad(zH + 0.001f), c1 = skyGrad(zTop);
					c1.x = mixf(c1.x, 1.00f, 0.35f * ds);
					c1.y = mixf(c1.y, 0.88f, 0.35f * ds);
					c1.z = mixf(c1.z, 0.60f, 0.25f * ds);

					emit({ x,y,zH }, { x + sx,y + sy,zTop }, c0, c1, baseTh * 0.28f, baseTh, cubes);
				}

				// sun chords (small bloom)
				{
					const int rays = 64; const float R0 = 0.13f, R1 = 0.20f;
					const float th = 0.0088f;
					for (int k = 0; k < rays; k++) {
						float a0 = (k / float(rays)) * TAU, a1 = a0 + (0.75f * TAU / rays);
						Vec3 s0{ sunX + R0 * std::cos(a0), sunY + R0 * std::sin(a0), sunZ };
						Vec3 e0{ sunX + R0 * std::cos(a1), sunY + R0 * std::sin(a1), sunZ };
						Vec3 s1{ sunX + R1 * std::cos(a0), sunY + R1 * std::sin(a0), sunZ };
						Vec3 e1{ sunX + R1 * std::cos(a1), sunY + R1 * std::sin(a1), sunZ };
						emit(s0, e0, { 1.0f,0.90f,0.60f }, { 1.0f,0.96f,0.85f }, th * 0.25f, th, 28);
						emit(s1, e1, { 1.0f,0.96f,0.85f }, { 1.0f,0.90f,0.60f }, th * 0.25f, th, 28);
					}
				}

				// a few cloud puffs (elliptical rings)
				{
					int clouds = 7;
					for (int c = 0; c < clouds; c++) {
						float cx = -1.3f + 2.6f * Random::generate_random_float_0_to_1();
						float cy = -1.0f + 2.0f * Random::generate_random_float_0_to_1();
						float cz = 0.55f + 0.55f * Random::generate_random_float_0_to_1();
						int puffs = 4 + (Random::random_int(0, 1000) % 3);
						float baseR = 0.10f + 0.08f * Random::generate_random_float_0_to_1();
						for (int p = 0; p < puffs; p++) {
							float pr = baseR * (0.75f + 0.5f * Random::generate_random_float_0_to_1());
							float px = cx + 0.65f * pr * Random::generate_random_float_minus_one_to_plus_one();
							float py = cy + 0.65f * pr * Random::generate_random_float_minus_one_to_plus_one();
							int segs = 20;
							for (int s = 0; s < segs; s++) {
								float a0 = (s / float(segs)) * TAU, a1 = ((s + 1) / float(segs)) * TAU;
								Vec3 A{ px + 0.5f * pr * std::cos(a0), py + 0.8f * pr * std::sin(a0), cz };
								Vec3 B{ px + 0.5f * pr * std::cos(a1), py + 0.8f * pr * std::sin(a1), cz };
								emit(A, B, { 0.94f,0.96f,0.98f }, { 0.88f,0.92f,0.98f }, 0.0045f, 0.0075f, 16);
							}
						}
					}
				}
			}

			// ===============================
			// C) HORIZON accent
			// ===============================
			{
				const int segs = 80; const float EX = 2.8f; const float zH = 0.002f;
				for (int i = 0; i < segs; i++) {
					float t0 = i / float(segs), t1 = (i + 1) / float(segs);
					float x0 = mixf(-0.5f * EX, 0.5f * EX, t0), x1 = mixf(-0.5f * EX, 0.5f * EX, t1);
					float y0 = 0.06f * std::sin(0.7f * t0), y1 = 0.06f * std::sin(0.7f * t1);
					emit({ x0,y0,zH }, { x1,y1,zH }, { 0.95f,0.98f,1.0f }, { 0.85f,0.95f,1.0f }, 0.0035f, 0.0060f, 22);
				}
			}

			// ===============================
			// D) Rising Glyph Band (YZ swapped feel)
			// ===============================
			{
				// minimal stroke font (normalized [0,1]); just enough for "MATH"
				struct V2 { float x, y; }; using Stroke = std::pair<V2, V2>;
				auto pv = [&](std::vector<Stroke>& S, float x, float y0, float y1) {S.push_back({ {x,y0},{x,y1} }); };
				auto ph = [&](std::vector<Stroke>& S, float y, float x0, float x1) {S.push_back({ {x0,y},{x1,y} }); };
				auto pd = [&](std::vector<Stroke>& S, float x0, float y0, float x1, float y1) {S.push_back({ {x0,y0},{x1,y1} }); };
				auto strokes = [&](char C) {
					std::vector<Stroke>S; float m = 0.10f, l = m, r = 1.0f - m, b = m, t = 1.0f - m, mid = 0.5f, hi = 0.62f;
					switch (C) {
					case 'M': pv(S, l, b, t); pv(S, r, b, t); pd(S, l, t, mid, b + 0.25f); pd(S, r, t, mid, b + 0.25f); break;
					case 'A': pd(S, l, b, 0.5f, t); pd(S, r, b, 0.5f, t); ph(S, hi, l + 0.15f, r - 0.15f); break;
					case 'T': ph(S, t, l, r); pv(S, mid, b, t); break;
					case 'H': pv(S, l, b, t); pv(S, r, b, t); ph(S, mid, l, r); break;
					default:  ph(S, t, l, r); ph(S, b, l, r); pv(S, l, b, t); pv(S, r, b, t); break;
					} return S;
					};

				const char* text = "MATH  MATH  MATH  ";
				int len = 0; while (text[len] != '\0') ++len;

				const float letterH = 0.16f, spacing = 0.26f, cell = letterH / spacing;
				const int cols = 22, rows = 2;
				const float bandY = -0.15f;        // start depth (near horizon)
				const float bandZ = 0.05f;        // start height just above horizon
				const float riseZ = 0.42f;        // how high letters rise
				const float swirl = 1.20f;        // spin around Z while rising
				const float yawY = 0.55f;        // yaw around Y to give parallax
				const float baseTh = 0.0068f; const int cubes = 22;

				auto rotZ = [&](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z }; };
				auto rotY = [&](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z }; };

				int idx = 0;
				float fullW = (cols - 1) * cell;
				for (int r = 0; r < rows; r++)
					for (int c = 0; c < cols; c++)
					{
						char ch = text[idx++ % len];
						auto S = strokes(ch);
						float cx = -0.5f * fullW + c * cell;
						float cy = bandY + 0.08f * r;
						float cz = bandZ + 0.02f * r;

						// color by grid
						float hue = std::fmod(0.10f * r + 0.07f * c, 1.0f);
						Vec3 c0{ 0.5f + 0.5f * std::cos(TAU * (hue + 0.00f)),
								 0.5f + 0.5f * std::cos(TAU * (hue + 0.33f)),
								 0.5f + 0.5f * std::cos(TAU * (hue + 0.66f)) };
						Vec3 c1{ 0.5f + 0.5f * std::cos(TAU * (hue + 0.10f)),
								 0.5f + 0.5f * std::cos(TAU * (hue + 0.43f)),
								 0.5f + 0.5f * std::cos(TAU * (hue + 0.76f)) };

						auto mapPt = [&](V2 p)->Vec3 {
							// glyph plane: XZ, stacked along Y (YZ swapped vibe)
							float X = cx + (p.x - 0.5f) * letterH;
							float Z = cz + (p.y - 0.5f) * letterH;
							float Y = cy;
							return Vec3{ X,Y,Z };
							};

						for (const auto& st : S)
						{
							Vec3 P0 = mapPt(st.first);
							Vec3 P1 = mapPt(st.second);

							// START: legible band near horizon
							Vec3 A0 = P0, B0 = P1;

							// END: swirl around Z, yaw around Y, rise in Z
							Vec3 q0 = rotZ(Vec3{ P0.x - cx, P0.y - cy, P0.z - cz }, swirl);
							q0 = rotY(q0, yawY);
							Vec3 q1 = rotZ(Vec3{ P1.x - cx, P1.y - cy, P1.z - cz }, swirl);
							q1 = rotY(q1, yawY);

							Vec3 A1{ cx + q0.x, cy + q0.y, cz + q0.z + riseZ };
							Vec3 B1{ cx + q1.x, cy + q1.y, cz + q1.z + riseZ };

							emit(A0, B0, c0, c1, baseTh * 0.30f, baseTh * 0.80f, cubes);
						}
					}
			}
		}



		void init_solar_flare()
		{
			lines.clear();

			// --- SECTION 1: Self-Contained Helpers (Vector Math Lambdas) ---
			auto vec_add = [](const Vec3& a, const Vec3& b) -> Vec3 { return { a.x + b.x, a.y + b.y, a.z + b.z }; };
			auto vec_scale = [](const Vec3& v, float s) -> Vec3 { return { v.x * s, v.y * s, v.z * s }; };
			auto vec_length = [](const Vec3& v) -> float { return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); };
			auto vec_normalize = [&](const Vec3& v) -> Vec3 {
				float len = vec_length(v);
				return (len > 1e-6f) ? vec_scale(v, 1.0f / len) : Vec3{ 0,0,0 };
				};

			// --- SECTION 2: Effect Configuration (The Fun Part!) ---
			const int   number_of_particles = 1500;   // More particles = bigger, denser flare
			const float max_distance = 1.8f;          // How far the particles travel
			const float spiral_tightness = 8.0f;      // Higher = more twists
			const float arc_strength = 0.5f;          // How much the spirals "fan out"
			const float tail_length = 0.08f;          // Length of each particle's trail

			// --- SECTION 3: Line Generation Logic ---
			for (int i = 0; i < number_of_particles; ++i)
			{
				Line& particle = add_line();

				// --- Step 1: Give each particle a unique, random trajectory ---

				// Pick a random direction on a sphere for the eruption "ray"
				float theta = Random::generate_random_float_0_to_1() * 2.0f * 3.14159265f; // Azimuth
				float phi = acos(1.0f - 2.0f * Random::generate_random_float_0_to_1()); // Polar (inclination)
				Vec3 main_direction = {
					sin(phi) * cos(theta),
					sin(phi) * sin(theta),
					cos(phi)
				};

				// Create two perpendicular vectors to define the plane of the spiral
				Vec3 tangent1 = vec_normalize({ -main_direction.y, main_direction.x, 0 });
				if (vec_length(tangent1) < 0.1f) { // Handle case where main_direction is along Z
					tangent1 = { 1, 0, 0 };
				}
				Vec3 tangent2 = vec_normalize({
					main_direction.y * tangent1.z - main_direction.z * tangent1.y,
					main_direction.z * tangent1.x - main_direction.x * tangent1.z,
					main_direction.x * tangent1.y - main_direction.y * tangent1.x
					});

				// Randomize this particle's specific properties
				float final_dist = max_distance * (0.5f + 0.5f * Random::generate_random_float_0_to_1());
				float spiral_angle = final_dist * spiral_tightness * (0.75f + 0.5f * Random::generate_random_float_0_to_1());

				// --- Step 2: Calculate the FINAL position (the end of the animation) ---

				// The base position is along the main eruption ray
				Vec3 base_pos = vec_scale(main_direction, final_dist);

				// The spiral offset is calculated on the plane defined by the tangents
				Vec3 spiral_offset_1 = vec_scale(tangent1, cos(spiral_angle) * final_dist * arc_strength);
				Vec3 spiral_offset_2 = vec_scale(tangent2, sin(spiral_angle) * final_dist * arc_strength);

				// The final head position is the base + spiral offsets
				Vec3 head_pos_end = vec_add(base_pos, vec_add(spiral_offset_1, spiral_offset_2));

				// The tail position is slightly behind the head, giving it direction
				Vec3 tail_direction = vec_normalize(head_pos_end); // Simplified tail
				Vec3 tail_pos_end = vec_add(head_pos_end, vec_scale(tail_direction, -tail_length));

				particle.x1.end = head_pos_end.x;
				particle.y1.end = head_pos_end.y;
				particle.z1.end = head_pos_end.z;
				particle.x0.end = tail_pos_end.x;
				particle.y0.end = tail_pos_end.y;
				particle.z0.end = tail_pos_end.z;

				// --- Step 3: Calculate the START position (the beginning of the animation) ---
				// Everything starts collapsed at the origin for a dramatic eruption.
				particle.x0.start = 0.0f; particle.y0.start = 0.0f; particle.z0.start = 0.0f;
				particle.x1.start = 0.0f; particle.y1.start = 0.0f; particle.z1.start = 0.0f;

				// --- Step 4: Styling for an energetic look ---
				// Starts hot, cools down
				particle.rgb_t0 = { 1.0f, 1.0f, 0.8f }; // Bright white-yellow core
				particle.rgb_t1 = { 1.0f, 0.4f + 0.3f * Random::generate_random_float_0_to_1(), 0.1f }; // Fiery orange/red

				// Starts thick, thins out
				particle.thickness.start = 0.012f;
				particle.thickness.end = 0.001f;
				particle.number_of_cubes = 50;
			}
		}


		// --- Glass of Water (Z is up; optional Y/Z swap toggle) ---
		void init_2020_glass_of_water()
		{
			lines.clear();

			const float TAU = 6.28318530718f;

			// flip this if you want Y<->Z swapped globally for the piece
			const bool yz_swapped = false;

			// small helper: emit a line (reveals from midpoint) with optional Y/Z swap
			auto emitXYZ = [&](float x0, float y0, float z0,
				float x1, float y1, float z1,
				Vec3 c0, Vec3 c1,
				float t0, float t1, int cubes)
				{
					Line& L = add_line();

					auto map = [&](float X, float Y, float Z)->Vec3 {
						return yz_swapped ? Vec3{ X, Z, Y } : Vec3{ X, Y, Z };
						};

					Vec3 A = map(x0, y0, z0);
					Vec3 B = map(x1, y1, z1);
					Vec3 M{ 0.5f * (A.x + B.x), 0.5f * (A.y + B.y), 0.5f * (A.z + B.z) };

					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;

					L.rgb_t0 = c0; L.thickness.start = t0; L.number_of_cubes = cubes;

					L.copy_start_to_end();

					L.x0.end = A.x; L.y0.end = A.y; L.z0.end = A.z;
					L.x1.end = B.x; L.y1.end = B.y; L.z1.end = B.z;

					L.rgb_t1 = c1; L.thickness.end = t1;
				};

			auto mixf = [](float a, float b, float t) { return a + (b - a) * t; };
			auto clamp01 = [](float v) { return std::max(0.0f, std::min(1.0f, v)); };

			// --------------------------------------------
			// SCENE PARAMETERS
			// --------------------------------------------
			const float tableZ = -0.02f;

			const float glassR_outer = 0.28f;
			const float glassR_inner = 0.25f;
			const float glassH = 0.75f;

			const float waterH = 0.46f;            // fill level
			const float meniscusUp = 0.016f;           // slight lift at the rim

			// colors (soft and believable)
			const Vec3  colGlassA{ 0.78f, 0.90f, 1.00f };
			const Vec3  colGlassB{ 0.92f, 0.97f, 1.00f };
			const Vec3  colWaterA{ 0.35f, 0.64f, 0.95f };
			const Vec3  colWaterB{ 0.55f, 0.86f, 1.00f };
			const Vec3  colRimHi{ 1.00f, 0.98f, 0.92f };
			const Vec3  colRimLo{ 0.92f, 0.98f, 1.00f };
			const Vec3  colCausticA{ 1.00f, 0.90f, 0.60f };
			const Vec3  colCausticB{ 0.98f, 0.98f, 0.85f };
			const Vec3  colTable{ 0.18f, 0.18f, 0.20f };

			// --------------------------------------------
			// 0) TABLE GRID (gives grounding + refraction context)
			// --------------------------------------------
			{
				const int   N = 12;
				const float span = 1.8f;
				const float step = span / float(N);
				const float th = 0.0025f;

				for (int i = -N; i <= N; ++i) {
					float y = i * step;
					emitXYZ(-span, y, tableZ, span, y, tableZ,
						colTable, colTable, th, th, 40);
					float x = i * step;
					emitXYZ(x, -span, tableZ, x, span, tableZ,
						colTable, colTable, th, th, 40);
				}
			}

			// --------------------------------------------
			// 1) GLASS OUTER CYLINDER (vertical ribs + base ring)
			// --------------------------------------------
			{
				const int ribs = 48;
				for (int i = 0; i < ribs; i++)
				{
					float a = (i / float(ribs)) * TAU;
					float x = glassR_outer * std::cos(a);
					float y = glassR_outer * std::sin(a);

					// fake Fresnel highlight (brighter near X)
					float fres = 0.25f + 0.75f * std::fabs(std::cos(a));
					Vec3 c0 = { mixf(colGlassA.x,colGlassB.x,fres),
								mixf(colGlassA.y,colGlassB.y,fres),
								mixf(colGlassA.z,colGlassB.z,fres) };

					emitXYZ(x, y, 0.0f, x, y, glassH,
						c0, c0, 0.0052f, 0.0072f, 36);
				}

				// base ring (ellipse)
				const int segs = 72;
				for (int s = 0; s < segs; s++) {
					float a0 = (s / float(segs)) * TAU;
					float a1 = ((s + 1) / float(segs)) * TAU;
					float x0 = glassR_outer * std::cos(a0);
					float y0 = glassR_outer * std::sin(a0);
					float x1 = glassR_outer * std::cos(a1);
					float y1 = glassR_outer * std::sin(a1);
					emitXYZ(x0, y0, 0.0f, x1, y1, 0.0f,
						colRimLo, colRimLo, 0.006f, 0.008f, 28);
				}
			}

			// --------------------------------------------
			// 2) GLASS INNER CYLINDER (suggest wall thickness)
			// --------------------------------------------
			{
				const int ribs = 36;
				for (int i = 0; i < ribs; i++)
				{
					float a = (i / float(ribs)) * TAU;
					float x = glassR_inner * std::cos(a);
					float y = glassR_inner * std::sin(a);

					// slightly dimmer inside
					Vec3 c = { 0.72f, 0.86f, 0.98f };
					emitXYZ(x, y, 0.0f, x, y, glassH,
						c, c, 0.0042f, 0.0062f, 30);
				}
			}

			// --------------------------------------------
			// 3) WATER SURFACE: rim ring + meniscus (short radial lifts)
			// --------------------------------------------
			{
				// rim ring at fill level inside the glass
				const int segs = 84;
				for (int s = 0; s < segs; s++) {
					float a0 = (s / float(segs)) * TAU;
					float a1 = ((s + 1) / float(segs)) * TAU;
					float r = glassR_inner * 0.996f;

					float x0 = r * std::cos(a0);
					float y0 = r * std::sin(a0);
					float x1 = r * std::cos(a1);
					float y1 = r * std::sin(a1);

					emitXYZ(x0, y0, waterH, x1, y1, waterH,
						colWaterB, colWaterA, 0.0065f, 0.0085f, 32);
				}

				// meniscus: short radial segments that rise near the wall
				const int spokes = 48;
				for (int k = 0; k < spokes; k++)
				{
					float a = (k / float(spokes)) * TAU;
					float r0 = glassR_inner * 0.92f;
					float r1 = glassR_inner * 1.00f;
					float x0 = r0 * std::cos(a);
					float y0 = r0 * std::sin(a);
					float x1 = r1 * std::cos(a);
					float y1 = r1 * std::sin(a);

					// ease the lifthigher near the wall, subtle in the middle
					float lift = meniscusUp * (0.6f + 0.4f * std::pow(std::fabs(std::cos(a)), 1.5f));

					emitXYZ(x0, y0, waterH, x1, y1, waterH + lift,
						colWaterA, colWaterB, 0.0055f, 0.0072f, 18);
				}
			}

			// --------------------------------------------
			// 4) WATER VOLUME: horizontal chords (gives body)
			// --------------------------------------------
			{
				const int layers = 14;
				const int chords = 38;
				for (int li = 0; li < layers; ++li)
				{
					float t = (li + 0.5f) / layers;
					float z = mixf(0.02f, waterH - 0.02f, t);
					float thk = mixf(0.0060f, 0.0045f, t); // thinner near surface
					float tint = mixf(0.75f, 1.00f, t);

					for (int ci = 0; ci < chords; ++ci)
					{
						// draw across diameter at various angles for crosshatch feel
						float a = (ci / float(chords)) * TAU;
						float r = glassR_inner * (0.98f - 0.04f * std::sin(3.0f * a));
						float x0 = -r * std::cos(a), y0 = -r * std::sin(a);
						float x1 = r * std::cos(a), y1 = r * std::sin(a);

						Vec3 c0{ colWaterA.x * tint, colWaterA.y * tint, colWaterA.z * tint };
						Vec3 c1{ colWaterB.x * tint, colWaterB.y * tint, colWaterB.z * tint };

						emitXYZ(x0, y0, z, x1, y1, z, c0, c1, thk * 0.65f, thk, 20);
					}
				}
			}

			// --------------------------------------------
			// 5) SPECULAR HIGHLIGHTS on the glass (two bright verticals)
			// --------------------------------------------
			{
				const int segs = 4 * 36;
				const float th = 0.010f;
				float angs[2] = { +0.65f, -0.65f };
				for (int h = 0; h < 2; ++h) {
					float a = angs[h];
					float x = glassR_outer * 0.995f * std::cos(a);
					float y = glassR_outer * 0.995f * std::sin(a);
					emitXYZ(x, y, 0.02f, x, y, glassH - 0.01f,
						colRimHi, colRimLo, th * 0.55f, th, segs);
				}
			}

			// --------------------------------------------
			// 6) BUBBLES (tiny rising segments inside the water)
			// --------------------------------------------
			{
				const int bubbles = 90;
				for (int b = 0; b < bubbles; b++)
				{
					float rr = glassR_inner * 0.78f * std::sqrt(Random::generate_random_float_0_to_1());
					float a = TAU * Random::generate_random_float_0_to_1();
					float x = rr * std::cos(a);
					float y = rr * std::sin(a);
					float z0 = 0.05f + (waterH - 0.10f) * Random::generate_random_float_0_to_1();
					float dz = 0.02f + 0.05f * Random::generate_random_float_0_to_1();

					emitXYZ(x, y, z0, x + 0.01f * std::cos(6 * a), y + 0.01f * std::sin(6 * a), z0 + dz,
						Vec3{ 0.92f,0.98f,1.00f }, Vec3{ 0.85f,0.95f,1.00f },
						0.0038f, 0.0048f, 8);
				}
			}

			// --------------------------------------------
			// 7) CAUSTIC STARBURST under the glass (on the table)
			// --------------------------------------------
			{
				const int rays = 28;
				for (int r = 0; r < rays; r++)
				{
					float a = (r / float(rays)) * TAU;
					float jitter = 0.08f * Random::generate_random_float_minus_one_to_plus_one();
					float r0 = 0.05f;
					float r1 = glassR_inner * (0.65f + 0.15f * std::sin(3.0f * a + jitter));

					float x0 = r0 * std::cos(a);
					float y0 = r0 * std::sin(a);
					float x1 = r1 * std::cos(a + 0.2f * jitter);
					float y1 = r1 * std::sin(a + 0.2f * jitter);

					float th = 0.0045f + 0.0025f * std::fabs(std::cos(2.0f * a));

					emitXYZ(x0, y0, tableZ, x1, y1, tableZ,
						colCausticA, colCausticB, th * 0.6f, th, 24);
				}

				// faint base circle for caustics
				const int segs = 60;
				for (int s = 0; s < segs; s++) {
					float a0 = (s / float(segs)) * TAU;
					float a1 = ((s + 1) / float(segs)) * TAU;
					float r = glassR_inner * 0.72f;
					float x0 = r * std::cos(a0), y0 = r * std::sin(a0);
					float x1 = r * std::cos(a1), y1 = r * std::sin(a1);
					emitXYZ(x0, y0, tableZ, x1, y1, tableZ,
						Vec3{ 0.55f,0.60f,0.40f }, Vec3{ 0.90f,0.92f,0.70f },
						0.0022f, 0.0030f, 18);
				}
			}

			// --------------------------------------------
			// 8) TOP RIM (bright ellipse)
			// --------------------------------------------
			{
				const int segs = 90;
				for (int s = 0; s < segs; s++) {
					float a0 = (s / float(segs)) * TAU;
					float a1 = ((s + 1) / float(segs)) * TAU;
					float x0 = glassR_outer * std::cos(a0);
					float y0 = glassR_outer * std::sin(a0);
					float x1 = glassR_outer * std::cos(a1);
					float y1 = glassR_outer * std::sin(a1);

					// emphasize facing directions
					float f0 = 0.45f + 0.55f * std::fabs(std::cos(a0));
					float f1 = 0.45f + 0.55f * std::fabs(std::cos(a1));
					Vec3 c0{ mixf(colRimLo.x,colRimHi.x,f0),
							 mixf(colRimLo.y,colRimHi.y,f0),
							 mixf(colRimLo.z,colRimHi.z,f0) };
					Vec3 c1{ mixf(colRimLo.x,colRimHi.x,f1),
							 mixf(colRimLo.y,colRimHi.y,f1),
							 mixf(colRimLo.z,colRimHi.z,f1) };

					emitXYZ(x0, y0, glassH, x1, y1, glassH,
						c0, c1, 0.0065f, 0.0090f, 36);
				}
			}
		}

		

		void init_calculus_morph()
		{
			lines.clear();

			// --- SECTION 1: Self-Contained Helpers (Vector Math Lambdas) ---
			auto vec_add = [](const Vec3& a, const Vec3& b) -> Vec3 { return { a.x + b.x, a.y + b.y, a.z + b.z }; };
			auto vec_scale = [](const Vec3& v, float s) -> Vec3 { return { v.x * s, v.y * s, v.z * s }; };
			auto lerp = [](float a, float b, float t) -> float { return a * (1.0f - t) + b * t; };

			// --- SECTION 2: Master Parametric Function ---
			// This is the heart of the effect. It calculates a point on the surface
			// for any given (u, v) coordinates and a morph factor 't'.
			// t = 0.0 -> Perfect Klein Bottle
			// t = 1.0 -> Perfect Torus
			auto calculate_point = [&](float u, float v, float t) -> Vec3 {
				const float PI = 3.1415926535f;

				// --- Klein Bottle Equations (Figure-8 immersion) ---
				float r_k = 2.0f - cos(v);
				float x_k, y_k, z_k;
				if (u < PI) {
					x_k = 6.0f * cos(u) * (1.0f + sin(u)) + r_k * cos(v) * cos(u);
					y_k = 16.0f * sin(u) + r_k * cos(v) * sin(u);
				}
				else {
					x_k = 6.0f * cos(u) * (1.0f + sin(u)) + r_k * cos(u + PI) * cos(v);
					y_k = 16.0f * sin(u);
				}
				z_k = r_k * sin(v);

				// --- Torus Equations ---
				float major_radius = 12.0f;
				float minor_radius = 4.0f;
				float x_t = (major_radius + minor_radius * cos(v)) * cos(u);
				float y_t = (major_radius + minor_radius * cos(v)) * sin(u);
				float z_t = minor_radius * sin(v);

				// --- Linearly Interpolate (Lerp) between the two shapes ---
				Vec3 result;
				result.x = lerp(x_k, x_t, t);
				result.y = lerp(y_k, y_t, t);
				result.z = lerp(z_k, z_t, t);

				return result;
				};

			// --- SECTION 3: Effect Configuration ---
			const int u_steps = 100; // Resolution along the main tube
			const int v_steps = 30;  // Resolution around the tube
			const float scale = 0.08f;
			Vec3 start_color = { 1.0f, 0.2f, 0.5f }; // Energetic Magenta for the Klein Bottle
			Vec3 end_color = { 0.2f, 0.8f, 1.0f }; // Calm Cyan for the Torus

			// --- SECTION 4: Wireframe Generation ---
			const float TAU = 6.2831853f;
			for (int i = 0; i < u_steps; ++i) {
				for (int j = 0; j < v_steps; ++j) {
					float u1 = (float)i / u_steps * TAU;
					float v1 = (float)j / v_steps * TAU;
					float u2 = (float)(i + 1) / u_steps * TAU;
					float v2 = (float)(j + 1) / v_steps * TAU;

					// Calculate the 4 corners of a quad on the surface for both start and end states
					Vec3 p11_start = vec_scale(calculate_point(u1, v1, 0.0f), scale);
					Vec3 p12_start = vec_scale(calculate_point(u1, v2, 0.0f), scale);
					Vec3 p21_start = vec_scale(calculate_point(u2, v1, 0.0f), scale);

					Vec3 p11_end = vec_scale(calculate_point(u1, v1, 1.0f), scale);
					Vec3 p12_end = vec_scale(calculate_point(u1, v2, 1.0f), scale);
					Vec3 p21_end = vec_scale(calculate_point(u2, v1, 1.0f), scale);

					// Create the two lines that form the wireframe grid for this quad

					// Line along the U direction (longitude)
					{
						Line& line_u = add_line();
						line_u.x0.start = p11_start.x; line_u.y0.start = p11_start.y; line_u.z0.start = p11_start.z;
						line_u.x1.start = p21_start.x; line_u.y1.start = p21_start.y; line_u.z1.start = p21_start.z;
						line_u.x0.end = p11_end.x; line_u.y0.end = p11_end.y; line_u.z0.end = p11_end.z;
						line_u.x1.end = p21_end.x; line_u.y1.end = p21_end.y; line_u.z1.end = p21_end.z;

						line_u.rgb_t0 = start_color; line_u.rgb_t1 = end_color;
						line_u.thickness.start = 0.003f; line_u.thickness.end = 0.004f;
						line_u.number_of_cubes = 20;
					}
					// Line along the V direction (latitude)
					{
						Line& line_v = add_line();
						line_v.x0.start = p11_start.x; line_v.y0.start = p11_start.y; line_v.z0.start = p11_start.z;
						line_v.x1.start = p12_start.x; line_v.y1.start = p12_start.y; line_v.z1.start = p12_start.z;
						line_v.x0.end = p11_end.x; line_v.y0.end = p11_end.y; line_v.z0.end = p11_end.z;
						line_v.x1.end = p12_end.x; line_v.y1.end = p12_end.y; line_v.z1.end = p12_end.z;

						line_v.rgb_t0 = start_color; line_v.rgb_t1 = end_color;
						line_v.thickness.start = 0.003f; line_v.thickness.end = 0.004f;
						line_v.number_of_cubes = 15;
					}
				}
			}
		}

		


		// --- Low-poly City: cubes + triangles wireframe world (Y<->Z aware) ---
		void init_2012_lowpoly_city_world()
		{
			lines.clear();

			// ===== knobs =====
			const bool  yz_swapped = true;   // << flip Y and Z to match your preferred camera
			const float TAU = 6.28318530718f;

			// City layout
			const int   GX = 18;               // grid width  (plots)
			const int   GZ = 14;               // grid depth  (plots)
			const float cellW = 0.28f;         // plot width  (X)
			const float cellD = 0.28f;         // plot depth  (Z)
			const float groundY = -0.85f;      // ground "plane" baseline (before Y/Z swap)
			const int   roadEvery = 4;         // every Nth row/col is a road
			const float roadInset = 0.08f;     // road eats this margin out of adjacent plots

			// Building scale
			const float minH = 0.20f;
			const float maxH = 1.00f;          // downtown will reach this
			const float baseFoot = 0.16f;      // baseline footprint half-size
			const float footJitter = 0.06f;    // per-building size jitter

			// Colors
			const Vec3 groundA{ 0.18f,0.19f,0.22f }, groundB{ 0.11f,0.12f,0.14f }; // asphalt / plaza
			const Vec3 roadC{ 0.08f,0.08f,0.09f };
			const Vec3 boxLow{ 0.55f,0.58f,0.63f }, boxHigh{ 0.82f,0.86f,0.92f }; // concrete/steel
			const Vec3 roofA{ 0.90f,0.46f,0.32f }, roofB{ 0.98f,0.70f,0.45f };   // terracotta-ish
			const Vec3 glassA{ 0.10f,0.40f,0.70f }, glassB{ 0.35f,0.65f,0.95f };  // glass accent
			const Vec3 treeG0{ 0.10f,0.55f,0.30f }, treeG1{ 0.20f,0.75f,0.45f };
			const Vec3 trunkC{ 0.35f,0.22f,0.14f };

			// Thickness
			const float baseTh = 0.0075f;
			const float roadTh = 0.0110f;
			const float parkTh = 0.0060f;
			const int   cubesLo = 16;
			const int   cubesMd = 26;
			const int   cubesHi = 40;

			// ---------- helpers ----------
			auto mapYZ = [&](float X, float Y, float Z)->Vec3 {
				return yz_swapped ? Vec3{ X, Z, Y } : Vec3{ X, Y, Z };
				};
			auto emit = [&](Vec3 A, Vec3 B, Vec3 c0, Vec3 c1, float t0, float t1, int cubes)
				{
					Line& L = add_line();
					Vec3 M{ 0.5f * (A.x + B.x), 0.5f * (A.y + B.y), 0.5f * (A.z + B.z) };
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = c0; L.thickness.start = t0; L.number_of_cubes = cubes;
					L.copy_start_to_end();
					L.x0.end = A.x; L.y0.end = A.y; L.z0.end = A.z;
					L.x1.end = B.x; L.y1.end = B.y; L.z1.end = B.z;
					L.rgb_t1 = c1; L.thickness.end = t1;
				};
			auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
			auto mix3 = [&](Vec3 a, Vec3 b, float t)->Vec3 {
				return Vec3{ lerp(a.x,b.x,t), lerp(a.y,b.y,t), lerp(a.z,b.z,t) };
				};
			auto edge = [&](float x0, float y0, float z0, float x1, float y1, float z1,
				Vec3 c0, Vec3 c1, float t0, float t1, int cubes) {
					emit(mapYZ(x0, y0, z0), mapYZ(x1, y1, z1), c0, c1, t0, t1, cubes);
				};

			// Axis-aligned cuboid wireframe (centered on cx,cz; from y to y+h)
			auto box_wire = [&](float cx, float cy, float cz, float sx, float sz, float h, Vec3 col0, Vec3 col1)
				{
					float x0 = cx - sx, x1 = cx + sx;
					float z0 = cz - sz, z1 = cz + sz;
					float y0 = cy, y1 = cy + h;

					// bottom rectangle
					edge(x0, y0, z0, x1, y0, z0, col0, col0, baseTh * 0.6f, baseTh, cubesMd);
					edge(x1, y0, z0, x1, y0, z1, col0, col0, baseTh * 0.6f, baseTh, cubesMd);
					edge(x1, y0, z1, x0, y0, z1, col0, col0, baseTh * 0.6f, baseTh, cubesMd);
					edge(x0, y0, z1, x0, y0, z0, col0, col0, baseTh * 0.6f, baseTh, cubesMd);

					// top rectangle
					edge(x0, y1, z0, x1, y1, z0, col1, col1, baseTh * 0.6f, baseTh, cubesMd);
					edge(x1, y1, z0, x1, y1, z1, col1, col1, baseTh * 0.6f, baseTh, cubesMd);
					edge(x1, y1, z1, x0, y1, z1, col1, col1, baseTh * 0.6f, baseTh, cubesMd);
					edge(x0, y1, z1, x0, y1, z0, col1, col1, baseTh * 0.6f, baseTh, cubesMd);

					// verticals
					edge(x0, y0, z0, x0, y1, z0, col0, col1, baseTh, baseTh, cubesHi);
					edge(x1, y0, z0, x1, y1, z0, col0, col1, baseTh, baseTh, cubesHi);
					edge(x1, y0, z1, x1, y1, z1, col0, col1, baseTh, baseTh, cubesHi);
					edge(x0, y0, z1, x0, y1, z1, col0, col1, baseTh, baseTh, cubesHi);
				};

			// Pyramid roof on a rectangle (apex above center)
			auto pyramid_roof = [&](float cx, float cy, float cz, float sx, float sz, float h, Vec3 ca, Vec3 cb)
				{
					float x0 = cx - sx, x1 = cx + sx;
					float z0 = cz - sz, z1 = cz + sz;
					float y = cy;
					// base rim
					edge(x0, y, z0, x1, y, z0, ca, cb, baseTh, baseTh, cubesLo);
					edge(x1, y, z0, x1, y, z1, ca, cb, baseTh, baseTh, cubesLo);
					edge(x1, y, z1, x0, y, z1, ca, cb, baseTh, baseTh, cubesLo);
					edge(x0, y, z1, x0, y, z0, ca, cb, baseTh, baseTh, cubesLo);
					// apex
					float ax = cx, ay = y + h, az = cz;
					edge(x0, y, z0, ax, ay, az, ca, cb, baseTh, baseTh, cubesLo);
					edge(x1, y, z0, ax, ay, az, ca, cb, baseTh, baseTh, cubesLo);
					edge(x1, y, z1, ax, ay, az, ca, cb, baseTh, baseTh, cubesLo);
					edge(x0, y, z1, ax, ay, az, ca, cb, baseTh, baseTh, cubesLo);
				};

			// Gable roof (ridge along X or Z)
			auto gable_roof = [&](float cx, float cy, float cz, float sx, float sz, float ridgeH, bool ridgeAlongX, Vec3 ca, Vec3 cb)
				{
					float x0 = cx - sx, x1 = cx + sx;
					float z0 = cz - sz, z1 = cz + sz;
					float y = cy;

					// base rim
					edge(x0, y, z0, x1, y, z0, ca, cb, baseTh, baseTh, cubesLo);
					edge(x1, y, z0, x1, y, z1, ca, cb, baseTh, baseTh, cubesLo);
					edge(x1, y, z1, x0, y, z1, ca, cb, baseTh, baseTh, cubesLo);
					edge(x0, y, z1, x0, y, z0, ca, cb, baseTh, baseTh, cubesLo);

					if (ridgeAlongX)
					{
						// ridge line along X
						edge(x0, y + ridgeH, cz, x1, y + ridgeH, cz, ca, cb, baseTh, baseTh, cubesLo);
						// hips
						edge(x0, y, z0, x0, y + ridgeH, cz, ca, cb, baseTh, baseTh, cubesLo);
						edge(x1, y, z0, x1, y + ridgeH, cz, ca, cb, baseTh, baseTh, cubesLo);
						edge(x0, y, z1, x0, y + ridgeH, cz, ca, cb, baseTh, baseTh, cubesLo);
						edge(x1, y, z1, x1, y + ridgeH, cz, ca, cb, baseTh, baseTh, cubesLo);
					}
					else
					{
						// ridge line along Z
						edge(cx, y + ridgeH, z0, cx, y + ridgeH, z1, ca, cb, baseTh, baseTh, cubesLo);
						// hips
						edge(x0, y, z0, cx, y + ridgeH, z0, ca, cb, baseTh, baseTh, cubesLo);
						edge(x1, y, z0, cx, y + ridgeH, z0, ca, cb, baseTh, baseTh, cubesLo);
						edge(x0, y, z1, cx, y + ridgeH, z1, ca, cb, baseTh, baseTh, cubesLo);
						edge(x1, y, z1, cx, y + ridgeH, z1, ca, cb, baseTh, baseTh, cubesLo);
					}
				};

			// Tent kiosk (triangular prism)
			auto tent_kiosk = [&](float cx, float cy, float cz, float sx, float sz, float h, Vec3 ca, Vec3 cb)
				{
					// base rectangle perimeter
					box_wire(cx, cy, cz, sx, sz, 0.001f, ca, ca);
					// top is ridge along Z, sides are triangles
					gable_roof(cx, cy, cz, sx, sz, h, false, ca, cb);
				};

			// Tiny low-poly tree: trunk + pyramid foliage
			auto tree = [&](float cx, float cy, float cz)
				{
					float tW = 0.015f, tD = 0.015f, tH = 0.06f;
					float fW = 0.06f, fD = 0.06f, fH = 0.10f;
					// trunk as a very thin box
					box_wire(cx, cy, cz, tW, tD, tH, trunkC, trunkC);
					// foliage pyramid on top
					pyramid_roof(cx, cy + tH, cz, fW, fD, fH, treeG0, treeG1);
				};

			// Ground grid (plaza) — subtle cross hatch so the city sits on something
			{
				const int   G = 36;
				const float W = (GX - 1) * cellW * 0.5f + 0.45f;
				const float D = (GZ - 1) * cellD * 0.5f + 0.45f;
				for (int i = 0; i <= G; i++)
				{
					float u = i / float(G);
					float x = -W + 2.0f * W * u;
					Vec3 c0 = mix3(groundA, groundB, u);
					// X-lines
					edge(-W, groundY, -D, W, groundY, -D + 2.0f * D * u, c0, c0, baseTh * 0.25f, baseTh * 0.35f, cubesLo);
					// Z-lines
					edge(-W + 2.0f * W * u, groundY, -D, -W + 2.0f * W * u, groundY, D, c0, c0, baseTh * 0.25f, baseTh * 0.35f, cubesLo);
				}
			}

			// Roads (thicker dark strokes along every Nth row/col)
			auto draw_road_strips = [&]()
				{
					float minX = -(GX - 1) * cellW * 0.5f, maxX = (GX - 1) * cellW * 0.5f;
					float minZ = -(GZ - 1) * cellD * 0.5f, maxZ = (GZ - 1) * cellD * 0.5f;

					for (int gz = 0; gz < GZ; ++gz)
					{
						if (gz % roadEvery == 0)
						{
							float z = minZ + gz * cellD;
							edge(minX - 0.25f, groundY, z, maxX + 0.25f, groundY, z, roadC, roadC, roadTh * 0.75f, roadTh, cubesHi);
						}
					}
					for (int gx = 0; gx < GX; ++gx)
					{
						if (gx % roadEvery == 0)
						{
							float x = minX + gx * cellW;
							edge(x, groundY, minZ - 0.25f, x, groundY, maxZ + 0.25f, roadC, roadC, roadTh * 0.75f, roadTh, cubesHi);
						}
					}
				};
			draw_road_strips();

			// Downtown height bias (taller near center, lower toward edges)
			auto height_bias = [&](float nx, float nz)->float {
				// nx, nz in [-1,1] roughly
				float r = std::sqrt(nx * nx + nz * nz); // 0 center .. ~1 edge
				float t = std::max(0.0f, 1.0f - r);
				t = std::pow(t, 0.6f);              // sharper center
				return lerp(minH, maxH, t);
				};

			// Iterate plots and spawn buildings/parks/trees/tents
			float startX = -(GX - 1) * cellW * 0.5f;
			float startZ = -(GZ - 1) * cellD * 0.5f;

			for (int gz = 0; gz < GZ; ++gz)
			{
				for (int gx = 0; gx < GX; ++gx)
				{
					// skip road cells
					if (gz % roadEvery == 0 || gx % roadEvery == 0) continue;

					float cx = startX + gx * cellW;
					float cz = startZ + gz * cellD;

					// plot local normalized for bias
					float nx = gx / float(GX - 1) * 2.0f - 1.0f;
					float nz = gz / float(GZ - 1) * 2.0f - 1.0f;

					// base footprint
					float sx = baseFoot + (Random::generate_random_float_0_to_1() - 0.5f) * footJitter;
					float sz = baseFoot + (Random::generate_random_float_0_to_1() - 0.5f) * footJitter;
					// keep a margin for roads
					sx = std::max(0.06f, sx - roadInset * 0.5f);
					sz = std::max(0.06f, sz - roadInset * 0.5f);

					// height
					float H = height_bias(nx, nz);
					// small randomness
					H *= (0.85f + 0.30f * Random::generate_random_float_0_to_1());

					// choose a lot type
					float r = Random::generate_random_float_0_to_1();
					if (r < 0.08f)
					{
						// pocket park with trees
						Vec3 pc0 = mix3(groundA, groundB, 0.5f);
						// outline the plot softly
						box_wire(cx, groundY + 0.001f, cz, sx, sz, 0.0008f, pc0, pc0);
						// a few trees inside
						int tcount = 3 + (Random::random_int(0, 99999) % 4);
						for (int ti = 0; ti < tcount; ++ti)
						{
							float ox = (Random::generate_random_float_0_to_1() - 0.5f) * (sx * 1.4f);
							float oz = (Random::generate_random_float_0_to_1() - 0.5f) * (sz * 1.4f);
							tree(cx + ox, groundY, cz + oz);
						}
						continue;
					}
					else if (r < 0.14f)
					{
						// kiosk tent (triangle prism)
						float h = lerp(0.10f, 0.18f, Random::generate_random_float_0_to_1());
						tent_kiosk(cx, groundY, cz, sx * 0.7f, sz * 0.7f, h, roofA, roofB);
						continue;
					}

					// building body (cuboid)
					Vec3 wall0 = mix3(boxLow, boxHigh, std::min(1.0f, (H - minH) / (maxH - minH + 1e-6f)));
					Vec3 wall1 = mix3(wall0, glassA, 0.15f);
					box_wire(cx, groundY, cz, sx, sz, H, wall0, wall1);

					// choose roof: pyramid, gable-X, gable-Z, or flat with glass crown
					float rr = Random::generate_random_float_0_to_1();
					if (rr < 0.33f)
					{
						float rh = lerp(0.08f, 0.22f, Random::generate_random_float_0_to_1());
						pyramid_roof(cx, groundY + H, cz, sx, sz, rh, roofA, roofB);
					}
					else if (rr < 0.66f)
					{
						float rh = lerp(0.06f, 0.18f, Random::generate_random_float_0_to_1());
						bool alongX = (Random::random_int(0, 9999) % 2) == 0;
						gable_roof(cx, groundY + H, cz, sx, sz, rh, alongX, roofA, roofB);
					}
					else
					{
						// flat: add a small glass crown
						float cw = sx * lerp(0.4f, 0.7f, Random::generate_random_float_0_to_1());
						float cd = sz * lerp(0.4f, 0.7f, Random::generate_random_float_0_to_1());
						float ch = lerp(0.06f, 0.14f, Random::generate_random_float_0_to_1());
						box_wire(cx, groundY + H, cz, cw, cd, ch, glassA, glassB);
					}

					// occasional skybridge between neighbors (triangular vibe)
					if (gx + 1 < GX && (Random::generate_random_float_0_to_1() < 0.08f) && (gx + 1) % roadEvery != 0)
					{
						float hB = groundY + H * lerp(0.55f, 0.85f, Random::generate_random_float_0_to_1());
						float xA = cx + sx, xB = cx + cellW + sx * 0.3f;
						float zM = cz;
						// a thin “bridge” box
						box_wire((xA + xB) * 0.5f, hB - 0.02f, zM, 0.03f, sz * 0.3f, 0.04f, glassB, glassA);
					}
					if (gz + 1 < GZ && (Random::generate_random_float_0_to_1() < 0.06f) && (gz + 1) % roadEvery != 0)
					{
						float hB = groundY + H * lerp(0.55f, 0.85f, Random::generate_random_float_0_to_1());
						float zA = cz + sz, zB = cz + cellD + sz * 0.3f;
						float xM = cx;
						box_wire(xM, hB - 0.02f, (zA + zB) * 0.5f, sx * 0.3f, 0.03f, 0.04f, glassB, glassA);
					}
				}
			}

			// A canal slicing the city (low-poly water rectangle with border)
			{
				if (GZ > 8)
				{
					int rz = GZ / 2 + 1;
					float z0 = -(GZ - 1) * cellD * 0.5f + (rz - 0.5f) * cellD - cellD * 0.6f;
					float z1 = z0 + cellD * 1.2f;
					float x0 = -(GX - 1) * cellW * 0.5f - 0.25f;
					float x1 = (GX - 1) * cellW * 0.5f + 0.25f;

					Vec3 wb0{ 0.10f,0.40f,0.65f }, wb1{ 0.15f,0.60f,0.85f };
					// borders
					edge(x0, groundY, z0, x1, groundY, z0, wb0, wb1, baseTh, baseTh, cubesMd);
					edge(x0, groundY, z1, x1, groundY, z1, wb0, wb1, baseTh, baseTh, cubesMd);
					edge(x0, groundY, z0, x0, groundY, z1, wb0, wb1, baseTh, baseTh, cubesMd);
					edge(x1, groundY, z0, x1, groundY, z1, wb0, wb1, baseTh, baseTh, cubesMd);
					// few cross “wave” dashes inside
					for (int i = 0; i < 28; i++)
					{
						float u = i / 27.0f;
						float x = lerp(x0 + 0.06f, x1 - 0.06f, u);
						float z = lerp(z0 + 0.06f, z1 - 0.06f, 0.5f + 0.45f * std::sin(3.0f * u * TAU));
						edge(x - 0.035f, groundY + 0.0015f, z, x + 0.035f, groundY + 0.0015f, z, wb1, wb0, baseTh * 0.55f, baseTh * 0.75f, cubesLo);
					}
				}
			}
		}




		// --- Simple & dynamic: concentric polygons that rotate, breathe and drift ---
		void init_2021_polygon_dance()
		{
			lines.clear();

			// ===== knobs (friendly to tweak) =====
			const bool  yz_swapped = true;     // flip Y/Z if you prefer Z-up camera
			const float TAU = 6.28318530718f;

			// rings
			struct Ring { int count, sides; float R, polyR, dAlpha, dOrient, dR, y; Vec3 c0, c1; };
			Ring rings[] = {
				//        tiles  sides    ringR  polyR  orbit   orient  breathe  Y      start color            end color
				/* inner */ { 24,   3,    0.36f, 0.055f,  0.55f,   1.25f,  0.06f,  0.00f, {0.95f,0.55f,0.30f},  {1.00f,0.75f,0.45f} },
				/* mid  */  { 18,   4,    0.62f, 0.070f, -0.40f,  -0.90f,  0.08f,  0.00f, {0.35f,0.70f,0.95f},  {0.55f,0.85f,1.00f} },
				/* outer */ { 12,   6,    0.88f, 0.080f,  0.25f,   0.65f,  0.05f,  0.00f, {0.55f,0.95f,0.60f},  {0.70f,1.00f,0.75f} }
			};

			const float thick0 = 0.0065f;
			const float thick1 = 0.0080f;
			const int   cubes = 40;

			// helpers
			auto mp = [&](float X, float Y, float Z)->Vec3 { return yz_swapped ? Vec3{ X,Z,Y } : Vec3{ X,Y,Z }; };
			auto emit = [&](const Vec3& A, const Vec3& B, const Vec3& c0, const Vec3& c1, float t0, float t1, int nCubes)
				{
					Line& L = add_line();
					Vec3 M{ 0.5f * (A.x + B.x), 0.5f * (A.y + B.y), 0.5f * (A.z + B.z) };
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = c0; L.thickness.start = t0; L.number_of_cubes = nCubes;
					L.copy_start_to_end();
					L.x0.end = A.x; L.y0.end = A.y; L.z0.end = A.z;
					L.x1.end = B.x; L.y1.end = B.y; L.z1.end = B.z;
					L.rgb_t1 = c1; L.thickness.end = t1;
				};
			auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
			auto mix3 = [&](Vec3 a, Vec3 b, float t) { return Vec3{ lerp(a.x,b.x,t), lerp(a.y,b.y,t), lerp(a.z,b.z,t) }; };

			// draw one polygon (edges only), with animated center/orientation/radius
			auto polygon = [&](Vec3 C0, Vec3 C1, int sides, float r0, float r1, float ang0, float ang1, Vec3 col0, Vec3 col1)
				{
					for (int k = 0; k < sides; ++k)
					{
						float u0 = (k / float(sides)) * TAU;
						float u1 = ((k + 1) / float(sides)) * TAU;

						// start vertices
						Vec3 A0 = mp(C0.x + r0 * std::cos(u0 + ang0), C0.y, C0.z + r0 * std::sin(u0 + ang0));
						Vec3 B0 = mp(C0.x + r0 * std::cos(u1 + ang0), C0.y, C0.z + r0 * std::sin(u1 + ang0));

						// end vertices (rotated + maybe different center/radius)
						Vec3 A1 = mp(C1.x + r1 * std::cos(u0 + ang1), C1.y, C1.z + r1 * std::sin(u0 + ang1));
						Vec3 B1 = mp(C1.x + r1 * std::cos(u1 + ang1), C1.y, C1.z + r1 * std::sin(u1 + ang1));

						emit(A0, B0, col0, col1, thick0, thick1, cubes);
						// we can also ghost a secondary radial chord for liveliness (optional, comment out if too busy)
						// emit(mp(C0.x,C0.y,C0.z), A0, mix3(col0,{1,1,1},0.12f), mix3(col1,{1,1,1},0.12f), thick0*0.5f, thick1*0.5f, 26);
					}
				};

			// build all rings
			for (const auto& R : rings)
			{
				for (int i = 0; i < R.count; ++i)
				{
					float a = (i / float(R.count)) * TAU;
					float j = 0.35f * std::sin(3.0f * a);               // tiny phase jitter per tile
					float a0 = a + j;
					float a1 = a + j + R.dAlpha;                       // orbit drift
					float o0 = a * 0.5f;                                 // start orientation
					float o1 = o0 + R.dOrient;                         // end orientation

					// centers (start on ring; end drifts along orbit)
					Vec3 C0 = { R.R * std::cos(a0), R.y, R.R * std::sin(a0) };
					Vec3 C1 = { (R.R + 0.03f * std::sin(2.0f * a)) * std::cos(a1),
								R.y,
								(R.R + 0.03f * std::sin(2.0f * a)) * std::sin(a1) };

					// polygon radii (breathe)
					float pr0 = R.polyR * (1.0f - 0.15f * std::sin(4.0f * a));
					float pr1 = pr0 * (1.0f + R.dR);

					// color drift across the ring
					float hueT = (i / float(R.count));
					Vec3 col0 = mix3(R.c0, R.c1, 0.25f + 0.75f * hueT);
					Vec3 col1 = mix3(R.c0, R.c1, 0.85f * hueT);

					polygon(C0, C1, R.sides, pr0, pr1, o0, o1, col0, col1);
				}
			}

			// subtle connecting chords between mid & outer rings to add flow
			{
				const int chords = 48;
				for (int i = 0; i < chords; ++i)
				{
					float u = i / float(chords);
					float a = u * TAU;
					float rMid = rings[1].R, rOut = rings[2].R;
					Vec3 M0 = mp(rMid * std::cos(a), 0.0f, rMid * std::sin(a));
					Vec3 O0 = mp(rOut * std::cos(a + 0.06f), 0.0f, rOut * std::sin(a + 0.06f));

					Vec3 M1 = mp((rMid + 0.02f) * std::cos(a + 0.30f), 0.0f, (rMid + 0.02f) * std::sin(a + 0.30f));
					Vec3 O1 = mp((rOut - 0.02f) * std::cos(a + 0.36f), 0.0f, (rOut - 0.02f) * std::sin(a + 0.36f));

					Vec3 cA = { 0.25f,0.85f,0.65f }, cB = { 0.15f,0.55f,0.95f };
					emit(M0, O0, cA, cB, thick0 * 0.6f, thick1 * 0.7f, 28);
					emit(M1, O1, cB, cA, thick0 * 0.5f, thick1 * 0.6f, 22);
				}
			}
		}


		void init_quantum_fluid_pool()
		{
			const float M_PI = 3.14159265359f;
			static float time_base = 0.0f;

			// Dynamic parameters that evolve over time
			float pool_radius = 0.4f * 2.0;
			float base_height = -0.6f * 2.0;
			int fluid_rings = 12;
			int particles_per_ring = 36 * 2.0;

			// Evolving wave parameters
			float primary_freq = 2.0f + sin(time_base * 0.3f) * 1.0f;
			float secondary_freq = 4.0f + cos(time_base * 0.4f) * 2.0f;
			float chaos_factor = 0.1f + 0.05f * sin(time_base * 0.5f);

			// Create the dynamic fluid surface with multiple interacting wave systems
			for (int ring = 0; ring < fluid_rings; ring++) {
				float ring_radius = (ring / float(fluid_rings - 1)) * pool_radius;
				float ring_energy = 1.0f - (ring / float(fluid_rings)) * 0.7f; // More energy at center

				for (int segment = 0; segment < particles_per_ring; segment++) {
					float angle1 = segment * (2.0f * M_PI / particles_per_ring);
					float angle2 = (segment + 1) * (2.0f * M_PI / particles_per_ring);

					// Calculate multiple overlapping wave systems
					float spiral_wave = sin(time_base * primary_freq + angle1 * 3.0f + ring * 2.0f) * 0.03f;
					float radial_wave = cos(time_base * secondary_freq + ring * 4.0f) * 0.02f;
					float chaotic_wave = sin(time_base * 7.0f + angle1 * 11.0f + ring * 13.0f) * chaos_factor;

					// Combine waves with non-linear interactions
					float wave_height1 = (spiral_wave + radial_wave) * ring_energy + chaotic_wave * 0.3f;
					float wave_height2 = (spiral_wave + radial_wave) * ring_energy + chaotic_wave * 0.3f;

					// Add vortex effects near center
					if (ring < 3) {
						float vortex = sin(time_base * 5.0f + angle1 * 8.0f) * 0.05f * (3 - ring);
						wave_height1 += vortex;
						wave_height2 += vortex * 1.1f;
					}

					// Calculate 3D positions with dynamic waves
					float x1 = ring_radius * cos(angle1) + 1.0;
					float y1 = base_height + wave_height1;
					float z1 = ring_radius * sin(angle1);

					float x2 = ring_radius * cos(angle2) + 1.0;
					float y2 = base_height + wave_height2;
					float z2 = ring_radius * sin(angle2);

					// Create surface connection line
					Line& surface_line = add_line();
					surface_line.x0.start = x1;
					surface_line.y0.start = y1;
					surface_line.z0.start = z1;
					surface_line.x1.start = x2;
					surface_line.y1.start = y2;
					surface_line.z1.start = z2;

					// Dynamic fluid colors based on wave energy and position
					float hue = angle1 * 0.5f + time_base * 2.0f + ring * 0.3f;
					float energy = abs(wave_height1) * 10.0f;

					surface_line.rgb_t0.x = 0.1f + 0.4f * sin(hue) + energy * 0.3f;
					surface_line.rgb_t0.y = 0.2f + 0.5f * sin(hue + 1.0f) + energy * 0.2f;
					surface_line.rgb_t0.z = 0.6f + 0.4f * sin(hue + 2.0f) + energy * 0.4f;

					surface_line.thickness.start = 0.008f + 0.01f * energy;
					surface_line.number_of_cubes = 12 + int(energy * 8);

					surface_line.copy_start_to_end();

					// Animate to completely different wave state
					float next_time = time_base + 0.2f;
					float next_spiral = sin(next_time * primary_freq * 1.3f + angle1 * 3.5f + ring * 2.2f) * 0.04f;
					float next_radial = cos(next_time * secondary_freq * 0.8f + ring * 4.5f) * 0.025f;
					float next_chaos = sin(next_time * 9.0f + angle1 * 13.0f + ring * 17.0f) * chaos_factor * 1.2f;

					float next_height1 = (next_spiral + next_radial) * ring_energy + next_chaos * 0.3f;
					float next_height2 = (next_spiral + next_radial) * ring_energy + next_chaos * 0.3f;

					if (ring < 3) {
						float next_vortex = sin(next_time * 6.0f + angle1 * 9.0f) * 0.06f * (3 - ring);
						next_height1 += next_vortex;
						next_height2 += next_vortex * 1.1f;
					}

					surface_line.x1.end = x2 + 0.02f * sin(next_time + angle2);
					surface_line.y1.end = base_height + next_height2;
					surface_line.z1.end = z2 + 0.02f * cos(next_time + angle2);

					// Create depth lines from surface to pool floor (only for some segments)
					if (segment % 4 == 0 && ring > 0) {
						Line& depth_line = add_line();
						depth_line.x0.start = x1;
						depth_line.y0.start = y1;
						depth_line.z0.start = z1;
						depth_line.x1.start = x1 * 0.3f; // Converge toward center at bottom
						depth_line.y1.start = base_height - 0.2f - 0.1f * sin(ring + angle1);
						depth_line.z1.start = z1 * 0.3f;

						// Depth-based color fading
						float depth_factor = 1.0f - (ring / float(fluid_rings)) * 0.7f;
						depth_line.rgb_t0.x = 0.05f + 0.2f * depth_factor;
						depth_line.rgb_t0.y = 0.1f + 0.3f * depth_factor;
						depth_line.rgb_t0.z = 0.3f + 0.4f * depth_factor;

						depth_line.thickness.start = 0.004f;
						depth_line.number_of_cubes = 8;
						depth_line.copy_start_to_end();

						// Animate depth lines
						depth_line.x1.end = x1 * 0.3f + 0.01f * sin(next_time * 2.0f + ring);
						depth_line.y1.end = base_height - 0.2f - 0.1f * sin(next_time + ring + angle1);
						depth_line.z1.end = z1 * 0.3f + 0.01f * cos(next_time * 2.0f + ring);
					}
				}
			}

			// Create emergent fluid particles that appear and disappear
			auto create_emergent_particles = [&]() {
				int particle_count = 25;

				for (int particle_idx = 0; particle_idx < particle_count; particle_idx++) {
					float life_phase = fmod(time_base * 0.5f + particle_idx * 0.7f, 1.0f);

					// Only create particles that are "alive" in their lifecycle
					if (life_phase < 0.7f) {
						float particle_radius = 0.02f + 0.03f * sin(particle_idx * 5.0f);
						float particle_angle = particle_idx * (2.0f * M_PI / particle_count) + time_base * 2.0f;
						float particle_distance = 0.1f + 0.3f * fmod(particle_idx * 0.3f + time_base * 0.2f, 1.0f);

						// Pulsating size based on lifecycle
						float size_pulse = sin(life_phase * M_PI) * particle_radius;

						// Create particle as a small dynamic shape
						int particle_segments = 6;
						for (int j = 0; j < particle_segments; j++) {
							float seg_angle1 = j * (2.0f * M_PI / particle_segments);
							float seg_angle2 = (j + 1) * (2.0f * M_PI / particle_segments);

							// Calculate wave-affected height for particles
							float particle_wave = sin(time_base * 3.0f + particle_angle * 4.0f) * 0.02f;

							Line& particle = add_line();
							particle.x0.start = particle_distance * cos(particle_angle) + size_pulse * cos(seg_angle1);
							particle.y0.start = base_height + particle_wave;
							particle.z0.start = particle_distance * sin(particle_angle) + size_pulse * sin(seg_angle1);
							particle.x1.start = particle_distance * cos(particle_angle) + size_pulse * cos(seg_angle2);
							particle.y1.start = base_height + particle_wave;
							particle.z1.start = particle_distance * sin(particle_angle) + size_pulse * sin(seg_angle2);

							// Particle color evolves with lifecycle
							float particle_hue = life_phase * 3.0f + particle_idx * 0.2f;
							particle.rgb_t0.x = 0.3f + 0.5f * sin(particle_hue);
							particle.rgb_t0.y = 0.4f + 0.4f * sin(particle_hue + 1.0f);
							particle.rgb_t0.z = 0.8f + 0.2f * sin(particle_hue + 2.0f);

							particle.thickness.start = 0.005f + 0.008f * life_phase;
							particle.number_of_cubes = 4;
							particle.copy_start_to_end();

							// Particles move in evolving patterns
							float next_phase = fmod(time_base * 0.5f + 0.1f + particle_idx * 0.7f, 1.0f);
							if (next_phase < 0.7f) {
								float next_pulse = sin(next_phase * M_PI) * particle_radius;
								float next_wave = sin((time_base + 0.1f) * 3.0f + particle_angle * 4.0f) * 0.025f;

								particle.x1.end = particle_distance * cos(particle_angle + 0.1f) + next_pulse * cos(seg_angle2);
								particle.y1.end = base_height + next_wave;
								particle.z1.end = particle_distance * sin(particle_angle + 0.1f) + next_pulse * sin(seg_angle2);
							}
							else {
								// Make particles disappear gracefully
								particle.x1.end = particle.x1.start;
								particle.y1.end = particle.y1.start;
								particle.z1.end = particle.z1.start;
							}
						}
					}
				}
				};

			// Create energy tendrils that reach out from the fluid
			auto create_energy_tendrils = [&]() {
				int tendril_count = 8;

				for (int tendril_idx = 0; tendril_idx < tendril_count; tendril_idx++) {
					float tendril_angle = tendril_idx * (2.0f * M_PI / tendril_count);
					float tendril_length = 0.3f + 0.2f * sin(time_base * 1.5f + tendril_idx);
					float tendril_phase = time_base * 2.0f + tendril_idx;

					int tendril_segments = 15;
					for (int seg = 0; seg < tendril_segments; seg++) {
						float seg_ratio = seg / float(tendril_segments - 1);

						// Tendril undulation
						float undulation = sin(tendril_phase + seg * 2.0f) * 0.05f * (1.0f - seg_ratio);
						float segment_x = (pool_radius + tendril_length * seg_ratio) * cos(tendril_angle) + undulation * cos(tendril_angle + M_PI / 2.0f);
						float segment_z = (pool_radius + tendril_length * seg_ratio) * sin(tendril_angle) + undulation * sin(tendril_angle + M_PI / 2.0f);
						float segment_y = base_height + 0.1f * sin(tendril_phase * 1.3f + seg * 3.0f) * seg_ratio;

						if (seg > 0) {
							float prev_undulation = sin(tendril_phase + (seg - 1) * 2.0f) * 0.05f * (1.0f - (seg - 1) / float(tendril_segments - 1));
							float prev_x = (pool_radius + tendril_length * (seg - 1) / float(tendril_segments - 1)) * cos(tendril_angle) + prev_undulation * cos(tendril_angle + M_PI / 2.0f);
							float prev_z = (pool_radius + tendril_length * (seg - 1) / float(tendril_segments - 1)) * sin(tendril_angle) + prev_undulation * sin(tendril_angle + M_PI / 2.0f);
							float prev_y = base_height + 0.1f * sin(tendril_phase * 1.3f + (seg - 1) * 3.0f) * (seg - 1) / float(tendril_segments - 1);

							Line& tendril = add_line();
							tendril.x0.start = prev_x;
							tendril.y0.start = prev_y;
							tendril.z0.start = prev_z;
							tendril.x1.start = segment_x;
							tendril.y1.start = segment_y;
							tendril.z1.start = segment_z;

							// Energy colors that fade along tendril
							float color_fade = 1.0f - seg_ratio;
							tendril.rgb_t0.x = 0.8f * color_fade;
							tendril.rgb_t0.y = 0.9f * color_fade;
							tendril.rgb_t0.z = 1.0f * color_fade;

							tendril.thickness.start = 0.006f * color_fade;
							tendril.number_of_cubes = 8;
							tendril.copy_start_to_end();

							// Animate tendril motion
							float next_phase = time_base * 2.0f + 0.1f + tendril_idx;
							float next_undulation = sin(next_phase + seg * 2.0f) * 0.06f * (1.0f - seg_ratio);
							float next_x = (pool_radius + tendril_length * seg_ratio) * cos(tendril_angle + 0.05f) + next_undulation * cos(tendril_angle + M_PI / 2.0f + 0.1f);
							float next_z = (pool_radius + tendril_length * seg_ratio) * sin(tendril_angle + 0.05f) + next_undulation * sin(tendril_angle + M_PI / 2.0f + 0.1f);
							float next_y = base_height + 0.12f * sin(next_phase * 1.3f + seg * 3.0f) * seg_ratio;

							tendril.x1.end = next_x;
							tendril.y1.end = next_y;
							tendril.z1.end = next_z;
						}
					}
				}
				};

			// ===== MAIN EXECUTION =====

			create_emergent_particles();
			create_energy_tendrils();

			// Increment time for animation
			time_base += 0.016f;
		}


		// --- Airplane: wireframe, animated flight, spinning fans, contrails ---
		void init_2030_airplane_wirefly()
		{
			lines.clear();

			// ===== knobs (tweak freely) =====
			const bool  yz_swapped = true;          // set false if you prefer classic Y-up
			const float scale = 1.00f;              // overall scene scale
			const float TAU = 6.28318530718f;

			// flight transforms
			struct Pose { Vec3 t; float yaw, pitch, roll, s; }; // yaw Z-up  around Y if yz_swapped=false
			Pose P0{ {-1.15f,  0.05f,  0.00f},  0.00f,  0.00f,  0.00f,  scale };   // start (left)
			Pose P1{ { 0.95f,  0.22f,  0.06f},  0.25f,  0.08f,  0.35f,  scale };   // end   (right, climbing & banking)

			// styling
			const Vec3 fuselageColA{ 0.80f,0.86f,0.95f }, fuselageColB{ 0.65f,0.72f,0.90f };
			const Vec3 wingColA{ 0.85f,0.92f,1.00f }, wingColB{ 0.70f,0.80f,0.95f };
			const Vec3 accentCol{ 0.95f,0.80f,0.35f };   // small accents (tips etc.)
			const Vec3 fanColA{ 0.25f,0.95f,0.65f }, fanColB{ 0.15f,0.65f,0.95f };
			const Vec3 trailCol{ 0.95f,0.98f,1.00f };   // contrails
			const float tThin = 0.0060f, tMed = 0.0080f, tThick = 0.0120f;

			const int cubesFine = 36, cubesMed = 28, cubesTrail = 20;

			// ===== math helpers =====
			auto mp = [&](float X, float Y, float Z)->Vec3 { return yz_swapped ? Vec3{ X,Z,Y } : Vec3{ X,Y,Z }; };
			auto addL = [&](Vec3 A0, Vec3 B0, Vec3 A1, Vec3 B1, Vec3 c0, Vec3 c1, float th0, float th1, int cubes)
				{
					Line& L = add_line();
					// midpoint reveal
					Vec3 M{ 0.5f * (A0.x + B0.x), 0.5f * (A0.y + B0.y), 0.5f * (A0.z + B0.z) };
					L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
					L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
					L.rgb_t0 = c0; L.thickness.start = th0; L.number_of_cubes = cubes;
					L.copy_start_to_end();
					L.x0.end = A1.x; L.y0.end = A1.y; L.z0.end = A1.z;
					L.x1.end = B1.x; L.y1.end = B1.y; L.z1.end = B1.z;
					L.rgb_t1 = c1; L.thickness.end = th1;
				};
			auto rotX = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ p.x, c * p.y - s * p.z, s * p.y + c * p.z }; };
			auto rotY = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z }; };
			auto rotZ = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z }; };
			auto applyPose = [&](const Pose& P, Vec3 v)->Vec3 {
				// local->world: scale then Rz*Ry*Rx then translate
				Vec3 q{ v.x * P.s, v.y * P.s, v.z * P.s };
				q = rotZ(q, P.roll);
				q = rotY(q, P.yaw);
				q = rotX(q, P.pitch);
				q = Vec3{ q.x + P.t.x, q.y + P.t.y, q.z + P.t.z };
				return mp(q.x, q.y, q.z);
				};
			auto rigid = [&](Vec3 aLocal, Vec3 bLocal, Vec3 cA, Vec3 cB, float thA, float thB, int cubes)
				{
					addL(applyPose(P0, aLocal), applyPose(P0, bLocal),
						applyPose(P1, aLocal), applyPose(P1, bLocal),
						cA, cB, thA, thB, cubes);
				};

			// ===== airplane geometry in LOCAL space (Y up, Z right/left) =====
			// Fuselage profile radii vs x (nose -> tail)
			auto fusRadY = [&](float x) { // nice hump mid fuselage
				// nose 0.9 .. tail -0.70 (len ~1.6)
				float cx = 0.10f, half = 0.80f;
				float t = std::clamp(1.0f - ((x - cx) * (x - cx)) / (half * half), 0.0f, 1.0f);
				return 0.16f * std::sqrt(t);
				};
			auto fusRadZ = [&](float x) {
				return 0.70f * fusRadY(x);
				};

			// 1) FUSELAGE OUTLINES (top/bottom and side rails)
			{
				const int N = 84;
				float x0 = 0.90f, x1 = -0.70f;
				for (int i = 0; i < N - 1; ++i)
				{
					float a = i / (N - 1.0f), b = (i + 1) / (N - 1.0f);
					float xa = x0 * (1 - a) + x1 * a;
					float xb = x0 * (1 - b) + x1 * b;

					float ya = fusRadY(xa), yb = fusRadY(xb);
					float za = fusRadZ(xa), zb = fusRadZ(xb);

					// top/bottom
					rigid(Vec3{ xa, +ya, 0.0f }, Vec3{ xb, +yb, 0.0f }, fuselageColA, fuselageColB, tMed, tMed, cubesFine);
					rigid(Vec3{ xa, -ya, 0.0f }, Vec3{ xb, -yb, 0.0f }, fuselageColA, fuselageColB, tMed, tMed, cubesFine);

					// side rails
					rigid(Vec3{ xa, 0.0f, +za }, Vec3{ xb, 0.0f, +zb }, fuselageColA, fuselageColB, tMed, tMed, cubesFine);
					rigid(Vec3{ xa, 0.0f, -za }, Vec3{ xb, 0.0f, -zb }, fuselageColA, fuselageColB, tMed, tMed, cubesFine);
				}

				// cockpit nose accent
				rigid(Vec3{ 0.90f, 0.00f, 0.00f }, Vec3{ 0.76f, 0.07f, 0.00f }, accentCol, fuselageColA, tMed, tThin, cubesMed);
				rigid(Vec3{ 0.90f, 0.00f, 0.00f }, Vec3{ 0.76f,-0.06f, 0.00f }, accentCol, fuselageColA, tMed, tThin, cubesMed);

				// windows (small dashes along +Z side)
				const int WN = 9;
				for (int i = 0; i < WN; ++i) {
					float u = i / (WN - 1.0f);
					float x = 0.70f - 0.65f * u;
					float y = 0.05f + 0.35f * fusRadY(x);
					float z = 0.55f * fusRadZ(x);
					rigid(Vec3{ x, y, z }, Vec3{ x - 0.05f, y, z }, Vec3{ 0.85f,0.95f,1.0f }, Vec3{ 0.65f,0.85f,1.0f }, tThin * 0.9f, tThin * 0.8f, 12);
				}
			}

			// 2) WINGS (planform triangles + rib)
			auto wing = [&](float dirZ) {
				// right wing dirZ=+1, left = -1
				float span = 0.85f * dirZ;
				Vec3 LE_root{ 0.25f,  0.02f,  0.15f * dirZ };
				Vec3 TE_root{ -0.22f, -0.00f,  0.05f * dirZ };
				Vec3 LE_tip{ 0.35f,  0.04f,  span };
				Vec3 TE_tip{ 0.02f, -0.02f,  span - 0.08f * dirZ };

				// edges
				rigid(LE_root, LE_tip, wingColA, wingColB, tThick, tThick, cubesFine);   // leading
				rigid(TE_root, TE_tip, wingColA, wingColB, tMed, tMed, cubesFine);   // trailing
				rigid(LE_tip, TE_tip, wingColB, wingColA, tMed, tMed, cubesFine);   // tip
				// rib
				rigid(Vec3{ 0.08f,0.02f,0.10f * dirZ }, Vec3{ 0.12f,0.03f,span - 0.06f * dirZ }, wingColB, wingColA, tThin, tThin, cubesMed);

				// wingtip accent
				rigid(LE_tip, Vec3{ LE_tip.x + 0.06f, LE_tip.y + 0.03f, LE_tip.z }, accentCol, accentCol, tThin, tThin, 16);
				};
			wing(+1.0f);
			wing(-1.0f);

			// 3) TAILPLANES (horizontal + vertical)
			{
				// horizontal stabilizer
				float s = 0.48f;
				Vec3 H_leR{ -0.52f, 0.03f,  s }, H_teR{ -0.70f,-0.02f, s - 0.08f };
				Vec3 H_leL{ -0.52f, 0.03f, -s }, H_teL{ -0.70f,-0.02f,-s + 0.08f };
				rigid(H_leR, H_teR, wingColA, wingColB, tMed, tMed, cubesMed);
				rigid(H_leL, H_teL, wingColA, wingColB, tMed, tMed, cubesMed);
				rigid(H_leR, H_leL, wingColB, wingColA, tMed, tMed, cubesMed);
				rigid(H_teR, H_teL, wingColB, wingColA, tThin, tThin, cubesMed);

				// vertical stabilizer (fin)
				Vec3 V_baseF{ -0.62f,  0.00f, 0.00f };
				Vec3 V_baseB{ -0.78f, -0.05f, 0.00f };
				Vec3 V_top{ -0.48f,  0.28f, 0.00f };
				rigid(V_baseF, V_top, fuselageColB, fuselageColA, tThick, tThick, cubesMed);
				rigid(V_baseB, V_top, fuselageColB, fuselageColA, tMed, tMed, cubesMed);
			}

			// 4) ENGINES: under wings (rings) + spinning fans
			auto engine = [&](Vec3 center, float r, int segs, float spinEnd)
				{
					for (int k = 0; k < segs; ++k) {
						float a0 = (k / float(segs)) * TAU;
						float a1 = ((k + 1) / float(segs)) * TAU;

						// circle in YZ-plane (axis along X)
						Vec3 A_local0{ center.x, center.y + r * std::cos(a0), center.z + r * std::sin(a0) };
						Vec3 B_local0{ center.x, center.y + r * std::cos(a1), center.z + r * std::sin(a1) };

						Vec3 A_local1{ center.x, center.y + r * std::cos(a0 + spinEnd), center.z + r * std::sin(a0 + spinEnd) };
						Vec3 B_local1{ center.x, center.y + r * std::cos(a1 + spinEnd), center.z + r * std::sin(a1 + spinEnd) };

						addL(applyPose(P0, A_local0), applyPose(P0, B_local0),
							applyPose(P1, A_local1), applyPose(P1, B_local1),
							fanColA, fanColB, tThin * 0.9f, tThin * 1.1f, 18);
					}
				};
			// engine placements (under wings)
			engine(Vec3{ 0.16f, -0.08f,  0.42f }, 0.085f, 24, 1.75f); // right
			engine(Vec3{ 0.16f, -0.08f, -0.42f }, 0.085f, 24, 1.75f); // left

			// 5) CONTRAILS: wing tips and engines (wavy, grow in)
			auto wavy_trail = [&](Vec3 P, Vec3 dir, float len, float amp, int segs) {
				// start short & calm, end longer & wavier
				for (int i = 0; i < segs; ++i) {
					float t0 = i / float(segs);
					float t1 = (i + 1) / float(segs);
					float Ls = 0.25f * len;                 // start length
					float Le = len;                       // end length

					auto local = [&](float tt, float L, float phase)->Vec3 {
						float x = P.x - L * tt;             // trail backwards along -X (airflow)
						float y = P.y + amp * 0.3f * std::sin(6.0f * tt + phase);
						float z = P.z + amp * 0.5f * std::sin(4.0f * tt + 1.6f * phase);
						return Vec3{ x,y,z };
						};
					Vec3 A0 = applyPose(P0, local(t0, Ls, 0.0f));
					Vec3 B0 = applyPose(P0, local(t1, Ls, 0.0f));
					Vec3 A1 = applyPose(P1, local(t0, Le, 0.9f));
					Vec3 B1 = applyPose(P1, local(t1, Le, 1.7f));

					addL(A0, B0, A1, B1, trailCol, trailCol, tThin * 0.7f, tThin * 0.5f, cubesTrail);
				}
				};
			// wing tip world-local anchors
			Vec3 tipR{ 0.35f, 0.04f,  0.85f };
			Vec3 tipL{ 0.35f, 0.04f, -0.85f };
			wavy_trail(tipR, Vec3{ -1,0,0 }, 0.85f, 0.08f, 32);
			wavy_trail(tipL, Vec3{ -1,0,0 }, 0.85f, 0.08f, 32);
			// engine exhaust trails (shorter)
			wavy_trail(Vec3{ 0.06f,-0.08f,  0.42f }, Vec3{ -1,0,0 }, 0.55f, 0.04f, 24);
			wavy_trail(Vec3{ 0.06f,-0.08f, -0.42f }, Vec3{ -1,0,0 }, 0.55f, 0.04f, 24);
		}


		void init_rainy_window_final_v2()
		{
			lines.clear();

			// --- SECTION 1: Self-Contained Helpers & Constants ---
			auto vec_scale = [](const Vec3& v, float s) -> Vec3 { return { v.x * s, v.y * s, v.z * s }; };

			// --- SECTION 2: The Artistic Palette & Scene Configuration ---
			// Every value here is a brushstroke. Tweak them to change the painting.
			const int num_streaks = 250;
			const float window_width = 2.0f;
			const float window_height = 2.2f;

			const Vec3 rain_color = { 0.6f, 0.7f, 0.8f };        // Cool white with a hint of blue
			const Vec3 city_glow_warm = { 1.0f, 0.7f, 0.3f };       // Distant traffic and office lights
			const Vec3 city_glow_cool = { 0.4f, 0.6f, 1.0f };       // Neon signs, colder lights
			const Vec3 city_ambient = { 0.1f, 0.15f, 0.25f };     // The dark, wet silhouette of the city
			const Vec3 frame_color = { 0.02f, 0.02f, 0.02f };       // A dark frame to draw the eye inward
			const Vec3 interior_reflection_color = { 0.5f, 0.3f, 0.15f }; // A very faint, warm shape of a lamp inside

			// --- SECTION 3: The Distant City - An Impressionistic Backdrop ---
			// These lines are static and far behind the window (positive Z).
			// Their thickness and placement create a "bokeh" or out-of-focus effect.
			for (int i = 0; i < 50; ++i) {
				float x = Random::generate_random_float_minus_one_to_plus_one() * window_width * 0.8f;
				float y = Random::generate_random_float_minus_one_to_plus_one() * window_height * 0.4f;
				float z = 1.0f + Random::generate_random_float_0_to_1() * 2.0f; // Far distance
				float brightness = 0.2f + Random::generate_random_float_0_to_1() * 0.8f;

				Line& bokeh_light = add_line();
				bokeh_light.x0.start = x; bokeh_light.y0.start = y; bokeh_light.z0.start = z;
				bokeh_light.x1.start = x; bokeh_light.y1.start = y; bokeh_light.z1.start = z; // It's a point

				// Mix of warm and cool city lights
				Vec3 color = (Random::generate_random_float_0_to_1() > 0.6f) ? city_glow_warm : city_glow_cool;
				bokeh_light.rgb_t0 = vec_scale(color, brightness);
				bokeh_light.thickness.start = 0.01f + Random::generate_random_float_0_to_1() * 0.04f;
				bokeh_light.number_of_cubes = 2;
				bokeh_light.copy_start_to_end();
				// Animate a gentle flicker or shimmer
				bokeh_light.thickness.end = bokeh_light.thickness.start * (0.5f + Random::generate_random_float_0_to_1());
				bokeh_light.rgb_t1 = vec_scale(bokeh_light.rgb_t0, 0.7f);
			}

			// --- SECTION 4: The Rain on the Glass - The Heart of the Scene ---
			// **THIS IS THE CRITICAL BUG FIX.** Instead of stretching a line, we now animate a
			// fixed-length segment that translates downwards, which correctly simulates a falling streak.
			for (int i = 0; i < num_streaks; ++i) {
				Line& streak = add_line();

				// Each streak has its own unique life
				float x_pos = Random::generate_random_float_minus_one_to_plus_one() * (window_width / 2.0f);
				float segment_length = 0.01f + Random::generate_random_float_0_to_1() * 0.2f;
				float start_y_pos = (window_height / 2.0f) + segment_length; // Start just out of view
				float end_y_pos = (-window_height / 2.0f) * (0.5f + Random::generate_random_float_0_to_1() * 0.5f); // End somewhere on the lower half
				float travel_distance = start_y_pos - end_y_pos;

				// The animation's speed and timing is varied by giving streaks different start/end points in their journey
				float anim_start_progress = pow(Random::generate_random_float_0_to_1(), 2.0f); // Bias towards starting early
				float anim_end_progress = anim_start_progress + 0.1f + Random::generate_random_float_0_to_1() * (1.0f - anim_start_progress);
				if (anim_end_progress > 1.0f) anim_end_progress = 1.0f;

				// START State: The streak segment at its initial position in the animation
				streak.x0.start = x_pos;
				streak.y0.start = start_y_pos - travel_distance * anim_start_progress;
				streak.z0.start = 0;
				streak.x1.start = x_pos;
				streak.y1.start = streak.y0.start - segment_length;
				streak.z1.start = 0;

				// END State: The streak segment at its final position
				streak.x0.end = x_pos;
				streak.y0.end = start_y_pos - travel_distance * anim_end_progress;
				streak.z0.end = 0;
				streak.x1.end = x_pos;
				streak.y1.end = streak.y0.end - segment_length;
				streak.z1.end = 0;

				// Styling: Streaks are bright but fade slightly as they fall
				float initial_brightness = 0.5f + Random::generate_random_float_0_to_1() * 0.5f;
				streak.rgb_t0 = vec_scale(rain_color, initial_brightness);
				streak.rgb_t1 = vec_scale(rain_color, initial_brightness * 0.5f); // Fade out
				streak.thickness.start = 0.001f + Random::generate_random_float_0_to_1() * 0.003f;
				streak.thickness.end = streak.thickness.start * 0.5f;
				streak.number_of_cubes = 15;
			}

			// --- SECTION 5: The Window Frame and Interior Reflections - Grounding the Viewer ---
			// These elements tell us we are *inside*, looking out.
			// The frame is static and dark.
			{ Line& l = add_line(); l.x0.start = -window_width / 2; l.y0.start = -window_height / 2; l.z0.start = 0; l.x1.start = window_width / 2; l.y1.start = -window_height / 2; l.z1.start = 0; l.rgb_t0 = frame_color; l.thickness.start = 0.04f; l.number_of_cubes = 50; l.copy_start_to_end(); }
			{ Line& l = add_line(); l.x0.start = -window_width / 2; l.y0.start = window_height / 2; l.z0.start = 0; l.x1.start = window_width / 2; l.y1.start = window_height / 2; l.z1.start = 0; l.rgb_t0 = frame_color; l.thickness.start = 0.04f; l.number_of_cubes = 50; l.copy_start_to_end(); }
			{ Line& l = add_line(); l.x0.start = -window_width / 2; l.y0.start = -window_height / 2; l.z0.start = 0; l.x1.start = -window_width / 2; l.y1.start = window_height / 2; l.z1.start = 0; l.rgb_t0 = frame_color; l.thickness.start = 0.04f; l.number_of_cubes = 50; l.copy_start_to_end(); }
			{ Line& l = add_line(); l.x0.start = window_width / 2; l.y0.start = -window_height / 2; l.z0.start = 0; l.x1.start = window_width / 2; l.y1.start = window_height / 2; l.z1.start = 0; l.rgb_t0 = frame_color; l.thickness.start = 0.04f; l.number_of_cubes = 50; l.copy_start_to_end(); }

			// A crucial detail: a very faint, warm, curved line on the glass.
			// This is the reflection of a lamp inside the room. It adds immense depth and mood.
			for (int i = 0; i < 50; ++i) {
				float progress = (float)i / 49.0f;
				float x = -0.5f + progress * 1.0f;
				float y = -0.3f - x * x * 0.5f; // A gentle downward curve

				Line& reflection_segment = add_line();
				reflection_segment.x0.start = x; reflection_segment.y0.start = y; reflection_segment.z0.start = -0.01f;
				reflection_segment.x1.start = x; reflection_segment.y1.start = y; reflection_segment.z1.start = -0.01f;
				reflection_segment.rgb_t0 = interior_reflection_color;
				reflection_segment.thickness.start = 0.02f;
				reflection_segment.number_of_cubes = 1;
				reflection_segment.copy_start_to_end();
			}
		}


		
		
		void init_spiral_torus_interference()
		{
			lines.clear();

			const float TAU = 6.2831853071795864769252867665590f;
			const float PI = 3.1415926535897932384626433832795f;

			// --- Configuration ---
			const int num_tori = 3; // Number of overlapping tori
			const float major_radius = 0.6f; // Radius of the main ring
			const float minor_radius = 0.15f; // Radius of the tube
			const int segments_per_torus = 100; // Resolution of each torus ring
			const int rings_per_torus = 40; // Number of rings around the major radius
			const float base_rotation_speed = 0.2f; // Base rotation speed
			const float rotation_speed_variance = 0.1f; // How much each torus differs in speed
			const float phase_variance = 0.3f; // Phase offset between tori
			const float hue_shift_per_torus = 0.33f; // Color difference between tori
			const float base_thickness = 0.004f;
			const int cubes_per_segment = 15;

			auto hue_to_rgb = [&](float h) -> Vec3 {
				h = fmodf(h, 1.0f);
				float r = 0.5f + 0.5f * std::cos(TAU * (h + 0.00f));
				float g = 0.5f + 0.5f * std::cos(TAU * (h + 0.33f));
				float b = 0.5f + 0.5f * std::cos(TAU * (h + 0.66f));
				return { r, g, b };
				};

			// --- Generate Interfering Tori ---
			for (int t = 0; t < num_tori; ++t) {
				float torus_rotation_speed = base_rotation_speed + (Random::generate_random_float_0_to_1() - 0.5f) * rotation_speed_variance;
				float torus_phase_offset = t * phase_variance;
				Vec3 base_color = hue_to_rgb(t * hue_shift_per_torus);

				for (int ring = 0; ring < rings_per_torus; ++ring) {
					float u = float(ring) / float(rings_per_torus); // 0 to 1
					float theta = u * TAU; // Angle around the major radius

					for (int seg = 0; seg < segments_per_torus; ++seg) {
						float v = float(seg) / float(segments_per_torus); // 0 to 1
						float phi = v * TAU; // Angle around the minor radius (tube)

						// Calculate base torus point
						float x_base = (major_radius + minor_radius * std::cos(phi)) * std::cos(theta);
						float y_base = minor_radius * std::sin(phi);
						float z_base = (major_radius + minor_radius * std::cos(phi)) * std::sin(theta);

						// Animate rotation for this specific torus
						float current_time = torus_phase_offset + torus_rotation_speed * 0.1; // 'time' from shader
						float cos_rot = std::cos(current_time);
						float sin_rot = std::sin(current_time);

						// Rotate the point around the Y-axis
						float x_rot = x_base * cos_rot - z_base * sin_rot;
						float y_rot = y_base;
						float z_rot = x_base * sin_rot + z_base * cos_rot;

						// Calculate the *next* point to form a line segment
						float v_next = float(seg + 1) / float(segments_per_torus);
						float phi_next = v_next * TAU;
						float x_base_next = (major_radius + minor_radius * std::cos(phi_next)) * std::cos(theta);
						float y_base_next = minor_radius * std::sin(phi_next);
						float z_base_next = (major_radius + minor_radius * std::cos(phi_next)) * std::sin(theta);

						float x_rot_next = x_base_next * cos_rot - z_base_next * sin_rot;
						float y_rot_next = y_base_next;
						float z_rot_next = x_base_next * sin_rot + z_base_next * cos_rot;

						// Create the line segment for this frame
						Line& line = add_line();

						// The line starts and ends at the current calculated positions (no animation start/end needed here
						// if we want a continuous, spinning torus, otherwise use animation as shown in other examples)
						// For a simple, non-revealing effect:
						line.x0.start = x_rot;
						line.y0.start = y_rot;
						line.z0.start = z_rot;
						line.x1.start = x_rot_next;
						line.y1.start = y_rot_next;
						line.z1.start = z_rot_next;

						line.copy_start_to_end(); // If no animation, start and end are the same

						line.x0.end = z_rot;
						line.y0.end = y_rot;
						line.z0.end = x_rot;
						line.x1.end = z_rot_next;
						line.y1.end = y_rot_next;
						line.z1.end = x_rot_next;
						


						// Apply color and style
						line.rgb_t0 = base_color;
						line.rgb_t1 = base_color;
						line.thickness.start = base_thickness;
						line.thickness.end = base_thickness;
						line.number_of_cubes = cubes_per_segment;
					}
				}
			}
		}


		

		
		// --- Airplane v2: stabilized wireframe delta-jet with contrails ---
		void init_2031_airplane_stabilized()
		{
			lines.clear();

			// ===== knobs =====
			const bool  yz_swapped = true;       // set false for classic Y-up
			const float scale = 1.00f;     // overall scene scale
			const float TAU = 6.28318530718f;

			// Animation poses (start -> end)
			struct Pose { Vec3 t; float yaw, pitch, roll, s; };
			const Pose P0{ {-1.15f, 0.04f, -0.02f},  0.00f,  0.02f, -0.10f, scale };
			const Pose P1{ { 1.05f, 0.26f,  0.14f},  0.22f,  0.08f,  0.28f, scale };

			// Palette / thickness
			const Vec3 bodyA{ 0.78f,0.86f,0.98f }, bodyB{ 0.62f,0.74f,0.94f };
			const Vec3 wingA{ 0.86f,0.94f,1.00f }, wingB{ 0.66f,0.82f,0.98f };
			const Vec3 accent{ 0.98f,0.80f,0.35f };
			const Vec3 trail{ 0.97f,0.99f,1.00f };

			const float tThin = 0.0060f;
			const float tMed = 0.0085f;
			const float tThick = 0.0120f;

			const int cubesFine = 34;
			const int cubesMed = 26;
			const int cubesTrail = 20;

			// ========= math helpers =========
			auto rotX = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ p.x, c * p.y - s * p.z, s * p.y + c * p.z }; };
			auto rotY = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z }; };
			auto rotZ = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z }; };

			auto applyPoseLocal = [&](const Pose& P, Vec3 v)->Vec3 {
				// local (X nosetail, Y up, Z leftright) -> posed local
				Vec3 q{ v.x * P.s, v.y * P.s, v.z * P.s };
				// Z-Y-X order is stable for aircraft-ish yaw/pitch/roll feel
				q = rotZ(q, P.roll);
				q = rotY(q, P.yaw);
				q = rotX(q, P.pitch);
				q = { q.x + P.t.x, q.y + P.t.y, q.z + P.t.z };
				// final axis mapping (kept to very last step)
				return yz_swapped ? Vec3{ q.x, q.z, q.y } : q;
				};

			auto addL = [&](Vec3 a0, Vec3 b0, Vec3 a1, Vec3 b1, Vec3 c0, Vec3 c1, float th0, float th1, int cubes) {
				Line& L = add_line();
				Vec3 M{ 0.5f * (a0.x + b0.x), 0.5f * (a0.y + b0.y), 0.5f * (a0.z + b0.z) };
				L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
				L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
				L.rgb_t0 = c0; L.thickness.start = th0; L.number_of_cubes = cubes;
				L.copy_start_to_end();
				L.x0.end = a1.x; L.y0.end = a1.y; L.z0.end = a1.z;
				L.x1.end = b1.x; L.y1.end = b1.y; L.z1.end = b1.z;
				L.rgb_t1 = c1; L.thickness.end = th1;
				};

			auto seg_local = [&](Vec3 A, Vec3 B, Vec3 cA, Vec3 cB, float thA, float thB, int cubes) {
				addL(applyPoseLocal(P0, A), applyPoseLocal(P0, B),
					applyPoseLocal(P1, A), applyPoseLocal(P1, B),
					cA, cB, thA, thB, cubes);
				};

			// ========= geometry (LOCAL space) =========
			// Fuselage radius along X (nose ~ +0.95 to tail ~ -0.85). Smooth, bounded.
			auto fusRadY = [&](float x) {
				// piecewise smooth dome: big center, taper to nose/tail
				float a = 0.18f, c = 0.10f, half = 0.78f;
				float t = 1.0f - ((x - c) * (x - c)) / (half * half);
				t = std::max(0.0f, t);
				return a * std::sqrt(t);
				};
			auto fusRadZ = [&](float x) { return 0.70f * fusRadY(x); };

			// (1) Longitudinal ribs (very stable): connect successive rings at fixed angles around the body
			{
				const int rings = 30;                  // along X
				const int spokes = 8;                  // around radius (in YZ)
				const float xNose = 0.95f, xTail = -0.85f;

				for (int s = 0; s < spokes; ++s) {
					float ang = (s / float(spokes)) * TAU; // angle in YZ
					float cy = std::cos(ang), sy = std::sin(ang);

					for (int i = 0; i < rings - 1; ++i) {
						float a = i / (rings - 1.0f), b = (i + 1) / (rings - 1.0f);
						float xa = xNose * (1 - a) + xTail * a;
						float xb = xNose * (1 - b) + xTail * b;

						// ellipse on YZ
						Vec3 Pa{ xa, fusRadY(xa) * cy, fusRadZ(xa) * sy };
						Vec3 Pb{ xb, fusRadY(xb) * cy, fusRadZ(xb) * sy };

						seg_local(Pa, Pb, bodyA, bodyB, tMed, tMed, cubesFine);
					}
				}

				// centerline (nose-to-tail)
				seg_local(Vec3{ xNose, 0, 0 }, Vec3{ xTail, 0, 0 }, bodyB, bodyA, tMed, tMed, cubesFine);

				// simple canopy line (top front)
				seg_local(Vec3{ 0.78f,  fusRadY(0.78f) * 0.85f, 0.0f },
					Vec3{ 0.58f,  fusRadY(0.58f) * 0.95f, 0.0f }, accent, bodyA, tMed, tThin, 20);
			}

			// (2) Delta wings (trapezoid planform), symmetric
			auto build_wing = [&](float side) {
				// side = +1 right wing (Z+), -1 left (Z-)
				const Vec3 rootLE{ 0.18f,  0.01f,  0.10f * side };
				const Vec3 rootTE{ -0.15f, -0.02f,  0.06f * side };
				const Vec3 tipLE{ 0.10f,  0.03f,  0.95f * side };
				const Vec3 tipTE{ -0.34f, -0.02f,  0.82f * side };

				// leading/trailing/tip edges
				seg_local(rootLE, tipLE, wingA, wingB, tThick, tThick, cubesFine);
				seg_local(rootTE, tipTE, wingA, wingB, tMed, tMed, cubesFine);
				seg_local(tipLE, tipTE, wingB, wingA, tMed, tMed, cubesFine);
				// one mid rib
				seg_local(Vec3{ -0.02f, 0.01f, 0.12f * side }, Vec3{ -0.06f, 0.02f, 0.84f * side }, wingB, wingA, tThin, tThin, cubesMed);

				// wingtip accent
				seg_local(tipLE, Vec3{ tipLE.x + 0.06f, tipLE.y + 0.03f, tipLE.z }, accent, accent, tThin, tThin, 16);
				};
			build_wing(+1.0f);
			build_wing(-1.0f);

			// (3) Tailplanes
			{
				// vertical fin
				Vec3 baseF{ -0.58f,  0.00f, 0.00f };
				Vec3 baseB{ -0.78f, -0.04f, 0.00f };
				Vec3 top{ -0.44f,  0.26f, 0.00f };
				seg_local(baseF, top, bodyB, bodyA, tThick, tThick, cubesMed);
				seg_local(baseB, top, bodyB, bodyA, tMed, tMed, cubesMed);

				// horizontal stabs
				Vec3 H_leR{ -0.52f, 0.02f,  0.52f }, H_teR{ -0.70f,-0.02f, 0.45f };
				Vec3 H_leL{ -0.52f, 0.02f, -0.52f }, H_teL{ -0.70f,-0.02f,-0.45f };
				seg_local(H_leR, H_teR, wingA, wingB, tMed, tMed, cubesMed);
				seg_local(H_leL, H_teL, wingA, wingB, tMed, tMed, cubesMed);
				seg_local(H_leR, H_leL, wingB, wingA, tMed, tMed, cubesMed);
				seg_local(H_teR, H_teL, wingB, wingA, tThin, tThin, cubesMed);
			}

			// (4) Engine glow (simple afterburner starlines at tail centerline)
			{
				Vec3 nozzle{ -0.80f, -0.01f, 0.0f };
				const int rays = 6;
				for (int i = 0; i < rays; ++i) {
					float a = (i / float(rays)) * TAU;
					Vec3 A{ nozzle.x, nozzle.y, nozzle.z };
					Vec3 B{ nozzle.x - 0.18f,
							nozzle.y + 0.04f * std::sin(a),
							nozzle.z + 0.06f * std::cos(a) };
					seg_local(A, B, Vec3{ 0.95f,0.75f,0.35f }, Vec3{ 1.0f,0.90f,0.55f }, tThin * 0.9f, tThin * 0.7f, 16);
				}
			}

			// (5) Contrails (smooth & stable). Start short & calm  end longer & wavier.
			auto contrail = [&](Vec3 anchor, float len, float amp, int segs) {
				for (int i = 0; i < segs; ++i) {
					float t0 = i / float(segs);
					float t1 = (i + 1) / float(segs);

					float Ls = 0.25f * len, Le = len;     // start vs end length

					auto curve = [&](float tt, float L, float phase)->Vec3 {
						// trail goes backwards in -X with gentle Y/Z waves
						float x = anchor.x - L * tt;
						float y = anchor.y + amp * 0.25f * std::sin(5.3f * tt + 1.1f * phase);
						float z = anchor.z + amp * 0.40f * std::sin(3.9f * tt + 2.0f * phase);
						return Vec3{ x,y,z };
						};

					Vec3 A0 = applyPoseLocal(P0, curve(t0, Ls, 0.0f));
					Vec3 B0 = applyPoseLocal(P0, curve(t1, Ls, 0.0f));
					Vec3 A1 = applyPoseLocal(P1, curve(t0, Le, 1.4f));
					Vec3 B1 = applyPoseLocal(P1, curve(t1, Le, 2.2f));

					addL(A0, B0, A1, B1, trail, trail, tThin * 0.70f, tThin * 0.55f, cubesTrail);
				}
				};
			// tips & tail contrails
			contrail(Vec3{ 0.10f, 0.03f,  0.95f }, 0.90f, 0.08f, 30);
			contrail(Vec3{ 0.10f, 0.03f, -0.95f }, 0.90f, 0.08f, 30);
			contrail(Vec3{ -0.72f,-0.01f,  0.00f }, 0.55f, 0.05f, 22);
		}


		



		void init_living_coral()
		{
			lines.clear();

			// ====================================================================
			// SECTION 1: ARTISTIC PARAMETERS & CONFIGURATION ("The Knobs")
			// Tweak these values to create different species of coral.
			// ====================================================================

			// --- Stalk Parameters ---
			const int   num_stalk_segments = 40;     // Resolution of the main stalk
			const float stalk_height = 1.6f;         // Final height of the coral
			const float stalk_curve_factor = 0.15f;    // How much the main stalk bends

			// --- Arm Parameters ---
			const float arm_probability = 0.35f;   // 35% chance for a stalk segment to grow an arm
			const int   num_arm_segments = 25;      // Resolution of each arm
			const float arm_length = 0.4f;         // How far arms extend
			const float arm_spiral_factor = 4.0f;  // How tightly the arms curl
			const float arm_sway_amount = 0.3f;    // How much the arms gently move at the end

			// --- Plankton Parameters ---
			const int   num_plankton = 150;        // Number of drifting particles
			const float plankton_sphere_radius = 1.2f; // The volume they occupy
			const float plankton_drift_speed = 0.1f; // How fast they move

			// --- Color Palette ---
			const Vec3 stalk_color = { 0.8f, 0.7f, 0.9f };     // Pale lavender
			const Vec3 arm_color_base = { 0.9f, 0.5f, 0.8f };      // Magenta base
			const Vec3 arm_color_tip = { 0.4f, 0.8f, 1.0f };       // Glowing cyan tip
			const Vec3 plankton_color = { 0.7f, 1.0f, 0.8f };      // Soft seafoam green

			// --- General Styling & Constants ---
			const float base_thickness = 0.012f;
			const float TAU = 6.28318530718f;
			const float PI = 3.14159265359f; // <<<<<<<<<<<<< FIXED: PI constant is now defined.

			// ====================================================================
			// SECTION 2: PROCEDURAL GENERATION OF THE CORAL
			// We build the stalk first, and probabilistically grow arms from it.
			// ====================================================================

			// --- Part 2a: The Stalk ---
			for (int i = 0; i < num_stalk_segments; ++i)
			{
				float t0 = (float)i / num_stalk_segments;
				float t1 = (float)(i + 1) / num_stalk_segments;

				// Calculate the start and end points of this stalk segment
				Vec3 p0 = {
					stalk_curve_factor * sin(t0 * PI), // Use PI for a single curve
					-stalk_height / 2.0f + t0 * stalk_height,
					stalk_curve_factor * cos(t0 * PI)
				};
				Vec3 p1 = {
					stalk_curve_factor * sin(t1 * PI),
					-stalk_height / 2.0f + t1 * stalk_height,
					stalk_curve_factor * cos(t1 * PI)
				};

				// Create the line for this stalk segment
				Line& stalk_segment = add_line();
				stalk_segment.x0.end = p0.x; stalk_segment.y0.end = p0.y; stalk_segment.z0.end = p0.z;
				stalk_segment.x1.end = p1.x; stalk_segment.y1.end = p1.y; stalk_segment.z1.end = p1.z;

				// ANIMATION: Grow the stalk from its base
				stalk_segment.x0.start = 0.0f; stalk_segment.y0.start = -stalk_height / 2.0f; stalk_segment.z0.start = 0.0f;
				stalk_segment.x1.start = 0.0f; stalk_segment.y1.start = -stalk_height / 2.0f; stalk_segment.z1.start = 0.0f;

				// Styling: Thicker at the base, thinner at the top
				stalk_segment.rgb_t0 = stalk_color;
				stalk_segment.rgb_t1 = stalk_color;
				stalk_segment.thickness.start = base_thickness * (1.0f - t0 * 0.5f);
				stalk_segment.thickness.end = base_thickness * (1.0f - t1 * 0.5f);
				stalk_segment.number_of_cubes = 10;

				// --- Part 2b: The Arms (Probabilistic Growth) ---
				if (Random::generate_random_float_0_to_1() < arm_probability && i > 5)
				{
					float arm_base_angle = Random::generate_random_float_0_to_1() * TAU;

					for (int j = 0; j < num_arm_segments; ++j)
					{
						float at0 = (float)j / num_arm_segments;
						float at1 = (float)(j + 1) / num_arm_segments;

						auto calculate_arm_point = [&](float progress, float sway_phase) -> Vec3 {
							float current_radius = progress * arm_length;
							float current_angle = arm_base_angle + progress * arm_spiral_factor;
							float sway_offset = arm_sway_amount * sin(progress * PI + sway_phase);

							Vec3 arm_point = {
								current_radius * cos(current_angle),
								current_radius * sin(current_angle) * 0.5f, // Use Y for upward curve
								current_radius * sin(current_angle)
							};

							float tilt = PI / 4.0f;
							arm_point = { arm_point.x, arm_point.y * cos(tilt) - arm_point.z * sin(tilt), arm_point.y * sin(tilt) + arm_point.z * cos(tilt) };

							arm_point.z += sway_offset;
							return vec_add(p1, arm_point);
							};

						Vec3 ap0 = calculate_arm_point(at0, 0.0f);
						Vec3 ap1 = calculate_arm_point(at1, 0.0f);
						Vec3 ap0_swayed = calculate_arm_point(at0, PI);
						Vec3 ap1_swayed = calculate_arm_point(at1, PI);

						Line& arm_segment = add_line();

						arm_segment.x0.start = p1.x; arm_segment.y0.start = p1.y; arm_segment.z0.start = p1.z;
						arm_segment.x1.start = p1.x; arm_segment.y1.start = p1.y; arm_segment.z1.start = p1.z;

						arm_segment.x0.end = ap0_swayed.x; arm_segment.y0.end = ap0_swayed.y; arm_segment.z0.end = ap0_swayed.z;
						arm_segment.x1.end = ap1_swayed.x; arm_segment.y1.end = ap1_swayed.y; arm_segment.z1.end = ap1_swayed.z;

						float color_mix = pow(at0, 0.7f);
						Vec3 segment_color = {
							(1 - color_mix) * arm_color_base.x + color_mix * arm_color_tip.x,
							(1 - color_mix) * arm_color_base.y + color_mix * arm_color_tip.y,
							(1 - color_mix) * arm_color_base.z + color_mix * arm_color_tip.z,
						};
						arm_segment.rgb_t0 = segment_color;
						arm_segment.rgb_t1 = segment_color;
						arm_segment.thickness.start = base_thickness * 0.5f * (1.0f - at0);
						arm_segment.thickness.end = base_thickness * 0.5f * (1.0f - at1);
						arm_segment.number_of_cubes = 8;
					}
				}
			}

			// ====================================================================
			// SECTION 3: ATMOSPHERIC PARTICLES (PLANKTON)
			// ====================================================================

			for (int i = 0; i < num_plankton; ++i)
			{
				Line& plankton = add_line();

				float theta = Random::generate_random_float_0_to_1() * TAU;
				float phi = acos(2.0f * Random::generate_random_float_0_to_1() - 1.0f);
				float r = plankton_sphere_radius * pow(Random::generate_random_float_0_to_1(), 0.5f);

				Vec3 p0 = {
					r * sin(phi) * cos(theta),
					r * cos(phi),
					r * sin(phi) * sin(theta)
				};

				Vec3 drift = {
					Random::generate_random_float_minus_one_to_plus_one(),
					Random::generate_random_float_minus_one_to_plus_one(),
					Random::generate_random_float_minus_one_to_plus_one()
				};
				drift = vec_normalize(drift);
				drift = vec_scale(drift, plankton_drift_speed);
				Vec3 p1 = vec_add(p0, drift);

				plankton.x0.start = p0.x; plankton.y0.start = p0.y; plankton.z0.start = p0.z;
				plankton.x1.start = p0.x; plankton.y1.start = p0.y; plankton.z1.start = p0.z;

				plankton.x0.end = p0.x; plankton.y0.end = p0.y; plankton.z0.end = p0.z;
				plankton.x1.end = p1.x; plankton.y1.end = p1.y; plankton.z1.end = p1.z;

				plankton.rgb_t0 = plankton_color;
				plankton.rgb_t1 = vec_scale(plankton_color, 0.5f);
				plankton.thickness.start = 0.0f;
				plankton.thickness.end = 0.005f;
				plankton.number_of_cubes = 5;
			}
		}


		// --- Submarine v1: stabilized wireframe with bubbles & sonar ---
		void init_2032_submarine_stabilized()
		{
			lines.clear();

			// ===== knobs =====
			const bool  yz_swapped = true;        // keep true if your scene prefers Z-up
			const float scale = 1.10f;       // overall size
			const float TAU = 6.28318530718f;

			// Motion (start -> end)
			struct Pose { Vec3 t; float yaw, pitch, roll, s; };
			const Pose P0{ {-1.05f, -0.15f, -0.10f},  0.00f,  0.00f, -0.02f, scale };
			const Pose P1{ { 0.90f, -0.10f,  0.06f},  0.14f, -0.04f,  0.10f, scale };

			// Colors
			const Vec3 hullA{ 0.18f,0.32f,0.42f }, hullB{ 0.10f,0.22f,0.32f }; // deep blue steel
			const Vec3 sailA{ 0.26f,0.40f,0.52f }, sailB{ 0.16f,0.30f,0.45f };
			const Vec3 accent{ 0.90f,0.78f,0.28f };                            // small highlights
			const Vec3 bubble{ 0.90f,0.98f,1.00f }, sonar{ 0.20f,0.95f,0.55f };

			const float tThin = 0.0060f;
			const float tMed = 0.0085f;
			const float tThick = 0.0125f;

			const int cubesFine = 32;
			const int cubesMed = 24;
			const int cubesBubble = 14;

			// === math helpers ===
			auto rotX = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ p.x, c * p.y - s * p.z, s * p.y + c * p.z }; };
			auto rotY = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x + s * p.z, p.y, -s * p.x + c * p.z }; };
			auto rotZ = [](Vec3 p, float a) { float c = std::cos(a), s = std::sin(a); return Vec3{ c * p.x - s * p.y, s * p.x + c * p.y, p.z }; };

			auto applyPoseLocal = [&](const Pose& P, Vec3 v)->Vec3 {
				// local axes: X forward (bowstern = +X  -X), Y up, Z starboard(+)/port(-)
				Vec3 q{ v.x * P.s, v.y * P.s, v.z * P.s };
				q = rotZ(q, P.roll);
				q = rotY(q, P.yaw);
				q = rotX(q, P.pitch);
				q = { q.x + P.t.x, q.y + P.t.y, q.z + P.t.z };
				return yz_swapped ? Vec3{ q.x, q.z, q.y } : q;
				};

			auto addL = [&](Vec3 a0, Vec3 b0, Vec3 a1, Vec3 b1, Vec3 c0, Vec3 c1, float th0, float th1, int cubes) {
				Line& L = add_line();
				Vec3 M{ 0.5f * (a0.x + b0.x), 0.5f * (a0.y + b0.y), 0.5f * (a0.z + b0.z) };
				L.x0.start = M.x; L.y0.start = M.y; L.z0.start = M.z;
				L.x1.start = M.x; L.y1.start = M.y; L.z1.start = M.z;
				L.rgb_t0 = c0; L.thickness.start = th0; L.number_of_cubes = cubes;
				L.copy_start_to_end();
				L.x0.end = a1.x; L.y0.end = a1.y; L.z0.end = a1.z;
				L.x1.end = b1.x; L.y1.end = b1.y; L.z1.end = b1.z;
				L.rgb_t1 = c1; L.thickness.end = th1;
				};

			auto seg_local = [&](Vec3 A, Vec3 B, Vec3 cA, Vec3 cB, float thA, float thB, int cubes) {
				addL(applyPoseLocal(P0, A), applyPoseLocal(P0, B),
					applyPoseLocal(P1, A), applyPoseLocal(P1, B),
					cA, cB, thA, thB, cubes);
				};

			// === Hull (cigar; longitudinal ribs for stability) ===
			auto radY = [&](float x) {
				// smooth capsule-ish profile (taller than wide)
				// body spans from tail -0.95 to bow +0.95
				float half = 0.85f, a = 0.22f, c = 0.00f;
				float t = 1.0f - ((x - c) * (x - c)) / (half * half);
				t = std::max(0.0f, t);
				return a * std::sqrt(t);
				};
			auto radZ = [&](float x) { return 0.85f * radY(x); };

			{
				const int rings = 36;
				const int spokes = 10;
				const float xBow = 0.95f, xTail = -0.95f;

				for (int s = 0; s < spokes; ++s)
				{
					float ang = (s / float(spokes)) * TAU; // around YZ
					float cy = std::cos(ang), sy = std::sin(ang);
					for (int i = 0; i < rings - 1; ++i)
					{
						float a = i / (rings - 1.0f), b = (i + 1) / (rings - 1.0f);
						float xa = xBow * (1 - a) + xTail * a;
						float xb = xBow * (1 - b) + xTail * b;

						Vec3 Pa{ xa, radY(xa) * cy, radZ(xa) * sy };
						Vec3 Pb{ xb, radY(xb) * cy, radZ(xb) * sy };

						seg_local(Pa, Pb, hullA, hullB, tMed, tMed, cubesFine);
					}
				}

				// dorsal & keel centerlines (clean silhouette)
				seg_local(Vec3{ xBow,  radY(0.6f), 0.0f }, Vec3{ xTail,  radY(-0.2f) * 0.6f, 0.0f }, hullA, hullB, tMed, tMed, cubesMed);
				seg_local(Vec3{ xBow, -radY(0.6f), 0.0f }, Vec3{ xTail, -radY(-0.2f) * 0.6f, 0.0f }, hullB, hullA, tMed, tMed, cubesMed);
			}

			// === Sail (conning tower) ===
			{
				// footprint on hull
				const float xF = 0.10f, xB = -0.16f;
				const float zW = 0.28f; // half width in Z
				const float h = 0.32f; // sail height above hull

				// front/back edges (slight rake)
				seg_local(Vec3{ xF,  0.02f, -zW }, Vec3{ xF,  h, -zW }, sailA, sailB, tThick, tMed, cubesMed);
				seg_local(Vec3{ xF,  0.02f,  zW }, Vec3{ xF,  h,  zW }, sailA, sailB, tThick, tMed, cubesMed);
				seg_local(Vec3{ xB, -0.01f,-zW }, Vec3{ xB, h * 0.92f,-zW }, sailB, sailA, tMed, tMed, cubesMed);
				seg_local(Vec3{ xB, -0.01f, zW }, Vec3{ xB, h * 0.92f, zW }, sailB, sailA, tMed, tMed, cubesMed);

				// top rim & base rim
				seg_local(Vec3{ xF, h, -zW }, Vec3{ xF, h,  zW }, sailA, sailB, tMed, tMed, cubesMed);
				seg_local(Vec3{ xB, h * 0.92f,-zW }, Vec3{ xB, h * 0.92f, zW }, sailB, sailA, tMed, tMed, cubesMed);
				seg_local(Vec3{ xF, 0.02f,-zW }, Vec3{ xB,-0.01f,-zW }, sailA, sailB, tMed, tMed, cubesMed);
				seg_local(Vec3{ xF, 0.02f, zW }, Vec3{ xB,-0.01f, zW }, sailA, sailB, tMed, tMed, cubesMed);

				// periscope & mast
				seg_local(Vec3{ xF - 0.02f, h, 0.00f }, Vec3{ xF - 0.02f, h + 0.22f, 0.00f }, accent, accent, tThin, tThin, 16);
				seg_local(Vec3{ xF - 0.02f, h + 0.18f, -0.04f }, Vec3{ xF - 0.02f, h + 0.18f, 0.04f }, accent, accent, tThin, tThin, 12);
			}

			// === Bow planes (fore dive planes) ===
			{
				const float x = 0.62f, y = 0.00f, w = 0.44f;
				seg_local(Vec3{ x, y + 0.02f, 0.10f }, Vec3{ x - 0.22f, y - 0.01f,  w }, hullA, hullB, tMed, tMed, cubesMed);
				seg_local(Vec3{ x, y + 0.02f,-0.10f }, Vec3{ x - 0.22f, y - 0.01f, -w }, hullA, hullB, tMed, tMed, cubesMed);
				seg_local(Vec3{ x - 0.22f, y - 0.01f,  w }, Vec3{ x - 0.22f, y - 0.01f, -w }, hullB, hullA, tThin, tThin, cubesMed);
			}

			// === Stern planes & rudder ===
			{
				// horizontal stabilizers
				seg_local(Vec3{ -0.62f, 0.00f,  0.42f }, Vec3{ -0.88f, -0.02f,  0.30f }, hullB, hullA, tMed, tMed, cubesMed);
				seg_local(Vec3{ -0.62f, 0.00f, -0.42f }, Vec3{ -0.88f, -0.02f, -0.30f }, hullB, hullA, tMed, tMed, cubesMed);
				// vertical rudder
				seg_local(Vec3{ -0.72f, -0.02f, 0.0f }, Vec3{ -0.92f,  0.28f,  0.0f }, hullB, hullA, tThick, tMed, cubesMed);
				seg_local(Vec3{ -0.72f, -0.02f, 0.0f }, Vec3{ -0.92f, -0.24f,  0.0f }, hullB, hullA, tThick, tMed, cubesMed);
			}

			// === Propeller star (simple blades) ===
			{
				const int blades = 6;
				Vec3 hub{ -0.96f, -0.02f, 0.0f };
				for (int i = 0; i < blades; ++i)
				{
					float a = (i / float(blades)) * TAU;
					Vec3 A{ hub.x, hub.y, hub.z };
					Vec3 B{ hub.x - 0.14f,
							hub.y + 0.05f * std::sin(a),
							hub.z + 0.07f * std::cos(a) };
					seg_local(A, B, Vec3{ 0.95f,0.78f,0.40f }, Vec3{ 1.00f,0.90f,0.55f }, tThin * 0.9f, tThin * 0.7f, 16);
				}
			}

			// === Bubbles (rise upward in local +Y  swap maps to scene +Z) ===
			auto bubble_chain = [&](Vec3 seed, float rise, float sway, int segs)
				{
					for (int i = 0; i < segs; ++i)
					{
						float t0 = i / float(segs);
						float t1 = (i + 1) / float(segs);

						auto P = [&](float t)->Vec3 {
							float x = seed.x - 0.12f * t + 0.04f * std::sin(7.0f * t + seed.z * 3.0f);
							float y = seed.y + rise * t;
							float z = seed.z + sway * std::sin(5.2f * t + seed.x * 4.0f);
							return Vec3{ x,y,z };
							};

						Vec3 A0 = applyPoseLocal(P0, P(t0));
						Vec3 B0 = applyPoseLocal(P0, P(t1));
						Vec3 A1 = applyPoseLocal(P1, P(t0));
						Vec3 B1 = applyPoseLocal(P1, P(t1));

						addL(A0, B0, A1, B1, bubble, bubble, tThin * 0.55f, tThin * 0.40f, cubesBubble);
					}
				};

			// seed a few trails near stern & planes
			bubble_chain(Vec3{ -0.82f,-0.03f, 0.22f }, 0.90f, 0.18f, 18);
			bubble_chain(Vec3{ -0.84f,-0.05f,-0.18f }, 0.88f, 0.22f, 18);
			bubble_chain(Vec3{ -0.70f, 0.00f, 0.40f }, 0.70f, 0.20f, 14);
			bubble_chain(Vec3{ -0.70f, 0.00f,-0.40f }, 0.70f, 0.20f, 14);

			// === Sonar rings (expanding chords in YZ plane near the sail) ===
			{
				const int rings = 3;
				const int chords = 26;
				const float xCenter = 0.05f;
				const float r0 = 0.10f, r1 = 0.85f;

				for (int k = 0; k < rings; ++k)
				{
					float phase = 0.28f * k;
					for (int c = 0; c < chords; ++c)
					{
						float a0 = (c / float(chords)) * TAU + phase;
						float a1 = a0 + (0.6f * TAU / chords);

						Vec3 A0s{ xCenter, r0 * std::cos(a0), r0 * std::sin(a0) };
						Vec3 B0s{ xCenter, r0 * std::cos(a1), r0 * std::sin(a1) };
						Vec3 A1e{ xCenter, r1 * std::cos(a0), r1 * std::sin(a0) };
						Vec3 B1e{ xCenter, r1 * std::cos(a1), r1 * std::sin(a1) };

						addL(applyPoseLocal(P0, A0s), applyPoseLocal(P0, B0s),
							applyPoseLocal(P1, A1e), applyPoseLocal(P1, B1e),
							sonar, sonar, tThin * 0.45f, tThin * 0.30f, 18);
					}
				}
			}
		}




		void init_cosmic_carousel()
		{
			lines.clear();

			// ====================================================================
			// SECTION 1: ARTISTIC PARAMETERS & CONFIGURATION ("The Director's Chair")
			// ====================================================================

			// --- Scene Composition ---
			const int   num_flowers = 12;            // The "horses" on the carousel
			const int   num_disk_spokes = 80;        // Resolution of the canopy/floor
			const int   num_pillar_segments = 100;   // Resolution of the central helix
			const int   num_sparks = 800;            // Number of particles in the firework burst

			// --- Animation & Physics ---
			const float carousel_rotation_speed = 1.5f; // Radians the main disks will rotate
			const float pillar_twist_factor = 3.0f;     // How many twists in the central helix
			const float flower_bob_height = 0.15f;    // How high the flowers bob up and down

			// --- Color Palette ---
			// We'll use a procedural rainbow palette for maximum vividness.
			const float TAU = 6.28318530718f;
			const float PI = 3.14159265359f;
			auto hue_color = [&](float h) -> Vec3 { // h from 0 to 1
				return {
					0.5f + 0.5f * cos(TAU * (h + 0.00f)),
					0.5f + 0.5f * cos(TAU * (h + 0.33f)),
					0.5f + 0.5f * cos(TAU * (h + 0.66f))
				};
				};

			// --- General Styling ---
			const float base_thickness = 0.008f;

			// ====================================================================
			// SECTION 2: THE CAROUSEL STRUCTURE (Pillar & Disks)
			// This creates the main rotating frame of the effect.
			// ====================================================================
			const float carousel_height = 1.5f;
			const float carousel_radius = 1.0f;

			// --- Part 2a: The Central Pillar ---
			for (int i = 0; i < num_pillar_segments; ++i) {
				float t0 = (float)i / num_pillar_segments;
				float t1 = (float)(i + 1) / num_pillar_segments;
				float angle0 = t0 * TAU * pillar_twist_factor;
				float angle1 = t1 * TAU * pillar_twist_factor;
				float pillar_radius = 0.05f;

				Vec3 p0_end = { pillar_radius * cos(angle0), -carousel_height / 2.f + t0 * carousel_height, pillar_radius * sin(angle0) };
				Vec3 p1_end = { pillar_radius * cos(angle1), -carousel_height / 2.f + t1 * carousel_height, pillar_radius * sin(angle1) };

				Line& l = add_line();
				// ANIMATION: Grow upwards from the base
				l.x0.start = 0; l.y0.start = -carousel_height / 2.f; l.z0.start = 0;
				l.x1.start = 0; l.y1.start = -carousel_height / 2.f; l.z1.start = 0;
				l.x0.end = p0_end.x; l.y0.end = p0_end.y; l.z0.end = p0_end.z;
				l.x1.end = p1_end.x; l.y1.end = p1_end.y; l.z1.end = p1_end.z;
				l.rgb_t0 = hue_color(t0); l.rgb_t1 = hue_color(t1);
				l.thickness.start = base_thickness * 0.5f; l.thickness.end = base_thickness;
				l.number_of_cubes = 8;
			}

			// --- Part 2b: The Canopy and Floor Disks ---
			for (int disk = 0; disk < 2; ++disk) { // 0=Floor, 1=Canopy
				float y = (disk == 0) ? -carousel_height / 2.0f : carousel_height / 2.0f;
				float rotation_dir = (disk == 0) ? 1.0f : -1.0f;
				for (int i = 0; i < num_disk_spokes; ++i) {
					float angle = (float)i / num_disk_spokes * TAU;
					Line& spoke = add_line();
					// ANIMATION: Expand outwards from the center and rotate
					spoke.x0.start = 0; spoke.y0.start = y; spoke.z0.start = 0;
					spoke.x1.start = 0; spoke.y1.start = y; spoke.z1.start = 0;
					spoke.x0.end = 0; spoke.y0.end = y; spoke.z0.end = 0;
					spoke.x1.end = carousel_radius * cos(angle + carousel_rotation_speed * rotation_dir);
					spoke.y1.end = y;
					spoke.z1.end = carousel_radius * sin(angle + carousel_rotation_speed * rotation_dir);
					spoke.rgb_t0 = hue_color((float)i / num_disk_spokes);
					spoke.rgb_t1 = hue_color(fmod((float)i / num_disk_spokes + 0.1f, 1.0f));
					spoke.thickness.start = base_thickness * 0.3f; spoke.thickness.end = base_thickness * 0.6f;
					spoke.number_of_cubes = 20;
				}
			}

			// ====================================================================
			// SECTION 3: THE LIVING "FLOWERS" AND THEIR POLES
			// Procedurally generated lifeforms that grow, bob, and orbit.
			// ====================================================================
			for (int i = 0; i < num_flowers; ++i) {
				float base_angle = (float)i / num_flowers * TAU;
				float flower_radius = carousel_radius * 0.85f;

				// --- Part 3a: The Pole ---
				float y_bob_start = 0;
				float y_bob_end = flower_bob_height * sin(base_angle * 3.f); // Bobbing motion

				Vec3 pole_bottom_start = { flower_radius * cos(base_angle), -carousel_height / 2.f, flower_radius * sin(base_angle) };
				Vec3 pole_top_start = { flower_radius * cos(base_angle), -carousel_height / 2.f + y_bob_start, flower_radius * sin(base_angle) };

				Vec3 pole_bottom_end = { flower_radius * cos(base_angle + carousel_rotation_speed), -carousel_height / 2.f, flower_radius * sin(base_angle + carousel_rotation_speed) };
				Vec3 pole_top_end = { flower_radius * cos(base_angle + carousel_rotation_speed), -carousel_height / 2.f + y_bob_end, flower_radius * sin(base_angle + carousel_rotation_speed) };

				Line& pole = add_line();
				// ANIMATION: Pole starts as zero-length and extends upwards while orbiting
				pole.x0.start = pole_bottom_start.x; pole.y0.start = pole_bottom_start.y; pole.z0.start = pole_bottom_start.z;
				pole.x1.start = pole_bottom_start.x; pole.y1.start = pole_bottom_start.y; pole.z1.start = pole_bottom_start.z;
				pole.x0.end = pole_bottom_end.x; pole.y0.end = pole_bottom_end.y; pole.z0.end = pole_bottom_end.z;
				pole.x1.end = pole_top_end.x; pole.y1.end = pole_top_end.y; pole.z1.end = pole_top_end.z;
				pole.rgb_t0 = hue_color(0.5f); pole.rgb_t1 = hue_color(0.6f);
				pole.thickness.start = base_thickness * 0.7f; pole.thickness.end = base_thickness * 0.7f;
				pole.number_of_cubes = 15;

				// --- Part 3b: The Flower (Recursive Growth) ---
				std::function<void(Vec3, Vec3, int)> grow_flower =
					[&](Vec3 origin, Vec3 dir, int depth) {
					if (depth <= 0) return;
					float len = 0.2f * pow(0.7f, 4 - depth);
					Vec3 end_point = vec_add(origin, vec_scale(dir, len));

					Line& petal = add_line();
					// ANIMATION: Each petal grows from its origin point and orbits/bobs with the whole flower
					petal.x0.start = pole_top_start.x; petal.y0.start = pole_top_start.y; petal.z0.start = pole_top_start.z;
					petal.x1.start = pole_top_start.x; petal.y1.start = pole_top_start.y; petal.z1.start = pole_top_start.z;

					// The final "breathing" state is a slight variation on the grown state
					Vec3 end_point_breathed = vec_add(origin, vec_scale(dir, len * 1.1f));

					petal.x0.end = origin.x; petal.y0.end = origin.y; petal.z0.end = origin.z;
					petal.x1.end = end_point_breathed.x; petal.y1.end = end_point_breathed.y; petal.z1.end = end_point_breathed.z;

					float color_mix = (4.0f - depth) / 4.0f;
					petal.rgb_t0 = hue_color(base_angle / TAU + color_mix * 0.1f);
					petal.rgb_t1 = hue_color(base_angle / TAU + color_mix * 0.1f + 0.05f);
					petal.thickness.start = base_thickness * (1.f - color_mix); petal.thickness.end = petal.thickness.start * 0.8f;
					petal.number_of_cubes = 8;

					// Branch into new petals
					float angle = PI / 2.5f; // 72 degrees for a 5-petal feel
					Vec3 dir_left = { dir.x * cos(angle) - dir.y * sin(angle), dir.x * sin(angle) + dir.y * cos(angle), dir.z };
					Vec3 dir_right = { dir.x, dir.y * cos(angle) - dir.z * sin(angle), dir.y * sin(angle) + dir.z * cos(angle) };
					grow_flower(end_point, vec_normalize(dir_left), depth - 1);
					grow_flower(end_point, vec_normalize(dir_right), depth - 1);
					};
				// Start the growth for this flower
				grow_flower(pole_top_end, { 0,1,0 }, 4);
			}

			// ====================================================================
			// SECTION 4: THE JOYFUL SPARKS (Atmosphere)
			// An explosive burst of colorful particles from the center.
			// ====================================================================
			for (int i = 0; i < num_sparks; ++i) {
				Line& spark = add_line();
				// Generate a random direction
				float theta = Random::generate_random_float_0_to_1() * TAU;
				float phi = acos(2.f * Random::generate_random_float_0_to_1() - 1.f);
				Vec3 dir = { sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta) };
				float dist = 0.5f + Random::generate_random_float_0_to_1() * 1.5f;
				Vec3 p1 = vec_scale(dir, dist);

				// ANIMATION: Explode from the center and fade away
				spark.x0.start = 0; spark.y0.start = 0; spark.z0.start = 0;
				spark.x1.start = 0; spark.y1.start = 0; spark.z1.start = 0;
				spark.x0.end = p1.x; spark.y0.end = p1.y; spark.z0.end = p1.z;
				spark.x1.end = p1.x; spark.y1.end = p1.y; spark.z1.end = p1.z;

				spark.rgb_t0 = hue_color(Random::generate_random_float_0_to_1());
				spark.rgb_t1 = vec_scale(spark.rgb_t0, 0.2f); // Fade to dark
				spark.thickness.start = base_thickness * 1.5f; // Start bright and thick
				spark.thickness.end = 0.0f; // End invisible
				spark.number_of_cubes = 15;
			}
		}


		void init_neural_aurora()
		{
			const float M_PI = 3.14159265359f;
			static float time_base = 0.0f;

			// =======================
			// Master "feel" controls
			// =======================
			const float GLOBAL_SPEED = 1.0f;     // overall animation speed
			const float BPM = 84.0f;    // musical feel (for pulses/bursts)
			const float ROTATE_SPEED = 0.15f;    // slow network yaw
			const float VORTEX_TWIST = 0.7f;     // swirl strength
			const float BREATH_DEPTH = 0.12f;    // radius breathing amplitude
			const float REWIRE_SECONDS = 8.0f;     // how often connections reshuffle
			const int   MAX_SPARKS = 32;       // small electrical arcs per frame

			// ================
			// Base parameters
			// ================
			int neuron_layers = 7;
			int neurons_per_layer = 22;
			float network_radius = 0.62f;
			float layer_spacing = 0.13f;

			// Aurora ribbons
			int   tendril_count_base = 10;
			float tendril_length = 1.45f;

			// Particles
			int particle_count = 40;

			// ======================
			// Handy helper lambdas
			// ======================
			auto fract = [](float x) { return x - std::floor(x); };
			auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
			auto smooth = [&](float x) { x = std::clamp(x, 0.0f, 1.0f); return x * x * (3.0f - 2.0f * x); };
			auto hash11 = [&](float p) { return fract(std::sin(p * 127.1f) * 43758.5453f); };
			auto hash31 = [&](int i) {
				// 3 independent hashes in [0,1)
				return std::tuple<float, float, float>{
					hash11(i * 0.1234f + 1.2345f),
						hash11(i * 0.3456f + 2.3456f),
						hash11(i * 0.5678f + 3.4567f)
				};
				};
			auto rotateY = [&](float& x, float& z, float a) {
				float cx = std::cos(a), sx = std::sin(a);
				float nx = cx * x - sx * z;
				float nz = sx * x + cx * z;
				x = nx; z = nz;
				};

			// Time & beats
			float t = time_base;
			float beatPhase = 2.0f * M_PI * (BPM / 60.0f) * t;
			float kick = std::max(0.0f, std::sin(beatPhase));   // 0..1
			kick *= kick;                                            // sharper peak
			float yaw = ROTATE_SPEED * t;

			// Global LFOs
			float breath = 1.0f + BREATH_DEPTH * std::sin(t * 0.7f);
			float wobbleA = 0.08f * std::sin(t * 0.9f);
			float wobbleB = 0.06f * std::cos(t * 1.3f);
			float modeF = std::fmod(t / REWIRE_SECONDS, 1.0f);   // 0..1 within rewire cycle
			int   mode = int(std::floor(t / REWIRE_SECONDS)) % 3;

			// Slightly vary counts with mode
			int tendril_count = tendril_count_base + (mode == 1 ? 4 : (mode == 2 ? -2 : 0));
			tendril_count = std::max(6, tendril_count);

			// =======================
			// Generate neuron shells
			// =======================
			struct N {
				float x, y, z, e; // position + energy
			};
			std::vector<N> neurons;
			neurons.reserve(neuron_layers * neurons_per_layer);

			for (int layer = 0; layer < neuron_layers; ++layer) {
				float lr01 = neuron_layers <= 1 ? 0.0f : layer / float(neuron_layers - 1);
				float layer_radius = (network_radius * breath) * (0.28f + 0.76f * lr01);
				float layer_height = -0.36f + layer * layer_spacing;

				for (int n = 0; n < neurons_per_layer; ++n) {
					float a = n * (2.0f * M_PI / neurons_per_layer);
					float jig = 0.06f * std::sin(3.0f * a + 1.7f * t + layer * 0.7f);
					float x = (layer_radius + wobbleA) * std::cos(a) + jig * std::cos(2.7f * a);
					float y = layer_height + 0.06f * std::sin(2.0f * a + 0.8f * layer + t * 0.6f) + wobbleB;
					float z = (layer_radius + wobbleB) * std::sin(a) + jig * std::sin(2.7f * a);

					// Vortex twist (radius-dependent)
					float r = std::sqrt(x * x + z * z) + 1e-4f;
					float twist = VORTEX_TWIST * (0.35f + 0.65f * std::sin(0.8f * t + r * 3.0f));
					float x2 = x, z2 = z; rotateY(x2, z2, yaw + twist * (0.2f + 0.6f * lr01));

					// Energy: mix of layer + index + beat
					float e = 0.55f + 0.45f * std::sin(1.6f * t + layer * 1.1f + n * 0.63f) * (0.6f + 0.4f * kick);

					neurons.push_back({ x2, y, z2, e });
				}
			}

			// ==========================================
			// Dynamic connections (rewire every cycle)
			// ==========================================
			// Rewiring offset changes each REWIRE_SECONDS using a stable hash per mode
			int rewire_seed = mode * 9176 + int(std::floor(t / REWIRE_SECONDS));
			int offset_base = 1 + (int(std::floor(hash11(float(rewire_seed)) * 7.0f)) * 2 + 1); // odd 1..13

			for (int i = 0; i < (int)neurons.size(); ++i) {
				auto [x1, y1, z1, e1] = neurons[i];

				// Variable connection count and stride across modes
				int connections_per = (mode == 0 ? 4 : (mode == 1 ? 6 : 3));
				for (int c = 0; c < connections_per; ++c) {
					int stride = offset_base + c; // spread
					int j = (i + stride) % (int)neurons.size();
					if (j == i) continue;

					auto [x2, y2, z2, e2] = neurons[j];
					float dx = x2 - x1, dy = y2 - y1, dz = z2 - z1;
					float d = std::sqrt(dx * dx + dy * dy + dz * dz);
					if (d > 0.001f && d < 0.55f) {
						Line& L = add_line();

						// Base line endpoints
						L.x0.start = x1; L.y0.start = y1; L.z0.start = z1;
						L.x1.start = x2; L.y1.start = y2; L.z1.start = z2;

						// Traveling pulse along the link
						float pulse_u = fract(0.15f * t + 0.11f * i + 0.07f * c);
						float pw = 0.06f * std::sin(6.28318f * pulse_u);
						L.x1.end = x2 + pw * dx;
						L.y1.end = y2 + pw * dy * 0.4f;
						L.z1.end = z2 + pw * dz;

						// Energy-biased thickness with end variation (shimmer)
						float strength = (1.0f - d) * 0.7f + 0.3f * ((e1 + e2) * 0.5f);
						float baseTh = 0.0035f + 0.017f * strength;
						L.thickness.start = baseTh;
						L.thickness.end = baseTh * (0.8f + 0.4f * std::sin(t * 3.0f + i * 0.37f + c));

						// Color: slow hue drift + altitude tint
						float hue = 1.3f * t + 0.55f * (y1 + y2) + 0.23f * (i + j);
						float sat = 0.65f + 0.35f * std::sin(1.2f * t + i * 0.11f);
						L.rgb_t0.x = 0.12f + 0.42f * std::sin(hue) * sat;
						L.rgb_t0.y = 0.28f + 0.62f * std::sin(hue + 1.1f) * sat;
						L.rgb_t0.z = 0.78f + 0.20f * std::sin(hue + 2.2f) * sat;

						// Segmenting helps “curvy” look even on straight lines
						L.number_of_cubes = 10 + int(12.0f * strength);
					}
				}

				// ===================
				// Neuron core glyphs
				// ===================
				Line& P = add_line();
				P.x0.start = x1; P.y0.start = y1; P.z0.start = z1;
				P.x1.start = x1 + 0.0012f; P.y1.start = y1; P.z1.start = z1;

				// Burstier core with beat accent and occasional spikes
				float spike = 0.0f;
				{
					float pr = hash11(i + std::floor(t * 0.5f));
					if (pr > 0.92f) spike = 0.02f * (pr - 0.92f) / 0.08f; // short rare spikes
				}
				float coreEnergy = e1 * (0.8f + 0.6f * kick) + spike;
				P.thickness.start = 0.018f * coreEnergy;
				P.thickness.end = P.thickness.start * (0.7f + 0.3f * std::sin(4.0f * t + i * 0.7f));
				P.number_of_cubes = 6;

				// Hot aurora-blue core
				P.rgb_t0.x = 0.85f + 0.15f * std::sin(2.0f * t + i * 0.37f);
				P.rgb_t0.y = 0.72f + 0.28f * std::cos(1.5f * t + i * 0.21f);
				P.rgb_t0.z = 1.00f;

				// Gentle quiver
				float np = 0.012f * std::sin(4.0f * t + i * 0.7f);
				P.x1.end = x1 + 0.0012f + np;
				P.y1.end = y1 + 0.5f * np;
			}

			// ==========================
			// Curling aurora “ribbons”
			// ==========================
			{
				for (int k = 0; k < tendril_count; ++k) {
					float base_angle = k * (2.0f * M_PI / tendril_count);
					float phase = 0.85f * t + 1.37f * k;
					int   segments = 28;
					for (int s = 0; s < segments; ++s) {
						float u = s / float(segments - 1);
						float u1 = (s - 1) / float(segments - 1);

						// Wavy sheet with precession
						float wX = 0.12f * std::sin(phase + 0.55f * s);
						float wY = 0.10f * std::cos(1.27f * phase + 0.67f * s);
						float wZ = 0.14f * std::sin(0.93f * phase + 0.61f * s);

						float rad = (network_radius * breath + tendril_length * u);
						float x = rad * std::cos(base_angle + 0.18f * std::sin(0.5f * t)) + wX;
						float y = lerp(-0.28f, 0.58f, u) + wY;
						float z = rad * std::sin(base_angle + 0.18f * std::sin(0.5f * t)) + wZ;
						float xPrev = 0, yPrev = 0, zPrev = 0;

						if (s > 0) {
							float radP = (network_radius * breath + tendril_length * u1);
							float wX1 = 0.12f * std::sin(phase + 0.55f * (s - 1));
							float wY1 = 0.10f * std::cos(1.27f * phase + 0.67f * (s - 1));
							float wZ1 = 0.14f * std::sin(0.93f * phase + 0.61f * (s - 1));

							xPrev = radP * std::cos(base_angle + 0.18f * std::sin(0.5f * t)) + wX1;
							yPrev = lerp(-0.28f, 0.58f, u1) + wY1;
							zPrev = radP * std::sin(base_angle + 0.18f * std::sin(0.5f * t)) + wZ1;

							Line& A = add_line();
							A.x0.start = xPrev; A.y0.start = yPrev; A.z0.start = zPrev;
							A.x1.start = x;     A.y1.start = y;     A.z1.start = z;

							float hue = 0.33f * t + 2.0f * u + 0.7f * k;
							A.rgb_t0.x = 0.10f + 0.30f * std::sin(hue);
							A.rgb_t0.y = 0.62f + 0.33f * std::sin(hue + 1.0f);
							A.rgb_t0.z = 0.82f + 0.18f * std::sin(hue + 2.0f);

							float taper = 1.0f - 0.7f * u;
							A.thickness.start = 0.008f * taper * (0.8f + 0.2f * kick);
							A.thickness.end = A.thickness.start * (0.85f + 0.25f * std::sin(3.0f * t + s * 0.3f));
							A.number_of_cubes = 12;

							// Gentle forward drift = sheet “flow”
							float drift = 0.015f * std::sin(phase + s * 0.25f);
							A.x1.end = x + drift * (x - xPrev);
							A.y1.end = y + 0.4f * drift * (y - yPrev);
							A.z1.end = z + drift * (z - zPrev);
						}
					}
				}
			}

			// ===========================
			// Orbiting energy particles
			// ===========================
			{
				for (int p = 0; p < particle_count; ++p) {
					float life = std::fmod(0.37f * t + 0.11f * p, 1.0f);
					if (life < 0.86f) {
						float ang0 = p * (2.0f * M_PI / particle_count) + 1.7f * t;
						float rad0 = 0.28f + 0.52f * life;
						float ht = -0.42f + 1.18f * life;

						float px = rad0 * std::cos(ang0) + 0.06f * std::cos(2.0f * ang0 + t * 1.1f);
						float py = ht + 0.05f * std::sin(ang0 + 0.7f * t);
						float pz = rad0 * std::sin(ang0) + 0.06f * std::sin(2.0f * ang0 + t * 1.1f);

						Line& L = add_line();
						L.x0.start = px;           L.y0.start = py;        L.z0.start = pz;
						L.x1.start = px + 0.002f;  L.y1.start = py;         L.z1.start = pz;

						float hue = 3.0f * life + 0.22f * p;
						L.rgb_t0.x = 0.80f + 0.20f * std::sin(hue);
						L.rgb_t0.y = 0.90f + 0.10f * std::sin(hue + 1.0f);
						L.rgb_t0.z = 1.00f;

						float th = 0.015f * (0.25f + 0.75f * life);
						L.thickness.start = th;
						L.thickness.end = th * (0.8f + 0.4f * std::sin(5.0f * t + p));
						L.number_of_cubes = 4;

						// forward step
						float life2 = life + 0.06f;
						if (life2 < 0.86f) {
							float ang1 = p * (2.0f * M_PI / particle_count) + 1.7f * (t + 0.16f);
							float rad1 = 0.28f + 0.52f * life2;
							float ht1 = -0.42f + 1.18f * life2;
							float nx = rad1 * std::cos(ang1);
							float ny = ht1;
							float nz = rad1 * std::sin(ang1);
							L.x1.end = nx + 0.002f;
							L.y1.end = ny;
							L.z1.end = nz;
						}
					}
				}
			}

			// ==========================
			// Background energy “field”
			// ==========================
			{
				int field_lines = 14;
				for (int i = 0; i < field_lines; ++i) {
					float a = i * (2.0f * M_PI / field_lines);
					float ph = 0.52f * t + 0.41f * i;

					Line& F = add_line();
					F.x0.start = 1.6f * std::cos(a);
					F.y0.start = -0.55f;
					F.z0.start = 1.6f * std::sin(a);
					F.x1.start = 0.85f * std::cos(a + 0.28f);
					F.y1.start = 0.52f;
					F.z1.start = 0.85f * std::sin(a + 0.28f);

					float hue = 0.22f * t + 0.51f * i;
					F.rgb_t0.x = 0.05f + 0.10f * std::sin(hue);
					F.rgb_t0.y = 0.10f + 0.10f * std::sin(hue + 1.0f);
					F.rgb_t0.z = 0.16f + 0.10f * std::sin(hue + 2.0f);

					F.thickness.start = 0.0022f;
					F.thickness.end = 0.0018f + 0.0009f * std::sin(3.3f * t + i);
					F.number_of_cubes = 9;

					float sway = 0.06f * std::sin(ph);
					F.x1.end = 0.85f * std::cos(a + 0.28f + sway);
					F.y1.end = 0.52f + 0.35f * sway;
					F.z1.end = 0.85f * std::sin(a + 0.28f + sway);
				}
			}

			// ==========================
			// Electric micro-sparks
			// ==========================
			{
				int count = MAX_SPARKS;
				for (int s = 0; s < count; ++s) {
					// Deterministic pseudo-random choice of neuron
					int ni = (s * 37 + int(std::floor(t * 60.0f))) % std::max(1, (int)neurons.size());
					auto [x, y, z, e] = neurons[ni];

					float life = fract(t * 4.0f + s * 0.37f);
					if (life < 0.25f) { // very short
						// Small jittery arc in a random direction
						auto [rx, ry, rz] = hash31(ni + s * 19);
						float len = 0.06f + 0.08f * rx;
						float ax = (rx - 0.5f), ay = (ry - 0.5f), az = (rz - 0.5f);
						float inv = 1.0f / std::sqrt(ax * ax + ay * ay + az * az + 1e-4f);
						ax *= inv; ay *= inv; az *= inv;

						Line& S = add_line();
						S.x0.start = x;        S.y0.start = y;        S.z0.start = z;
						S.x1.start = x + ax * len * 0.4f; S.y1.start = y + ay * len * 0.4f; S.z1.start = z + az * len * 0.4f;

						S.rgb_t0.x = 0.9f; S.rgb_t0.y = 0.95f; S.rgb_t0.z = 1.0f;
						S.thickness.start = 0.0045f + 0.004f * (0.5f + 0.5f * std::sin(20.0f * life));
						S.thickness.end = 0.0035f;
						S.number_of_cubes = 3;

						// snap to a slightly different end to feel “crackly”
						float jitter = (0.4f + 0.6f * life);
						S.x1.end = x + ax * len * jitter;
						S.y1.end = y + ay * len * jitter;
						S.z1.end = z + az * len * jitter;
					}
				}
			}

			// Advance time
			time_base += 0.016f * GLOBAL_SPEED;
		}



		// Add this new, self-contained function inside your 'struct Lines'

		
		
		
		void init_fractal_flow()
		{
			const float M_PI = 3.14159265359f;
			static float time_base = 0.0f;

			// ======= Feel / knobs =======
			const int   BASE_TRUNKS = 5;     // roots around a ring
			const int   MAX_LEVELS = 7;     // total recursion depth ( 5*(2^7 -1) lines)
			const float BASE_LEN = 0.48f; // trunk length
			const float SCALE = 0.66f; // child length factor per level
			const float SPLIT_ANGLE = 0.62f; // base branching angle (radians)
			const float TWIST_AROUND = 0.45f; // twist of the split plane around the branch
			const float WIND_SWAY = 0.22f; // wind sway amount
			const float WIND_SPEED = 0.7f;  // wind speed
			const float GROW_SPEED = 0.10f; // cycles levels revealing/hiding
			const float BREATH_DEPTH = 0.10f; // global breathing (length modulation)
			const float ROTATE_SPEED = 0.18f; // slow global rotation
			const float PULSE_SPEED = 0.35f; // speed of bright pulses on branches

			auto fract = [](float x) { return x - std::floor(x); };
			auto clampf = [](float x, float a, float b) { return std::max(a, std::min(b, x)); };
			auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
			auto smooth = [&](float x) { x = clampf(x, 0.0f, 1.0f); return x * x * (3.0f - 2.0f * x); };
			auto hash11 = [&](float p) { return fract(std::sin(p * 127.1f) * 43758.5453f); };

			// vec helpers
			auto dot3 = [](float ax, float ay, float az, float bx, float by, float bz) { return ax * bx + ay * by + az * bz; };
			auto cross3 = [](float ax, float ay, float az, float bx, float by, float bz) {
				return std::tuple<float, float, float>(ay * bz - az * by, az * bx - ax * bz, ax * by - ay * bx);
				};
			auto norm3 = [&](float& x, float& y, float& z) {
				float L = std::sqrt(x * x + y * y + z * z) + 1e-6f; x /= L; y /= L; z /= L;
				};
			auto rodrigues = [&](float vx, float vy, float vz, float nx, float ny, float nz, float ang) {
				float c = std::cos(ang), s = std::sin(ang), d = dot3(nx, ny, nz, vx, vy, vz);
				auto [cx, cy, cz] = cross3(nx, ny, nz, vx, vy, vz);
				float rx = vx * c + cx * s + nx * d * (1.0f - c);
				float ry = vy * c + cy * s + ny * d * (1.0f - c);
				float rz = vz * c + cz * s + nz * d * (1.0f - c);
				return std::tuple<float, float, float>(rx, ry, rz);
				};
			auto rotateY = [&](float& x, float& z, float a) {
				float c = std::cos(a), s = std::sin(a); float nx = c * x - s * z, nz = s * x + c * z; x = nx; z = nz;
				};

			// time & global motion
			float t = time_base;
			float yaw = ROTATE_SPEED * t;
			float breath = 1.0f + BREATH_DEPTH * std::sin(t * 0.8f);

			// triangular growth wave 0..1..0..1..
			float gphase = fract(t * GROW_SPEED);
			float grow = 1.0f - std::fabs(2.0f * gphase - 1.0f);       // triangle 0..1..0
			float levelCut = grow * (MAX_LEVELS + 0.999f);             // which level is partial
			int   Lfull = (int)std::floor(levelCut);                // fully grown levels
			float Lalpha = smooth(fract(levelCut));                  // partial progress of next

			struct Node { float x, y, z; float dx, dy, dz; int level; };
			std::vector<Node> curr, next;
			curr.reserve(BASE_TRUNKS);

			// place roots in a ring, directions tilted upwards
			float r0 = 0.12f;
			for (int i = 0; i < BASE_TRUNKS; ++i) {
				float a = i * (2.0f * M_PI / BASE_TRUNKS) + yaw * 0.6f;
				float x = r0 * std::cos(a), z = r0 * std::sin(a), y = -0.55f;
				float dx = 0.35f * std::cos(a), dy = 1.0f, dz = 0.35f * std::sin(a);
				norm3(dx, dy, dz);
				curr.push_back({ x,y,z,dx,dy,dz,0 });
			}

			int globalSegId = 0;

			for (int level = 0; level < MAX_LEVELS; ++level) {
				float len = BASE_LEN * std::pow(SCALE, (float)level) * breath;
				float thick = 0.020f * std::pow(0.78f, (float)level);  // base thickness per level
				float hueLv = 0.5f + 0.2f * std::sin(0.7f * t + 1.3f * level);

				// wind sway increases with height/level
				float windPhase = t * WIND_SPEED + level * 0.7f;
				float windYaw = WIND_SWAY * 0.25f * std::sin(windPhase);
				float windBend = WIND_SWAY * 0.35f * std::sin(windPhase * 1.3f);

				for (const Node& n : curr)
				{
					// progress on this level (full or partial or skip)
					float segAlpha = 1.0f;
					if (level > Lfull) { continue; }
					if (level == Lfull) { segAlpha = Lalpha; if (segAlpha <= 0.001f) continue; }

					// start/end positions
					float sx = n.x, sy = n.y, sz = n.z;
					float dx = n.dx, dy = n.dy, dz = n.dz;

					// wind: yaw around Y and bend around a stable axis
					{
						// yaw sway
						float tx = dx, tz = dz; rotateY(tx, tz, windYaw); dx = tx; dz = tz; norm3(dx, dy, dz);
						// bend sway (around axis perpendicular to direction & Y-up)
						float ax, ay, az;
						{
							auto [cx, cy, cz] = cross3(dx, dy, dz, 0.0f, 1.0f, 0.0f);
							float m = std::sqrt(cx * cx + cy * cy + cz * cz) + 1e-6f; ax = cx / m; ay = cy / m; az = cz / m;
						}
						auto [bdx, bdy, bdz] = rodrigues(dx, dy, dz, ax, ay, az, windBend);
						dx = bdx; dy = bdy; dz = bdz; norm3(dx, dy, dz);
					}

					float ex = sx + dx * (len * segAlpha);
					float ey = sy + dy * (len * segAlpha);
					float ez = sz + dz * (len * segAlpha);

					// draw the branch segment
					{
						Line& L = add_line();
						L.x0.start = sx; L.y0.start = sy; L.z0.start = sz;
						L.x1.start = ex; L.y1.start = ey; L.z1.start = ez;

						// gentle flow along the branch
						float flow = 0.015f * std::sin(3.0f * t + globalSegId * 0.23f);
						L.x1.end = ex + flow * (ex - sx);
						L.y1.end = ey + 0.4f * flow * (ey - sy);
						L.z1.end = ez + flow * (ez - sz);

						// aurora-ish palette: level hue + altitude tint + time drift
						float hue = 0.9f * t + hueLv + 0.35f * (sy + ey);
						float sat = 0.65f + 0.35f * std::sin(1.7f * t + 0.11f * globalSegId);
						L.rgb_t0.x = 0.10f + 0.38f * std::sin(hue) * sat;
						L.rgb_t0.y = 0.28f + 0.62f * std::sin(hue + 1.05f) * sat;
						L.rgb_t0.z = 0.82f + 0.18f * std::sin(hue + 2.10f) * sat;

						// thickness breath + slight taper with motion
						float th = thick * (0.85f + 0.30f * std::sin(2.5f * t + level));
						L.thickness.start = th;
						L.thickness.end = th * (0.8f + 0.4f * std::sin(4.1f * t + globalSegId * 0.17f));
						L.number_of_cubes = 8 + (int)(10.0f * len);

						++globalSegId;
					}

					// bright pulse racing along this branch (sparse)
					{
						float r = hash11(globalSegId * 0.173f + level * 2.7f);
						if (r > 0.55f) {
							float u = fract(PULSE_SPEED * t + r * 13.0f);
							float v = clampf(u * segAlpha, 0.0f, segAlpha);
							float px = sx + dx * len * v, py = sy + dy * len * v, pz = sz + dz * len * v;
							float q = 0.035f + 0.045f * std::sin(6.28318f * u);

							Line& P = add_line();
							P.x0.start = px; P.y0.start = py; P.z0.start = pz;
							P.x1.start = px + dx * q; P.y1.start = py + dy * q; P.z1.start = pz + dz * q;
							P.rgb_t0.x = 0.92f; P.rgb_t0.y = 0.96f; P.rgb_t0.z = 1.0f;
							P.thickness.start = 0.006f + 0.008f * std::sin(12.0f * u);
							P.thickness.end = 0.004f;
							P.number_of_cubes = 3;
						}
					}

					// spawn children from the end (only if theres room to grow)
					if (level + 1 < MAX_LEVELS)
					{
						// make a stable split plane using hashed axis per segment
						float h = hash11((float)globalSegId * 0.731f);
						float axS = std::sin(h * 23.0f), ayS = std::cos(h * 19.0f), azS = std::sin(h * 17.0f);
						// axis perpendicular to current direction
						auto [ax0, ay0, az0] = cross3(dx, dy, dz, axS, ayS, azS);
						float Lm = std::sqrt(ax0 * ax0 + ay0 * ay0 + az0 * az0) + 1e-6f;
						float ax = ax0 / Lm, ay = ay0 / Lm, az = az0 / Lm;

						// twist the split plane around the branch
						float twist = TWIST_AROUND * (h * 2.0f - 1.0f) + 0.6f * std::sin(0.7f * t + level);
						auto [ax1, ay1, az1] = rodrigues(ax, ay, az, dx, dy, dz, twist);

						// final split angle (breathe a bit)
						float ang = SPLIT_ANGLE * (0.85f + 0.25f * std::sin(0.6f * t + level));

						// two children, symmetric
						auto [lx, ly, lz] = rodrigues(dx, dy, dz, ax1, ay1, az1, +ang);
						auto [rx, ry, rz] = rodrigues(dx, dy, dz, ax1, ay1, az1, -ang);
						norm3(lx, ly, lz); norm3(rx, ry, rz);

						next.push_back({ ex,ey,ez, lx,ly,lz, level + 1 });
						next.push_back({ ex,ey,ez, rx,ry,rz, level + 1 });
					}
				}

				curr.swap(next);
				next.clear();
			}

			// Optional: faint fractal halo rings that rotate and bind the shape
			{
				int rings = 3;
				for (int r = 0; r < rings; ++r) {
					float rad = 0.35f + 0.22f * r;
					float ayaw = yaw * (0.6f + 0.2f * r) + 0.4f * std::sin(t * 0.5f + r);
					int   segs = 64;
					float y = -0.45f + 0.45f * r;
					for (int i = 1; i < segs; ++i) {
						float a0 = (i - 1) * (2.0f * M_PI / segs) + ayaw;
						float a1 = (i) * (2.0f * M_PI / segs) + ayaw;
						Line& H = add_line();
						H.x0.start = rad * std::cos(a0); H.y0.start = y; H.z0.start = rad * std::sin(a0);
						H.x1.start = rad * std::cos(a1); H.y1.start = y; H.z1.start = rad * std::sin(a1);
						float hue = 0.35f * t + 1.3f * r + a0;
						H.rgb_t0.x = 0.08f + 0.26f * std::sin(hue);
						H.rgb_t0.y = 0.20f + 0.30f * std::sin(hue + 1.0f);
						H.rgb_t0.z = 0.32f + 0.30f * std::sin(hue + 2.0f);
						H.thickness.start = 0.0018f;
						H.thickness.end = 0.0018f;
						H.number_of_cubes = 6;
						// slight shear for motion
						float sh = 0.02f * std::sin(2.0f * t + i * 0.2f + r);
						H.x1.end = H.x1.start + sh * (H.x1.start - H.x0.start);
						H.y1.end = y + 0.3f * sh;
						H.z1.end = H.z1.start + sh * (H.z1.start - H.z0.start);
					}
				}
			}

			// advance time
			time_base += 0.016f;
		}



		
		
		









		// ------





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
				// lines.init_0013_sea_and_sky_zup();
				// lines.init_temporal_echo_cascade();
				// lines.init_2010_ocean_glyph_overture();
				// lines.init_solar_flare();
				// lines.init_2020_glass_of_water();
				// lines.init_calculus_morph();
				// lines.init_2012_lowpoly_city_world();
				// lines.init_2021_polygon_dance();
				// lines.init_quantum_fluid_pool();
				// lines.init_2030_airplane_wirefly();
				// lines.init_rainy_window_final_v2();
				// lines.init_spiral_torus_interference();
				// lines.init_2031_airplane_stabilized();
				// lines.init_living_coral();
				// lines.init_2032_submarine_stabilized();
				// lines.init_cosmic_carousel();
				// lines.init_neural_aurora();
				// lines.init_fractal_flow();
				



				

				

				

				

				






				
				

				
				
				// --
				

				
				

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