#pragma once

namespace DefaultShaderSourceCode_
{
    std::string GLSL_Vertex = R"glsl(


#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

out vec3 color_vs;

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
vec2  saturate(vec2  x) { return clamp(x, 0.0, 1.0); }
vec3  saturate(vec3  x) { return clamp(x, 0.0, 1.0); }

float remap(float x, float a, float b, float c, float d) {
    return c + (d - c) * ((x - a) / (b - a));
}

vec3 safeNormalize(vec3 v) {
    float m = max(length(v), 1e-20);
    return v / m;
}

// safe division that preserves sign and avoids blowups
float safeDiv(float num, float den, float eps) {
    float a = abs(den);
    return num / max(a, eps) * sign(den);
}

// sign-preserving power to avoid NaNs with negative bases
float spow(float x, float e) {
    return sign(x) * pow(abs(x), e);
}

// ---------- Quick polar helpers ----------
vec2 polar(float r, float a) { return r * vec2(cos(a), sin(a)); }
vec3 spherical(float r, float theta, float phi) {
    // theta: azimuth [0,2 * pi), phi: polar [0,pi]
    return vec3(
        r * sin(phi) * cos(theta),
        r * cos(phi),
        r * sin(phi) * sin(theta)
    );
}

// theta01: 0..1  -> azimuth 0..2 * pi (around Y axis)
// phi01  : 0..1  -> polar   0..pi  (0 = north pole, 1 = south pole)
vec3 spherical01(float r, float theta01, float phi01) {
    const float PI = 3.14159265358979323846;
    const float TAU = 6.28318530717958647692; // 2*PI

    float theta = theta01 * TAU; // azimuth
    float phi = phi01 * PI;  // polar

    float sphi = sin(phi);
    return vec3(
        r * sphi * cos(theta),
        r * cos(phi),
        r * sphi * sin(theta)
    );
}

// --------- Small hash (PCG-ish) ----------
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

// Axis-angle -> mat3 (Rodrigues)
mat3 axisAngleToMat3(vec3 axis, float a) {
    float c = cos(a), s = sin(a);
    vec3 t = (1.0 - c) * axis;
    return mat3(
        t.x * axis.x + c, t.x * axis.y - s * axis.z, t.x * axis.z + s * axis.y,
        t.y * axis.x + s * axis.z, t.y * axis.y + c, t.y * axis.z - s * axis.x,
        t.z * axis.x - s * axis.y, t.z * axis.y + s * axis.x, t.z * axis.z + c
    );
}

// Scene Functions












// ---------- Weight selector config ----------
const int N_WEIGHTS = 22;       // number of effects you want to choose from (0..20)
const float PERIOD = 1.0;      // seconds per "scene" (try 4..12)
const float AMP_MIN = 0.35;     // min per-slot amplitude
const float AMP_MAX = 1.00;     // max per-slot amplitude
const float FADE_SHAPE = 1.0;   // 1=linear Hermite-ish; >1 sharper, <1 softer

// Stagger the 4 slots so they don`t all flip at once (keeps scene readable)
const vec4 SLOT_OFFSETS = vec4(0.00, 0.27, 0.54, 0.79);

// Hash -> random float without mutating an in/out state
float rand01u(uint x) {
    x = pcg_hash(x);
    return float(x) * (1.0 / 4294967295.0);
}

// Get a random int in [0, maxExclusive)
int randInt(uint seed, int maxExclusive) {
    return int(uint(pcg_hash(seed)) % uint(maxExclusive));
}

// Pick 4 distinct indices using simple rehash-on-collision
ivec4 pick4Distinct(uint seed, int maxExclusive) {
    int a = randInt(seed + 11u, maxExclusive);

    int b = a;
    uint sb = seed + 23u;
    for (int i = 0; i < 16 && b == a; ++i) { b = randInt(sb, maxExclusive); sb += 97u; }

    int c = a;
    uint sc = seed + 37u;
    for (int i = 0; i < 24 && (c == a || c == b); ++i) { c = randInt(sc, maxExclusive); sc += 97u; }

    int d = a;
    uint sd = seed + 59u;
    for (int i = 0; i < 32 && (d == a || d == b || d == c); ++i) { d = randInt(sd, maxExclusive); sd += 97u; }

    return ivec4(a, b, c, d);
}

// Smooth 0->1 curve you can tune
float ease01(float x) {
    x = clamp(x, 0.0, 1.0);
    // Hermite, then optional shaping
    float h = x * x * (3.0 - 2.0 * x);
    return pow(h, FADE_SHAPE);
}




// HSV -> RGB (fast, branchless)
vec3 hsv2rgb(vec3 c) {
    vec3 p = abs(fract(c.xxx + vec3(0.0, 1.0, 2.0) / 3.0) * 6.0 - 3.0);
    return c.z * mix(vec3(1.0), clamp(p - 1.0, 0.0, 1.0), c.y);
}

// RGB -> HSV (matches hsv2rgb above)
vec3 rgb2hsv(vec3 c) {
    float cMax = max(c.r, max(c.g, c.b));
    float cMin = min(c.r, min(c.g, c.b));
    float d = cMax - cMin;

    float h = 0.0;
    if (d > 1e-6) {
        if (cMax == c.r)      h = mod((c.g - c.b) / d, 6.0);
        else if (cMax == c.g) h = ((c.b - c.r) / d) + 2.0;
        else                  h = ((c.r - c.g) / d) + 4.0;
        h /= 6.0;
        if (h < 0.0) h += 1.0;
    }
    float s = (cMax <= 1e-6) ? 0.0 : d / cMax;
    float v = cMax;
    return vec3(h, s, v);
}



// A simple deterministic palette per effect index
vec3 effectColor(int i) {
    // Golden-ratio hue spacing keeps colors distinct across 21 effects
    float h = fract(0.61803398875 * float(i) + 0.17);
    float s = 0.70;
    float v = 0.90;
    return hsv2rgb(vec3(h, s, v));
}





void main()
{
    int id = gl_InstanceID;

    // Constanst
    const int number_per_dimension = 1000;

    // Well usualls universally usefull Calculations
    id = id + (number_per_dimension * number_per_dimension) * int(uDrawcallNumber);


    uint s_0 = uSeed + uint(id);
    float rnd_x = rand01(s_0);

    uint s_1 = uSeed + uint(id + 42);
    float rnd_y = rand01(s_1);

    float px = rnd_x;
    float py = rnd_y;
    float pz = 0.0;


    // --- SPHERE CALCULATION ---
    float radius = 100.0;
    
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    px = sphere_position.x;
    py = sphere_position.y;
    pz = sphere_position.z;

    



    float scale_cube = 0.01 * 0.7 * 2.0 * 2.0 * 2.0 * 0.4 * 10.0;

    // 312312
    float color_r = 0.2;
    float color_g = 0.2;
    float color_b = 0.2;

    // Prepering data for the next block
    color_vs = vec3(color_r, color_g, color_b);
    vec3 pos = vec3(px, pz, py);
    vec3 scale = vec3(scale_cube, scale_cube, scale_cube);




    // The block that stays the same 
    {
        // Matrices to calculate instance transform

        mat4 T = mat4(1.0);
        mat4 R = mat4(1.0);
        mat4 S = mat4(1.0);



        // Set Transform
        // pos = vec3(id * 2.0, 0.0, 0.0);

        T[3] = vec4(pos, 1.0);

        // Set rotation
        vec3 axis = vec3(0.0, 1.0, 0.0);
        axis = normalize(axis);

        float angle = uTime * 1.0;

        mat3 R3 = axisAngleToMat3(axis, angle);

        R = mat4(
            vec4(R3[0], 0.0),
            vec4(R3[1], 0.0),
            vec4(R3[2], 0.0),
            vec4(0.0, 0.0, 0.0, 1.0)
        );

        // Set Scale
        S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;



        mat4 instanceModel = T * R * S;

        

        gl_Position = projection * view * model * instanceModel * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }


}



)glsl";
















    std::string GLSL_Fragment = R"glsl(
