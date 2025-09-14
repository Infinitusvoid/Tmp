// universe_2.h — auto-generated stub
#pragma once

// Put your universe 2 content here.
// This file is safe to include via universes/universe.h
int universe(int argc, char* argv[])
{
	std::cout << "universe_2\n";
	

	Scene_::Scene scene = Scene_::Scene();

	// configuration
	{
		Program program;

		program.le.halfLife = 0.02;
		// program.le.exposure = 4.0;
		program.le.brightness = 0.0;

		program.capture.capture = true;

		// CAPTURED: { pos: [-142.341278, 171.578140, 448.985260] , yaw : 80.639992, pitch : 19.440002, fov : 45.000000 }

		program.camera_start.x = -142.341278f;
		program.camera_start.y = 171.578140f;
		program.camera_start.z = 448.985260f;

		program.camera_start.yaw = 80.639992f;
		program.camera_start.pitch = 19.440002f;

		program.camera_start.fov = 45.0f;

		

		// CAPTURED: { pos: [-15.359919, -13.098754, 52.493931] , yaw : 20.400003, pitch : 16.679998, fov : 45.000000 }

		program.camera_end.x = -15.359919f;
		program.camera_end.y = -13.098754f;
		program.camera_end.z =  52.493931f;

		program.camera_end.yaw = 20.400003f;
		program.camera_end.pitch = 16.679998f;

		program.camera_end.fov = 45.0f;

		program.render_display.number_of_frames = 60 * 4;
		program.render_display.render_time_start = 60.0 * 4.0 * float(int(0));
		

		program.configure(scene);
	}



	// shaders
	{
		add_shader(scene, 6, [](Program::Shader& sh) {


			// Instance 0
			{
				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(10)
					.set_position_start(0.0f, 0.0f, 0.0f)
					.set_position_end(0.0f, 0.0f, 0.0f)
					.set_euler_start(0.0f, 0.0f, 0.0f)
					.set_euler_end(0.0f, 0.0f, 0.0f)
					.set_scale_start(1.0f, 1.0f, 1.0f)
					.set_scale_end(1.0f, 1.0f, 1.0f);


				constexpr int kU = 10;
				for (int u = 0; u < kU; ++u)
				{
					const float v_start = 0.1f * static_cast<float>(u);
					const float v_end = 1.0f - 0.1f * static_cast<float>(u);
					I.set_u_start_end(u, v_start, v_end);
				}

				I.set_u_start_end(0, 72.29710, 72.29710);
			}


			// Instance 1
			
			{

				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(10)
					.set_position_start(10.0f, 0.0f, 0.0f)
					.set_position_end(10.0f, 0.0f, 0.0f)
					.set_euler_start(0.0f, 0.0f, 0.0f)
					.set_euler_end(0.0f, 0.0f, 0.0f)
					.set_scale_start(1.0f, 1.0f, 1.0f)
					.set_scale_end(1.0f, 1.0f, 1.0f);


				constexpr int kU = 10;
				for (int u = 0; u < kU; ++u)
				{
					const float v_start = 0.1f * static_cast<float>(u);
					const float v_end = 1.0f - 0.1f * static_cast<float>(u);
					I.set_u_start_end(u, v_start, v_end);
				}

				I.set_u_start_end(0, 4.237, 4.237);
			}


			});


		add_shader(scene, 7, [](Program::Shader& sh) {


			// Instance 0
			
			{
				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(10)
					.set_position_start(0.0f, 10.0f, 0.0f)
					.set_position_end(0.0f, 10.0f, 0.0f)
					.set_euler_start(0.0f, 0.0f, 0.0f)
					.set_euler_end(0.0f, 0.0f, 0.0f)
					.set_scale_start(1.0f, 1.0f, 1.0f)
					.set_scale_end(1.0f, 1.0f, 1.0f);


				constexpr int kU = 10;
				for (int u = 0; u < kU; ++u)
				{
					const float v_start = 0.1f * static_cast<float>(u);
					const float v_end = 1.0f - 0.1f * static_cast<float>(u);
					I.set_u_start_end(u, v_start, v_end);
				}

				I.set_u_start_end(0, 1.427, 1.427);
			}


			// Instance 1
			
			{

				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(1)
					.set_position_start(10.0f, 10.0f, 10.0f)
					.set_position_end(10.0f, 10.0f, 10.0f)
					.set_euler_start(0.0f, 0.0f, 0.0f)
					.set_euler_end(0.0f, 0.0f, 0.0f)
					.set_scale_start(1.0f, 1.0f, 1.0f)
					.set_scale_end(1.0f, 1.0f, 1.0f);


				constexpr int kU = 10;
				for (int u = 0; u < kU; ++u)
				{
					const float v_start = 0.1f * static_cast<float>(u);
					const float v_end = 1.0f - 0.1f * static_cast<float>(u);
					I.set_u_start_end(u, v_start, v_end);
				}

				I.set_u_start_end(0, 5.497, 5.497);

			}

			

			});
	}




	{
		// scene.print();
		std::string program_name = NameGenerators_::generate_prefix_timestamp_suffix_name();
		save_program(scene, program_name);
		run_program(program_name);
		Video::generate();
	}



	return 0;
}