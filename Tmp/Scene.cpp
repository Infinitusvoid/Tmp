#include "Scene.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include "../External_libs/tomlplusplus/tomlplusplus/toml.hpp"

#include <string>
#include <optional>
#include <cstdint>
#include <iostream>



namespace Scene_
{
    class TomlLite {
    public:
        class Node {
        public:
            Node() : nv_{} {}
            explicit Node(toml::node_view<const toml::node> nv) : nv_(nv) {}

            Node operator[](std::string_view key) const {
                if (auto t = nv_.as_table())
                    return Node{ toml::node_view<const toml::node>{ (*t)[key] } };
                return Node{};
            }
            Node at(size_t idx) const {
                if (auto a = nv_.as_array())
                    return Node{ toml::node_view<const toml::node>{ (*a)[idx] } };
                return Node{};
            }

            int         as_int(int def = 0)        const { if (auto i = nv_.value<std::int64_t>()) return (int)*i; if (auto d = nv_.value<double>()) return (int)*d; return def; }
            float       as_float(float def = 0.f)   const { if (auto d = nv_.value<double>()) return (float)*d; if (auto i = nv_.value<std::int64_t>()) return (float)*i; return def; }
            bool        as_bool(bool def = false)   const { if (auto b = nv_.value<bool>()) return *b; return def; }
            std::string as_string(std::string def = {})const { if (auto s = nv_.value<std::string>()) return *s; return def; }

            int  size()     const { if (auto a = nv_.as_array()) return (int)a->size(); if (auto t = nv_.as_table()) return (int)t->size(); return 0; }
            bool exists()   const { return nv_.node() != nullptr; }
            bool is_array() const { return nv_.as_array() != nullptr; }
            bool is_table() const { return nv_.as_table() != nullptr; }

            toml::node_view<const toml::node> view() const { return nv_; }

        private:
            toml::node_view<const toml::node> nv_;
        };

        TomlLite() = default;
        explicit TomlLite(toml::table tbl) : root_(std::move(tbl)) {}

        static std::optional<TomlLite> from_file(const std::string& path, std::string* err = nullptr) {
            try {
                return TomlLite{ toml::parse_file(path) };
            }
            catch (const toml::parse_error& e) {
                if (err) {
                    const auto& src = e.source();
                    std::ostringstream os;
                    os << "parse error: " << e.description();
                    if (src.path) os << " at " << *src.path;               // path is an optional shared_ptr<string>
                    os << " (" << src.begin.line << ":" << src.begin.column << ")"; // no as_string()
                    *err = os.str();
                }
                return std::nullopt;
            }
        }

        Node operator[](std::string_view key) const {
            return Node{ toml::node_view<const toml::node>{ root_[key] } };
        }

        const toml::table& root() const { return root_; }

    private:
        toml::table root_;
    };
    

