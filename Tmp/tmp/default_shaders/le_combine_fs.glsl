
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



