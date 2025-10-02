
#include "../vibes/Vibe_01_10_2025_11_21.h"

#include <unordered_map>
#include <limits>

namespace Universe_
{
	using namespace Vibe_01_10_2025_11_21_;

	
	struct Vec3_StartEnd
	{
		Vec3 start;
		Vec3 end;
	};

	
	
	void add_sphere
	(
		Spheres_shader_20& spheres_shader_20,
		Vec3_StartEnd position,
		Float_start_end radius,
		Vec3_StartEnd color,
		Float_start_end cube_size, 
		Float_start_end x_rnd_min,
		Float_start_end x_rnd_max,
		Float_start_end y_rnd_min,
		Float_start_end y_rnd_max, 
		Float_start_end thickness,
		Float_start_end jitter
	)
	{
		Spheres_shader_20::Sphere sphere;

		sphere.radius = radius;

		sphere.start_position.x = position.start.x - radius.start * 0.5f;
		sphere.start_position.y = position.start.y - radius.start * 0.5f;
		sphere.start_position.z = position.start.z - radius.start * 0.5f;

		sphere.end_position.x = position.end.x - radius.end * 0.5f;
		sphere.end_position.y = position.end.y - radius.end * 0.5f;
		sphere.end_position.z = position.end.z - radius.end * 0.5f;

		sphere.cube_size = cube_size;

		sphere.x_rnd_min = x_rnd_min;
		sphere.x_rnd_max = x_rnd_max;

		sphere.y_rnd_min = y_rnd_min;
		sphere.y_rnd_max = y_rnd_max;

		sphere.start_color = color.start;
		sphere.end_color = color.end;

		sphere.thickness = thickness;
		sphere.jitter = jitter;

		spheres_shader_20.spheres.push_back(std::move(sphere));
		
	}
	
	
	struct ASCI_Letters
	{
		struct Line2d
		{
			Vec2 start;
			Vec2 end;
		};


		std::unordered_map<int, std::vector<Line2d>> letters;
	};

	namespace ASCI_Letters_
	{
		

		

