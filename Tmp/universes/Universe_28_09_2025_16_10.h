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

		
		// "City Lattice Compact"  smaller beads, strict unit-cube fit, global snap, skybridges
		void init_city_lattice_compact(int number)
		{
			spheres.clear();
			spheres.reserve(number);

			auto RND = []() -> float { return Random::generate_random_float_0_to_1(); };
			auto lerp = [](float a, float b, float t) -> float { return a + (b - a) * t; };
			auto clampf = [](float v, float lo, float hi) -> float { return (v < lo) ? lo : (v > hi ? hi : v); };
			auto clampP = [&](XYZ p, float m)->XYZ {
				p.x = clampf(p.x, m, 1.f - m); p.y = clampf(p.y, m, 1.f - m); p.z = clampf(p.z, m, 1.f - m); return p;
				};

			const float margin = 0.06f;          // safety margin to keep everything inside the box
			const float ground = margin;         // ground plane sits at margin
			const float snapStep = 0.01f;          // global lattice step for crisp alignment

			auto snap = [&](float v) -> float {
				float s = snapStep;
				float q = std::round(v / s) * s;
				return clampf(q, margin, 1.f - margin);
				};
			auto snapP = [&](XYZ p) -> XYZ { p.x = snap(p.x); p.y = snap(p.y); p.z = snap(p.z); return p; };

			// Grid of buildings (tweak to taste; 3x3 keeps it readable)
			const int gridX = 3, gridZ = 3;
			const int buildings = gridX * gridZ;

			// Per-building soft allocation; push_bead checks global cap.
			const int per_build_budget = std::max(24, number / std::max(1, buildings));

			// Bead profile  SMALLER than previous version
			const float bead_base_lo = 0.0060f, bead_base_hi = 0.0090f;   // radius (instance scale)
			const float cube_lo = 0.00030f, cube_hi = 0.00050f; // cube size per bead
			const float thick_lo = 0.006f, thick_hi = 0.010f;   // shell thickness
			const float jit_start_lo = 0.80f, jit_start_hi = 1.10f;
			const float jit_end_lo = 0.12f, jit_end_hi = 0.18f;

			auto push_bead = [&](const XYZ& startP, const XYZ& endP,
				const XYZ& c0, const XYZ& c1,
				float beadS, float beadE,
				float cube0, float cube1,
				float thick0, float thick1,
				float jit0, float jit1)
				{
					if ((int)spheres.size() >= number) return false;

					Sphere S;
					S.start_position = clampP(startP, margin);
					S.end_position = clampP(endP, margin);

					S.start_color = c0;
					S.end_color = c1;

					S.radious = { beadS, beadE };
					S.cube_size = { cube0, cube1 };

					// full sampling for round, clean beads
					S.x_rnd_min = { 0.f, 0.f }; S.x_rnd_max = { 1.f, 1.f };
					S.y_rnd_min = { 0.f, 0.f }; S.y_rnd_max = { 1.f, 1.f };

					S.thickness = { thick0, thick1 };
					S.jitter = { jit0,   jit1 };

					spheres.push_back(std::move(S));
					return true;
				};

			// City cell sizes
			const float cellW = (1.f - 2.f * margin) / float(gridX);
			const float cellD = (1.f - 2.f * margin) / float(gridZ);

			// Loop cells
			for (int gz = 0; gz < gridZ && (int)spheres.size() < number; ++gz)
				for (int gx = 0; gx < gridX && (int)spheres.size() < number; ++gx)
				{
					// Cell center (snapped)
					float cx = margin + (gx + 0.5f) * cellW;
					float cz = margin + (gz + 0.5f) * cellD;
					cx = snap(cx); cz = snap(cz);

					// Building footprint, snug inside cell (snapped)
					float bw = (0.55f + 0.25f * RND()) * cellW;   // width
					float bd = (0.55f + 0.25f * RND()) * cellD;   // depth
					float halfW = 0.5f * bw, halfD = 0.5f * bd;

					// Small jitter then clamp & snap to stay inside margins
					cx = snap(clampf(cx + (RND() - 0.5f) * 0.03f * cellW, margin + halfW, 1.f - margin - halfW));
					cz = snap(clampf(cz + (RND() - 0.5f) * 0.03f * cellD, margin + halfD, 1.f - margin - halfD));

					// Height: choose floor height first, then floors to fit under (1 - margin)
					float floorH = lerp(0.018f, 0.030f, RND());                     // smaller floors
					int   maxFlo = (int)std::floor(((1.f - margin) - ground) / floorH);
					int   floors = std::clamp(int(maxFlo * lerp(0.55f, 0.85f, RND())), 8, std::min(24, maxFlo));
					float topY = clampf(ground + floors * floorH, ground + 2 * floorH, 1.f - margin);

					// Corners (snapped)
					XYZ c00 = snapP({ cx - halfW, ground, cz - halfD });
					XYZ c10 = snapP({ cx + halfW, ground, cz - halfD });
					XYZ c01 = snapP({ cx - halfW, ground, cz + halfD });
					XYZ c11 = snapP({ cx + halfW, ground, cz + halfD });

					// Per-building bead counts (kept moderate)
					int beads_per_col = std::clamp(floors * 2, 10, 48);
					int beads_per_beam = std::clamp(10 + int(8 * RND()), 10, 22);

					// Colors (neutral steel + faint warm beams)
					auto steel = [&](float t)->XYZ { return { 0.60f + 0.20f * t, 0.65f + 0.18f * t, 0.72f + 0.15f * t }; };
					auto amber = [&](float t)->XYZ { return { 0.92f, 0.70f + 0.25f * t, 0.38f + 0.22f * t }; };

					// Bead parameters (small)
					float bead_base = lerp(bead_base_lo, bead_base_hi, RND());
					float bead_post = bead_base * 1.35f;
					float cube0 = lerp(cube_lo, cube_hi, RND());
					float cube1 = cube0 * lerp(0.72f, 0.82f, RND());
					float thick0 = lerp(thick_lo, thick_hi, RND());
					float thick1 = thick0 * lerp(1.05f, 1.25f, RND());
					float jit0 = lerp(jit_start_lo, jit_start_hi, RND());
					float jit1 = lerp(jit_end_lo, jit_end_hi, RND());

					// Helper to add a vertical column
					auto add_column = [&](const XYZ& base, bool isPost)
						{
							for (int k = 0; k < beads_per_col && (int)spheres.size() < number; ++k)
							{
								float t = (k + 0.5f) / float(beads_per_col);
								XYZ endP = { base.x, snap(lerp(ground, topY, t)), base.z };
								XYZ startP = { cx, ground, cz }; // assemble from core

								float bS = bead_base * (isPost ? 1.10f : 1.00f);
								float bE = (k % 6 == 0) ? bead_post : bead_base * 1.20f;

								XYZ col0 = steel(0.20f + 0.5f * t), col1 = steel(0.55f + 0.4f * t);
								push_bead(startP, endP, col0, col1, bS, bE, cube0, cube1, thick0, thick1, jit0, jit1);
							}
						};

					// Four corner columns
					add_column(c00, true);
					add_column(c10, false);
					add_column(c01, false);
					add_column(c11, true);

					if ((int)spheres.size() >= number) continue;

					// Floor beams (rect per floor)
					auto add_beam = [&](const XYZ& A, const XYZ& B, bool warm)
						{
							for (int k = 0; k < beads_per_beam && (int)spheres.size() < number; ++k)
							{
								float t = (k + 0.5f) / float(beads_per_beam);
								XYZ endP = { snap(lerp(A.x, B.x, t)), snap(A.y), snap(lerp(A.z, B.z, t)) };
								XYZ startP = { cx, ground, cz };

								bool post = (k % 5 == 0);
								float bS = bead_base * (warm ? 1.05f : 1.00f);
								float bE = post ? bead_post : bead_base * (warm ? 1.30f : 1.18f);

								XYZ c0 = warm ? amber(0.30f + 0.5f * t) : steel(0.30f + 0.5f * t);
								XYZ c1 = warm ? amber(0.60f + 0.4f * t) : steel(0.60f + 0.4f * t);

								push_bead(startP, endP, c0, c1, bS, bE, cube0, cube1, thick0, thick1, jit0, jit1);
							}
						};

					for (int f = 0; f <= floors && (int)spheres.size() < number; ++f)
					{
						float y = snap(ground + f * floorH);
						if (y > topY) break;

						XYZ a00 = { c00.x, y, c00.z }, a10 = { c10.x, y, c10.z };
						XYZ a01 = { c01.x, y, c01.z }, a11 = { c11.x, y, c11.z };

						add_beam(a00, a10, true);   // front
						add_beam(a10, a11, false);  // right
						add_beam(a11, a01, true);   // back
						add_beam(a01, a00, false);  // left
					}

					if ((int)spheres.size() >= number) continue;

					// SKYBRIDGES: connect to neighbors along +X and +Z at a few floors
					auto add_skybridges_to_neighbor = [&](int nx, int nz)
						{
							if (nx >= gridX || nz >= gridZ) return;

							// Neighbor's center
							float ncx = margin + (nx + 0.5f) * cellW;
							float ncz = margin + (nz + 0.5f) * cellD;
							ncx = snap(ncx); ncz = snap(ncz);

							// Shared mid floors (2-3 bridges)
							int bridges = 2 + (int)(RND() * 2.f);
							for (int i = 0; i < bridges && (int)spheres.size() < number; ++i)
							{
								float level = (i + 1.f) / float(bridges + 1);
								float y = snap(ground + level * (topY - ground));

								// From our outer edge to neighbor's outer edge (straight line)
								XYZ A, B;
								if (nx != gx) { // X-bridge
									float zmid = snap(cz);
									A = { snap(cx + (nx > gx ? halfW : -halfW)), y, zmid };
									B = { snap(ncx + (nx > gx ? -0.5f * (0.55f * cellW) : +0.5f * (0.55f * cellW))), y, zmid };
								}
								else {        // Z-bridge
									float xmid = snap(cx);
									A = { xmid, y, snap(cz + (nz > gz ? halfD : -halfD)) };
									B = { xmid, y, snap(ncz + (nz > gz ? -0.5f * (0.55f * cellD) : +0.5f * (0.55f * cellD))) };
								}

								// Slim, warm-toned bridge
								int bridge_beads = std::clamp(8 + (int)(6 * RND()), 8, 16);
								for (int k = 0; k < bridge_beads && (int)spheres.size() < number; ++k)
								{
									float t = (k + 0.5f) / float(bridge_beads);
									XYZ endP = { snap(lerp(A.x, B.x, t)), y, snap(lerp(A.z, B.z, t)) };
									XYZ startP = { cx, ground, cz };

									float bS = bead_base * 0.95f;
									float bE = bead_base * 1.15f;

									XYZ c0 = amber(0.35f + 0.5f * t), c1 = amber(0.65f + 0.3f * t);
									push_bead(startP, endP, c0, c1,
										bS, bE,
										cube0 * 0.95f, cube1 * 0.95f,
										thick0 * 0.95f, thick1 * 0.95f,
										jit0 * 0.95f, jit1 * 0.95f);
								}
							}
						};

					add_skybridges_to_neighbor(gx + 1, gz); // +X neighbor
					add_skybridges_to_neighbor(gx, gz + 1); // +Z neighbor
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
				// Spheres sphere;
				// sphere.init(20);
				// sphere.draw(scene, 1000);

				
				Spheres sp;
				sp.init_city_lattice_compact(/*number=*/4000);  // try 800–4000 depending on perf
				sp.draw(scene, /*cubes per bead*/ 10000);
				
				
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