#include "Engine.h"

#include "CppCommponents/NameGenerators.h"






int main(int argc, char* argv[])
{
	std::cout << "Tmp\n";


	init();





	{
		Scene_::Scene scene = Scene_::Scene();

		// configuration
		{
			Program program;

			program.le.halfLife = 0.02;
			program.capture.capture = true;

			// CAPTURED: { pos: [-10.521732, -9.406315, 44.638687] , yaw : 24.840000, pitch : 23.040001, fov : 45.000000 }

			program.camera_start.x = -10.521732f;
			program.camera_start.y = -9.406315f;
			program.camera_start.z = 44.638687f;

			program.camera_start.yaw = 24.840000f;
			program.camera_start.pitch = 23.040001f;

			program.camera_start.fov = 45.0f;

			// CAPTURED: { pos:[-7.651373,4.471294,134.883682], yaw:13.920000, pitch:3.720003, fov:45.000000 }

			program.camera_end.x = -7.651373f;
			program.camera_end.y = 4.471294f;
			program.camera_end.z = 134.883682f;

			program.camera_end.yaw = 13.920000f;
			program.camera_end.pitch = 3.720003f;

			program.camera_end.fov = 45.0f;



			program.configure(scene);
		}



		// shaders
		{
			add_shader(scene, 1, [](Program::Shader& sh) {


				// Instance 0
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(1000, 1000, 1)
						.set_drawcalls(4)
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
				}


				// Instance 1
				{

					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(1000, 1000, 1)
						.set_drawcalls(4)
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

				}


				});


			add_shader(scene, 0, [](Program::Shader& sh) {


				// Instance 0
				{
					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(1000, 1000, 1)
						.set_drawcalls(4)
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
				}


				// Instance 1
				{

					auto id = sh.create_instance();
					auto I = sh.instance(id);
					I.set_group_size(1000, 1000, 1)
						.set_drawcalls(4)
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

	}





	return 0;
}

