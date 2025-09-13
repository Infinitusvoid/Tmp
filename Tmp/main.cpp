#include "CppCommponents/Folder.h"
#include "CppCommponents/File.h"

#include "Utils.h"

#include <assert.h>


#include <iostream>

#include "Scene.h"

const std::string folder_ouput = "C:/Users/Cosmos/Desktop/output/tmp/";
const std::string folder_output_shaders = folder_ouput + "shaders/";
const std::string folder_output_frames = folder_ouput + "frames/";
const std::string folder_output_commands = folder_ouput + "commands/";



struct ShadersFilepath
{
    ShadersFilepath()
    {

    }


    std::string get_vertex(int index)
    {
        return ShadersFilepath::filepath + "vertex_" + std::to_string(index) + std::string(".glsl");
    }

    std::string get_freagment(int index)
    {
        return ShadersFilepath::filepath + "fragment_" + std::to_string(index) + std::string(".glsl");
    }


private:
    const std::string filepath = "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/";
};



ShadersFilepath shaders;



void write_commands_using_scene()
{
    Scene_::Scene scene = Scene_::Scene();
    
    // ----- render / display settings -----
    scene.set_width(1920 * 2);
    scene.set_height(1080 * 2);
    scene.set_render_fps(60);
    scene.set_number_of_frames(100);
    scene.set_render_time_start(0.0f);

    // ----- capture -----
    scene.set_capture(false);
    scene.set_capture_png(true);
    scene.set_capture_bmp(false);

    // ----- camera start -----
    scene.set_camera_start_x(0.0f);
    scene.set_camera_start_y(0.0f);
    scene.set_camera_start_z(0.0f);
    scene.set_camera_start_pitch(0.0f);
    scene.set_camera_start_yaw(0.0f);
    scene.set_camera_start_fov(45.0f);

    // ----- camera end -----
    scene.set_camera_end_x(0.0f);
    scene.set_camera_end_y(0.0f);
    scene.set_camera_end_z(0.0f);
    scene.set_camera_end_pitch(0.0f);
    scene.set_camera_end_yaw(0.0f);
    scene.set_camera_end_fov(45.0f);

    // ----- LE settings -----
    scene.set_le_halfLife(0.2f);
    scene.set_le_bloomThreshold(1.1f);
    scene.set_le_bloomSoftKnee(0.7f);
    scene.set_le_bloomStrength(2.0f);
    scene.set_le_bloomIterations(6);
    scene.set_le_exposure(0.3f);
    scene.set_le_gamma(2.2f);
    scene.set_le_brightness(0.0f);
    scene.set_le_contrast(1.0f);
    scene.set_le_saturation(1.0f);
    scene.set_le_msaaSamples(4);


    // ----- Shader 0 -----
    // const std::string vs = "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/tmp/default_shaders/vertex_shader.glsl";
    std::string vs = shaders.get_vertex(0);
       
    // const std::string fs = "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/tmp/default_shaders/fragment_shader.glsl";
    std::string fs = shaders.get_freagment(0);
    const size_t shader0 = scene.add_shader(vs, fs);

    // sanity: you can rely on the returned index instead of assuming 0
    assert(scene.has_shader(shader0));

    // ----- Instances — Shader 0 -----
    const size_t inst0 = scene.add_instance(shader0);

    // group size 1000 1000 1
    assert(scene.set_instance_group_size(shader0, inst0, 1000, 1000, 1));

    // drawcalls 4
    assert(scene.set_instance_drawcalls(shader0, inst0, 4));

    // start transform
    assert(scene.set_instance_position_start(shader0, inst0, 0.0f, 0.0f, 0.0f));
    assert(scene.set_instance_euler_start(shader0, inst0, 0.0f, 0.0f, 0.0f));
    assert(scene.set_instance_scale_start(shader0, inst0, 1.0f, 1.0f, 1.0f));

    // end transform
    assert(scene.set_instance_position_end(shader0, inst0, 5.0f, 0.0f, 0.0f));
    assert(scene.set_instance_euler_end(shader0, inst0, 0.0f, 1.570796f, 0.0f)); // 90° about Y
    assert(scene.set_instance_scale_end(shader0, inst0, 1.0f, 1.0f, 1.0f));

    // start uniforms (u0..u9) = 0.0, 0.1, ..., 0.9
    for (int u = 0; u <= 9; ++u) {
        const float v = 0.1f * static_cast<float>(u);
        assert(scene.set_instance_uniform_start(shader0, inst0, u, v));
    }

    // end uniforms (u0..u9) = 1.0, 0.9, ..., 0.1
    for (int u = 0; u <= 9; ++u) {
        const float v = 1.0f - 0.1f * static_cast<float>(u);
        assert(scene.set_instance_uniform_end(shader0, inst0, u, v));
    }

    // optional: print to verify
    // scene.print();

    std::string filepath = folder_output_commands + "commands.txt";
    Scene_::save(scene, filepath);
}



int main(int argc, char* argv[])
{
    std::cout << "Tmp\n";
    
    Folder::create_folder_if_does_not_exist_already(folder_ouput);
    Folder::create_folder_if_does_not_exist_already(folder_output_shaders);
    Folder::create_folder_if_does_not_exist_already(folder_output_frames);
    Folder::create_folder_if_does_not_exist_already(folder_output_commands);
    
    // write_commands();
    write_commands_using_scene();

    if(true)
    {
        // Example: run "C:\Tools\my app\tool.exe" --mode test "C:\path with spaces\file.txt"
        std::string cmd = std::string("LightPainting.exe") + std::string(" ") + folder_output_commands + std::string("commands.txt") + std::string(" ") + std::string(folder_output_frames);
            // R"("LightPainting.exe"";      // quote the exe path
            // " " + folder_output_commands + "commands.txt)";
            // R"("C:\path with spaces\file.txt")"; // quote args with spaces

        int exitCode = std::system(cmd.c_str());
    }
    


    return 0;
}

