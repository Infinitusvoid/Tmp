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
		

		void init(int number)
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

		void init_orbital_bands(int number)
		{
			spheres.clear();
			spheres.reserve(number);

			auto RND = []() { return Random::generate_random_float_0_to_1(); };
			auto fract = [](float v) { return v - std::floor(v); };
			auto clamp01 = [](float v) { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); };

			// simple cosine palette (pleasant cycling)
			auto palette = [](float t) -> XYZ {
				const float TAU = 6.28318530718f;
				return {
					0.5f + 0.5f * std::cos(TAU * (t + 0.00f)),
					0.5f + 0.5f * std::cos(TAU * (t + 0.33f)),
					0.5f + 0.5f * std::cos(TAU * (t + 0.67f))
				};
				};

			const float TAU = 6.28318530718f;
			const float goldenAngle = 2.39996322973f; // radians
			const XYZ   center = { 0.5f, 0.5f, 0.5f }; // keep inside your [0..1] world

			for (int i = 0; i < number; ++i)
			{
				const float t = (i + 0.5f) / float(number);     // 0..1
				const float a = goldenAngle * i;                // spiral angle
				const float ro = 0.18f + 0.28f * t;              // orbit radius (stay within unit box)
				const float tilt = 0.22f * std::sin(a * 0.37f);  // slow precession in Y

				// start / end positions orbiting around center; end is rotated forward
				XYZ p0 = { center.x + ro * std::cos(a),
						   center.y + tilt,
						   center.z + ro * std::sin(a) };

				XYZ p1 = { center.x + ro * std::cos(a + goldenAngle * 0.75f),
						   center.y - tilt,
						   center.z + ro * std::sin(a + goldenAngle * 0.75f) };

				// color shift along the spiral
				XYZ c0 = palette(fract(t + 0.05f * std::sin(a * 0.2f)));
				XYZ c1 = palette(fract(t + 0.25f + 0.07f * std::cos(a * 0.17f)));

				// cloud (instance) scale breathes slightly over time
				const float s0 = 0.014f + 0.012f * (0.3f + 0.7f * RND());
				const float s1 = s0 * (0.88f + 0.24f * RND());

				// narrow azimuth arc (x-range) and latitude band (y-range)
				// x ~ azimuth (0..1), y ~ polar/latitude (0..1) as your shader interprets them
				// Make clean belts: small band width + small arc length
				float phi_norm = fract((a / TAU) + 0.13f * RND());
				float arc_len = 0.08f + 0.12f * RND();          // azimuth arc length
				float band_center = clamp01(0.10f + 0.80f * fract(i * 0.3183099f + 0.31f * RND()));
				float band_width = 0.035f + 0.055f * RND();        // latitude band thickness

				// keep [min,max] valid inside [0,1] (avoid wrap for safety)
				float x_min = phi_norm;
				float x_max = (phi_norm + arc_len <= 1.0f) ? (phi_norm + arc_len) : 1.0f;
				if (x_max - x_min < 0.02f) { x_min = std::max(0.0f, 1.0f - arc_len); x_max = 1.0f; }

				float y_min = clamp01(band_center - 0.5f * band_width);
				float y_max = clamp01(band_center + 0.5f * band_width);
				if (y_max - y_min < 0.02f) { y_min = clamp01(band_center - 0.02f); y_max = clamp01(band_center + 0.02f); }

				// shell thickness through the sphere & jitter evolution
				const float thickness = 0.010f + 0.025f * RND();   // thin shells
				const float jit_start = 1.10f + 0.25f * RND();     // noisy  condense
				const float jit_end = 0.25f + 0.15f * RND();

				// per-cube size (small, slightly shrinking)
				const float cube_s0 = 0.00055f + 0.00045f * RND();
				const float cube_s1 = cube_s0 * (0.85f + 0.10f * RND());

				Sphere S;
				S.start_position = p0;
				S.end_position = p1;

				S.start_color = c0;
				S.end_color = c1;

				S.radious = { s0, s1 };           // instance (cloud) scale
				S.cube_size = { cube_s0, cube_s1 };

				S.x_rnd_min = { x_min, x_min };
				S.x_rnd_max = { x_max, x_max };

				S.y_rnd_min = { y_min, y_min };
				S.y_rnd_max = { y_max, y_max };

				S.thickness = { thickness, thickness };

				S.jitter = { jit_start, jit_end };

				spheres.push_back(std::move(S));
			}
		}

		void init_twisted_loxodrome_ribbons(int number)
		{
			// spheres.clear();
			// spheres.reserve(number);

			auto RND = []() -> float { return Random::generate_random_float_0_to_1(); };
			auto fract01 = [](float v) -> float { return v - std::floor(v); };
			auto clamp01 = [](float v) -> float { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); };

			// Pleasant cycling palette (phase-shifted cosines)
			auto palette = [](float t) -> XYZ {
				const float TAU = 6.28318530718f;
				return {
					0.55f + 0.45f * std::cos(TAU * (t + 0.00f)),
					0.55f + 0.45f * std::cos(TAU * (t + 0.33f)),
					0.55f + 0.45f * std::cos(TAU * (t + 0.67f))
				};
				};

			const float TAU = 6.28318530718f;
			const float goldenAngle = 2.39996322973f;     // distributes well
			const XYZ   center = { 0.5f, 0.5f, 0.5f };

			// How many macro strands (visual ribbons) to interleave
			// (instances are assigned to strands round-robin)
			const int strands = 5 + int(3 * RND());       // 5..7 ribbons
			const float strand_phase = RND();

			for (int i = 0; i < number; ++i)
			{
				const int   s = i % strands;                     // strand index
				const float sf = float(s) / float(strands);       // 0..(1-1/strands)
				const float t = (i + 0.5f) / float(number);      // 0..1 along all instances
				const float a = goldenAngle * i;                 // base angle (good spread)

				// Handedness alternates per strand  counter-rotation feel
				const float handed = (s & 1) ? +1.0f : -1.0f;

				// Orbit radius (stay comfortably in your unit cube)
				const float ro0 = 0.16f + 0.30f * fract01(t + 0.37f * sf + 0.11f * RND());
				const float ro1 = ro0 * (0.95f + 0.20f * RND());  // tiny radial drift

				// Start/end positions sweep around center; end advances in angle,
				// and climbs/descends slightly to emphasize loxodrome motion.
				const float dphi = handed * (0.8f + 1.1f * RND()) * goldenAngle;
				const float ywig = 0.12f * std::sin(a * (0.25f + 0.15f * RND()) + sf * TAU);

				XYZ p0 = { center.x + ro0 * std::cos(a),
						   center.y + ywig + 0.04f * std::sin(3.0f * a + sf * TAU),
						   center.z + ro0 * std::sin(a) };

				XYZ p1 = { center.x + ro1 * std::cos(a + dphi),
						   center.y - ywig + 0.04f * std::cos(2.0f * a + sf * TAU),
						   center.z + ro1 * std::sin(a + dphi) };

				// Instance (cloud) radius: expand slightly  more presence as ribbons coalesce
				const float rad0 = 0.012f + 0.012f * (0.35f + 0.65f * RND());
				const float rad1 = rad0 * (1.25f + 0.25f * RND());

				// Cube size: start small, get even more precise at the end
				const float cube0 = 0.00040f + 0.00045f * RND();
				const float cube1 = cube0 * (0.80f + 0.10f * RND());

				// Each ribbon is a *moving* narrow azimuth arc + sliding latitude band.
				// We map your shaders normalized x/y selectors to [0,1] lon/lat-ish slots.
				// Start arc:
				const float phi_norm0 = fract01((a / TAU) + 0.19f * sf + 0.07f * RND());
				float arc_len0 = 0.05f + 0.07f * RND();               // thin ribbon arc
				float x_min0 = phi_norm0;
				float x_max0 = (phi_norm0 + arc_len0 <= 1.0f) ? (phi_norm0 + arc_len0) : 1.0f;
				if (x_max0 - x_min0 < 0.02f) { x_min0 = std::max(0.0f, 1.0f - arc_len0); x_max0 = 1.0f; }

				// End arc slides forward with extra twist
				const float twist_turns = 1.5f + 2.5f * RND();        // 1.5..4.0 nominal turns mapped into [0,1]
				const float phi_norm1 = fract01(phi_norm0 + handed * twist_turns * 0.18f);
				float arc_len1 = arc_len0 * (0.85f + 0.35f * RND());  // arcs can widen a hair
				float x_min1 = phi_norm1;
				float x_max1 = (phi_norm1 + arc_len1 <= 1.0f) ? (phi_norm1 + arc_len1) : 1.0f;
				if (x_max1 - x_min1 < 0.02f) { x_min1 = std::max(0.0f, 1.0f - arc_len1); x_max1 = 1.0f; }

				// Latitude band slides (climb/descend) to produce the loxodrome effect
				const float band_center0 = clamp01(0.15f + 0.70f * fract01(sf * 0.37f + t * 0.61f + 0.21f * RND()));
				const float band_width0 = 0.030f + 0.060f * RND();
				float y_min0 = clamp01(band_center0 - 0.5f * band_width0);
				float y_max0 = clamp01(band_center0 + 0.5f * band_width0);
				if (y_max0 - y_min0 < 0.02f) { y_min0 = clamp01(band_center0 - 0.02f); y_max0 = clamp01(band_center0 + 0.02f); }

				const float band_slide = handed * (0.22f + 0.35f * RND()); // how far the ribbon climbs
				const float band_center1 = clamp01(band_center0 + band_slide);
				const float band_width1 = band_width0 * (0.95f + 0.35f * RND());
				float y_min1 = clamp01(band_center1 - 0.5f * band_width1);
				float y_max1 = clamp01(band_center1 + 0.5f * band_width1);
				if (y_max1 - y_min1 < 0.02f) { y_min1 = clamp01(band_center1 - 0.02f); y_max1 = clamp01(band_center1 + 0.02f); }

				// Shell thickness + jitter evolution: condensate into sharp cords
				const float thick0 = 0.008f + 0.020f * RND();
				const float thick1 = thick0 * (1.8f + 1.0f * RND());  // bloom thicker
				const float jit0 = 1.10f + 0.60f * RND();           // noisy start
				const float jit1 = 0.18f + 0.20f * RND();           // crisp end

				// Colors: shift hue across strands and advance at the end
				XYZ c0 = palette(fract01(0.12f + sf * 0.65f + 0.27f * t));
				XYZ c1 = palette(fract01(0.62f + sf * 0.65f + 0.47f * t));

				Sphere S;
				S.start_position = p0;
				S.end_position = p1;

				S.start_color = c0;
				S.end_color = c1;

				S.radious = { rad0, rad1 };
				S.cube_size = { cube0, cube1 };

				S.x_rnd_min = { x_min0, x_min1 };
				S.x_rnd_max = { x_max0, x_max1 };

				S.y_rnd_min = { y_min0, y_min1 };
				S.y_rnd_max = { y_max0, y_max1 };

				S.thickness = { thick0, thick1 };
				S.jitter = { jit0,   jit1 };

				spheres.push_back(std::move(S));
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

			program.capture.capture = true;
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
				
				// sphere.init(200);
				
				sphere.init_orbital_bands(2000);
				sphere.draw(scene, 10000);

				// sphere.init_twisted_loxodrome_ribbons(/*number=*/240);  // try 150–500
				// sphere.draw(scene, /*number_of_cube_per_sphere=*/256 * 256);
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

		const bool enable_shader_10_unit_cube = false;
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