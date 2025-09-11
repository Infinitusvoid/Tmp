#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <iomanip>
#include <string>
#include <utility>   // std::move
#include <cstddef>   // std::ptrdiff_t

// add these at the top of the file (with your other includes)
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip> // for std::quoted

namespace Scene_
{

    
    struct Scene
    {
    private:
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
            float halfLife = 0.7f * 0.02f;
            float bloomThreshold = 1.4f;
            float bloomSoftKnee = 0.7f;
            float bloomStrength = 0.5f * 4.0f;
            int   bloomIterations = 6;
            float exposure = 0.7f * 2.0f;
            float gamma = 2.2f;
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

    public:

        static constexpr size_t kInvalidIndex = static_cast<size_t>(-1);

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

        // yaw
        void  set_camera_start_yaw(float v) { camera_start.yaw = v; }
        float get_camera_start_yaw() const { return camera_start.yaw; }
        
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

        // alias pair: yaw
        void  set_camera_end_yaw(float v) { camera_end.yaw = v; }
        float get_camera_end_yaw() const { return camera_end.yaw; }
        
        void  set_camera_end_fov(float v) { camera_end.fov = v; }
        float get_camera_end_fov() const { return camera_end.fov; }


        // ----------------- Shaders: container-level -----------------
        size_t shader_count() const { return shaders.size(); }

        size_t add_shader(const char* vertex_path, const char* fragment_path) {
            Shader s;
            s.vertex = vertex_path ? vertex_path : "";
            s.fragment = fragment_path ? fragment_path : "";
            shaders.push_back(std::move(s));
            return shaders.size() - 1;
        }

        // overloads for std::string if you prefer
        size_t add_shader(const std::string& vertex_path, const std::string& fragment_path) {
            return add_shader(vertex_path.c_str(), fragment_path.c_str());
        }

        bool remove_shader(size_t shader_idx) {
            if (!valid_shader_index(shader_idx)) return false;
            shaders.erase(shaders.begin() + static_cast<std::ptrdiff_t>(shader_idx));
            return true;
        }
        void clear_shaders() { shaders.clear(); }

        bool set_shader_vertex_path(size_t shader_idx, const char* path) {
            if (!valid_shader_index(shader_idx)) return false;
            shaders[shader_idx].vertex = path ? path : "";
            return true;
        }
        bool set_shader_fragment_path(size_t shader_idx, const char* path) {
            if (!valid_shader_index(shader_idx)) return false;
            shaders[shader_idx].fragment = path ? path : "";
            return true;
        }
        std::string get_shader_vertex_path(size_t shader_idx) const {
            if (!valid_shader_index(shader_idx)) return {};
            return shaders[shader_idx].vertex;
        }
        std::string get_shader_fragment_path(size_t shader_idx) const {
            if (!valid_shader_index(shader_idx)) return {};
            return shaders[shader_idx].fragment;
        }

        // ----------------- Instances per shader -----------------
        size_t instance_count(size_t shader_idx) const {
            if (!valid_shader_index(shader_idx)) return 0;
            return shaders[shader_idx].instances.size();
        }

        

        // ONLY built-ins: create default instance and return its index
        size_t add_instance(size_t shader_idx) {
            if (!valid_shader_index(shader_idx)) return static_cast<size_t>(kInvalidIndex);
            shaders[shader_idx].instances.emplace_back();            // default-constructed Instance
            return shaders[shader_idx].instances.size() - 1;
        }

        bool remove_instance(size_t shader_idx, size_t instance_idx) {
            if (!valid_instance_index(shader_idx, instance_idx)) return false;
            auto& v = shaders[shader_idx].instances;
            v.erase(v.begin() + static_cast<std::ptrdiff_t>(instance_idx));
            return true;
        }

