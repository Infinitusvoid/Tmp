

#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 color_vs;

// NEW: feed the FS
out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 model;       // can be identity
uniform mat4 view;
uniform mat4 projection;

// -------- Controls (few scalars only) --------
uniform ivec3 uGrid;      // number of instances along X,Y,Z (instanceCount = X*Y*Z)
uniform float uSpacing;   // distance between grid cells
uniform vec3  uOrigin;    // base world offset
uniform vec3  uScaleMin;  // min scale per axis
uniform vec3  uScaleMax;  // max scale per axis
uniform float uTime;      // time (seconds)
uniform float uRotSpeed;  // radians/sec
uniform uint  uSeed;      // global random seed

uniform uint uDrawcallNumber;
uniform vec3 uCameraPos;
uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;

// ---------- Constants & tiny helpers ----------
const float PI = 3.1415926535897932384626433832795;
const float TAU = 6.2831853071795864769252867665590;

float saturate(float x) { return clamp(x, 0.0, 1.0); }

uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}
float rand01(inout uint s) { s = pcg_hash(s); return float(s) * (1.0 / 4294967295.0); }

vec3 spherical01(float r, float theta01, float phi01) {
    float theta = theta01 * TAU; // azimuth
    float phi = phi01 * PI;   // polar
    float sphi = sin(phi);
    return vec3(r * sphi * cos(theta), r * cos(phi), r * sphi * sin(theta));
}

mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c, t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c, t.y * axis.z - s * axis.x,
        t.z * axis.x - s * axis.y, t.z * axis.y + s * axis.x, t.z * axis.z + c
    );
}

// Axis-aligned cube face normal from aPos (local space)
vec3 localCubeFaceNormal(vec3 p) {
    vec3 ap = abs(p);
    if (ap.x >= ap.y && ap.x >= ap.z) return vec3(sign(p.x), 0.0, 0.0);
    if (ap.y >= ap.x && ap.y >= ap.z) return vec3(0.0, sign(p.y), 0.0);
    return vec3(0.0, 0.0, sign(p.z));
}

void main()
{
    int id = gl_InstanceID;
    const int number_per_dimension = 1000;
    id =  id + (number_per_dimension * number_per_dimension) * int(uDrawcallNumber);



    // Per-instance randomness
    uint s0 = uSeed + uint(id + 0);
    uint s1 = uSeed + uint(id + 42);
    float rnd_x = rand01(s0);
    float rnd_y = rand01(s1);

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


    float radius = 4.0 + radius_offset;
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    float px = sphere_position.x;
    float py = sphere_position.y;
    float pz = sphere_position.z;

    if(false)
    {
        float nx = py;
        float ny = px;

        float factor = fract(rnd_cube_rotation_x * 10.2);
        float inv_factor = 1.0f - factor;
        px = nx * factor + px * inv_factor;
        py = ny * factor + py * inv_factor;
    }

    
    

    // Instance transform (tiny cubes, uniform scale)
    float scale_cube = 0.01 * 0.7 * 2.0 * 2.0 * 2.0 * 0.4 * 0.27 + 0.1 * radius_offset;
    vec3  pos = vec3(px, pz, py);
    vec3  scale = vec3(scale_cube);

    




    float x = float(id / 1000);
    float y = float(id % 1000) * 0.01;
    float z = float(id) * 0.1;


    uint s0n = uSeed + uint(id + 10);
    uint s1n = uSeed + uint(id + 142);
    float rnd_xn = rand01(s0n);
    float rnd_yn = rand01(s1n);
    
    
    

    // pos.x = x * 0.01;
    // pos.y = 0.0;
    // pos.z = y;
    // The output color

    float factor_radius_offset = pow(max(0.0, radius_offset), 4.0) * 10.0;
    

    float color_r = px + sin(radius * 10.0);
    float color_g = py + factor_radius_offset * 100.0;
    float color_b = pz;

    color_r *= 0.1;
    color_g *= 0.1;
    color_b *= 0.1;

    
    
    
    vec4 new_position = vec4(vec3(pos), 1.0);

    {
        
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


        vec3 axis = normalize(vec3(1.0, 1.0, 0.0));
        float angle = uTime;
        mat3 R3 = axisAngleToMat3(axis, angle);
        mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));


        
        mat4 T = mat4(1.0);

        vec3 offset = vec3(sin(uTime + rnd_instance_0 * 10.0) * 10.0, sin(uTime + rnd_instance_1 * 0.0) * 10.0, 0.0);
        T[3] = vec4(offset, 1.0);


        mat4 S = mat4(1.0);
        S[0][0] = 1.0;
        S[1][1] = 1.0;
        S[2][2] = 2.0 + 1.0 * sin(uTime + rnd_instance_scale_z * 10.0);

        new_position =  T * R * S * new_position;
    }
    
    

    pos = new_position.xyz;




    // Per-instance tint (kept neutral here)
    color_vs = vec3(color_r, color_g, color_b);

    // Build TRS
    mat4 T = mat4(1.0); T[3] = vec4(pos, 1.0);
    vec3 axis = normalize(vec3(rnd_cube_rotation_x, rnd_cube_rotation_y, rnd_cube_rotation_z));
    float angle = rnd_cube_rotation_angle;//uTime * 0.0;
    mat3 R3 = axisAngleToMat3(axis, angle);
    mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
    mat4 S = mat4(1.0); S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;

    mat4 instanceModel = T * R * S;
    mat4 M = model * instanceModel;

    // World-space position (for lighting)
    vec4 wp = M * vec4(aPos, 1.0);
    vWorldPos = wp.xyz;

    // World-space normal:
    // Fast path (assumes uniform scale): rotate the face normal by model rotation and R3.
    // If you later use non-uniform model scale, switch to normal matrix:
    //   mat3 N = transpose(inverse(mat3(M)));
    //   vNormal = normalize(N * nLocal);
    vec3 nLocal = localCubeFaceNormal(aPos);
    vNormal = normalize(mat3(model) * (R3 * nLocal)); // uniform-scale assumption

    // Clip-space position and UV
    gl_Position = projection * view * wp;
    TexCoord = aTexCoord;


    // World position color

    // float world_x = wp.x;
    // float world_y = wp.y;
    // float world_z = wp.z;
    // color_vs = vec3(sin(world_x * 10.0), sin(world_y * 10.0), sin(world_z * 10.0));
}



