#include "Scene.h"



namespace Scene_
{
	void unit_test_with_round_trip()
	{
		using Scene_::Scene;

		auto feq = [](float a, float b, float eps = 1e-5f) {
			float d = a - b; if (d < 0) d = -d; return d <= eps;
			};
		int checks = 0, fails = 0;
		auto CHECK = [&](bool ok, const char* what) {
			++checks;
			if (!ok) { ++fails; std::cerr << "[FAIL] " << what << "\n"; }
			};

		// ---------- Build reference scene S ----------
		Scene s;
		// s.set_frame_size(1234, 888);
		s.set_width(1234);
		s.set_height(888);
		s.set_render_fps(61);
		s.set_number_of_frames(777);
		s.set_render_time_start(3.141593f);

		s.set_capture(true);
		s.set_capture_png(true);
		s.set_capture_bmp(false);

		s.set_le_halfLife(0.012345f);
		s.set_le_bloomThreshold(1.234567f);
		s.set_le_bloomSoftKnee(0.765432f);
		s.set_le_bloomStrength(3.5f);
		s.set_le_bloomIterations(7);
		s.set_le_exposure(1.111111f);
		s.set_le_gamma(2.222222f);
		s.set_le_brightness(-0.05f);
		s.set_le_contrast(1.05f);
		s.set_le_saturation(0.95f);
		s.set_le_msaaSamples(8);

		// Camera start
		s.set_camera_start_x(1.0f);
		s.set_camera_start_y(2.0f);
		s.set_camera_start_z(3.0f);
		s.set_camera_start_pitch(0.10f);
		s.set_camera_start_yaw(0.20f);
		s.set_camera_start_fov(60.0f);

		// Camera end
		s.set_camera_end_x(4.0f);
		s.set_camera_end_y(5.0f);
		s.set_camera_end_z(6.0f);
		s.set_camera_end_pitch(0.30f);
		s.set_camera_end_yaw(0.40f);
		s.set_camera_end_fov(45.0f);

		// Shaders & instances
		size_t sh0 = s.add_shader("vs0.glsl", "fs0.glsl");
		size_t sh1 = s.add_shader("vs1.glsl", "fs1.glsl");

		size_t i0 = s.add_instance(sh0);
		size_t i1 = s.add_instance(sh0);
		size_t i2 = s.add_instance(sh1);

		// Instance 0 (shader 0) — rich settings
		s.set_instance_group_size(sh0, i0, 8, 9, 10);
		s.set_instance_drawcalls(sh0, i0, 3);
		s.set_instance_position_start(sh0, i0, 1.0f, 2.0f, 3.0f);
		s.set_instance_euler_start(sh0, i0, 0.1f, 0.2f, 0.3f);
		s.set_instance_scale_start(sh0, i0, 1.0f, 2.0f, 3.0f);
		s.set_instance_position_end(sh0, i0, 4.0f, 5.0f, 6.0f);
		s.set_instance_euler_end(sh0, i0, 0.4f, 0.5f, 0.6f);
		s.set_instance_scale_end(sh0, i0, 4.0f, 5.0f, 6.0f);
		for (int u = 0; u <= 9; ++u) {
			s.set_instance_uniform_start(sh0, i0, u, u * 0.5f);
			s.set_instance_uniform_end(sh0, i0, u, u * 1.5f);
		}

		// Instance 1 (shader 0) — minimal but non-default
		s.set_instance_group_size(sh0, i1, 1, 1, 1);
		s.set_instance_drawcalls(sh0, i1, 1);
		s.set_instance_position_start(sh0, i1, -1.0f, 0.0f, 1.0f);
		s.set_instance_position_end(sh0, i1, -2.0f, 0.5f, 2.0f);
		s.set_instance_scale_start(sh0, i1, 0.5f, 0.5f, 0.5f);
		s.set_instance_scale_end(sh0, i1, 2.0f, 2.0f, 2.0f);
		s.set_instance_euler_start(sh0, i1, 0.0f, 0.1f, 0.2f);
		s.set_instance_euler_end(sh0, i1, 0.3f, 0.4f, 0.5f);

		// Instance 2 (shader 1)
		s.set_instance_group_size(sh1, i2, 16, 8, 1);
		s.set_instance_drawcalls(sh1, i2, 2);

		// ---------- Round-trip ----------
		const char* fileA = "roundtrip_A.txt";
		const char* fileB = "roundtrip_B.txt";

		Scene_::save(s, fileA);
		auto loaded = Scene_::load(fileA);
		CHECK(loaded.has_value(), "load(fileA) should succeed");
		if (!loaded) { std::cerr << "[UNIT] ABORT: load failed\n"; return; }
		Scene t = *loaded;

		Scene_::save(t, fileB);

		auto read_all = [](const char* path) {
			std::ifstream fin(path, std::ios::binary);
			std::ostringstream ss;
			ss << fin.rdbuf();
			return ss.str();
			};
		const std::string A = read_all(fileA);
		const std::string B = read_all(fileB);
		CHECK(A == B, "textual save(load(save(S))) should be byte-identical");

		// ---------- Programmatic comparisons ----------
		CHECK(s.get_width() == t.get_width(), "width");
		CHECK(s.get_height() == t.get_height(), "height");
		CHECK(s.get_render_fps() == t.get_render_fps(), "render_fps");
		CHECK(s.get_number_of_frames() == t.get_number_of_frames(), "number_of_frames");
		CHECK(feq(s.get_render_time_start(), t.get_render_time_start()), "render_time_start");

		CHECK(s.get_capture() == t.get_capture(), "capture");
		CHECK(s.get_capture_png() == t.get_capture_png(), "capture_png");
		CHECK(s.get_capture_bmp() == t.get_capture_bmp(), "capture_bmp");

		CHECK(feq(s.get_le_halfLife(), t.get_le_halfLife()), "le.halfLife");
		CHECK(feq(s.get_le_bloomThreshold(), t.get_le_bloomThreshold()), "le.bloomThreshold");
		CHECK(feq(s.get_le_bloomSoftKnee(), t.get_le_bloomSoftKnee()), "le.bloomSoftKnee");
		CHECK(feq(s.get_le_bloomStrength(), t.get_le_bloomStrength()), "le.bloomStrength");
		CHECK(s.get_le_bloomIterations() == t.get_le_bloomIterations(), "le.bloomIterations");
		CHECK(feq(s.get_le_exposure(), t.get_le_exposure()), "le.exposure");
		CHECK(feq(s.get_le_gamma(), t.get_le_gamma()), "le.gamma");
		CHECK(feq(s.get_le_brightness(), t.get_le_brightness()), "le.brightness");
		CHECK(feq(s.get_le_contrast(), t.get_le_contrast()), "le.contrast");
		CHECK(feq(s.get_le_saturation(), t.get_le_saturation()), "le.saturation");
		CHECK(s.get_le_msaaSamples() == t.get_le_msaaSamples(), "le.msaaSamples");

		// Cameras
		CHECK(feq(s.get_camera_start_x(), t.get_camera_start_x()), "cam.start.x");
		CHECK(feq(s.get_camera_start_y(), t.get_camera_start_y()), "cam.start.y");
		CHECK(feq(s.get_camera_start_z(), t.get_camera_start_z()), "cam.start.z");
		CHECK(feq(s.get_camera_start_pitch(), t.get_camera_start_pitch()), "cam.start.pitch");
		CHECK(feq(s.get_camera_start_yaw(), t.get_camera_start_yaw()), "cam.start.yaw");
		CHECK(feq(s.get_camera_start_fov(), t.get_camera_start_fov()), "cam.start.fov");

		CHECK(feq(s.get_camera_end_x(), t.get_camera_end_x()), "cam.end.x");
		CHECK(feq(s.get_camera_end_y(), t.get_camera_end_y()), "cam.end.y");
		CHECK(feq(s.get_camera_end_z(), t.get_camera_end_z()), "cam.end.z");
		CHECK(feq(s.get_camera_end_pitch(), t.get_camera_end_pitch()), "cam.end.pitch");
		CHECK(feq(s.get_camera_end_yaw(), t.get_camera_end_yaw()), "cam.end.yaw");
		CHECK(feq(s.get_camera_end_fov(), t.get_camera_end_fov()), "cam.end.fov");

		// Shaders & instances
		CHECK(s.shader_count() == t.shader_count(), "shader_count");
		for (size_t si = 0; si < s.shader_count(); ++si) {
			CHECK(s.get_shader_vertex_path(si) == t.get_shader_vertex_path(si), "shader.vertex path");
			CHECK(s.get_shader_fragment_path(si) == t.get_shader_fragment_path(si), "shader.fragment path");

			CHECK(s.instance_count(si) == t.instance_count(si), "instance_count");
			for (size_t ii = 0; ii < s.instance_count(si); ++ii) {
				int sgx = 0, sgy = 0, sgz = 0, tgx = 0, tgy = 0, tgz = 0, sn = 0, tn = 0;

				CHECK(s.get_instance_group_size(si, ii, sgx, sgy, sgz), "get_instance_group_size S");
				CHECK(t.get_instance_group_size(si, ii, tgx, tgy, tgz), "get_instance_group_size T");
				CHECK(sgx == tgx && sgy == tgy && sgz == tgz, "group_size equal");

				CHECK(s.get_instance_drawcalls(si, ii, sn), "get_instance_drawcalls S");
				CHECK(t.get_instance_drawcalls(si, ii, tn), "get_instance_drawcalls T");
				CHECK(sn == tn, "drawcalls equal");

				float sx, sy, sz, tx, ty, tz;

				CHECK(s.get_instance_position_start(si, ii, sx, sy, sz), "pos_start S");
				CHECK(t.get_instance_position_start(si, ii, tx, ty, tz), "pos_start T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "pos_start equal");

				CHECK(s.get_instance_euler_start(si, ii, sx, sy, sz), "euler_start S");
				CHECK(t.get_instance_euler_start(si, ii, tx, ty, tz), "euler_start T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "euler_start equal");

				CHECK(s.get_instance_scale_start(si, ii, sx, sy, sz), "scale_start S");
				CHECK(t.get_instance_scale_start(si, ii, tx, ty, tz), "scale_start T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "scale_start equal");

				CHECK(s.get_instance_position_end(si, ii, sx, sy, sz), "pos_end S");
				CHECK(t.get_instance_position_end(si, ii, tx, ty, tz), "pos_end T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "pos_end equal");

				CHECK(s.get_instance_euler_end(si, ii, sx, sy, sz), "euler_end S");
				CHECK(t.get_instance_euler_end(si, ii, tx, ty, tz), "euler_end T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "euler_end equal");

				CHECK(s.get_instance_scale_end(si, ii, sx, sy, sz), "scale_end S");
				CHECK(t.get_instance_scale_end(si, ii, tx, ty, tz), "scale_end T");
				CHECK(feq(sx, tx) && feq(sy, ty) && feq(sz, tz), "scale_end equal");

				for (int u = 0; u <= 9; ++u) {
					float us = 0, ut = 0;
					CHECK(s.get_instance_uniform_start(si, ii, u, us), "uniform_start S");
					CHECK(t.get_instance_uniform_start(si, ii, u, ut), "uniform_start T");
					CHECK(feq(us, ut), "uniform_start equal");

					CHECK(s.get_instance_uniform_end(si, ii, u, us), "uniform_end S");
					CHECK(t.get_instance_uniform_end(si, ii, u, ut), "uniform_end T");
					CHECK(feq(us, ut), "uniform_end equal");
				}
			}
		}

		// A couple of negative checks (invalid indices)
		float dummy;
		CHECK(!t.get_instance_uniform_start(999, 0, 0, dummy), "invalid shader index rejected");
		CHECK(!t.get_instance_uniform_start(0, 999, 0, dummy), "invalid instance index rejected");

		std::cout << "[UNIT] round-trip textual equality: " << (A == B ? "true" : "false") << "\n";
		std::cout << "[UNIT] checks=" << checks << " failures=" << fails << "\n";
		std::cout << (fails == 0 ? "[UNIT] PASS\n" : "[UNIT] FAIL\n");
	}
}


int main()
{
    std::cout << "Tmp\n";

    Scene_::unit_test_with_round_trip();

    return 0;

    // std::string filepath = "commands.txt";

    Scene_::Scene scene;

    scene.set_width(1000);
    scene.set_height(1000);
    

    scene.set_camera_start_x(0.0f);
    scene.set_camera_start_y(0.0f);
    scene.set_camera_start_z(0.0f);
    scene.set_camera_start_fov(45.0f);
    scene.set_camera_start_pitch(0.0);
    scene.set_camera_start_yaw(0.0);
    
    

    scene.add_shader("vertex_file_path", "fragment_file_path");
    scene.add_instance(0);

    scene.set_instance_uniform_start(0, 0, 0, 0.0f);
    scene.set_instance_uniform_end(0, 0, 0, 1.0f);
    
    std::cout << "--- scene loading ---\n";
    std::optional<Scene_::Scene> scene_from_file = Scene_::load("commands_new.txt");
    if (scene_from_file.has_value())
    {
        std::cout << "--- scene printing ---\n";
        scene_from_file.value().print();

        
        Scene_::save(scene_from_file.value(), "exported_commands.txt");
    }
    
    
    
    

    


    return 0;
}