        // -------- Instance: groups & drawcalls --------
        bool set_instance_group_size(size_t s, size_t i, int gx, int gy, int gz) {
            if (!valid_instance_index(s, i)) return false;
            auto& inst = shaders[s].instances[i];
            inst.group_size_x = gx; inst.group_size_y = gy; inst.group_size_z = gz;
            return true;
        }
        bool get_instance_group_size(size_t s, size_t i, int& gx, int& gy, int& gz) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& inst = shaders[s].instances[i];
            gx = inst.group_size_x; gy = inst.group_size_y; gz = inst.group_size_z;
            return true;
        }
        bool set_instance_drawcalls(size_t s, size_t i, int n) {
            if (!valid_instance_index(s, i)) return false;
            shaders[s].instances[i].number_of_drawcalls = n;
            return true;
        }
        bool get_instance_drawcalls(size_t s, size_t i, int& n) const {
            if (!valid_instance_index(s, i)) return false;
            n = shaders[s].instances[i].number_of_drawcalls;
            return true;
        }

        // -------- Instance: transforms (start) --------
        bool set_instance_position_start(size_t s, size_t i, float x, float y, float z) {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_start;
            t.position_x = x; t.position_y = y; t.position_z = z; return true;
        }
        bool set_instance_euler_start(size_t s, size_t i, float ex, float ey, float ez) {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_start;
            t.euler_x = ex; t.euler_y = ey; t.euler_z = ez; return true;
        }
        bool set_instance_scale_start(size_t s, size_t i, float sx, float sy, float sz) {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_start;
            t.scale_x = sx; t.scale_y = sy; t.scale_z = sz; return true;
        }
        bool get_instance_position_start(size_t s, size_t i, float& x, float& y, float& z) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& t = shaders[s].instances[i].transform_start;
            x = t.position_x; y = t.position_y; z = t.position_z; return true;
        }
        bool get_instance_euler_start(size_t s, size_t i, float& ex, float& ey, float& ez) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& t = shaders[s].instances[i].transform_start;
            ex = t.euler_x; ey = t.euler_y; ez = t.euler_z; return true;
        }
        bool get_instance_scale_start(size_t s, size_t i, float& sx, float& sy, float& sz) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& t = shaders[s].instances[i].transform_start;
            sx = t.scale_x; sy = t.scale_y; sz = t.scale_z; return true;
        }

        // -------- Instance: transforms (end) --------
        bool set_instance_position_end(size_t s, size_t i, float x, float y, float z) {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_end;
            t.position_x = x; t.position_y = y; t.position_z = z; return true;
        }
        bool set_instance_euler_end(size_t s, size_t i, float ex, float ey, float ez) {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_end;
            t.euler_x = ex; t.euler_y = ey; t.euler_z = ez; return true;
        }
        bool set_instance_scale_end(size_t s, size_t i, float sx, float sy, float sz) {
            if (!valid_instance_index(s, i)) return false;
            auto& t = shaders[s].instances[i].transform_end;
            t.scale_x = sx; t.scale_y = sy; t.scale_z = sz; return true;
        }
        bool get_instance_position_end(size_t s, size_t i, float& x, float& y, float& z) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& t = shaders[s].instances[i].transform_end;
            x = t.position_x; y = t.position_y; z = t.position_z; return true;
        }
        bool get_instance_euler_end(size_t s, size_t i, float& ex, float& ey, float& ez) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& t = shaders[s].instances[i].transform_end;
            ex = t.euler_x; ey = t.euler_y; ez = t.euler_z; return true;
        }
        bool get_instance_scale_end(size_t s, size_t i, float& sx, float& sy, float& sz) const {
            if (!valid_instance_index(s, i)) return false;
            const auto& t = shaders[s].instances[i].transform_end;
            sx = t.scale_x; sy = t.scale_y; sz = t.scale_z; return true;
        }

        // -------- Instance: uniforms (start/end, u0..u9 by index) --------
        bool set_instance_uniform_start(size_t s, size_t i, int u_index, float value) {
            if (!valid_instance_index(s, i)) return false;
            return set_uniform_by_index(shaders[s].instances[i].uniforms_start, u_index, value);
        }
        bool set_instance_uniform_end(size_t s, size_t i, int u_index, float value) {
            if (!valid_instance_index(s, i)) return false;
            return set_uniform_by_index(shaders[s].instances[i].uniforms_end, u_index, value);
        }

        bool get_instance_uniform_start(size_t s, size_t i, int u_index, float& out_value) const {
            if (!valid_instance_index(s, i) || u_index < 0 || u_index > 9) return false;
            out_value = get_uniform_by_index(shaders[s].instances[i].uniforms_start, u_index);
            return true;
        }
        bool get_instance_uniform_end(size_t s, size_t i, int u_index, float& out_value) const {
            if (!valid_instance_index(s, i) || u_index < 0 || u_index > 9) return false;
            out_value = get_uniform_by_index(shaders[s].instances[i].uniforms_end, u_index);
            return true;
        }

        // --- Image/output size: explicit setters/getters ---
        void setWidth(int w) { width = w; }
        void setHeight(int h) { height = h; }
        int  getWidth() const { return width; }
        int  getHeight() const { return height; }

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
                ind(4); os << "yaw: " << c.yaw << "\n";
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












