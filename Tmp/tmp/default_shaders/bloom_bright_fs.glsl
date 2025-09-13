
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

