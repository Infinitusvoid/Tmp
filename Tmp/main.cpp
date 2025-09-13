#include "CppCommponents/Folder.h"
#include "CppCommponents/File.h"

#include <assert.h>


#include <iostream>

#include "Scene.h"

const std::string folder_ouput = "C:/Users/Cosmos/Desktop/output/tmp/";
const std::string folder_output_shaders = folder_ouput + "shaders/";
const std::string folder_output_frames = folder_ouput + "frames/";
const std::string folder_output_commands = folder_ouput + "commands/";


void write_commands()
{
    std::string commands_example_string = R"glsl(
    

// render / display settings
render.width 3840
render.height 2160
render.fps 60
render.number_of_frames 100
render.time.start 0.0

// capture
capture true
capture.png true
capture.bmp false

// camera start
camera.start.x 0.0
camera.start.y 0.0
camera.start.z 0.0
camera.start.pitch 0.0
camera.start.yaw 0.0
camera.start.fov 45.0

// camera end
camera.end.x 0.0
camera.end.y 0.0
camera.end.z 0.0
camera.end.pitch 0.0
camera.end.yaw 0.0
camera.end.fov 45.0

// LE
le.halfLife 0.2
le.bloomThreshold 1.1
le.bloomSoftKnee 0.7
le.bloomStrength 2.0
le.bloomIterations 6
le.exposure 0.3
le.gamma 2.2
le.brightness 0.0
le.contrast 1.0
le.saturation 1.0

le.msaaSamples 4



// Shader 0
shader.add "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/tmp/default_shaders/vertex_shader.glsl" "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/tmp/default_shaders/fragment_shader.glsl"

# (Optional) override a path later
# shader.vertex 0 shaders/experiment_vs.glsl
# shader.fragment 0 shaders/experiment_fs.glsl

# =========================
# Instances — Shader 0
# =========================

# Instance 0 on Shader 0
shader.instance.add 0              # -> instance index 0
shader.instance.group 0 0 1000 1000 1

// shader 0
// instance 0
// drawcalls 4
shader.instance.drawcalls 0 0 4

# start transform
shader.instance.start.pos   0 0  0.000000  0.000000  0.000000
shader.instance.start.euler 0 0  0.000000  0.000000  0.000000
shader.instance.start.scale 0 0  1.000000  1.000000  1.000000

# end transform
shader.instance.end.pos     0 0  5.000000  0.000000  0.000000
shader.instance.end.euler   0 0  0.000000  1.570796  0.000000    # 90 deg about Y
shader.instance.end.scale   0 0  1.000000  1.000000  1.000000

# start uniforms (u0..u9)
shader.instance.start.u 0 0 0 0.000000
shader.instance.start.u 0 0 1 0.100000
shader.instance.start.u 0 0 2 0.200000
shader.instance.start.u 0 0 3 0.300000
shader.instance.start.u 0 0 4 0.400000
shader.instance.start.u 0 0 5 0.500000
shader.instance.start.u 0 0 6 0.600000
shader.instance.start.u 0 0 7 0.700000
shader.instance.start.u 0 0 8 0.800000
shader.instance.start.u 0 0 9 0.900000

# end uniforms (u0..u9)
shader.instance.end.u   0 0 0 1.000000
shader.instance.end.u   0 0 1 0.900000
shader.instance.end.u   0 0 2 0.800000
shader.instance.end.u   0 0 3 0.700000
shader.instance.end.u   0 0 4 0.600000
shader.instance.end.u   0 0 5 0.500000
shader.instance.end.u   0 0 6 0.400000
shader.instance.end.u   0 0 7 0.300000
shader.instance.end.u   0 0 8 0.200000
shader.instance.end.u   0 0 9 0.100000


















# -------------------------
# Instance 1 on Shader 0
// shader.instance.add 0              # -> instance index 1
// shader.instance.group 0 1 512 512 4
// shader.instance.drawcalls 0 1 8

# start transform
// shader.instance.start.pos   0 1  -2.000000  0.500000  1.000000
// shader.instance.start.euler 0 1   0.100000  0.200000  0.300000
// shader.instance.start.scale 0 1   0.500000  0.500000  0.500000

# end transform
// shader.instance.end.pos     0 1   2.000000  1.000000 -1.000000
// shader.instance.end.euler   0 1   0.300000  0.200000  0.100000
// shader.instance.end.scale   0 1   1.500000  1.500000  1.500000

# a few uniforms
// shader.instance.start.u 0 1 0 0.250000
// shader.instance.start.u 0 1 1 0.750000
// shader.instance.end.u   0 1 0 0.900000
// shader.instance.end.u   0 1 1 0.100000

# =========================
# Instances — Shader 1
# =========================

# Instance 0 on Shader 1
// shader.instance.add 1              # -> instance index 0
// shader.instance.group 1 0 256 256 1
// shader.instance.drawcalls 1 0 2

# start transform
// shader.instance.start.pos   1 0   0.000000  0.000000  5.000000
// shader.instance.start.euler 1 0   0.000000  0.000000  0.000000
// shader.instance.start.scale 1 0   2.000000  2.000000  2.000000

# end transform
// shader.instance.end.pos     1 0   0.000000  0.000000  5.000000
// shader.instance.end.euler   1 0   0.000000  6.283185  0.000000    # 360 deg about Y
// shader.instance.end.scale   1 0   2.000000  2.000000  2.000000

# full uniform sweep start/end
// shader.instance.start.u 1 0 0 0.000000
// shader.instance.start.u 1 0 1 0.111111
// shader.instance.start.u 1 0 2 0.222222
// shader.instance.start.u 1 0 3 0.333333
// shader.instance.start.u 1 0 4 0.444444
// shader.instance.start.u 1 0 5 0.555555
// shader.instance.start.u 1 0 6 0.666666
// shader.instance.start.u 1 0 7 0.777777
// shader.instance.start.u 1 0 8 0.888888
// shader.instance.start.u 1 0 9 1.000000

// shader.instance.end.u   1 0 0 1.000000
// shader.instance.end.u   1 0 1 0.888888
// shader.instance.end.u   1 0 2 0.777777
// shader.instance.end.u   1 0 3 0.666666
// shader.instance.end.u   1 0 4 0.555555
// shader.instance.end.u   1 0 5 0.444444
// shader.instance.end.u   1 0 6 0.333333
// shader.instance.end.u   1 0 7 0.222222
// shader.instance.end.u   1 0 8 0.111111
// shader.instance.end.u   1 0 9 0.000000



    )glsl";

    File::writeFile_OverrideIfExistAlready(folder_output_commands + "commands.txt", commands_example_string);
}

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
    const std::string vs =
        "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/tmp/default_shaders/vertex_shader.glsl";
    const std::string fs =
        "C:/Users/Cosmos/Documents/GitHub/Tmp/Tmp/tmp/default_shaders/fragment_shader.glsl";
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

