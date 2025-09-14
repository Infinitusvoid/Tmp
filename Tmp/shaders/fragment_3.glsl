
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



