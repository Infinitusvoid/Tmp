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
							.set_position_end(0.0f, 20.0f, 0.0f)
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
							.set_position_end(10.0f, 20.0f, 10.0f)
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
			// Video::generate();
		}
		
	}





	return 0;
}

