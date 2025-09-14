
#version 450 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 color_vs;

// from VS
in vec3 vWorldPos;
in vec3 vNormal;

uniform float uTime;
uniform float uRingFreq;
uniform float uSpeed;
uniform float uGlowWidth;
uniform float uGlowStrength;
uniform float uStripeCount;
uniform float uStripeWidth;
uniform float uBase;

uniform vec3 uCameraPos;
uniform vec3 uCameraDir;  // world-space forward

// kept for engine compatibility (unused here)
uniform float u0, u1, u2, u3, u4, u5, u6, u7, u8, u9;

const float PI = 3.14159265359;

float gauss(float x, float s) { return exp(-(x * x) / (2.0 * s * s)); }

vec3 palette(float t) {
    vec3 a = vec3(0.55, 0.45, 0.60);
    vec3 b = vec3(0.45, 0.35, 0.40);
    vec3 c = vec3(1.00, 1.00, 1.00);
    vec3 d = vec3(0.00, 0.33, 0.67);
    return a + b * cos(2.0 * PI * (c * t + d));
}

// Robust ONB for offset placement (avoids pole artifacts)
void buildONB(vec3 n, out vec3 b1, out vec3 b2) {
    float s = n.z >= 0.0 ? 1.0 : -1.0;
    float a = -1.0 / (s + n.z);
    float b = n.x * n.y * a;
    b1 = vec3(1.0 + s * n.x * n.x * a, s * b, -s * n.x);
    b2 = vec3(b, s + n.y * n.y * a, -n.y);
}

void main()
{
    // ---------------- stylized face glow (your look) ----------------
    vec2 uv = TexCoord;
    vec2 p = uv - 0.5;
    float r = length(p) + 1e-6;
    float a = atan(p.y, p.x);

    float saw = fract(r * uRingFreq - uSpeed * uTime);
    float ring = gauss(saw - 0.5, uGlowWidth);

    float ang01 = (a / (2.0 * PI)) + 0.5;
    float sawAng = fract(ang01 * uStripeCount - 0.2 * uSpeed * uTime);
    float stripes = gauss(sawAng - 0.5, uStripeWidth);

    float edge = min(min(uv.x, 1.0 - uv.x), min(uv.y, 1.0 - uv.y));
    float edgeGlow = smoothstep(0.15, 0.0, edge);

    float glowMask = ring + 0.6 * stripes + 0.4 * edgeGlow;
    vec3  hue = palette(r * 1.2 + 0.05 * uTime + ang01 * 0.15);

    vec3 base = hue * uBase;
    vec3 emissive = hue * glowMask * uGlowStrength;

    // ---------------- camera-following HEADLIGHT ----------------
    vec3 N = normalize(vNormal);

    // Camera basis for placing the light a bit in front/above the camera
    vec3 F = normalize(uCameraDir);
    vec3 R, U; buildONB(F, R, U);

    // Light position relative to camera (right, up, forward)
    const vec3 kOffsetView = vec3(0.0, 0.0, 0.0);
    vec3 lightPos = uCameraPos + R * kOffsetView.x + U * kOffsetView.y + F * kOffsetView.z;

    // Directions
    vec3 Lvec = lightPos - vWorldPos;
    float d2 = max(dot(Lvec, Lvec), 1e-6);
    vec3  L = Lvec * inversesqrt(d2);
    vec3  V = normalize(uCameraPos - vWorldPos);

    // ---- SPOT factor: make lighting follow camera *direction*, not just position
    // Inner/outer cone (degrees) -> cosine space
    const float spotInnerDeg = 18.0;
    const float spotOuterDeg = 26.0 * 2.0;
    float cosInner = cos(radians(spotInnerDeg));
    float cosOuter = cos(radians(spotOuterDeg));

    // Angle between light facing (F) and the vector toward the point (-L)
    float spotCos = max(dot(-L, F), 0.0);
    float spot = smoothstep(cosOuter, cosInner, spotCos); // 0 outside, 1 inside, smooth edge

    // Shading terms
    float ndl = max(dot(N, L), 0.0);
    vec3  H = normalize(L + V);
    float ndh = max(dot(N, H), 0.0);

    // Tunables
    const float kAmb = 0.06;
    const float kDiff = 1.40;
    const float kSpec = 0.50;
    const float kPow = 64.0;
    const float kInvSq = 0.12 * 2.0;

    // Attenuation + spotlight cone
    float atten = (1.0 / (1.0 + kInvSq * d2)) * spot;

    // (Optional) add a little shaping to ndl for crisper headlight look
    float ndl_sharp = pow(ndl, 2.0);

    float diff = ndl_sharp * kDiff;
    float spec = pow(ndh, kPow) * kSpec;

    vec3 lighting = vec3(kAmb) + atten * (diff + spec);

    // ---------------- final color ----------------
    // lighting *= 100.0;
    //lighting = vec3(0.0);

    emissive = vec3(0.0);

    base = vec3(0.0);
    //emissive = vec3(0.0);

    lighting *= 100.0;

    // lighting = pow(lighting, vec3(4.0));
    //lighting *= 100.0;

    //emissive *= vec3(0.0);

    vec3 w_color = color_vs * lighting;




    // --------------------------------------------.


    vec3 color = base * lighting + emissive + w_color;
    FragColor = vec4(color * color_vs, 1.0);
}