	// ----------------- tiny helpers -----------------
    static inline void warn(Report& r, std::string msg) { r.warns.push_back(std::move(msg)); }

	
	Scene load(std::string filepath, Callbacks& callbacks, Report& report)
	{
		Scene scene;

        toml::table t;
        try
        {
            t = toml::parse_file(filepath);
        }
        catch (const toml::parse_error& e)
        {
            std::ostringstream os;
            os << "parse error: " << e.description() << " at " << e.source().begin;
            warn(report, os.str());
            return init_default();
        }


        Scene s; // start empty; fill fields that exist

        
        
        std::string err;
        auto opt = TomlLite::from_file("example.toml", &err);
        
        if (!opt)
        {
            std::cout << "error : " << err << "\n";
        }
        TomlLite toml = std::move(*opt);

        // window
        s.width = toml["window"]["width"].as_int(s.width);
        s.height = toml["window"]["height"].as_int(s.height);

        // capture
        s.capture_settings.capture_frames = toml["capture"]["frames"].as_bool(s.capture_settings.capture_frames);
        s.capture_settings.capture_png = toml["capture"]["png"].as_bool(s.capture_settings.capture_png);
        {
            bool bmp = toml["capture"]["bmp"].as_bool(s.capture_settings.cpature_bmp);
            s.capture_settings.cpature_bmp = toml["capture"]["cpature_bmp"].as_bool(bmp); // accept either key
        }

        // le (optional)
        s.le_settings.halfLife = toml["le"]["halfLife"].as_float(s.le_settings.halfLife);
        s.le_settings.bloomThreshold = toml["le"]["bloomThreshold"].as_float(s.le_settings.bloomThreshold);
        s.le_settings.bloomSoftKnee = toml["le"]["bloomSoftKnee"].as_float(s.le_settings.bloomSoftKnee);
        s.le_settings.bloomStrength = toml["le"]["bloomStrength"].as_float(s.le_settings.bloomStrength);
        s.le_settings.bloomIterations = toml["le"]["bloomIterations"].as_float(s.le_settings.bloomIterations);
        s.le_settings.exposure = toml["le"]["exposure"].as_float(s.le_settings.exposure);
        s.le_settings.gamma = toml["le"]["gamma"].as_float(s.le_settings.gamma);
        s.le_settings.brightness = toml["le"]["brightness"].as_float(s.le_settings.brightness);
        s.le_settings.contrast = toml["le"]["contrast"].as_float(s.le_settings.contrast);
        s.le_settings.saturation = toml["le"]["saturation"].as_float(s.le_settings.saturation);
        s.le_settings.msaaSamples = toml["le"]["msaaSamples"].as_int(s.le_settings.msaaSamples);


        // camera.start / camera.end
        auto read_cam = [&](TomlLite::Node node, Camera& c) {
            auto pos = node["pos"];
            if (pos.is_array() && pos.size() >= 3) {
                c.camera_x = pos.at(0).as_float(0.f);
                c.camera_y = pos.at(1).as_float(0.f);
                c.camera_z = pos.at(2).as_float(0.f);
            }
            c.yew = node["yaw"].as_float(node["yew"].as_float(c.yew)); // prefer yaw, fallback yew
            c.pitch = node["pitch"].as_float(c.pitch);
            c.fov = node["fov"].as_float(c.fov);
            };
        read_cam(toml["camera"]["start"], s.camera_start);
        read_cam(toml["camera"]["end"], s.camera_end);

        // shaders / instances
        int shader_count = toml["shaders"].size();
        s.shaders.reserve(shader_count);
        for (int si = 0; si < shader_count; ++si) {
            Shader sh;
            auto S = toml["shaders"].at(si);
            sh.vertex = S["vertex"].as_string("");
            sh.fragment = S["fragment"].as_string("");

            int inst_count = S["instances"].size();
            sh.instances.reserve(inst_count);
            for (int ii = 0; ii < inst_count; ++ii) {
                auto I = S["instances"].at(ii);
                Instance inst;

                // uniforms (u0..u9)
                auto read_uniforms = [](TomlLite::Node arr, Instance::Uniforms& U) {
                    for (int i = 0; i < 10 && i < arr.size(); ++i) {
                        float v = arr.at(i).as_float(0.f);
                        switch (i) {
                        case 0: U.u0 = v; break; case 1: U.u1 = v; break; case 2: U.u2 = v; break; case 3: U.u3 = v; break; case 4: U.u4 = v; break;
                        case 5: U.u5 = v; break; case 6: U.u6 = v; break; case 7: U.u7 = v; break; case 8: U.u8 = v; break; case 9: U.u9 = v; break;
                        }
                    }
                    };
                read_uniforms(I["uniforms_start"], inst.uniforms_start);
                read_uniforms(I["uniforms_end"], inst.uniforms_end);

                // transforms
                auto read_tr = [](TomlLite::Node T, Transform& tr) {
                    auto p = T["position"]; if (p.is_array() && p.size() >= 3) { tr.position_x = p.at(0).as_float(0.f); tr.position_y = p.at(1).as_float(0.f); tr.position_z = p.at(2).as_float(0.f); }
                    auto e = T["euler"];    if (e.is_array() && e.size() >= 3) { tr.euler_x = e.at(0).as_float(0.f); tr.euler_y = e.at(1).as_float(0.f); tr.euler_z = e.at(2).as_float(0.f); }
                    auto s3 = T["scale"];    if (s3.is_array() && s3.size() >= 3) { tr.scale_x = s3.at(0).as_float(0.f); tr.scale_y = s3.at(1).as_float(0.f); tr.scale_z = s3.at(2).as_float(0.f); }
                    };
                read_tr(I["transform_start"], inst.transform_start);
                read_tr(I["transform_end"], inst.transform_end);

                // sizes
                auto gs = I["group_size"];
                if (gs.is_array() && gs.size() >= 3) {
                    inst.group_size_x = gs.at(0).as_int(1);
                    inst.group_size_y = gs.at(1).as_int(1);
                    inst.group_size_z = gs.at(2).as_int(1);
                }
                inst.number_of_drawcalls = I["number_of_drawcalls"].as_int(inst.number_of_drawcalls);

                sh.instances.push_back(std::move(inst));
            }
            s.shaders.push_back(std::move(sh));
        }

        // minimal defaults if missing
        if (s.width <= 0) s.width = 1920 * 2;
        if (s.height <= 0) s.height = 1080 * 2;
        if (s.camera_start.fov == 0.f) s.camera_start.fov = 45.f;
        if (s.camera_end.fov == 0.f) s.camera_end.fov = 45.f;

        return s;
	
	}
    


    


