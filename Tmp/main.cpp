#include <iostream>
#include <array>
#include <fstream>
#include <assert.h>

#include "../External_libs/tomlplusplus/tomlplusplus/toml.hpp"


#include "Scene.h"

// create scene
// set_camera_start
// set_camera_end

// LE_settings
// create_configuration_capture
// capture true
// capture_png true
// capture_bmp false
// 
// create shader
// vertex.shader ""
// fragment.shader "frag.0"
// create_instance
// set_uniforms
// u0 0.0 1.0
// u1 0.0 1.0
// u2 0.0 1.0
// u3 0.0 1.0
// u4 0.0 1.0
// u5 0.0 1.0
// u6 0.0 1.0
// u7 0.0 1.0
// u8 0.0 1.0
// u9 0.0 1.0
// u10 0.0 1.0
// create_transform
// x 0.0 0.0
// y 0.0 0.0
// z 0.0 0.0
// euler_x 0.0 0.0
// euler_y 0.0 0.0
// euler_z 0.0 0.0
// scale_x 0.0 0.0
// scale_y 0.0 0.0
// scale_z 0.0 0.0
// group_size_x 1000
// group_size_y 1000
// group_size_z 1
// number_of_drawcalls 4
// create_instance
// set_uniforms
// u0 0.0 1.0
// u1 0.0 1.0
// u2 0.0 1.0
// u3 0.0 1.0
// u4 0.0 1.0
// u5 0.0 1.0
// u6 0.0 1.0
// u7 0.0 1.0
// u8 0.0 1.0
// u9 0.0 1.0
// u10 0.0 1.0
// create_transform
// x 0.0 0.0
// y 0.0 0.0
// z 0.0 0.0
// euler_x 1.0 0.0
// euler_y 0.0 0.0
// euler_z 0.0 0.0
// scale_x 0.0 0.0
// scale_y 0.0 0.0
// scale_z 0.0 0.0
// group_size_x 1000
// group_size_y 1000
// group_size_z 1
// number_of_drawcalls 4
//
// 
// create shader
// vertex.shader ""
// fragment.shader "frag.0"
// set_uniforms
// u0 0.0 1.0
// u1 0.0 1.0
// u2 0.0 1.0
// u3 0.0 1.0
// u4 0.0 1.0
// u5 0.0 1.0
// u6 0.0 1.0
// u7 0.0 1.0
// u8 0.0 1.0
// u9 0.0 1.0
// u10 0.0 1.0
// create_transform
// x 0.0 0.0
// y 0.0 0.0
// z 0.0 0.0
// euler_x 0.0 0.0
// euler_y 0.0 0.0
// euler_z 0.0 0.0
// scale_x 0.0 0.0
// scale_y 0.0 0.0
// scale_z 0.0 0.0
// group_size_x 1000
// group_size_y 1000
// group_size_z 1
// number_of_drawcalls 4




int main()
{
	// int width = 1920 * 2;
	// int height = 1080 * 2;
	// std::string title = "My App";
	// bool vsync = true;
	// std::array<float, 3> clear_color{ 0.1f, 0.1f, 0.1f };

	Scene_::Callbacks callbacks;
	Scene_::Report report;

	Scene_::Scene scene = Scene_::load("example.toml", callbacks, report);
 
	Scene_::print(scene);

	
}

// TODO
// fix the bug of no valid shader for interactive mode 