// universe_2.h — auto-generated stub
#pragma once


#include "Extension_Folder.h"

namespace Universe_
{
	struct CameraRecording
	{
		float x;
		float y;
		float z;
		float yaw;
		float pitch;
		float fov;

		int start_frame;
		int end_frame;
	};

	struct CameraPath
	{
		std::vector<CameraRecording> path;

		void load_last(Program& program)
		{
			CameraRecording recording = path.back();

			program.camera_start.x = recording.x;
			program.camera_start.y = recording.y;
			program.camera_start.z = recording.z;
			program.camera_start.yaw = recording.yaw;
			program.camera_start.pitch = recording.pitch;
			program.camera_start.fov = recording.fov;

			program.render_display.render_time_start = float(recording.end_frame) * (1.0 / 60.0f);
			
		}

		void set_index_for_rendering(int index, Program& program, const CameraPath& camera_path)
		{
			{
				const CameraRecording& start = camera_path.path.at(index);

				program.camera_start.x = start.x;
				program.camera_start.y = start.y;
				program.camera_start.z = start.z;
				program.camera_start.yaw = start.yaw;
				program.camera_start.pitch = start.pitch;
				program.camera_start.fov = start.fov;

				
			}

			{
				const CameraRecording& end = camera_path.path.at(index + 1);

				program.camera_end.x = end.x;
				program.camera_end.y = end.y;
				program.camera_end.z = end.z;
				program.camera_end.yaw = end.yaw;
				program.camera_end.pitch = end.pitch;
				program.camera_end.fov = end.fov;
			}
			
			
			program.render_display.number_of_frames = camera_path.path.at(index + 1).end_frame - camera_path.path.at(index).end_frame;
		}
	};

	
	void build_path(CameraPath& path)
	{
		{
			CameraRecording recording;
			recording.x = -142.341278f;
			recording.y = 171.578140f;
			recording.z = 448.985260f;
			recording.yaw = 80.639992f;
			recording.pitch = 19.440002f;
			recording.fov = 45.0;

			recording.start_frame = 60 * 4 * 0;
			recording.end_frame = 60 * 4 * 1;

			path.path.push_back(recording);
		}
		
		{
			CameraRecording recording;
			recording.x = -15.359919f;
			recording.y = -13.098754f;
			recording.z = 52.493931f;
			recording.yaw = 20.400003f;
			recording.pitch = 16.679998f;
			recording.fov = 45.0f;

			recording.start_frame = 60 * 4 * 1;
			recording.end_frame = 60 * 4 * 2;

			path.path.push_back(recording);
		}

		{
			CameraRecording recording;
			recording.x = 56.266338;
			recording.y = -12.129290;
			recording.z = -12.411207;
			recording.yaw = -103.920052;
			recording.pitch = 22.559982;
			recording.fov = 45.0;

			recording.start_frame = 60 * 4 * 2;
			recording.end_frame = 60 * 4 * 3;

			path.path.push_back(recording);
		}

		{
			//CAPTURED: { pos: [73.614182, -21.104589, 42.984325] , yaw : -50.880009, pitch : 20.279987, fov : 45.000000 }

			CameraRecording recording;
			recording.x = 73.614182;
			recording.y = -21.104589;
			recording.z = 42.984325;
			recording.yaw = -50.880009;
			recording.pitch = 20.279987;
			recording.fov = 45.0;

			recording.start_frame = 60 * 4 * 3;
			recording.end_frame = 60 * 4 * 4;

			path.path.push_back(recording);
		}
	}
}


// Put your universe 2 content here.
// This file is safe to include via universes/universe.h
int universe(int argc, char* argv[])
{
	std::cout << "universe_2\n";
	

	Scene_::Scene scene = Scene_::Scene();

	const int section = 7;

	// configuration
	{
		Program program;

		program.le.halfLife = 0.02;
		// program.le.exposure = 4.0;
		program.le.brightness = 0.0;

		program.capture.capture = false;

		Universe_::CameraPath path;
		Universe_::build_path(path);

		path.load_last(program);

		//if(false) // 0
		{
			// CAPTURED: { pos: [-142.341278, 171.578140, 448.985260] , yaw : 80.639992, pitch : 19.440002, fov : 45.000000 }


			// CAPTURED: { pos: [-15.359919, -13.098754, 52.493931] , yaw : 20.400003, pitch : 16.679998, fov : 45.000000 }

			
		}

		// if (true) // 1
		{
			// CAPTURED: { pos: [-142.341278, 171.578140, 448.985260] , yaw : 80.639992, pitch : 19.440002, fov : 45.000000 }
			// CAPTURED: { pos: [56.266338, -12.129290, -12.411207] , yaw : -103.920052, pitch : 22.559982, fov : 45.000000 }

			



			// CAPTURED: { pos: [-15.359919, -13.098754, 52.493931] , yaw : 20.400003, pitch : 16.679998, fov : 45.000000 }

			program.camera_start.x = -15.359919f;
			program.camera_start.y = -13.098754f;
			program.camera_start.z = 52.493931f;
			program.camera_start.yaw = 20.400003f;
			program.camera_start.pitch = 16.679998f;
			program.camera_start.fov = 45.0f;

			program.camera_end.x = -15.359919f;
			program.camera_end.y = -13.098754f;
			program.camera_end.z = 52.493931f;
			program.camera_end.yaw = 20.400003f;
			program.camera_end.pitch = 16.679998f;
			program.camera_end.fov = 45.0f;

			
			program.render_display.number_of_frames = 240;
			program.render_display.render_time_start = ((1.0 / 60.0) * 240.0) * section;
			
		}
		

		// program.render_display.number_of_frames = 60 * 4;
		// program.render_display.render_time_start = 60.0 * 4.0 * float(int(1));
		

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


			// Instance 1
			
			{

				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(1)
					.set_position_start(10.0f, 0.0f, 0.0f)
					.set_position_end(10.0f, 0.0f, 0.0f)
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

				I.set_u_start_end(0, 4.237, 4.237);
			}


			});


		add_shader(scene, 7, [](Program::Shader& sh) {


			// Instance 0
			
			{
				auto id = sh.create_instance();
				auto I = sh.instance(id);
				I.set_group_size(1000, 1000, 1)
					.set_drawcalls(1)
					.set_position_start(0.0f, 10.0f, 0.0f)
					.set_position_end(0.0f, 10.0f, 0.0f)
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
					float v_start = 0.1f * static_cast<float>(u);
					float v_end = 1.0f - 0.1f * static_cast<float>(u);

					v_start = 0.0;
					v_end = 0.0;

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

		std::string name = "output_" + std::to_string(section);
		// Video::generate(name);
		
		
	}



	return 0;
}