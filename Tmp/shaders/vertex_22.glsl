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
    // ----- Per-instance parameter along the arc [0..1]
    int   N = max(uGrid.x, 1);
    float s = (N == 1) ? 0.0 : float(gl_InstanceID) / float(N - 1);

    // ----- Endpoints on the sphere
    float lon0 = fract(u0);
    float lat0 = clamp(u1, 0.0, 1.0);
    float lon1 = fract(u2);
    float lat1 = clamp(u3, 0.0, 1.0);
    float R = max(u5, 1e-6);

    //vec3 d0 = dir_from_lonlat01(lon0, lat0);
    //vec3 d1 = dir_from_lonlat01(lon1, lat1);

    //// Great-circle plane normal
    //vec3 n = normalize(cross(d0, d1));
    //if (length(n) < 1e-5) {
    //    // Degenerate (same/similar points): pick any perpendicular to d0
    //    n = normalize(cross(d0, vec3(0.0, 1.0, 0.0)));
    //    if (length(n) < 1e-5) n = normalize(cross(d0, vec3(1.0, 0.0, 0.0)));
    //}

    //// Position on the arc
    //vec3 D = slerp_unit(d0, d1, s);     // radial direction
    //vec3 P = D * R;                      // point on sphere

    //// Build nice orientation for the cube:
    //// Tangent along the great-circle, Radial normal outwards, Binormal completes basis.
    //vec3 T = normalize(cross(n, D));     // tangent
    //vec3 B = cross(D, T);                // binormal (already unit if D,T are)
    //mat3 R3 = mat3(T, D, B);             // columns: X->tangent, Y->radial, Z->binormal


    vec3 d0 = dir_from_lonlat01(lon0, lat0);
    vec3 d1 = dir_from_lonlat01(lon1, lat1);

    // Classify intent
    float dLon = abs(wrapDelta01(lon1 - lon0));
    float dLat = abs(lat1 - lat0);
    float ddot = dot(normalize(d0), normalize(d1));

    bool vertical = (dLon < 1e-5);     // constant longitude -> full meridian ring
    bool horizontal = (dLat < 1e-5);     // constant latitude  -> full parallel ring
    bool antipodal = (ddot < -0.9999);  // opposite points -> treat as vertical ring

    vec3 D, T, B; // radial (out), tangent, binormal

    if (vertical || antipodal)
    {
        // Full meridian at longitude lon0: great circle using basis {u,v}
        float theta = lon0 * TAU;
        vec3  u = vec3(cos(theta), 0.0, sin(theta)); // equator dir at lon0
        vec3  v = vec3(0.0, 1.0, 0.0);               // +Y
        float a = TAU * s;                            // sweep full 0..2
        D = normalize(u * cos(a) + v * sin(a));
        T = normalize(-u * sin(a) + v * cos(a));
        B = cross(D, T);
    }
    else if (horizontal)
    {
        // Full parallel at latitude lat0: sweep longitude 0..2
        float theta = wrap01(lon0 + s);               // wrap 0..1
        D = normalize(spherical01(1.0, theta, lat0)); // unit radial
        // Eastward tangent
        T = normalize(cross(vec3(0.0, 1.0, 0.0), D));
        if (length(T) < 1e-6) T = normalize(cross(D, vec3(1.0, 0.0, 0.0)));
        B = cross(D, T);
    }
    else
    {
        // Shortest geodesic segment
        vec3 n = normalize(cross(d0, d1));
        if (length(n) < 1e-6) { // paranoid fallback
            float theta = lon0 * TAU;
            vec3  u = vec3(cos(theta), 0.0, sin(theta));
            vec3  v = vec3(0.0, 1.0, 0.0);
            float a = PI * s;
            D = normalize(u * cos(a) + v * sin(a));
            T = normalize(-u * sin(a) + v * cos(a));
            B = cross(D, T);
        }
        else {
            D = slerp_unit(normalize(d0), normalize(d1), s);
            T = normalize(cross(n, D));
            B = cross(D, T);
        }
    }

    // Position on sphere
    vec3 P = D * R;
    mat3 R3 = mat3(T, D, B);


    // Thickness (uniform scale)
    float thick = max(u9, 1e-6);

    // If your world expects Y/Z swapped (as in your previous shader), keep the swap:
    vec3 pos_world = vec3(P.x, P.y, P.z);          // swap Y/Z
    mat3 R3_swapped = mat3(
        vec3(R3[0].x, R3[0].z, R3[0].y),  // swap Y/Z per column
        vec3(R3[1].x, R3[1].z, R3[1].y),
        vec3(R3[2].x, R3[2].z, R3[2].y)
    );

    // Build TRS
    mat4 Tm = mat4(1.0); Tm[3] = vec4(pos_world, 1.0);
    mat4 Rm = mat4(
        vec4(R3_swapped[0], 0.0),
        vec4(R3_swapped[1], 0.0),
        vec4(R3_swapped[2], 0.0),
        vec4(0, 0, 0, 1)
    );
    mat4 Sm = mat4(1.0);
    Sm[0][0] = thick; Sm[1][1] = thick; Sm[2][2] = thick;

    mat4 instanceModel = Tm * Rm * Sm;
    mat4 M = model * instanceModel;

    // World-space position for the cube vertex
    vec4 wp = M * vec4(aPos, 1.0);
    vWorldPos = wp.xyz;

    // World-space normal (uniform scale path)
    vec3 nLocal = localCubeFaceNormal(aPos);
    vNormal = normalize(mat3(model) * (R3_swapped * nLocal));

    // Color (engine already interpolated u6..u8 for the current time)
    color_vs = vec3(u6, u7, u8);

    // Clip-space position with tiny deterministic jitter (to reduce aliasing)
    vec4 clip = projection * view * wp;

    uint j0 = pcg_hash(uint(gl_InstanceID) ^ (uDrawcallNumber * 0x85EBCA6Bu) ^ uSeed);
    uint j1 = pcg_hash(j0 ^ 0xB5297A4Du);
    vec2 jitterPx = vec2(float(j0 >> 8), float(j1 >> 8)) * (1.0 / 16777216.0) - 0.5;
    jitterPx *= 0.75; // ~±0.375 px

    // pixel jitter -> clip: delta clip = (2 px / viewport) * w
    clip.xy += jitterPx * (2.0 / kViewportPx) * clip.w;

    uint jz = pcg_hash(j1 ^ 0x68BC21EBu);
    float dz = (float(jz >> 8) * (1.0 / 16777216.0) - 0.5);
    clip.z += dz * 1e-5 * clip.w;

    gl_Position = clip;

    TexCoord = aTexCoord;
}