namespace Scene_ {

    static inline std::string trim_copy(std::string s) {
        auto issp = [](unsigned char c) { return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char c) { return !issp(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](unsigned char c) { return !issp(c); }).base(), s.end());
        return s;
    }

    static inline void strip_comments(std::string& s) {
        auto ph = s.find('#');
        auto ps = s.find("//");
        size_t cut = std::string::npos;
        if (ph != std::string::npos) cut = ph;
        if (ps != std::string::npos) cut = (cut == std::string::npos) ? ps : std::min(cut, ps);
        if (cut != std::string::npos) s.erase(cut);
    }

    static inline bool parse_bool(const std::string& in, bool& out) {
        std::string s = in;
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)std::tolower(c); });
        if (s == "true" || s == "1" || s == "yes" || s == "on") { out = true;  return true; }
        if (s == "false" || s == "0" || s == "no" || s == "off") { out = false; return true; }
        return false;
    }
    static inline bool parse_int(const std::string& in, int& out) { std::istringstream iss(in); iss >> out; return (bool)iss && iss.eof(); }
    static inline bool parse_float(const std::string& in, float& out) { std::istringstream iss(in); iss >> out; return (bool)iss && iss.eof(); }

    std::optional<Scene> load(std::string filepath)
    {
        Scene scene;
        scene.clear_shaders();

        std::ifstream in(filepath);
        if (!in) {
            std::cerr << "[Scene::load] ERROR: cannot open file: " << filepath << "\n";
            return std::nullopt;
        }

        std::string line; int lineno = 0;

        while (std::getline(in, line)) {
            ++lineno;

            // Strip BOM on first line if present
            if (lineno == 1 && line.size() >= 3 &&
                (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
                line.erase(0, 3);
            }

            strip_comments(line);
            line = trim_copy(line);
            if (line.empty()) continue;

            // Split into key + rest
            std::string key, rest;
            {
                std::istringstream iss(line);
                iss >> key;
                std::getline(iss, rest);
                if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
                rest = trim_copy(rest);
            }

            // alias: camrea.* -> camera.*
            if (key.rfind("camrea.", 0) == 0) key.replace(0, 7, "camera.");

            auto warn = [&](const char* msg) {
                std::cerr << "[Scene::load] line " << lineno << ": " << msg << "  (" << key << " " << rest << ")\n";
                };

            // ===== render.* =====
            if (key == "render.fps" || key == "render.framerate") {
                int v; if (parse_int(rest, v)) scene.set_render_fps(v); else warn("expected int");
            }
            else if (key == "render.number_of_frames") {
                int v; if (parse_int(rest, v)) scene.set_number_of_frames(v); else warn("expected int");
            }
            else if (key == "render.time.start") {
                float v; if (parse_float(rest, v)) scene.set_render_time_start(v); else warn("expected float");
            }

            // ===== capture.* =====
            else if (key == "capture") {
                bool v; if (parse_bool(rest, v)) scene.set_capture(v); else warn("expected bool");
            }
            else if (key == "capture.png") {
                bool v; if (parse_bool(rest, v)) scene.set_capture_png(v); else warn("expected bool");
            }
            else if (key == "capture.bmp") {
                bool v; if (parse_bool(rest, v)) scene.set_capture_bmp(v); else warn("expected bool");
            }

            // ===== camera.start.* =====
            else if (key.rfind("camera.start.", 0) == 0) {
                std::string prop = key.substr(std::string("camera.start.").size());
                float v; if (!parse_float(rest, v)) { warn("expected float"); continue; }

                if (prop == "x")     scene.set_camera_start_x(v);
                else if (prop == "y")     scene.set_camera_start_y(v);
                else if (prop == "z")     scene.set_camera_start_z(v);
                else if (prop == "pitch") scene.set_camera_start_pitch(v);
                else if (prop == "yaw" || prop == "yew") scene.set_camera_start_yaw(v);
                else if (prop == "fov")   scene.set_camera_start_fov(v);
                else warn("unknown camera.start.* key");
            }

            // ===== camera.end.* =====
            else if (key.rfind("camera.end.", 0) == 0) {
                std::string prop = key.substr(std::string("camera.end.").size());
                float v; if (!parse_float(rest, v)) { warn("expected float"); continue; }

                if (prop == "x")     scene.set_camera_end_x(v);
                else if (prop == "y")     scene.set_camera_end_y(v);
                else if (prop == "z")     scene.set_camera_end_z(v);
                else if (prop == "pitch") scene.set_camera_end_pitch(v);
                else if (prop == "yaw" || prop == "yew") scene.set_camera_end_yaw(v);
                else if (prop == "fov")   scene.set_camera_end_fov(v);
                else warn("unknown camera.end.* key");
            }

            // ===== le.* =====
            else if (key.rfind("le.", 0) == 0) {
                std::string prop = key.substr(3);
                if (prop == "msaaSamples") {
                    int vi; if (parse_int(rest, vi)) scene.set_le_msaaSamples(vi); else warn("expected int");
                }
                else if (prop == "bloomIterations") {
                    int vi; if (parse_int(rest, vi)) scene.set_le_bloomIterations(vi); else warn("expected int");
                }
                else {
                    float vf;
                    if (!parse_float(rest, vf)) { warn("expected float"); continue; }
                    if (prop == "halfLife")       scene.set_le_halfLife(vf);
                    else if (prop == "bloomThreshold") scene.set_le_bloomThreshold(vf);
                    else if (prop == "bloomSoftKnee")  scene.set_le_bloomSoftKnee(vf);
                    else if (prop == "bloomStrength")  scene.set_le_bloomStrength(vf);
                    else if (prop == "exposure")       scene.set_le_exposure(vf);
                    else if (prop == "gamma")          scene.set_le_gamma(vf);
                    else if (prop == "brightness")     scene.set_le_brightness(vf);
                    else if (prop == "contrast")       scene.set_le_contrast(vf);
                    else if (prop == "saturation")     scene.set_le_saturation(vf);
                    else warn("unknown le.* key");
                }
            }

            // ===== shader.add / shader.vertex / shader.fragment =====
            else if (key == "shader.add") {
                std::istringstream ss(rest);
                std::string vtx, frag;
                if (!(ss >> std::quoted(vtx))) { ss.clear(); ss.str(rest); if (!(ss >> vtx)) { warn("usage: shader.add <vertex> <fragment>"); continue; } }
                if (!(ss >> std::quoted(frag))) { if (!(ss >> frag)) { warn("usage: shader.add <vertex> <fragment>"); continue; } }
                scene.add_shader(vtx, frag);
            }
            else if (key == "shader.vertex") {
                std::istringstream ss(rest);
                size_t idx; if (!(ss >> idx)) { warn("usage: shader.vertex <index> <path>"); continue; }
                std::string path;
                if (!(ss >> std::quoted(path))) { if (!(ss >> path)) { warn("usage: shader.vertex <index> <path>"); continue; } }
                if (!scene.set_shader_vertex_path(idx, path.c_str())) warn("invalid shader index");
            }
            else if (key == "shader.fragment") {
                std::istringstream ss(rest);
                size_t idx; if (!(ss >> idx)) { warn("usage: shader.fragment <index> <path>"); continue; }
                std::string path;
                if (!(ss >> std::quoted(path))) { if (!(ss >> path)) { warn("usage: shader.fragment <index> <path>"); continue; } }
                if (!scene.set_shader_fragment_path(idx, path.c_str())) warn("invalid shader index");
            }

            // ===== shader.instance.* =====
            else if (key == "shader.instance.add") {
                std::istringstream ss(rest);
                size_t s; if (!(ss >> s)) { warn("usage: shader.instance.add <shader_idx>"); continue; }
                size_t idx = scene.add_instance(s);
                if (idx == static_cast<size_t>(-1)) warn("invalid shader index");
            }
            else if (key == "shader.instance.group") {
                std::istringstream ss(rest);
                size_t s, i; int gx, gy, gz;
                if (!(ss >> s >> i >> gx >> gy >> gz)) { warn("usage: shader.instance.group <shader_idx> <instance_idx> <gx> <gy> <gz>"); continue; }
                if (!scene.set_instance_group_size(s, i, gx, gy, gz)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.drawcalls") {
                std::istringstream ss(rest);
                size_t s, i; int n;
                if (!(ss >> s >> i >> n)) { warn("usage: shader.instance.drawcalls <shader_idx> <instance_idx> <n>"); continue; }
                if (!scene.set_instance_drawcalls(s, i, n)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.start.pos") {
                std::istringstream ss(rest);
                size_t s, i; float x, y, z;
                if (!(ss >> s >> i >> x >> y >> z)) { warn("usage: shader.instance.start.pos <s> <i> <x> <y> <z>"); continue; }
                if (!scene.set_instance_position_start(s, i, x, y, z)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.start.euler") {
                std::istringstream ss(rest);
                size_t s, i; float ex, ey, ez;
                if (!(ss >> s >> i >> ex >> ey >> ez)) { warn("usage: shader.instance.start.euler <s> <i> <ex> <ey> <ez>"); continue; }
                if (!scene.set_instance_euler_start(s, i, ex, ey, ez)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.start.scale") {
                std::istringstream ss(rest);
                size_t s, i; float sx, sy, sz;
                if (!(ss >> s >> i >> sx >> sy >> sz)) { warn("usage: shader.instance.start.scale <s> <i> <sx> <sy> <sz>"); continue; }
                if (!scene.set_instance_scale_start(s, i, sx, sy, sz)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.end.pos") {
                std::istringstream ss(rest);
                size_t s, i; float x, y, z;
                if (!(ss >> s >> i >> x >> y >> z)) { warn("usage: shader.instance.end.pos <s> <i> <x> <y> <z>"); continue; }
                if (!scene.set_instance_position_end(s, i, x, y, z)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.end.euler") {
                std::istringstream ss(rest);
                size_t s, i; float ex, ey, ez;
                if (!(ss >> s >> i >> ex >> ey >> ez)) { warn("usage: shader.instance.end.euler <s> <i> <ex> <ey> <ez>"); continue; }
                if (!scene.set_instance_euler_end(s, i, ex, ey, ez)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.end.scale") {
                std::istringstream ss(rest);
                size_t s, i; float sx, sy, sz;
                if (!(ss >> s >> i >> sx >> sy >> sz)) { warn("usage: shader.instance.end.scale <s> <i> <sx> <sy> <sz>"); continue; }
                if (!scene.set_instance_scale_end(s, i, sx, sy, sz)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.start.u") {
                std::istringstream ss(rest);
                size_t s, i; int u; float v;
                if (!(ss >> s >> i >> u >> v)) { warn("usage: shader.instance.start.u <s> <i> <u_index:0-9> <value>"); continue; }
                if (u < 0 || u > 9) { warn("u_index out of range (0..9)"); continue; }
                if (!scene.set_instance_uniform_start(s, i, u, v)) warn("invalid shader/instance index");
            }
            else if (key == "shader.instance.end.u") {
                std::istringstream ss(rest);
                size_t s, i; int u; float v;
                if (!(ss >> s >> i >> u >> v)) { warn("usage: shader.instance.end.u <s> <i> <u_index:0-9> <value>"); continue; }
                if (u < 0 || u > 9) { warn("u_index out of range (0..9)"); continue; }
                if (!scene.set_instance_uniform_end(s, i, u, v)) warn("invalid shader/instance index");
            }

            // ===== image/render size =====
            else if (key == "render.size" || key == "image.size") {
                std::istringstream ss(rest);
                int w, h;
                if (!(ss >> w >> h)) { warn("usage: render.size <width> <height>"); }
                else { scene.set_frame_size(w, h); }
                }
            else if (key == "image.width" || key == "render.width") {
                    int w; if (parse_int(rest, w)) scene.setWidth(w); else warn("expected int");
                    }
            else if (key == "image.height" || key == "render.height") {
                        int h; if (parse_int(rest, h)) scene.setHeight(h); else warn("expected int");
                        }

            // ===== unknown =====
            else {
                warn("unknown key");
            }
        }

        return std::move(scene);
    }

    void save(Scene& scene, std::string filepath)
    {
        std::ofstream out(filepath);
        if (!out) {
            std::cerr << "[Scene::save] ERROR: cannot open file: " << filepath << "\n";
            return;
        }

        auto b = [](bool v) { return v ? "true" : "false"; };

        out << std::fixed << std::setprecision(6);

        // Header / meta
        out << "# =========================\n";
        out << "# Scene config (auto-saved)\n";
        out << "# =========================\n\n";

        // ---- Render ----
        out << "# ---- Render ----\n";
        out << "render.fps " << scene.get_render_fps() << "\n";
        out << "render.number_of_frames " << scene.get_number_of_frames() << "\n";
        out << "render.time.start " << scene.get_render_time_start() << "\n";
        out << "render.size " << scene.get_width() << " " << scene.get_height() << "\n\n";

        // ---- Capture ----
        out << "# ---- Capture ----\n";
        out << "capture " << b(scene.get_capture()) << "\n";
        out << "capture.png " << b(scene.get_capture_png()) << "\n";
        out << "capture.bmp " << b(scene.get_capture_bmp()) << "\n\n";

        // ---- Camera: start ----
        out << "# ---- Camera: start ----\n";
        out << "camera.start.x " << scene.get_camera_start_x() << "\n";
        out << "camera.start.y " << scene.get_camera_start_y() << "\n";
        out << "camera.start.z " << scene.get_camera_start_z() << "\n";
        out << "camera.start.pitch " << scene.get_camera_start_pitch() << "\n";
        out << "camera.start.yaw " << scene.get_camera_start_yaw() << "\n";
        out << "camera.start.fov " << scene.get_camera_start_fov() << "\n\n";

        // ---- Camera: end ----
        out << "# ---- Camera: end ----\n";
        out << "camera.end.x " << scene.get_camera_end_x() << "\n";
        out << "camera.end.y " << scene.get_camera_end_y() << "\n";
        out << "camera.end.z " << scene.get_camera_end_z() << "\n";
        out << "camera.end.pitch " << scene.get_camera_end_pitch() << "\n";
        out << "camera.end.yaw " << scene.get_camera_end_yaw() << "\n";
        out << "camera.end.fov " << scene.get_camera_end_fov() << "\n\n";

        // ---- LE / Post ----
        out << "# ---- LE / Post ----\n";
        out << "le.halfLife " << scene.get_le_halfLife() << "\n";
        out << "le.bloomThreshold " << scene.get_le_bloomThreshold() << "\n";
        out << "le.bloomSoftKnee " << scene.get_le_bloomSoftKnee() << "\n";
        out << "le.bloomStrength " << scene.get_le_bloomStrength() << "\n";
        out << "le.bloomIterations " << scene.get_le_bloomIterations() << "\n";
        out << "le.exposure " << scene.get_le_exposure() << "\n";
        out << "le.gamma " << scene.get_le_gamma() << "\n";
        out << "le.brightness " << scene.get_le_brightness() << "\n";
        out << "le.contrast " << scene.get_le_contrast() << "\n";
        out << "le.saturation " << scene.get_le_saturation() << "\n";
        out << "le.msaaSamples " << scene.get_le_msaaSamples() << "\n\n";

        // =========================
        // Shaders & Instances
        // =========================
        out << "# =========================\n";
        out << "# Shaders\n";
        out << "# =========================\n";
        const size_t sc = scene.shader_count();
        for (size_t s = 0; s < sc; ++s) {
            const std::string vtx = scene.get_shader_vertex_path(s);
            const std::string frag = scene.get_shader_fragment_path(s);
            out << "shader.add " << std::quoted(vtx) << " " << std::quoted(frag) << "\n";

            const size_t ic = scene.instance_count(s);
            for (size_t i = 0; i < ic; ++i) {
                out << "shader.instance.add " << s << "\n";

                // Group sizes
                int gx = 0, gy = 0, gz = 0;
                if (scene.get_instance_group_size(s, i, gx, gy, gz)) {
                    out << "shader.instance.group " << s << " " << i << " "
                        << gx << " " << gy << " " << gz << "\n";
                }

                // Drawcalls
                int n = 0;
                if (scene.get_instance_drawcalls(s, i, n)) {
                    out << "shader.instance.drawcalls " << s << " " << i << " " << n << "\n";
                }

                // Transforms: start
                float x, y, z, ex, ey, ez, sx, sy, sz;
                if (scene.get_instance_position_start(s, i, x, y, z))
                    out << "shader.instance.start.pos " << s << " " << i << " "
                    << x << " " << y << " " << z << "\n";
                if (scene.get_instance_euler_start(s, i, ex, ey, ez))
                    out << "shader.instance.start.euler " << s << " " << i << " "
                    << ex << " " << ey << " " << ez << "\n";
                if (scene.get_instance_scale_start(s, i, sx, sy, sz))
                    out << "shader.instance.start.scale " << s << " " << i << " "
                    << sx << " " << sy << " " << sz << "\n";

                // Transforms: end
                if (scene.get_instance_position_end(s, i, x, y, z))
                    out << "shader.instance.end.pos " << s << " " << i << " "
                    << x << " " << y << " " << z << "\n";
                if (scene.get_instance_euler_end(s, i, ex, ey, ez))
                    out << "shader.instance.end.euler " << s << " " << i << " "
                    << ex << " " << ey << " " << ez << "\n";
                if (scene.get_instance_scale_end(s, i, sx, sy, sz))
                    out << "shader.instance.end.scale " << s << " " << i << " "
                    << sx << " " << sy << " " << sz << "\n";

                // Uniforms: start
                for (int u = 0; u <= 9; ++u) {
                    float uv = 0.0f;
                    if (scene.get_instance_uniform_start(s, i, u, uv))
                        out << "shader.instance.start.u " << s << " " << i << " " << u << " " << uv << "\n";
                }
                // Uniforms: end
                for (int u = 0; u <= 9; ++u) {
                    float uv = 0.0f;
                    if (scene.get_instance_uniform_end(s, i, u, uv))
                        out << "shader.instance.end.u " << s << " " << i << " " << u << " " << uv << "\n";
                }

                out << "\n";
            }

            if (s + 1 < sc) out << "\n"; // spacer between shaders
        }

        out.flush();
    }

}
