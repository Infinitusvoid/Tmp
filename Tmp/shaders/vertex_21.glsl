#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

// outputs to fragment
out vec2 TexCoord;
out vec3 color_vs;
out vec3 vWorldPos;
out vec3 vNormal;

// uniforms
uniform mat4 model;       // can be identity
uniform mat4 view;
uniform mat4 projection;
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

// // ---------- Constants & tiny helpers ----------
const float PI = 3.1415926535897932384626433832795;
const float TAU = 6.2831853071795864769252867665590;

const vec2 kViewportPx = vec2(3840.0, 2160.0); // UHD-4K

float saturate(float x) { return clamp(x, 0.0, 1.0); }


uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}


// float rand01(inout uint s) { s = pcg_hash(s); return float(s) * (1.0 / 4294967295.0); }

float rand01(inout uint s) {
    s += 0x9E3779B9u;               // Weyl step
    uint z = s;                     // splitmix32 finalizer
    z ^= z >> 16; z *= 0x7FEB352Du;
    z ^= z >> 15; z *= 0x846CA68Bu;
    z ^= z >> 16;
    return float(z >> 8) * (1.0 / 16777216.0);
}

// Drop-in replacement: same signature
/*
float rand01(inout uint s)
{
    // 1) Weyl sequence step to decorrelate nearby seeds
    s += 0x9E3779B9u;

    // 2) splitmix32 finalizer (strong 32-bit mixer)
    uint z = s;
    z ^= z >> 16;
    z *= 0x7FEB352Du;
    z ^= z >> 15;
    z *= 0x846CA68Bu;
    z ^= z >> 16;

    // 3) Map top 24 bits to [0,1) to avoid float-mantissa bias
    return float(z >> 8) * (1.0 / 16777216.0);
}
*/

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

    id =  id + (uGrid.x * uGrid.y * uGrid.z) * int(uDrawcallNumber);


    // well could be calculate the id max ? 
    // well yea well if we are going to have just one draw call per line which is kinda resonable as we will only have up to 1 million cube per one line which can be drawn in one drawcall 
    // than sure we can calculate that
    // the maximum is (uGrid.x * uGrid.y * uGrid.z) 
    
    int drawcalls_max = 1;
    int id_max = uGrid.x * uGrid.y * uGrid.z * drawcalls_max;
    float step_one_id = 1.0 / float(id_max);

    float factor_id = float(id) * step_one_id;


    
    // Per-instance randomness
        // uint s0 = uSeed + uint(id + 23);
        // uint s1 = uSeed + uint(id + 42);
        // uint s2 = uSeed + uint(id + 142);

        uint s0 = (id ^ (uSeed * 0xA511E9B3u)) + 0x9E3779B9u;
        uint s1 = (id ^ (uSeed * 0xC2B2AE35u)) + 0x85EBCA6Bu;
        uint s2 = (id ^ (uSeed * 0x27D4EB2Du)) + 0x165667B1u;

        float rnd_x = rand01(s0);
        float rnd_y = rand01(s1);
        float rnd_z = rand01(s2);

        

        uint s0r = uSeed + uint(id + 140);
        uint s1g = uSeed + uint(id + 2742);
        uint s2b = uSeed + uint(id + 32142);
        float rnd_r = rand01(s0r);
        float rnd_g = rand01(s1g);
        float rnd_b = rand01(s2b);
    
        // The instancd cube rotation randomization
        uint s0_rot_x = uSeed + uint(id + 2431);
        uint s1_rot_y = uSeed + uint(id + 4412);
        uint s2_rot_y = uSeed + uint(id + 1234);
        uint s3_rot_angle = uSeed + uint(id + 2332);
        float rnd_cube_rotation_x = rand01(s0_rot_x);
        float rnd_cube_rotation_y = rand01(s1_rot_y);
        float rnd_cube_rotation_z = rand01(s2_rot_y);
        float rnd_cube_rotation_angle = rand01(s3_rot_angle);

    
    const float jitter_scale = 1.0;

    /*
    I.set_u_start_end(0, x0.start, x0.end); // u0
			I.set_u_start_end(1, y0.start, y0.end); // u1
			I.set_u_start_end(2, z0.start, z0.end); // u2

			I.set_u_start_end(3, x1.start, x1.end); // u3
			I.set_u_start_end(4, y1.start, y1.end); // u4
			I.set_u_start_end(5, z1.start, z1.end); // u5

			I.set_u_start_end(6, rgb_t0.x, rgb_t1.x); // u6
			I.set_u_start_end(7, rgb_t0.y, rgb_t1.y); // u7
			I.set_u_start_end(8, rgb_t0.z, rgb_t1.z); // u8

			I.set_u_start_end(9, thickness.start, thickness.end); // u9
    */

    vec3 position_0 = vec3(u0, u1, u2);
    vec3 position_1 = vec3(u3, u4, u5);
    vec3 position = mix(position_0, position_1, factor_id);

    float color_r = u6;
    float color_g = u7;
    float color_b = u8;

    float scale_cube = u9;

    
    float px = position.x;
    float py = position.y;
    float pz = position.z;
    
    // Instances Cube Scale
    // float scale_cube = 0.001 * 0.1;
    vec3  pos = vec3(px, pz, py);
    vec3  scale = vec3(scale_cube, scale_cube, scale_cube);
    
    
    // Whole object rotation
    vec3 rotation_axis = vec3(0.0, 1.0, 0.0);
    float rotation_angle = uTime * 0.0; // using uTime will not be wise after we will be interpolating between two values
    
    // Whole object scale
    vec3 scale_object = vec3(1.0, 1.0, 1.0);
    
    
    vec4 new_position = vec4(vec3(pos), 1.0);
    
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
            // vec3 axis = normalize(vec3(0.0, 1.0, 1.0));
            vec3 axis = normalize(rotation_axis);
            // float angle = uTime;
            float angle = rotation_angle;
            mat3 R3 = axisAngleToMat3(axis, angle);
            mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
    
            // Translation
            mat4 T = mat4(1.0);
            vec3 offset = vec3(sin(uTime + rnd_instance_0 * 10.0) * 10.0, sin(uTime + rnd_instance_1 * 0.0) * 10.0, 0.0);
            offset = vec3(0.5, 0.5, 0.5);
            T[3] = vec4(offset, 1.0);
    
    
    
            // Scale
            mat4 S = mat4(1.0);
            S[0][0] = scale_object.x;
            S[1][1] = scale_object.y;
            S[2][2] = scale_object.z;
    
            new_position = T * R * S * new_position;
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
        // gl_Position = projection * view * wp;
        // ... after you compute wp (world pos) and BEFORE gl_Position:
        vec4 clip = projection * view * wp;

        // per-instance deterministic jitter in pixels (apporximitly +-0.5 px)
        uint j0 = pcg_hash(uint(gl_InstanceID) ^ (uDrawcallNumber * 0x85EBCA6Bu) ^ uSeed);
        uint j1 = pcg_hash(j0 ^ 0xB5297A4Du);
        vec2 jitterPx = vec2(float(j0 >> 8), float(j1 >> 8)) * (1.0 / 16777216.0) - 0.5;

        // scale if you want (0.0-1.0 px is typical)
        jitterPx *= 0.75 * 1.0 * jitter_scale;

        // convert pixel jitter -> clip-space: delta clip = (2 px / viewport) * w
        clip.xy += jitterPx * (2.0 / kViewportPx) * clip.w;

        // add this:
        uint jz = pcg_hash(j1 ^ 0x68BC21EBu);
        float dz = (float(jz >> 8) * (1.0 / 16777216.0) - 0.5);
        clip.z += dz * 1e-5 * clip.w;

        gl_Position = clip; // instead of projection*view*wp
        
        
        TexCoord = aTexCoord;
    
    
        // World position color
    
        // float world_x = wp.x;
        // float world_y = wp.y;
        // float world_z = wp.z;
        // color_vs = vec3(sin(world_x * 10.0), sin(world_y * 10.0), sin(world_z * 10.0)) * vec3(0.01, 0.01, 0.01);
}
