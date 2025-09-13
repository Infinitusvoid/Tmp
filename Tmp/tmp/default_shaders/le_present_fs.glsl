
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



