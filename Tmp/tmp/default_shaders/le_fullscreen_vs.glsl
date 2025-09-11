
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



