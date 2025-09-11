#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <iomanip>

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
        float yaw = 0.0f;
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

        float start_time = 0.0f;
    };

    struct Callbacks
    {
        std::function<bool(std::string)> f_valid_file = nullptr;
        std::function<bool(std::string vertex_shader_file_path, std::string fragment_shader_file_path)> f_shader_valid = nullptr;
    };

    struct Scene
    {
    public:

        // ----------------- Render / output size -----------------
        void  set_frame_size(int w, int h) { width = w; height = h; }
        int   get_width() const { return width; }
        int   get_height() const { return height; }

        // ----------------- Render timing -----------------
        void  set_render_fps(int number) { render_fps = number; }
        int   get_render_fps() const { return render_fps; }

        void  set_number_of_frames(int number) { number_of_frames = number; }
        int   get_number_of_frames() const { return number_of_frames; }

        void  set_render_time_start(float v) { capture_settings.start_time = v; }
        float get_render_time_start() const { return capture_settings.start_time; }

        // ----------------- Capture toggles -----------------
        void set_capture(bool v) { capture_settings.capture_frames = v; }
        bool get_capture() const { return capture_settings.capture_frames; }

        void set_capture_png(bool v) { capture_settings.capture_png = v; }
        bool get_capture_png() const { return capture_settings.capture_png; }

        void set_capture_bmp(bool v) { capture_settings.capture_bmp = v; }
        bool get_capture_bmp() const { return capture_settings.capture_bmp; }


        // ----------------- LE (post) settings -----------------
        void  set_le_halfLife(float v) { le_settings.halfLife = v; }
        float get_le_halfLife() const { return le_settings.halfLife; }

        void  set_le_bloomThreshold(float v) { le_settings.bloomThreshold = v; }
        float get_le_bloomThreshold() const { return le_settings.bloomThreshold; }

        void  set_le_bloomSoftKnee(float v) { le_settings.bloomSoftKnee = v; }
        float get_le_bloomSoftKnee() const { return le_settings.bloomSoftKnee; }

        void  set_le_bloomStrength(float v) { le_settings.bloomStrength = v; }
        float get_le_bloomStrength() const { return le_settings.bloomStrength; }

        void  set_le_bloomIterations(int v) { le_settings.bloomIterations = v; }
        int   get_le_bloomIterations() const { return le_settings.bloomIterations; }

        void  set_le_exposure(float v) { le_settings.exposure = v; }
        float get_le_exposure() const { return le_settings.exposure; }

        void  set_le_gamma(float v) { le_settings.gamma = v; }
        float get_le_gamma() const { return le_settings.gamma; }

        void  set_le_brightness(float v) { le_settings.brightness = v; }
        float get_le_brightness() const { return le_settings.brightness; }

        void  set_le_contrast(float v) { le_settings.contrast = v; }
        float get_le_contrast() const { return le_settings.contrast; }

        void  set_le_saturation(float v) { le_settings.saturation = v; }
        float get_le_saturation() const { return le_settings.saturation; }

        void  set_le_msaaSamples(int v) { le_settings.msaaSamples = v; }
        int   get_le_msaaSamples() const { return le_settings.msaaSamples; }

        // ----------------- Camera start -----------------
        void  set_camera_start_x(float v) { camera_start.camera_x = v; }
        float get_camera_start_x() const { return camera_start.camera_x; }

        void  set_camera_start_y(float v) { camera_start.camera_y = v; }
        float get_camera_start_y() const { return camera_start.camera_y; }

        void  set_camera_start_z(float v) { camera_start.camera_z = v; }
        float get_camera_start_z() const { return camera_start.camera_z; }

        void  set_camera_start_pitch(float v) { camera_start.pitch = v; }
        float get_camera_start_pitch() const { return camera_start.pitch; }

        // alias pair: yaw/yew
        void  set_camera_start_yaw(float v) { camera_start.yaw = v; }
        float get_camera_start_yaw() const { return camera_start.yaw; }
        void  set_camera_start_yew(float v) { set_camera_start_yaw(v); }
        float get_camera_start_yew() const { return get_camera_start_yaw(); }

        void  set_camera_start_fov(float v) { camera_start.fov = v; }
        float get_camera_start_fov() const { return camera_start.fov; }

        // ----------------- Camera end -----------------
        void  set_camera_end_x(float v) { camera_end.camera_x = v; }
        float get_camera_end_x() const { return camera_end.camera_x; }

        void  set_camera_end_y(float v) { camera_end.camera_y = v; }
        float get_camera_end_y() const { return camera_end.camera_y; }

        void  set_camera_end_z(float v) { camera_end.camera_z = v; }
        float get_camera_end_z() const { return camera_end.camera_z; }

        void  set_camera_end_pitch(float v) { camera_end.pitch = v; }
        float get_camera_end_pitch() const { return camera_end.pitch; }

        // alias pair: yaw/yew
        void  set_camera_end_yaw(float v) { camera_end.yaw = v; }
        float get_camera_end_yaw() const { return camera_end.yaw; }
        void  set_camera_end_yew(float v) { set_camera_end_yaw(v); }
        float get_camera_end_yew() const { return get_camera_end_yaw(); }

        void  set_camera_end_fov(float v) { camera_end.fov = v; }
        float get_camera_end_fov() const { return camera_end.fov; }

        // ----------------- Shaders: container-level -----------------
        size_t shader_count() const { return shaders.size(); }

        // Add/remove shaders
        size_t add_shader(const std::string& vertex_path,
            const std::string& fragment_path)
        {
            Shader s;
            s.vertex = vertex_path;
            s.fragment = fragment_path;
            shaders.push_back(std::move(s));
            return shaders.size() - 1;
        }

        bool remove_shader(size_t shader_idx)
        {
            if (!valid_shader_index(shader_idx)) return false;
            shaders.erase(shaders.begin() + static_cast<std::ptrdiff_t>(shader_idx));
            return true;
        }

        void clear_shaders() { shaders.clear(); }

        // Shader paths
        bool set_shader_vertex_path(size_t shader_idx, const std::string& path)
        {
            if (!valid_shader_index(shader_idx)) return false;
            shaders[shader_idx].vertex = path;
            return true;
        }

        bool set_shader_fragment_path(size_t shader_idx, const std::string& path)
        {
            if (!valid_shader_index(shader_idx)) return false;
            shaders[shader_idx].fragment = path;
            return true;
        }

        std::string get_shader_vertex_path(size_t shader_idx) const
        {
            if (!valid_shader_index(shader_idx)) return {};
            return shaders[shader_idx].vertex;
        }

        std::string get_shader_fragment_path(size_t shader_idx) const
        {
            if (!valid_shader_index(shader_idx)) return {};
            return shaders[shader_idx].fragment;
        }

        // ----------------- Instances per shader -----------------
        size_t instance_count(size_t shader_idx) const
        {
            if (!valid_shader_index(shader_idx)) return 0;
            return shaders[shader_idx].instances.size();
        }

        // Create a default instance; returns its index
        size_t add_instance(size_t shader_idx, const Instance& inst = Instance{})
        {
            if (!valid_shader_index(shader_idx)) return static_cast<size_t>(-1);
            shaders[shader_idx].instances.push_back(inst);
            return shaders[shader_idx].instances.size() - 1;
        }

        bool remove_instance(size_t shader_idx, size_t instance_idx)
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return false;
            auto& v = shaders[shader_idx].instances;
            v.erase(v.begin() + static_cast<std::ptrdiff_t>(instance_idx));
            return true;
        }

        // -------- Instance: draw + group --------
        bool set_instance_group_size(size_t shader_idx, size_t instance_idx, int gx, int gy, int gz)
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return false;
            auto& inst = shaders[shader_idx].instances[instance_idx];
            inst.group_size_x = gx; inst.group_size_y = gy; inst.group_size_z = gz;
            return true;
        }

        std::tuple<int, int, int> get_instance_group_size(size_t shader_idx, size_t instance_idx) const
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return { 0,0,0 };
            const auto& inst = shaders[shader_idx].instances[instance_idx];
            return { inst.group_size_x, inst.group_size_y, inst.group_size_z };
        }

        bool set_instance_drawcalls(size_t shader_idx, size_t instance_idx, int n)
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return false;
            shaders[shader_idx].instances[instance_idx].number_of_drawcalls = n;
            return true;
        }

        int get_instance_drawcalls(size_t shader_idx, size_t instance_idx) const
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return 0;
            return shaders[shader_idx].instances[instance_idx].number_of_drawcalls;
        }

        // -------- Instance: transforms (start/end) --------
        bool set_instance_transform_start(size_t shader_idx, size_t instance_idx, const Transform& t)
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return false;
            shaders[shader_idx].instances[instance_idx].transform_start = t;
            return true;
        }
        bool set_instance_transform_end(size_t shader_idx, size_t instance_idx, const Transform& t)
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return false;
            shaders[shader_idx].instances[instance_idx].transform_end = t;
            return true;
        }

        Transform get_instance_transform_start(size_t shader_idx, size_t instance_idx) const
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return Transform{};
            return shaders[shader_idx].instances[instance_idx].transform_start;
        }
        Transform get_instance_transform_end(size_t shader_idx, size_t instance_idx) const
        {
            if (!valid_instance_index(shader_idx, instance_idx)) return Transform{};
            return shaders[shader_idx].instances[instance_idx].transform_end;
        }

        // Convenience setters for positions/scales if you don't want to build a Transform
        bool set_instance_position_start(size_t s, size_t i, float x, float y, float z)
        {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_start;
            t.position_x = x; t.position_y = y; t.position_z = z; return true;
        }
        bool set_instance_position_end(size_t s, size_t i, float x, float y, float z)
        {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_end;
            t.position_x = x; t.position_y = y; t.position_z = z; return true;
        }
        bool set_instance_scale_start(size_t s, size_t i, float sx, float sy, float sz)
        {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_start;
            t.scale_x = sx; t.scale_y = sy; t.scale_z = sz; return true;
        }
        bool set_instance_scale_end(size_t s, size_t i, float sx, float sy, float sz)
        {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_end;
            t.scale_x = sx; t.scale_y = sy; t.scale_z = sz; return true;
        }
        bool set_instance_euler_start(size_t s, size_t i, float ex, float ey, float ez)
        {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_start;
            t.euler_x = ex; t.euler_y = ey; t.euler_z = ez; return true;
        }
        bool set_instance_euler_end(size_t s, size_t i, float ex, float ey, float ez)
        {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_end;
            t.euler_x = ex; t.euler_y = ey; t.euler_z = ez; return true;
        }

        // -------- Instance: uniforms (start/end, u0..u9 by index) --------
        bool set_instance_uniform_start(size_t s, size_t i, int u_index, float value)
        {
            if (!valid_instance_index(s, i)) return false;
            return set_uniform_by_index(shaders[s].instances[i].uniforms_start, u_index, value);
        }
        bool set_instance_uniform_end(size_t s, size_t i, int u_index, float value)
        {
            if (!valid_instance_index(s, i)) return false;
            return set_uniform_by_index(shaders[s].instances[i].uniforms_end, u_index, value);
        }

        float get_instance_uniform_start(size_t s, size_t i, int u_index) const
        {
            if (!valid_instance_index(s, i)) return 0.0f;
            return get_uniform_by_index(shaders[s].instances[i].uniforms_start, u_index);
        }
        float get_instance_uniform_end(size_t s, size_t i, int u_index) const
        {
            if (!valid_instance_index(s, i)) return 0.0f;
            return get_uniform_by_index(shaders[s].instances[i].uniforms_end, u_index);
        }

        // Optional: direct (unsafe) references if you need to pass to render code
        Shader& ref_shader(size_t shader_idx) { return shaders[shader_idx]; }
        const Shader& ref_shader(size_t shader_idx) const { return shaders[shader_idx]; }


        // Pretty-print the entire scene state
        void print(std::ostream& os = std::cout) const
        {
            using std::fixed;
            using std::setprecision;

            auto b = [](bool v) { return v ? "true" : "false"; };
            auto ind = [&](int n) { for (int i = 0; i < n; ++i) os.put(' '); };

            os << fixed << setprecision(6);

            os << "Scene\n";
            ind(2); os << "resolution: " << width << " x " << height << "\n";

            // Render
            ind(2); os << "render:\n";
            ind(4); os << "fps: " << render_fps << "\n";
            ind(4); os << "number_of_frames: " << number_of_frames << "\n";
            ind(4); os << "time:\n";
            ind(6); os << "start: " << capture_settings.start_time << "\n";

            // Capture
            ind(2); os << "capture:\n";
            ind(4); os << "frames: " << b(capture_settings.capture_frames) << "\n";
            ind(4); os << "png:    " << b(capture_settings.capture_png) << "\n";
            ind(4); os << "bmp:    " << b(capture_settings.capture_bmp) << "\n";

            // LE / Post
            ind(2); os << "post (le):\n";
            ind(4); os << "halfLife: " << le_settings.halfLife << "\n";
            ind(4); os << "bloomThreshold: " << le_settings.bloomThreshold << "\n";
            ind(4); os << "bloomSoftKnee: " << le_settings.bloomSoftKnee << "\n";
            ind(4); os << "bloomStrength: " << le_settings.bloomStrength << "\n";
            ind(4); os << "bloomIterations: " << le_settings.bloomIterations << "\n";
            ind(4); os << "exposure: " << le_settings.exposure << "\n";
            ind(4); os << "gamma: " << le_settings.gamma << "\n";
            ind(4); os << "brightness: " << le_settings.brightness << "\n";
            ind(4); os << "contrast: " << le_settings.contrast << "\n";
            ind(4); os << "saturation: " << le_settings.saturation << "\n";
            ind(4); os << "msaaSamples: " << le_settings.msaaSamples << "\n";

            // Cameras
            auto print_cam = [&](const char* label, const Camera& c) {
                ind(2); os << "camera." << label << ":\n";
                ind(4); os << "x: " << c.camera_x << "\n";
                ind(4); os << "y: " << c.camera_y << "\n";
                ind(4); os << "z: " << c.camera_z << "\n";
                ind(4); os << "pitch: " << c.pitch << "\n";
                ind(4); os << "yew: " << c.yaw << "\n";
                ind(4); os << "fov: " << c.fov << "\n";
                };
            print_cam("start", camera_start);
            print_cam("end", camera_end);

            // Shaders
            ind(2); os << "shaders: " << shaders.size() << "\n";

            for (size_t si = 0; si < shaders.size(); ++si)
            {
                const auto& S = shaders[si];
                ind(4); os << "[" << si << "]:\n";
                ind(6); os << "vertex:   " << S.vertex << "\n";
                ind(6); os << "fragment: " << S.fragment << "\n";
                ind(6); os << "instances: " << S.instances.size() << "\n";

                for (size_t ii = 0; ii < S.instances.size(); ++ii)
                {
                    const auto& I = S.instances[ii];
                    ind(8); os << "[" << ii << "]:\n";

                    // Groups / drawcalls
                    ind(10); os << "groups: "
                        << I.group_size_x << " "
                        << I.group_size_y << " "
                        << I.group_size_z << "\n";
                    ind(10); os << "drawcalls: " << I.number_of_drawcalls << "\n";

                    // Transforms
                    auto print_xf = [&](const char* tag, const Transform& T) {
                        ind(10); os << tag << ":\n";
                        ind(12); os << "pos:   (" << T.position_x << ", "
                            << T.position_y << ", "
                            << T.position_z << ")\n";
                        ind(12); os << "euler: (" << T.euler_x << ", "
                            << T.euler_y << ", "
                            << T.euler_z << ")\n";
                        ind(12); os << "scale: (" << T.scale_x << ", "
                            << T.scale_y << ", "
                            << T.scale_z << ")\n";
                        };
                    print_xf("transform_start", I.transform_start);
                    print_xf("transform_end", I.transform_end);

                    // Uniforms (u0..u9)
                    auto print_uniforms = [&](const char* tag, const Instance::Uniforms& U) {
                        ind(10); os << tag << ":";
                        for (int ui = 0; ui <= 9; ++ui)
                        {
                            // reuse class helper
                            float v = get_uniform_by_index(U, ui);
                            os << " u" << ui << "=" << v;
                        }
                        os << "\n";
                        };
                    print_uniforms("uniforms_start", I.uniforms_start);
                    print_uniforms("uniforms_end", I.uniforms_end);
                }
            }

            os.flush();
        }

    private:
        // --------------- Helpers ---------------
        static bool set_uniform_by_index(Instance::Uniforms& u, int idx, float v)
        {
            switch (idx)
            {
            case 0: u.u0 = v; break; case 1: u.u1 = v; break; case 2: u.u2 = v; break;
            case 3: u.u3 = v; break; case 4: u.u4 = v; break; case 5: u.u5 = v; break;
            case 6: u.u6 = v; break; case 7: u.u7 = v; break; case 8: u.u8 = v; break;
            case 9: u.u9 = v; break; default: return false;
            }
            return true;
        }

        static float get_uniform_by_index(const Instance::Uniforms& u, int idx)
        {
            switch (idx)
            {
            case 0: return u.u0; case 1: return u.u1; case 2: return u.u2;
            case 3: return u.u3; case 4: return u.u4; case 5: return u.u5;
            case 6: return u.u6; case 7: return u.u7; case 8: return u.u8;
            case 9: return u.u9; default: return 0.0f;
            }
        }

        bool valid_shader_index(size_t s) const
        {
            return s < shaders.size();
        }

        bool valid_instance_index(size_t s, size_t i) const
        {
            if (!valid_shader_index(s)) return false;
            return i < shaders[s].instances.size();
        }

       

    private:
        int width = 1920 * 2;
        int height = 1080 * 2;

        int render_fps = 60;
        int number_of_frames = 100;

        CapturingSettings capture_settings;
        LE_settings       le_settings;

        Camera camera_start;
        Camera camera_end;

        std::vector<Shader> shaders;
    };

    
}