#version 450 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float uTime;          // seconds
uniform float uRingFreq;      // rings per face radius (e.g. 8.0)
uniform float uSpeed;         // animation speed (e.g. 0.35)
uniform float uGlowWidth;     // ring width, in [0.01..0.25] (e.g. 0.08)
uniform float uGlowStrength;  // HDR emission gain (e.g. 3.0)
uniform float uStripeCount;   // angular stripes (e.g. 12.0)
uniform float uStripeWidth;   // stripe width (e.g. 0.18)
uniform float uBase;          // base albedo scale (e.g. 0.03)


uniform vec3 uCameraPos;
uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;

const float PI = 3.14159265359;

in vec3 color_vs;

// Little color palette (Inigo Quilez style)
vec3 palette(float t) {
    vec3 a = vec3(0.55, 0.45, 0.60);
    vec3 b = vec3(0.45, 0.35, 0.40);
    vec3 c = vec3(1.00, 1.00, 1.00);
    vec3 d = vec3(0.00, 0.33, 0.67);
    return a + b * cos(2.0 * PI * (c * t + d));
}

// Gaussian helper (for soft peaks)
float gauss(float x, float s) {
    return exp(-(x * x) / (2.0 * s * s));
}

void main()
{
    // Face-local UV centered at 0
    vec2 uv = TexCoord;
    vec2 p = uv - 0.5;
    float r = length(p) + 1e-6;      // radial distance
    float a = atan(p.y, p.x);        // angle [-pi,pi]

    // ----- Concentric rings (animated) -----
    // Repeating “saw” over radius, then shape into a bright thin ring with gaussian
    float saw = fract(r * uRingFreq - uSpeed * uTime);  // 0..1 over each ring period
    float ring = gauss(saw - 0.5, uGlowWidth);          // thin bright arc

    // ----- Angular stripes (also animated) -----
    // Map angle to 0..1, then to N stripes and shape with gaussian
    float ang01 = (a / (2.0 * PI)) + 0.5;
    float sawAng = fract(ang01 * uStripeCount - 0.2 * uSpeed * uTime);
    float stripes = gauss(sawAng - 0.5, uStripeWidth);

    // ----- Edge halo so cube borders pop a bit -----
    float edge = min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y)); // distance to nearest edge
    float edgeGlow = smoothstep(0.15, 0.0, edge); // 1 at edges, 0 inside

    // Compose an intensity field (keep it in ~0..1 range before emission)
    float glowMask = ring + 0.6 * stripes + 0.4 * edgeGlow;

    // Vary hue with radius + time for a nice gradient
    vec3 hue = palette(r * 1.2 + 0.05 * uTime + ang01 * 0.15);

    // Base (very dim albedo so faces aren’t pitch black)
    vec3 base = hue * uBase;

    // Emissive HDR glow (this is what bloom picks up!)
    vec3 emissive = hue * glowMask * uGlowStrength;

    vec3 color = base + emissive;  // HDR
    FragColor = vec4(color * color_vs, 1.0);
}



)glsl";




















    std::string GLSL_bloom_blur_fs = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uTex;
