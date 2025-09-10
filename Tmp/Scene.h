#pragma once
#include <string>
#include <vector>
#include <functional>

namespace Scene_ {

    struct Transform
    {
        float position_x = 0.0;
        float position_y = 0.0;
        float position_z = 0.0;
        float euler_x = 0.0;
        float euler_y = 0.0;
        float euler_z = 0.0;
        float scale_x = 1.0;
        float scale_y = 1.0;
        float scale_z = 1.0;
    };

    struct Instance
    {
        struct Uniforms
        {
            float u0 = 0.0;
            float u1 = 0.0;
            float u2 = 0.0;
            float u3 = 0.0;
            float u4 = 0.0;
            float u5 = 0.0;
            float u6 = 0.0;
            float u7 = 0.0;
            float u8 = 0.0;
            float u9 = 0.0;
        };

        Uniforms  uniforms_start;
        Uniforms  uniforms_end;

        Transform transform_start;
        Transform transform_end;

        int group_size_x = 1000;
        int group_size_y = 1000;
        int group_size_z = 1;

        int number_of_drawcalls = 4;
    };

    struct Shader
    {
        std::string fragment = "";
        std::string vertex = "";
        std::vector<Instance> instances;
    };

    struct Camera
    {
        float camera_x = 0.0f;
        float camera_y = 0.0f;
        float camera_z = 0.0f;
        float fov = 45.0f;
        float yew = 0.0f;
        float pitch = 0.0f;
    };

    struct LE_settings
    {
        float halfLife =  0.7f * 0.02f;
        float bloomThreshold = 1.4f;
        float bloomSoftKnee =  0.7f;
        float bloomStrength =  0.5f * 4.0f;
        int   bloomIterations = 6;
        float exposure = 0.7f * 2.0f;
        float gamma =  2.2f;
        float brightness = 0.0f;
        float contrast = 1.0f;
        float saturation = 1.0f;

        int   msaaSamples = 10;
    };

    struct CapturingSettings
    {
        bool capture_frames = false;
        bool capture_png = false;
        bool capture_bmp = false;
    };

    struct Callbacks
    {
        std::function<bool(std::string)> f_valid_file = nullptr;
        std::function<bool(std::string vertex_shader_file_path, std::string fragment_shader_file_path)> f_shader_valid = nullptr;
    };

    struct Scene
    {
        int width = 1920 * 2;
        int height = 1080 * 2;

        CapturingSettings capture_settings;
        LE_settings       le_settings;

        Camera            camera_start;
        Camera camera_end;

        std::vector<Shader> shaders;
    };
}
