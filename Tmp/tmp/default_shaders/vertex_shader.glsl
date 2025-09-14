

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
const float PI  = 3.1415926535897932384626433832795;
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
    float phi   = phi01  * PI;   // polar
    float sphi = sin(phi);
    return vec3(r * sphi * cos(theta), r * cos(phi), r * sphi * sin(theta));
}

mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c,     t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c,     t.y * axis.z - s * axis.x,
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
    id += (number_per_dimension * number_per_dimension) * int(uDrawcallNumber);

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
    float radius = 1.0 + 0.2 * sin(rnd_x * TAU * 10.0) + 0.001 * sin(rnd_y * 100 * TAU + uTime * 1.0);
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    float px = sphere_position.x;
    float py = sphere_position.y;
    float pz = sphere_position.z;

    // Instance transform (tiny cubes, uniform scale)
    float scale_cube = 0.01 * 0.7 * 2.0 * 2.0 * 2.0 * 0.4 * 0.1;
    vec3  pos   = vec3(px, pz, py);
    vec3  scale = vec3(scale_cube);

    // Per-instance tint (kept neutral here)
    color_vs = vec3(0.2, 0.2, 0.2);

    // Build TRS
    mat4 T = mat4(1.0); T[3] = vec4(pos, 1.0);
    vec3 axis = normalize(vec3(rnd_cube_rotation_x, rnd_cube_rotation_y, rnd_cube_rotation_z));
    float angle = rnd_cube_rotation_angle;//uTime * 0.0;
    mat3 R3 = axisAngleToMat3(axis, angle);
    mat4 R = mat4(vec4(R3[0],0.0), vec4(R3[1],0.0), vec4(R3[2],0.0), vec4(0,0,0,1));
    mat4 S = mat4(1.0); S[0][0]=scale.x; S[1][1]=scale.y; S[2][2]=scale.z;

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
}




