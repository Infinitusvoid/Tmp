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

struct Video
{
    static int generate()
    {
        FFmpeg_::FfmpegImageToVideo job;
        job.input_dir = folder_output_frames; // "C:/renders/shot01";     // folder with frame_000000.png etc.
        job.input_pattern = "frame_%06d.png";        // change if your naming differs
        job.start_number = 0;
        job.input_fps = 60;
        job.output_fps = 60;
        job.preset = "slow";
        job.crf = 14;
        job.pix_fmt = "yuv420p";
        job.faststart = true;
        job.output_path = folder_output_frames + std::string("output.mp4"); //"C:/renders/shot01/output.mp4";

        return job.run();
    }
};

struct Camera
{

    void init()
    {
        // CAPTURED: { pos: [0.187691, -0.043775, 0.381654] , yaw : -24.120005, pitch : 5.879999, fov : 45.000000 }

        x = 0.187691;
        y = -0.043775;
        z = 0.381654;
        yaw = -24.120005;
        pitch = 5.879999;
        fov = 45.000000;
    }


    void init2()
    {
        // CAPTURED: { pos: [2.695665, -0.675831, 5.983052] , yaw : -24.120005, pitch : 5.879999, fov : 45.000000 }
        
        x = 2.695665;
        y = -0.675831;
        z = 5.983052;
        yaw = -24.120005;
        pitch = 5.879999;
        fov = 45.000000;
    }

    void init3()
    {
        // CAPTURED: { pos:[-1.245056,-2.551750,19.370687], yaw:5.759995, pitch:4.920000, fov:45.000000 }

        x = -1.245056;
        y = -2.551750;
        z = 19.370687;
        yaw = 5.759995;
        pitch = 4.920000;
        fov = 45.000000;
    }

    double x = 0;
    double y = 0;
    double z = 0;
    double yaw = 0;
    double pitch = 0;
    double fov = 45;

};

void write_commands_using_scene(Camera camera_start, Camera camera_end)
{
    Scene_::Scene scene = Scene_::Scene();
    
    // ----- render / display settings -----
    scene.set_width(1920 * 2);
    scene.set_height(1080 * 2);
    scene.set_render_fps(60);
    scene.set_number_of_frames(100);
    scene.set_render_time_start(0.0f);

    // ----- capture -----
    scene.set_capture(true);
    scene.set_capture_png(true);
    scene.set_capture_bmp(false);

    // ----- camera start -----
    scene.set_camera_start_x(camera_start.x);
    scene.set_camera_start_y(camera_start.y);
    scene.set_camera_start_z(camera_start.z);
    scene.set_camera_start_pitch(camera_start.pitch);
    scene.set_camera_start_yaw(camera_start.yaw);
    scene.set_camera_start_fov(camera_start.fov);

    // ----- camera end -----
    scene.set_camera_end_x(camera_end.x);
    scene.set_camera_end_y(camera_end.y);
    scene.set_camera_end_z(camera_end.z);
    scene.set_camera_end_pitch(camera_end.pitch);
    scene.set_camera_end_yaw(camera_end.yaw);
    scene.set_camera_end_fov(camera_end.fov);

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
    {
        std::string vs = shaders.get_vertex(1);
        std::string fs = shaders.get_freagment(1);

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
        assert(scene.set_instance_position_end(shader0, inst0, 0.0f, 0.0f, 0.0f));
        assert(scene.set_instance_euler_end(shader0, inst0, 0.0f, 0.0f, 0.0f)); // 90° about Y
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
    }
   

    // optional: print to verify
    // scene.print();

    std::string filepath = folder_output_commands + "commands.txt";
    Scene_::save(scene, filepath);
}

void generate_video(int fps)
{
    
    // ffmpeg -framerate 60 -start_number 0 -i "frame_%06d.png" -c:v libx264 -preset slow -crf 14 -pix_fmt yuv420p -r 60 -movflags +faststart output.mp4

    int start_number = 0;

    std::string output_name = "output.mp4";

    std::string("ffmpeg -framerate ") + std::to_string(fps) + std::string(" -start_number ") + std::to_string(start_number) + std::string(" -i \"frame_%06d.png\" -c:v libx264 -preset slow -crf 14 -pix_fmt yuv420p -r 60 -movflags +faststart ") + output_name;
}

int main(int argc, char* argv[])
{
    std::cout << "Tmp\n";
    
    Folder::create_folder_if_does_not_exist_already(folder_ouput);
    Folder::create_folder_if_does_not_exist_already(folder_output_shaders);
    Folder::create_folder_if_does_not_exist_already(folder_output_frames);
    Folder::create_folder_if_does_not_exist_already(folder_output_commands);
    
    // write_commands();

    Camera camera_start;
    camera_start.init2();
    Camera camera_end;
    camera_end.init3();

    write_commands_using_scene(camera_start, camera_end);

    if(true)
    {
        // Example: run "C:\Tools\my app\tool.exe" --mode test "C:\path with spaces\file.txt"
        std::string cmd = std::string("LightPainting.exe") + std::string(" ") + folder_output_commands + std::string("commands.txt") + std::string(" ") + std::string(folder_output_frames);
            // R"("LightPainting.exe"";      // quote the exe path
            // " " + folder_output_commands + "commands.txt)";
            // R"("C:\path with spaces\file.txt")"; // quote args with spaces

        int exitCode = std::system(cmd.c_str());
    }
    
    
    {
        Video::generate();
    }
    

    return 0;
}