uniform vec2 uTexelSize;   // 1.0/width, 1.0/height
uniform vec2 uDirection;   // (1,0)=horizontal, (0,1)=vertical

void main() {
    // 5-tap Gaussian weights (normalized)
    float w0 = 0.4026199469; // center
    float w1 = 0.2442013420; // ±1
    float w2 = 0.0544886845; // ±2

    vec2 off1 = uDirection * uTexelSize * 1.0;
    vec2 off2 = uDirection * uTexelSize * 2.0;

    vec3 c = texture(uTex, vUV).rgb * w0
        + texture(uTex, vUV + off1).rgb * w1
        + texture(uTex, vUV - off1).rgb * w1
        + texture(uTex, vUV + off2).rgb * w2
        + texture(uTex, vUV - off2).rgb * w2;

    FragColor = vec4(c, 1.0);
}

)glsl";


    std::string GLSL_bloom_bright_fs = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uInput;   // HDR input (accumTex)
uniform float uThreshold;   // e.g. 1.0 (if your emissive is ~1–4)
uniform float uSoftKnee;    // 0..1 (e.g. 0.5)
uniform bool  uUseLuma;     // true: use luminance, false: use max channel

vec3 toLumaCoeff() { return vec3(0.2126, 0.7152, 0.0722); }