	bool save(const Scene& scene, Callbacks& callbacks, std::string filepath, Report& report)
	{
        toml::table t;

        // window
        t.insert_or_assign("window", toml::table{
            {"width",  scene.width},
            {"height", scene.height}
            });

        // capture
        t.insert_or_assign("capture", toml::table{
            {"frames", scene.capture_settings.capture_frames},
            {"png",    scene.capture_settings.capture_png},
            {"bmp",    scene.capture_settings.cpature_bmp} // normalized key when writing
            });

        // le
        t.insert_or_assign("le", toml::table{
            {"halfLife",        scene.le_settings.halfLife},
            {"bloomThreshold",  scene.le_settings.bloomThreshold},
            {"bloomSoftKnee",   scene.le_settings.bloomSoftKnee},
            {"bloomStrength",   scene.le_settings.bloomStrength},
            {"bloomIterations", scene.le_settings.bloomIterations},
            {"exposure",        scene.le_settings.exposure},
            {"gamma",           scene.le_settings.gamma},
            {"brightness",      scene.le_settings.brightness},
            {"contrast",        scene.le_settings.contrast},
            {"saturation",      scene.le_settings.saturation},
            {"msaaSamples",     scene.le_settings.msaaSamples},
            });

        // camera
        auto mkcam = [](const Camera& c) {
            return toml::table{
                {"pos",   toml::array{ c.camera_x, c.camera_y, c.camera_z }},
                {"yaw",   c.yew},   // write 'yaw'; loader also accepts legacy 'yew'
                {"pitch", c.pitch},
                {"fov",   c.fov},
            };
            };
        t.insert_or_assign("camera", toml::table{
            {"start", mkcam(scene.camera_start)},
            {"end",   mkcam(scene.camera_end)}
            });

        // shaders
        toml::array sharr;
        for (const auto& sh : scene.shaders) {
            toml::table sht{
                {"vertex",   sh.vertex},
                {"fragment", sh.fragment}
            };

            toml::array instarr;
            for (const auto& in : sh.instances) {
                toml::table it;

                auto push_uniforms = [](const Instance::Uniforms& U) {
                    return toml::array{ U.u0,U.u1,U.u2,U.u3,U.u4,U.u5,U.u6,U.u7,U.u8,U.u9 };
                    };
                it.insert_or_assign("uniforms_start", push_uniforms(in.uniforms_start));
                it.insert_or_assign("uniforms_end", push_uniforms(in.uniforms_end));

                auto trtbl = [](const Transform& tr) {
                    return toml::table{
                        {"position", toml::array{ tr.position_x, tr.position_y, tr.position_z }},
                        {"euler",    toml::array{ tr.euler_x,    tr.euler_y,    tr.euler_z }},
                        {"scale",    toml::array{ tr.scale_x,    tr.scale_y,    tr.scale_z }},
                    };
                    };
                it.insert_or_assign("transform_start", trtbl(in.transform_start));
                it.insert_or_assign("transform_end", trtbl(in.transform_end));

                it.insert_or_assign("group_size", toml::array{ in.group_size_x, in.group_size_y, in.group_size_z });
                it.insert_or_assign("number_of_drawcalls", in.number_of_drawcalls);

                instarr.push_back(std::move(it));
            }
            sht.insert_or_assign("instances", std::move(instarr));
            sharr.push_back(std::move(sht));
        }
        t.insert_or_assign("shaders", std::move(sharr));

        std::ofstream ofs(filepath);
        if (!ofs) {
            warn(report, "cannot open for writing: " + filepath);
            return false;
        }
        ofs << t; // toml++ pretty-prints
        return true;
	}

	Scene init_default()
	{
        Scene s;
        s.width = 1920 * 2;
        s.height = 1080 * 2;
        s.camera_start.fov = 45.0f;
        s.camera_end.fov = 45.0f;

        Shader sh;
        sh.vertex = "shaders/default_vs.glsl";
        sh.fragment = "shaders/default_fs.glsl";

        Instance inst;

        inst.group_size_x = 1000;
        inst.group_size_y = 1000;
        inst.group_size_z = 1;

        inst.number_of_drawcalls = 1;

        sh.instances.push_back(inst);
        s.shaders.push_back(std::move(sh));

        return s;
	}


    void print(Scene& scene)
    {
        std::cout << "window \n";
        std::cout << "width = " << scene.width << "\n";
        std::cout << "height = " << scene.height << "\n";
    }
	
}