		// Build a simple stick font (A–Z). Coordinates are on a 5×7 grid (0..4, 0..6).
		ASCI_Letters make_ascii_letters()
		{

			// Helper: push a grid-aligned segment (x,y are integers on a 0..4 by 0..6 grid).
			auto push_grid_seg = [](
				std::vector<ASCI_Letters::Line2d>& out,
				int x1, int y1, int x2, int y2
				)
				{
					constexpr float sx = 1.0f / 4.0f; // grid width 4
					constexpr float sy = 1.0f / 6.0f; // grid height 6
					out.push_back(ASCI_Letters::Line2d{
						Vec2{ x1 * sx, y1 * sy },
						Vec2{ x2 * sx, y2 * sy }
						});
				};

			auto normalize_letters = [](ASCI_Letters& letters)
				{
					constexpr float BOX_MIN = 0.0f;
					constexpr float BOX_MAX = 1.0f;
					constexpr float EPS = 1e-9f;

					auto saturate = [](float v) -> float {
						if (v < 0.0f) return 0.0f;
						if (v > 1.0f) return 1.0f;
						return v;
						};

					for (auto& kv : letters.letters)
					{
						auto& segs = kv.second;
						if (segs.empty()) continue; // e.g., space

						// 1) Find glyph bounds
						float minX = std::numeric_limits<float>::infinity();
						float minY = std::numeric_limits<float>::infinity();
						float maxX = -std::numeric_limits<float>::infinity();
						float maxY = -std::numeric_limits<float>::infinity();

						for (const auto& s : segs)
						{
							minX = std::min(minX, std::min(s.start.x, s.end.x));
							minY = std::min(minY, std::min(s.start.y, s.end.y));
							maxX = std::max(maxX, std::max(s.start.x, s.end.x));
							maxY = std::max(maxY, std::max(s.start.y, s.end.y));
						}

						float w = std::max(maxX - minX, EPS);
						float h = std::max(maxY - minY, EPS);

						// 2) Uniformly scale to fit INSIDE 1x1 (preserve aspect)
						const float boxSize = BOX_MAX - BOX_MIN; // = 1
						const float s = boxSize / std::max(w, h); // <= 1/w or 1/h

						// Scaled size (<= 1)
						const float newW = w * s;
						const float newH = h * s;

						// 3) Center inside the box
						const float offX = BOX_MIN + 0.5f * (boxSize - newW);
						const float offY = BOX_MIN + 0.5f * (boxSize - newH);

						// 4) Apply: translate to origin, scale, then offset to center
						for (auto& sgm : segs)
						{
							float x0 = (sgm.start.x - minX) * s + offX;
							float y0 = (sgm.start.y - minY) * s + offY;
							float x1 = (sgm.end.x - minX) * s + offX;
							float y1 = (sgm.end.y - minY) * s + offY;

							// 5) Clamp to obliterate tiny FP overshoots
							sgm.start.x = saturate(x0);
							sgm.start.y = saturate(y0);
							sgm.end.x = saturate(x1);
							sgm.end.y = saturate(y1);
						}
					}
				};


			ASCI_Letters font;

			// SPACE
			font.letters[' '] = {}; // no strokes; just advance when you render

			// A
			{
				auto& g = font.letters['A'];
				push_grid_seg(g, 0, 0, 2, 6); // left leg
				push_grid_seg(g, 4, 0, 2, 6); // right leg
				push_grid_seg(g, 1, 3, 3, 3); // crossbar
			}

			// B
			{
				auto& g = font.letters['B'];
				push_grid_seg(g, 0, 0, 0, 6); // spine
				// upper bowl
				push_grid_seg(g, 0, 6, 3, 6);
				push_grid_seg(g, 3, 6, 4, 5);
				push_grid_seg(g, 4, 5, 3, 4);
				push_grid_seg(g, 3, 4, 0, 4);
				// lower bowl
				push_grid_seg(g, 0, 4, 3, 4);
				push_grid_seg(g, 3, 4, 4, 3);
				push_grid_seg(g, 4, 3, 3, 2);
				push_grid_seg(g, 3, 2, 0, 2);
				push_grid_seg(g, 0, 2, 0, 0);
			}

			// C
			{
				auto& g = font.letters['C'];
				push_grid_seg(g, 4, 5, 3, 6);
				push_grid_seg(g, 3, 6, 1, 6);
				push_grid_seg(g, 1, 6, 0, 5);
				push_grid_seg(g, 0, 5, 0, 1);
				push_grid_seg(g, 0, 1, 1, 0);
				push_grid_seg(g, 1, 0, 3, 0);
				push_grid_seg(g, 3, 0, 4, 1);
			}

			// D
			{
				auto& g = font.letters['D'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 0, 6, 2, 6);
				push_grid_seg(g, 2, 6, 4, 4);
				push_grid_seg(g, 4, 4, 4, 2);
				push_grid_seg(g, 4, 2, 2, 0);
				push_grid_seg(g, 2, 0, 0, 0);
			}

			// E
			{
				auto& g = font.letters['E'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 0, 6, 4, 6);
				push_grid_seg(g, 0, 3, 3, 3);
				push_grid_seg(g, 0, 0, 4, 0);
			}

			// F
			{
				auto& g = font.letters['F'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 0, 6, 4, 6);
				push_grid_seg(g, 0, 3, 3, 3);
			}

			// G
			{
				auto& g = font.letters['G'];
				// C-like outer
				push_grid_seg(g, 4, 5, 3, 6);
				push_grid_seg(g, 3, 6, 1, 6);
				push_grid_seg(g, 1, 6, 0, 5);
				push_grid_seg(g, 0, 5, 0, 1);
				push_grid_seg(g, 0, 1, 1, 0);
				push_grid_seg(g, 1, 0, 3, 0);
				push_grid_seg(g, 3, 0, 4, 1);
				// inner bar
				push_grid_seg(g, 2, 3, 4, 3);
				push_grid_seg(g, 4, 3, 4, 2);
			}

			// H
			{
				auto& g = font.letters['H'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 4, 0, 4, 6);
				push_grid_seg(g, 0, 3, 4, 3);
			}

			// I
			{
				auto& g = font.letters['I'];
				push_grid_seg(g, 1, 6, 3, 6);
				push_grid_seg(g, 2, 6, 2, 0);
				push_grid_seg(g, 1, 0, 3, 0);
			}

			// J
			{
				auto& g = font.letters['J'];
				push_grid_seg(g, 1, 6, 3, 6);
				push_grid_seg(g, 2, 6, 2, 1);
				push_grid_seg(g, 2, 1, 1, 0);
				push_grid_seg(g, 1, 0, 0, 1);
			}

			// K
			{
				auto& g = font.letters['K'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 0, 3, 4, 6);
				push_grid_seg(g, 0, 3, 4, 0);
			}

			// L
			{
				auto& g = font.letters['L'];
				push_grid_seg(g, 0, 6, 0, 0);
				push_grid_seg(g, 0, 0, 4, 0);
			}

			// M
			{
				auto& g = font.letters['M'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 4, 0, 4, 6);
				push_grid_seg(g, 0, 6, 2, 3);
				push_grid_seg(g, 2, 3, 4, 6);
			}

			// N
			{
				auto& g = font.letters['N'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 4, 0, 4, 6);
				push_grid_seg(g, 0, 6, 4, 0);
			}

			// O
			{
				auto& g = font.letters['O'];
				push_grid_seg(g, 1, 0, 3, 0);
				push_grid_seg(g, 3, 0, 4, 1);
				push_grid_seg(g, 4, 1, 4, 5);
				push_grid_seg(g, 4, 5, 3, 6);
				push_grid_seg(g, 3, 6, 1, 6);
				push_grid_seg(g, 1, 6, 0, 5);
				push_grid_seg(g, 0, 5, 0, 1);
				push_grid_seg(g, 0, 1, 1, 0);
			}

			// P
			{
				auto& g = font.letters['P'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 0, 6, 3, 6);
				push_grid_seg(g, 3, 6, 4, 5);
				push_grid_seg(g, 4, 5, 3, 4);
				push_grid_seg(g, 3, 4, 0, 4);
			}

			// Q
			{
				auto& g = font.letters['Q'];
				// O
				push_grid_seg(g, 1, 0, 3, 0);
				push_grid_seg(g, 3, 0, 4, 1);
				push_grid_seg(g, 4, 1, 4, 5);
				push_grid_seg(g, 4, 5, 3, 6);
				push_grid_seg(g, 3, 6, 1, 6);
				push_grid_seg(g, 1, 6, 0, 5);
				push_grid_seg(g, 0, 5, 0, 1);
				push_grid_seg(g, 0, 1, 1, 0);
				// tail
				push_grid_seg(g, 2, 2, 4, 0);
			}

			// R
			{
				auto& g = font.letters['R'];
				push_grid_seg(g, 0, 0, 0, 6);
				push_grid_seg(g, 0, 6, 3, 6);
				push_grid_seg(g, 3, 6, 4, 5);
				push_grid_seg(g, 4, 5, 3, 4);
				push_grid_seg(g, 3, 4, 0, 4);
				push_grid_seg(g, 0, 4, 4, 0); // leg
			}

			// S
			{
				auto& g = font.letters['S'];
				push_grid_seg(g, 4, 5, 3, 6);
				push_grid_seg(g, 3, 6, 1, 6);
				push_grid_seg(g, 1, 6, 0, 5);
				push_grid_seg(g, 0, 5, 1, 4);
				push_grid_seg(g, 1, 4, 3, 2);
				push_grid_seg(g, 3, 2, 4, 1);
				push_grid_seg(g, 4, 1, 3, 0);
				push_grid_seg(g, 3, 0, 1, 0);
				push_grid_seg(g, 1, 0, 0, 1);
			}

			// T
			{
				auto& g = font.letters['T'];
				push_grid_seg(g, 0, 6, 4, 6);
				push_grid_seg(g, 2, 6, 2, 0);
			}

			// U
			{
				auto& g = font.letters['U'];
				push_grid_seg(g, 0, 6, 0, 1);
				push_grid_seg(g, 0, 1, 1, 0);
				push_grid_seg(g, 1, 0, 3, 0);
				push_grid_seg(g, 3, 0, 4, 1);
				push_grid_seg(g, 4, 1, 4, 6);
			}

			// V
			{
				auto& g = font.letters['V'];
				push_grid_seg(g, 0, 6, 2, 0);
				push_grid_seg(g, 4, 6, 2, 0);
			}

			// W
			{
				auto& g = font.letters['W'];
				push_grid_seg(g, 0, 6, 1, 0);
				push_grid_seg(g, 1, 0, 2, 3);
				push_grid_seg(g, 2, 3, 3, 0);
				push_grid_seg(g, 3, 0, 4, 6);
			}

			// X
			{
				auto& g = font.letters['X'];
				push_grid_seg(g, 0, 6, 4, 0);
				push_grid_seg(g, 0, 0, 4, 6);
			}

			// Y
			{
				auto& g = font.letters['Y'];
				push_grid_seg(g, 0, 6, 2, 3);
				push_grid_seg(g, 4, 6, 2, 3);
				push_grid_seg(g, 2, 3, 2, 0);
			}

			// Z
			{
				auto& g = font.letters['Z'];
				push_grid_seg(g, 0, 6, 4, 6);
				push_grid_seg(g, 4, 6, 0, 0);
				push_grid_seg(g, 0, 0, 4, 0);
			}

			// Lowercase = reuse uppercase strokes
			for (char c = 'A'; c <= 'Z'; ++c)
				font.letters[static_cast<int>(c - 'A' + 'a')] = font.letters[c];


			normalize_letters(font);

			return font;
		}
	}
	
	