void main() {
    vec3 c = texture(uInput, vUV).rgb;

    float m = uUseLuma
        ? dot(c, toLumaCoeff())
        : max(max(c.r, c.g), c.b);

    float t = max(uThreshold, 1e-6);
    float k = max(t * uSoftKnee, 1e-6);

    // Soft knee around threshold
    float x = max(m - (t - k), 0.0);
    float y = (x * x) / (4.0 * k + x);  // 0..~m, smooth near t

    // Color-preserving: scale original color by y/m
    float gain = (m > 1e-6) ? (y / m) : 0.0;
    vec3 outc = c * gain;

    FragColor = vec4(outc, 1.0);
}

)glsl";



    std::string GLSL_le_combine_fs = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;
uniform sampler2D uPrevAccum;
uniform sampler2D uScene;
uniform float uDecay;    // e.g. 0.97
uniform float uAddGain;  // 1.0 for neutral

void main() {
    vec3 prev = texture(uPrevAccum, vUV).rgb;
    vec3 curr = texture(uScene, vUV).rgb;
    float k = clamp(1.0 - uDecay, 0.0, 1.0);
    vec3 outc = prev * uDecay + curr * (k * uAddGain);
    FragColor = vec4(outc, 1.0);
}



)glsl";



    std::string GLSL_le_fullscreen_vs = R"glsl(
#version 450 core
out vec2 vUV;
void main() {
    // Fullscreen triangle covering NDC [-1,1]
    // (0,0), (2,0), (0,2) in clip space trick
    const vec2 pos[3] = vec2[3](
        vec2(-1.0, -1.0),
        vec2(3.0, -1.0),
        vec2(-1.0, 3.0)
        );
    vUV = (pos[gl_VertexID] * 0.5) + 0.5; // map NDC to [0,1]
    gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
}



)glsl";


    std::string GLSL_le_present_fs = R"glsl(
#version 450 core
in vec2 vUV;
layout(location = 0) out vec4 FragColor;

uniform sampler2D uAccum;        // HDR after long exposure
uniform sampler2D uBloom;        // blurred bright regions (can be half-res)
uniform float uBloomStrength;    // 0..2  (e.g. 0.8)

uniform float uExposure;         // HDR exposure before tone map (e.g. 1.2)
uniform float uGamma;            // 2.2

// LDR post FX
uniform float uBrightness;       // add after tone-map  (-1..+1, e.g. 0.0)
uniform float uContrast;         // scale around 0.5    (0..2,   e.g. 1.0)
uniform float uSaturation;       // 0..2 (1=identity)
uniform float uVignetteStrength; // 0..1
uniform float uVignetteStart;    // 0..1 (radius where vignetting starts)

void main() {
    vec3 hdr = texture(uAccum, vUV).rgb;
    vec3 glow = texture(uBloom, vUV).rgb;

    // Add bloom in HDR
    hdr += glow * uBloomStrength;

    // Simple tonemap (exponential)
    vec3 mapped = vec3(1.0) - exp(-hdr * uExposure);

    // LDR post-processing
    // Contrast around 0.5
    mapped = (mapped - 0.5) * uContrast + 0.5;
    // Brightness (add)
    mapped += vec3(uBrightness);
    // Saturation
    float luma = dot(mapped, vec3(0.2126, 0.7152, 0.0722));
    mapped = mix(vec3(luma), mapped, uSaturation);
    // Vignette
    float d = length(vUV - vec2(0.5)) * 1.41421356; // 0..~1 across diagonal
    float v = smoothstep(uVignetteStart, 1.0, d);
    mapped *= (1.0 - uVignetteStrength * v);

    // Gamma
    mapped = pow(max(mapped, 0.0), vec3(1.0 / max(uGamma, 1e-6)));
    FragColor = vec4(clamp(mapped, 0.0, 1.0), 1.0);
}



)glsl";



}




