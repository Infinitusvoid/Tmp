
#include "../vibes/Vibe_01_10_2025_11_21.h"

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
		const bool enable_shader_20_sphere = true; // sphere
		const bool enable_shader_21_line = true; // line
		const bool enable_shader_22_geodesic_line = true; // line with t


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

			clip.capture = true;
			clip.capture_png = true;
			clip.capture_bmp = false;

			clip.generate(0, init);

			// for (int i = 1; i < 57; i++)
			// {
			// 	clip.generate(i);
			// }

		}



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