	void bug_fix_line_position(float& x, float& y, float& z)
	{
		std::swap(y, z);

		z -= 0.5f;
		x -= 0.5f;
		y -= 0.5f;
	}
	
	

	void init(Scene_::Scene& scene, const int clip_number, const int clip_fps, const int clip_length_seconds, const bool capture, const bool capture_png, const bool capture_bmp)
	{
		const bool enable_shader_10_unit_cube = true;
		const bool enable_shader_20_sphere = false; // sphere
		const bool enable_shader_21_line = false; // line
		const bool enable_shader_22_geodesic_line = false; // line with t
		
		const bool enable_ASCII_letters = true;

		{
			Program program;
			program.le.halfLife = 0.02f * 4.0f * 0.0001f;
			program.le.brightness = 0.0f;
			program.le.exposure = 1.0;
			program.le.msaaSamples = 10;

			program.capture.capture = capture;
			program.capture.capture_png = capture_png;
			program.capture.capture_bmp = capture_bmp;

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
			program.configure(scene);

			// program.camera_end.x = program.camera_start.x;
			// program.camera_end.y = program.camera_start.y;
			// program.camera_end.z = program.camera_start.z;

			// program.camera_end.yaw = program.camera_start.yaw;
			// program.camera_end.pitch = program.camera_start.pitch;
			// program.camera_end.fov = program.camera_start.fov;
		}




		if (enable_ASCII_letters)
		{
			std::cout << "enable_ASCII_letters : " << enable_ASCII_letters << "\n";

			ASCI_Letters letters =  ASCI_Letters_::make_ascii_letters();
			

			Lines_shader_21 lines;

			{
				auto draw_line_2d = [&lines](float x0, float y0, float x1, float y1)
					{
						Lines_shader_21::Line& line = lines.add_line();

						line.x0.start = x0;
						line.y0.start = y0;
						line.z0.start = 0.5;

						line.x1.start = x1;
						line.y1.start = y1;
						line.z1.start = 0.5;

						line.thickness.start = 0.01f;

						line.rgb_t0.x = Random::generate_random_float_0_to_1();
						line.rgb_t0.y = Random::generate_random_float_0_to_1();
						line.rgb_t0.z = Random::generate_random_float_0_to_1();

						line.rgb_t1.x = Random::generate_random_float_0_to_1();
						line.rgb_t1.y = Random::generate_random_float_0_to_1();
						line.rgb_t1.z = Random::generate_random_float_0_to_1();

						line.number_of_cubes = 1000;

						line.thickness.end = 0.01f;

						bug_fix_line_position(line.x0.start, line.y0.start, line.z0.start);
						bug_fix_line_position(line.x1.start, line.y1.start, line.z1.start);

						line.copy_start_to_end();

					};

				// A ok
				// B fail
				// C ok
				// D ok
				// F ok
				// G fail
				// H ok
				// F ok
				// I ok
				// J ok
				// K ok
				// L ok
				// M ok
				// N ok
				// O ok
				// P ok
				// R ok
				// S ok 
				// T ok
				// U ok
				// V ok
				// Z ok
				// W ok
				// X ok
				// Y ok
				// Z ok
				// Q ok

				
				
				for (auto& l : letters.letters.at('1'))
				{
					draw_line_2d(l.start.x, l.start.y, l.end.x, l.end.y);
				}
				


				lines.draw(scene);
			}
			



			
		}

		if (enable_shader_20_sphere) // sphered
		{

			auto spheres_on_lines_init = [](Spheres_shader_20& spheres, Lines_shader_21& lines, int number)
				{

					for (int i = 0; i < number; i++)
					{
						// Spheres_shader_20::Sphere sphere;


						float factor_i = (1.0f / float(number)) * i;

						float radius_start =  Random::generate_random_float_0_to_1() * 0.2;
						float radius_end = radius_start + Random::generate_random_float_0_to_1() * 0.2;

						{
							Lines_shader_21::Line& line = lines.add_line();

							line.x0.start = Random::generate_random_float_0_to_1();
							line.y0.start = 0.0;
							line.z0.start = Random::generate_random_float_0_to_1();

							line.x1.start = line.x0.start;
							line.y1.start = 1.0;
							line.z1.start = line.z0.start;

							line.thickness.start = 0.01f;

							line.rgb_t0.x = Random::generate_random_float_0_to_1();
							line.rgb_t0.y = Random::generate_random_float_0_to_1();
							line.rgb_t0.z = Random::generate_random_float_0_to_1();

							

							line.number_of_cubes = 1000;

							{
								Vec3 position_start = { line.x0.start, line.y0.start, line.z0.start };
								Vec3 position_end = { line.x1.start, line.y1.start, line.z1.start };

								bug_fix_line_position(line.x0.start, line.y0.start, line.z0.start);
								bug_fix_line_position(line.x1.start, line.y1.start, line.z1.start);

								line.copy_start_to_end();

								line.rgb_t1.x = Random::generate_random_float_0_to_1();
								line.rgb_t1.y = Random::generate_random_float_0_to_1();
								line.rgb_t1.z = Random::generate_random_float_0_to_1();


								Vec3 color_start = { Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1() };
								Vec3 color_end = { Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1(),  Random::generate_random_float_0_to_1() };

								float cube_size_start = 0.001;
								float cube_size_end = 0.002;

								Float_start_end x_rnd_min{ 0.0, 0.0 };
								Float_start_end x_rnd_max{ 1.0, 1.0 };
								Float_start_end y_rnd_min{ 0.0, 0.0 };
								Float_start_end y_rnd_max{ 1.0, 1.0 };
								Float_start_end thickness{ 0.1, 0.1 };
								Float_start_end jitter{ 1.0, 1.0 };


								add_sphere
								(
									spheres,
									{
										position_start,
										position_end
									},
							{
								radius_start,
								radius_end
							},
							{
								color_start,
								color_end
							},
									{ cube_size_start, cube_size_end },
									x_rnd_min,
									x_rnd_max,
									y_rnd_min,
									y_rnd_max,
									thickness,
									jitter
								);
							}
						}


						
						
					}
				};

			Spheres_shader_20 spheres;
			// spheres_init_1(sphere, 10);
			Lines_shader_21 lines;

			spheres_on_lines_init(spheres, lines, 10);

			spheres.draw(scene, 1000);
			lines.draw(scene);
		}

		if (enable_shader_21_line) // lines
		{
			auto lines_init = [](Lines_shader_21& lines)
				{
					int number_of_lines = 100;
					const float TAU = 6.2831853071795864769252867665590;
					float step_size = (1.0 / float(number_of_lines)) * TAU;

					for (int i = 0; i < number_of_lines; i++)
					{
						Lines_shader_21::Line& line = lines.add_line();


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





				};

			Lines_shader_21 lines;
			lines_init(lines);
			lines.draw(scene);
		}

		if (enable_shader_22_geodesic_line) // lines with t
		{
			auto init_lines_geodesic = [](LinesGeodesic_shader_22& lines_geodesic)
			{
				if (true)
				{
					auto draw_sphere = [](std::vector<LinesGeodesic_shader_22::LineGeodesic>& l, float radius, float x, float y, float z, float thickness)
						{
							for (int i = 0; i < 10; i++)
							{
								LinesGeodesic_shader_22::LineGeodesic L;
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
								LinesGeodesic_shader_22::LineGeodesic L;
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
								LinesGeodesic_shader_22::LineGeodesic L;
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
									LinesGeodesic_shader_22::LineGeodesic L;
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

					draw_sphere(lines_geodesic.lines, 0.025, 0.5, 0.5, 0.5, 0.00025); // center of unit cube

					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 0.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 0.0, 1.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 1.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 0.0, 1.0, 1.0, 0.001);

					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 0.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 0.0, 1.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 1.0, 0.0, 0.001);
					draw_sphere(lines_geodesic.lines, 0.1, 1.0, 1.0, 1.0, 0.001);

				}

				if (true)
				{

					for (int i = 0; i < 1000; i++)
					{
						LinesGeodesic_shader_22::LineGeodesic& L = lines_geodesic.add_line();

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

				
			};

			LinesGeodesic_shader_22 lines_with_t;
			init_lines_geodesic(lines_with_t);
			lines_with_t.draw(scene);
		}

		if (enable_shader_10_unit_cube)
		{
			UnitCube_shader_10 unit_cube;
			unit_cube.init();
			unit_cube.draw(scene);
		}
	}

	void generate()
	{
		Clip clip;

		// for(int i = 0; i < 30; i++)
		{
			std::cout << "clip : " << clip.clip_number << "\n";

			clip.capture = false;
			clip.capture_png = true;
			clip.capture_bmp = false;

			clip.generate(0, init);

			// for (int i = 1; i < 57; i++)
			// {
			// 	clip.generate(i);
			// }

		}

		// TODO
		// create letters


		// TODO
		// DONE enigine set use F drive ( well in that mode create new folder for each run so that you can run generate many concurently nicely )
		// write the motion extraction kind of thing 
		// some format like ply write and read, texture write and read ( you will generate model that you will upload to sketfab, usual polygon based with textures that go along and pointclouds)
		// audio reading, writing we need first steps into generating visuals that are aligned with audio explore procedural audio generation 

		// Write about how  the code is about expression and understanding not just utility or a way to build tools but itself a peace of art well yea 

		// The coordinate system that we use
		// Y is up 
		// Z is forward 
		// X with Z forms the plane 

		// Unit cube 
		// center 
		// x 0.5
		// y 0.5 when 1.0 top side
		// z 0.5

	}

}




int universe(int argc, char* argv[])
{
	std::cout << "Universe\n";

	Universe_::generate();

	return 0;
}