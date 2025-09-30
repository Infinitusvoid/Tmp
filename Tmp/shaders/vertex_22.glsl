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


// Wrap helpers (0..1 domain)
float wrap01(float x) { return x - floor(x); }
float wrapDelta01(float d) { return wrap01(d + 0.5) - 0.5; } // shortest wrapped delta

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

// Map (lon01, lat01) in [0,1]x[0,1] to unit direction on Y-up sphere
vec3 dir_from_lonlat01(float lon01, float lat01) {
    float theta = lon01 * TAU; // azimuth around Y
    float phi = lat01 * PI;  // polar from +Y
    float sphi = sin(phi);
    return vec3(sphi * cos(theta), cos(phi), sphi * sin(theta));
}

// Slerp between unit vectors (shortest arc)
vec3 slerp_unit(vec3 a, vec3 b, float t) {
    float d = clamp(dot(a, b), -1.0, 1.0);
    float w = acos(d);
    if (w < 1e-5) return normalize(mix(a, b, t));
    float s = sin(w);
    return (sin((1.0 - t) * w) / s) * a + (sin(t * w) / s) * b;
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
    // ---- Instance param t in [0,1]
    int   N = max(uGrid.x, 1);
    float t = (N == 1) ? 0.0 : float(gl_InstanceID) / float(N - 1);

    // ---- Unwrapped inputs (no fract/clamp)
   // Interpret x as longitude, y as latitude
    float lon0 = u1;  // x0 = longitude start
    float lat0 = u0;  // y0 = latitude start
    float lon1 = u3;  // x1 = longitude end
    float lat1 = u2;  // y1 = latitude end

    float turns = u4;   // extra whole revolutions in longitude (1.0 == +360°)
    float R = max(u5, 1e-6);
    float thick = max(u9, 1e-6);

    // ---- Linear sweep in lon/lat space
    float dLon = (lon1 - lon0) + turns;   // add full loops explicitly
    float dLat = (lat1 - lat0);
    float lon = lon0 + t * dLon;
    float lat = lat0 + t * dLat;

    // ---- Map (lon,lat) -> unit direction D on a Y-up sphere
    float theta = lon * TAU;          // azimuth
    float phi = lat * PI;           // polar from +Y
    float sphi = sin(phi), cphi = cos(phi);
    float cth = cos(theta), sth = sin(theta);

    vec3 D = normalize(vec3(sphi * cth,  // X
        cphi,        // Y
        sphi * sth));// Z

    // ---- Analytic tangent (stable cube orientation)
    vec3 dD_dlon = TAU * vec3(-sphi * sth, 0.0, sphi * cth);
    vec3 dD_dlat = PI * vec3(cphi * cth, -sphi, cphi * sth);
    vec3 dDds = dD_dlon * dLon + dD_dlat * dLat;

    vec3 T = normalize(dDds);         // tangent along the sweep
    vec3 B = normalize(cross(D, T));  // binormal
    T = cross(B, D);             // re-orthonormalize

    // ---- Place and orient the cube
    vec3 P = D * R;
    mat3 R3 = mat3(T, D, B);          // columns: X=tangent, Y=radial, Z=binormal

    mat4 Tm = mat4(1.0); Tm[3] = vec4(P, 1.0);
    mat4 Rm = mat4(vec4(R3[0], 0.0),
        vec4(R3[1], 0.0),
        vec4(R3[2], 0.0),
        vec4(0.0, 0.0, 0.0, 1.0));
    mat4 Sm = mat4(1.0);
    Sm[0][0] = thick; Sm[1][1] = thick; Sm[2][2] = thick;

    mat4 instanceModel = Tm * Rm * Sm;
    mat4 M = model * instanceModel;

    // ---- World position
    vec4 wp = M * vec4(aPos, 1.0);
    vWorldPos = wp.xyz;

    // ---- World normal (uniform scale path -> just rotate)
    vec3 nLocal = localCubeFaceNormal(aPos);
    vNormal = normalize(mat3(model) * (R3 * nLocal));

    // ---- Vertex color (already interpolated by host)
    color_vs = vec3(u6, u7, u8);
    TexCoord = aTexCoord;

    // ---- Tiny deterministic jitter to reduce aliasing
    vec4 clip = projection * view * wp;

    uint j0 = pcg_hash(uint(gl_InstanceID) ^ (uDrawcallNumber * 0x85EBCA6Bu) ^ uSeed);
    uint j1 = pcg_hash(j0 ^ 0xB5297A4Du);
    vec2 jitterPx = vec2(float(j0 >> 8), float(j1 >> 8)) * (1.0 / 16777216.0) - 0.5;
    jitterPx *= 0.75; // ~±0.375 px
    clip.xy += jitterPx * (2.0 / kViewportPx) * clip.w;

    uint jz = pcg_hash(j1 ^ 0x68BC21EBu);
    float dz = (float(jz >> 8) * (1.0 / 16777216.0) - 0.5);
    clip.z += dz * 1e-5 * clip.w;

    gl_Position = clip;
}