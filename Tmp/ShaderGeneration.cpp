#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "Writer.h"

#include "CppCommponents/Random.h"

namespace ShaderGeneration_
{
	// -------------- Example generation --------------
	static void generate_vertex_shader(const std::filesystem::path& outPath) {
		Writer_::Writer w;


		w.line("#version 450 core");
		w.blank();

		w.comment("Auto-generated vertex shader");
		w.line("layout(location = 0) in vec3 aPos;");
		w.line("layout(location = 1) in vec2 aTexCoord;");
		w.blank();

		w.line("uniform mat4 model;");
		w.line("uniform mat4 view;");
		w.line("uniform mat4 projection;");
		w.blank();



		w.line("out vec2 vTex;");
		w.line("out vec2 TexCoord;");
		w.line("out vec3 color_vs;");
		w.line("out vec3 vWorldPos;");
		w.line("out vec3 vNormal;");
		w.blank();

		w.comment("-------- Controls (few scalars only) --------");

		w.line("uniform ivec3 uGrid;      // number of instances along X,Y,Z (instanceCount = X*Y*Z)");
		w.line("uniform float uSpacing;   // distance between grid cells");
		w.line("uniform vec3  uOrigin;    // base world offset");
		w.line("uniform vec3  uScaleMin;  // min scale per axis");
		w.line("uniform vec3  uScaleMax;  // max scale per axis");
		w.line("uniform float uTime;      // time (seconds)");
		w.line("uniform float uRotSpeed;  // radians/sec");
		w.line("uniform uint  uSeed;      // global random seed");
		w.blank();

		w.line("uniform uint uDrawcallNumber;");
		w.line("uniform vec3 uCameraPos;");
		w.line("uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;");
		w.blank();




		w.comment("----------Constants & tiny helpers----------");
		w.line("const float PI = ${PI};", { {"PI","3.1415926535897932384626433832795"} });
		w.line("const float TAU = ${TAU};", { {"TAU","6.2831853071795864769252867665590"} });
		w.blank();



		w.line("void main()");
		w.open();
		w.line("vTex = aTexCoord;");
		w.line("gl_Position = projection * view * model * vec4(aPos, 1.0);");
		w.close();

		w.save(outPath);
	}

	static void generate_fragment_shader(const std::filesystem::path& outPath, bool useTint, bool useTimeSine)
	{
		Writer_::Writer w;


		w.line("#version 450 core");
		w.blank();

		w.comment("Auto-generated fragment shader");
		w.line("in vec2 vTex;");
		w.line("out vec4 FragColor;");
		w.blank();

		w.save(outPath);
	}

