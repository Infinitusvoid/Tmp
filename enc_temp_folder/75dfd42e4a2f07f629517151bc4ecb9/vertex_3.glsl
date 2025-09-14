#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 color_vs;

// Matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// --- You already have these; keep whatever you use from them ---
uniform float uTime;
uniform uint  uSeed;
uniform uint  uDrawcallNumber;
// (u0..u9 etc. available if you use them)

// Lighting varyings
out vec3 vWorldPos;
out vec3 vCubeLocalPos;
// NOTE: some drivers dislike flat mat3. If that happens, pass 3 flat vec3 columns instead.
flat out mat3 vNormalMat;

// Camera basis (derived from 'view'; no CPU changes needed)
flat out vec3 vCamPos;   // camera world position
flat out vec3 vCamFwd;   // camera forward in world space (-Z in camera space)

// --------- Helpers you actually use ----------
uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}
float rand01(inout uint s) {
    s = pcg_hash(s);
    return float(s) * (1.0 / 4294967295.0);
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

// Spherical helper (you were using this)
vec3 spherical01(float r, float theta01, float phi01) {
    const float PI = 3.14159265358979323846;
    const float TAU = 6.28318530717958647692;
    float theta = theta01 * TAU; // azimuth
    float phi = phi01 * PI;  // polar
    float sphi = sin(phi);
    return vec3(r * sphi * cos(theta), r * cos(phi), r * sphi * sin(theta));
}

void main() {
    // -------- Example instance placement (keep your own logic if you wish) --------
    int id = gl_InstanceID;
    const int N = 1000; // matches your code intent
    id += (N * N) * int(uDrawcallNumber);

    uint s0 = uSeed + uint(id);
    float rx = rand01(s0);
    uint s1 = uSeed + uint(id + 42u);
    float ry = rand01(s1);

    // Positions on a sphere (your earlier pattern)
    float radius = 2.0;
    vec3 spherePos = spherical01(radius, rx, ry);

    // You swapped Y/Z in your code; keep that to stay visually consistent
    vec3 pos = vec3(spherePos.x, spherePos.z, spherePos.y);
    vec3 scale = vec3(0.01);

    // Simple (optionally static) rotation; you had uTime*0.0 previously
    vec3 axis = normalize(vec3(0.0, 1.0, 0.0));
    float ang = 0.0; // keep zero if you dont want rotation
    mat3  R3 = axisAngleToMat3(axis, ang);

    // Build instance model
    mat4 T = mat4(1.0); T[3] = vec4(pos, 1.0);
    mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
    mat4 S = mat4(1.0); S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;
    mat4 instanceModel = T * R * S;

    // World position / normal matrix / local pos
    vec4 wp = model * instanceModel * vec4(aPos, 1.0);
    vWorldPos = wp.xyz;
    vCubeLocalPos = aPos;
    vNormalMat = transpose(inverse(mat3(model * instanceModel)));

    // -------- Derive camera basis from 'view' (NO inverse(mat4) needed) --------
    // view = [ R  t; 0 1 ] with R = world->camera 3x3, t = translation column
    mat3 Rwc = mat3(view);           // world->camera rotation
    mat3 Rcw = transpose(Rwc);       // camera->world rotation (orthonormal)
    vec3 t = view[3].xyz;          // translation column of 'view'
    vCamPos = -Rcw * t;            // camera world position
    vCamFwd = normalize(Rcw * vec3(0.0, 0.0, -1.0)); // -Z (camera)  world

    // Standard position
    gl_Position = projection * view * wp;

    // Any tint you like (kept simple here)
    color_vs = vec3(1.0);
    TexCoord = aTexCoord;
}
