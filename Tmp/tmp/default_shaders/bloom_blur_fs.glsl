
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