	// Writes the provided vertex shader to `outPath`.
	// Uses Writer_::Writer features: line(), blank(), comment(), lines(), open()/close() where it helps readability.
	inline void write_vertex_shader_edge_snap(const std::filesystem::path& outPath)
	{
		Writer_::Writer w;

		// Header + I/O
		w.line("#version 450 core");
		w.line("layout(location = 0) in vec3 aPos;");
		w.line("layout(location = 1) in vec2 aTexCoord;");
		w.blank();
		w.line("out vec2 TexCoord;");
		w.line("out vec3 color_vs;");
		w.blank();
		w.comment("NEW: feed the FS");
		w.line("out vec3 vWorldPos;");
		w.line("out vec3 vNormal;");
		w.blank();

		// Matrices
		w.line("uniform mat4 model;       // can be identity");
		w.line("uniform mat4 view;");
		w.line("uniform mat4 projection;");
		w.blank();

		// Controls
		w.comment("-------- Controls (few scalars only) --------");
		w.line("uniform ivec3 uGrid;      // number of instances along X,Y,Z (instanceCount = X*Y*Z)");
		w.line("uniform float uSpacing;   // distance between grid cells");
		w.line("uniform vec3  uOrigin;    // base world offset");
		w.line("uniform vec3  uScaleMin;  // min scale per axis");
		w.line("uniform vec3  uScaleMax;  // max scale per axis");
		w.line("uniform float uTime;      // time (seconds)");
		w.line("uniform float uRotSpeed;  // radians/sec");
		w.line("uniform uint  uSeed;      // global random seed");
		w.blank();
		w.line("uniform uint uDrawcallNumber;");
		w.line("uniform vec3 uCameraPos;");
		w.line("uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;");
		w.blank();

		// Constants & helpers
		w.comment("---------- Constants & tiny helpers ----------");
		w.line("const float PI = 3.1415926535897932384626433832795;");
		w.line("const float TAU = 6.2831853071795864769252867665590;");
		w.blank();

		w.line("float saturate(float x) { return clamp(x, 0.0, 1.0); }");
		w.blank();

		// pcg_hash + rand01
		w.line("uint pcg_hash(uint x)");
		w.open("{");
		w.line("x = x * 747796405u + 2891336453u;");
		w.line("x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;");
		w.line("x = (x >> 22u) ^ x;");
		w.line("return x;");
		w.close();
		w.line("float rand01(inout uint s) { s = pcg_hash(s); return float(s) * (1.0 / 4294967295.0); }");
		w.blank();

		// spherical01
		w.lines(R"GLSL(
vec3 spherical01(float r, float theta01, float phi01) {
    float theta = theta01 * TAU; // azimuth
    float phi = phi01 * PI;      // polar
    float sphi = sin(phi);
    return vec3(r * sphi * cos(theta), r * cos(phi), r * sphi * sin(theta));
}
)GLSL", {});
		w.blank();

		// axisAngleToMat3
		w.lines(R"GLSL(
mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c,      t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c,      t.y * axis.z - s * axis.x,
        t.z * axis.x - s * axis.y, t.z * axis.y + s * axis.x, t.z * axis.z + c
    );
}
)GLSL", {});
		w.blank();

		// localCubeFaceNormal
		w.lines(R"GLSL(
vec3 localCubeFaceNormal(vec3 p) {
    vec3 ap = abs(p);
    if (ap.x >= ap.y && ap.x >= ap.z) return vec3(sign(p.x), 0.0, 0.0);
    if (ap.y >= ap.x && ap.y >= ap.z) return vec3(0.0, sign(p.y), 0.0);
    return vec3(0.0, 0.0, sign(p.z));
}
)GLSL", {});
		w.blank();

		// snapToNearestEdge
		w.lines(R"GLSL(
vec3 snapToNearestEdge(vec3 u)
{
    // distance to the nearest face on each axis
    vec3 dist = min(u, 1.0 - u);         // smaller = closer to face
    // choose which axis to KEEP (largest dist => farthest from faces)
    int k = (dist.x > dist.y)
        ? ((dist.x > dist.z) ? 0 : 2)
        : ((dist.y > dist.z) ? 1 : 2);

    // decide side (0 or 1) for snapping
    vec3 side = step(vec3(0.5), u);      // 0 if closer to 0-face, 1 if closer to 1-face
    vec3 snapped = mix(vec3(0.0), vec3(1.0), side);

    // build the result: keep axis k, snap the other two
    if (k == 0) return vec3(u.x, snapped.y, snapped.z);
    if (k == 1) return vec3(snapped.x, u.y, snapped.z);
    return vec3(snapped.x, snapped.y, u.z);
}
)GLSL", {});
		w.blank();

		// main()
		w.lines(R"GLSL(
void main()
{
    int id = gl_InstanceID;
    id = id + (uGrid.x * uGrid.y * uGrid.z) * int(uDrawcallNumber);

    // Per-instance randomness
    uint s0 = uSeed + uint(id + 0);
    uint s1 = uSeed + uint(id + 42);
    uint s2 = uSeed + uint(id + 142);
    float rnd_x = rand01(s0);
    float rnd_y = rand01(s1);
    float rnd_z = rand01(s2);

    uint s_rnd_0 = uSeed + uint(id + 1142);
    float rnd_0 = rand01(s_rnd_0);

    uint s_rnd_1 = uSeed + uint(id + 1147);
    float rnd_1 = rand01(s_rnd_1);

    uint s_rnd_2 = uSeed + uint(id + 1107);
    float rnd_2 = rand01(s_rnd_2);

    uint s0_rot_x = uSeed + uint(id + 2431);
    uint s1_rot_y = uSeed + uint(id + 4412);
    uint s2_rot_y = uSeed + uint(id + 1234);
    uint s3_rot_angle = uSeed + uint(id + 2332);

    float rnd_cube_rotation_x = rand01(s0_rot_x);
    float rnd_cube_rotation_y = rand01(s1_rot_y);
    float rnd_cube_rotation_z = rand01(s2_rot_y);
    float rnd_cube_rotation_angle = rand01(s3_rot_angle);

    // Position on sphere
    float radius_offset = 0.2 * sin(rnd_x * TAU * 10.0) + 0.02 * sin(rnd_y * 20 * TAU + uTime * 1.0);

    int n_x = 10;
    float radius = 1.0 + 0.1 * sin(rnd_x * TAU * int(n_x));
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    float px = sphere_position.x;
    float py = sphere_position.y;
    float pz = sphere_position.z;

    float color_r = 1.0;
    float color_g = 1.0;
    float color_b = 1.0;

    vec3 color_base_dark = vec3(0.02, 0.08, 0.15);
    vec3 color_base_bright = vec3(0.10, 0.50, 0.75);

    vec3 color_base = vec3(0.01, 0.01, 0.01);

    color_r = 0.02;
    color_g = 0.02;
    color_b = 0.02;

    float scale_cube = 0.01;
    vec3  pos = vec3(px,pz, py);
    vec3  scale = vec3(scale_cube, scale_cube, scale_cube);

    pos.x = rnd_x; // 0.0 to 1.0
    pos.y = rnd_y; // 0.0 to 1.0
    pos.z = rnd_z; // 0.0 to 1.0

    {
        const bool full_cube = false;

        if (full_cube)
        {
            if
                (
                    pos.x > 0.01 &&
                    pos.y > 0.01 &&
                    pos.y < 0.99 &&
                    // pos.z > 0.01 &&
                    pos.z < 0.99
                )
            {
                pos.x = 1.0;
            }
        }
        else
        {
            pos = snapToNearestEdge(pos);
        }
    }

    vec4 new_position = vec4(vec3(pos), 1.0);

    vec3 offset_location = vec3(0.0, 0.0, 0.0);

    const int num_cube_size_x = 1;
    const int num_cube_size_y = 1;
    const int num_cube_size_z = 1;

    float xi = int(rnd_0 * float(num_cube_size_x));
    float yi = int(rnd_1 * float(num_cube_size_y));
    float zi = int(rnd_2 * float(num_cube_size_z));

    offset_location = vec3(xi, yi, zi);

    if (true) {
        uint s0_instance_0 = uSeed + uint(uint(u0 * 1000.0f));
        uint s0_instance_1 = uSeed + uint(uint(u0 * 1421.0f));
        float rnd_instance_0 = rand01(s0_instance_0);
        float rnd_instance_1 = rand01(s0_instance_1);

        uint s0_instance_x_scale = uSeed + uint(uint(u0 * 14024.0f));
        uint s0_instance_y_scale = uSeed + uint(uint(u0 * 15214.0f));
        uint s0_instance_z_scale = uSeed + uint(uint(u0 * 14215.0f));
        float rnd_instance_scale_x = rand01(s0_instance_x_scale);
        float rnd_instance_scale_y = rand01(s0_instance_y_scale);
        float rnd_instance_scale_z = rand01(s0_instance_z_scale);

        // Rotation
        vec3 axis = normalize(vec3(1.0, 1.0, 0.0));
        float angle = uTime * 0.0;
        mat3 R3 = axisAngleToMat3(axis, angle);
        mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));

        // Translation
        mat4 T = mat4(1.0);
        T[3] = vec4(offset_location, 1.0);

        // Scale
        mat4 S = mat4(1.0);
        S[0][0] = 1.0;
        S[1][1] = 1.0;
        S[2][2] = 1.0;

        new_position = T * R * S * new_position;
    }

    pos = new_position.xyz;

    // Per-instance tint (kept neutral here)
    color_vs = vec3(color_r, color_g, color_b);

    // Build TRS
    mat4 T = mat4(1.0); T[3] = vec4(pos, 1.0);
    vec3 axis = normalize(vec3(rnd_cube_rotation_x, rnd_cube_rotation_y, rnd_cube_rotation_z));
    float angle = rnd_cube_rotation_angle * 1000.0 + uTime * 1.0;
    mat3 R3 = axisAngleToMat3(axis, angle);
    mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
    mat4 S = mat4(1.0); S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;

    mat4 instanceModel = T * R * S;
    mat4 M = model * instanceModel;

    // World-space position (for lighting)
    vec4 wp = M * vec4(aPos, 1.0);
    vWorldPos = wp.xyz;

    // World-space normal
    vec3 nLocal = localCubeFaceNormal(aPos);
    vNormal = normalize(mat3(model) * (R3 * nLocal)); // uniform-scale assumption

    // Clip-space position and UV
    gl_Position = projection * view * wp;
    TexCoord = aTexCoord;

    // World position color (commented demo)
    // float world_x = wp.x;
    // float world_y = wp.y;
    // float world_z = wp.z;
    // color_vs = vec3(sin(world_x * 10.0), sin(world_y * 10.0), sin(world_z * 10.0));
}
)GLSL", {});

		// Finally, save
		w.save(outPath);
	}






	void run()
	{
		std::cout << "ShaderGeneration_";

		//Writer_::Writer w;
		//w.line("#version 450 core");
		//w.line("layout(location = 0) in vec3 aPos;");
		//w.line("layout(location = 1) in vec2 aTexCoord;");

		//{
		//	const std::filesystem::path outDir = std::filesystem::path("C:/Users/Cosmos/Desktop/output/tmp/shaders/");
		//	const std::filesystem::path vsPath = outDir / "generated_vertex.glsl";
		//	const std::filesystem::path fsPath = outDir / "generated_fragment.glsl";

		//	// Produce minimal, valid shaders. Flip toggles here to include/exclude sections.
		//	const bool USE_TINT = true;
		//	const bool USE_TIME_SINE = true;

		//	generate_vertex_shader(vsPath);
		//	generate_fragment_shader(fsPath, USE_TINT, USE_TIME_SINE);
  //          write_vertex_shader_edge_snap(vsPath);

		//	std::cout << "Generated:\n  " << vsPath.string()
		//		<< "\n  " << fsPath.string() << "\n";
		//	std::cout << "ShaderGeneration_ done\n";
		//}

		Writer_::Writer w;




		const std::filesystem::path outDir = std::filesystem::path("C:/Users/Cosmos/Desktop/output/tmp/shaders/");
		const std::filesystem::path vsPath = outDir / "generated_vertex.glsl";
		// const std::filesystem::path fsPath = outDir / "generated_fragment.glsl";


		/*
		int n_x = 10;
		float n_x_offset = 0.1;
		float n_x_amplitude = 0.02;
		float n_x_t = uTime * 0.1;

		*/

		

		class Wave
		{
		public:
			enum class Direction
			{
				X,
				Y 
			};

			Direction direction = Direction::X;
			
			int frequency_index = 1;
			float offset = 0.0f;
			float amplitude = 1.0f;
			float time_multiplier = 1.13214f;
			int function_to_use = 0;

			void write(Writer_::Writer& w, int index, std::string name)
			{
				std::string direction_txt = "x";
				if (direction == Direction::Y)
				{
					direction_txt = "y";
				}

				w.comment("${NAME} ${DIRECTION} ${INDEX} ", { {"NAME", name}, {"DIRECTION", direction_txt}, {"INDEX", std::to_string(index)}});
				w.linef("int {}_{}_{}_frequency = int({});", name, index, direction_txt, frequency_index);
				w.linef("float {}_{}_{}_offset = float({});", name, index, direction_txt, offset);
				w.linef("float {}_{}_{}_amplitude = float({});", name, index, direction_txt, amplitude);
				w.linef("float {}_{}_{}_t = uTime * float({});", name, index, direction_txt,float( this->time_multiplier));
				w.blank();
			}

			static void generate_waves(std::vector<Wave>& waves, int num)
			{
				for (int i = 0; i < num; i++)
				{
					Wave wave;
					wave.frequency_index = Random::random_int(1, 100);
					wave.offset = Random::generate_random_float_minus_one_to_plus_one() * 0.4f;
					wave.amplitude = Random::generate_random_float_minus_one_to_plus_one() * 0.2f;
					wave.time_multiplier = Random::generate_random_float_minus_one_to_plus_one() * 0.1f;
					wave.function_to_use = Random::random_int(0, 10);

					if (Random::generate_random_float_0_to_1() > 0.5f)
					{
						wave.direction = Wave::Direction::X;
					}
					else
					{
						wave.direction = Wave::Direction::Y;
					}

					waves.push_back(wave);
				}
			}

			static void normalize_amplitude(std::vector<Wave>& waves)
			{
				float total_amplitude = 0.0;
				for (int i = 0; i < waves.size(); i++)
				{
					total_amplitude += waves[i].amplitude;
				}

				float factor = 1.0 / total_amplitude;

				if (std::abs(total_amplitude) > 1.0)
				{
					for (int i = 0; i < waves.size(); i++)
					{
						waves[i].amplitude = factor * waves[i].amplitude;
					}
				}
			}


			static void write(Writer_::Writer& w, std::vector<Wave>& waves, std::string name)
			{
				{
					for (int i = 0; i < waves.size(); i++)
					{
						waves[i].write(w, i, name);
					}

				}

				{
					w.blank();
					w.line("float ${NAME} = 0.0f;", { {"NAME", name}});



					for (int i = 0; i < waves.size(); i++)
					{
						int function_to_use = waves[i].function_to_use;



						w.line
						(
							"${NAME} += ${NAME}_${INDEX}_${DIRECTION}_amplitude * f_periodic_${PERIODIC_FUNCTION}(f_adjust_to_two_pi(${NAME}_${INDEX}_${DIRECTION}_offset + rnd_x * TAU * ${NAME}_${INDEX}_${DIRECTION}_frequency + ${NAME}_${INDEX}_${DIRECTION}_t * uTime));",
							{
								{"NAME", name},
								{"INDEX", std::to_string(i)},
								{"DIRECTION", ((waves[i].direction == Wave::Direction::X) ? "x" : "y")},
								{"PERIODIC_FUNCTION", std::to_string(function_to_use)},
							}
							);
					}


					w.blank();
					w.linef("{} *= float({});", name, 0.2f);

				}
			}
		};



		std::string name_0 = "first_wave";
		std::string name_1 = "second_wave";


		{
			std::vector<Wave> waves;

			Wave::generate_waves(waves, 10);
			Wave::normalize_amplitude(waves);

			Wave::write(w, waves, name_0);

			w.blank();
		}
		

		{
			std::vector<Wave> waves;

			Wave::generate_waves(waves, 10);
			Wave::normalize_amplitude(waves);

			Wave::write(w, waves, name_1);

			w.blank();

		}
		

		{
			w.line("float f_0 = fract(uTime * 0.1);");
			w.line("float f_1 = 1.0 - f_0;");
			w.blank();
			w.line("float w = f_1 * ${NAME_0} + f_0 * ${NAME_1};", { {"NAME_0", name_0} , {"NAME_1", name_1} });

			
		}
		
		
		

		
		/*
		float wave_first = n_x_amplitude * f_periodic_0(f_adjust_to_two_pi(n_x_offset + rnd_x * TAU * n_x + n_x_t * uTime)) +
                       n_y_amplitude * f_periodic_0(f_adjust_to_two_pi(n_y_offset + rnd_y * TAU * n_y + n_y_t * uTime));

    float wave_second = n_x_amplitude * f_periodic_4(f_adjust_to_two_pi(n_x_offset + rnd_x * TAU * n_x + n_x_t * uTime)) +
                        n_y_amplitude * f_periodic_2(f_adjust_to_two_pi(n_y_offset + rnd_y * TAU * n_y + n_y_t * uTime));

		*/

		


		w.save(vsPath);
	}
}