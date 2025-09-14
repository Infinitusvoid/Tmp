#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 color_vs;

// From VS
in vec3 vWorldPos;
in vec3 vCubeLocalPos;
flat in mat3 vNormalMat;
flat in vec3 vCamPos;
flat in vec3 vCamFwd;

// Your existing controls (no new uniforms required)
uniform float uTime, uRingFreq, uSpeed, uGlowWidth, uGlowStrength, uStripeCount, uStripeWidth, uBase;
// u0..u9 available; well use u0 as head-light distance if > 0
uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;

const float PI = 3.14159265359;
const float AMBIENT = 0.08;
const float LIGHT_INT = 25.0;
const float LIGHT_RANGE = 12.0;
const float SPEC_POWER = 32.0;
const float RIM_STRENGTH = 0.18;
const float RIM_POWER = 2.0;
const float GAMMA = 2.2;

// ----- small helpers you actually use -----
vec3 palette(float t) {
    vec3 a = vec3(0.55, 0.45, 0.60);
    vec3 b = vec3(0.45, 0.35, 0.40);
    vec3 c = vec3(1.00, 1.00, 1.00);
    vec3 d = vec3(0.00, 0.33, 0.67);
    return a + b * cos(2.0 * PI * (c * t + d));
}
float gauss(float x, float s) { return exp(-(x * x) / (2.0 * s * s)); }

vec3 faceNormalLocal(vec3 p) {
    vec3 ap = abs(p);
    if (ap.x >= ap.y && ap.x >= ap.z) return vec3(sign(p.x), 0, 0);
    if (ap.y >= ap.z)               return vec3(0, sign(p.y), 0);
    return                            vec3(0, 0, sign(p.z));
}

void main() {
    // ----- your stylized pattern (albedo + emissive) -----
    vec2 uv = TexCoord, q = uv - 0.5;
    float r = length(q) + 1e-6;
    float ang01 = (atan(q.y, q.x) / (2.0 * PI)) + 0.5;

    float ring = gauss(fract(r * uRingFreq - uSpeed * uTime) - 0.5, uGlowWidth);
    float stripes = gauss(fract(ang01 * uStripeCount - 0.2 * uSpeed * uTime) - 0.5, uStripeWidth);
    float edge = min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y));
    float edgeGlow = smoothstep(0.15, 0.0, edge);

    float glowMask = ring + 0.6 * stripes + 0.4 * edgeGlow;
    vec3  hue = palette(r * 1.2 + 0.05 * uTime + ang01 * 0.15);

    vec3 albedo = hue * uBase;
    vec3 emissive = hue * glowMask * uGlowStrength;

    // ----- per-pixel normal (analytic cube face) -----
    vec3 N = normalize(vNormalMat * faceNormalLocal(vCubeLocalPos));
    // fallback: N = normalize(cross(dFdx(vWorldPos), dFdy(vWorldPos)));

    // ======= HEAD-LIGHT exactly in front of camera =======
    float headDist = 0.0;// (u0 > 0.0) ? u0 : 2.0; // meters in front; tweak with your existing u0
    vec3  lightPos = vCamPos + vCamFwd * headDist;

    vec3  Lvec = lightPos - vWorldPos;
    float d = length(Lvec);
    vec3  L = (d > 0.0) ? (Lvec / d) : vec3(0, 1, 0);

    // inverse-square with soft range
    float atten = 1.0 / max(d * d, 1e-6);
    float s = clamp(1.0 - d / LIGHT_RANGE, 0.0, 1.0);
    atten *= s * s;

    float NdotL = max(dot(N, L), 0.0);

    // view direction uses the SAME camera source
    vec3 V = normalize(vCamPos - vWorldPos);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), SPEC_POWER);

    // rim (helps readability on tiny cubes)
    float rim = pow(clamp(1.0 - max(dot(N, V), 0.0), 0.0, 1.0), RIM_POWER);
    vec3  rimTerm = RIM_STRENGTH * rim * hue;

    // combine
    vec3 radiance = vec3(LIGHT_INT) * atten;
    vec3 lit = AMBIENT * albedo + (albedo * NdotL + spec) * radiance;
    lit += rimTerm;
    lit += emissive;

    // gamma
    vec3 outRGB = pow(max(lit, 0.0), vec3(1.0 / GAMMA));
    FragColor = vec4(outRGB * color_vs, 1.0);
}
