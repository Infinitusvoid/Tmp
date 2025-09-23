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

float saturate(float x) { return clamp(x, 0.0, 1.0); }


uint pcg_hash(uint x) {
    x = x * 747796405u + 2891336453u;
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return x;
}

float rand01(inout uint s) { s = pcg_hash(s); return float(s) * (1.0 / 4294967295.0); }


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


// 0 to 1
float f_periodic_0(float x)
{
    return 2.0 * abs(fract(x + 0.5) - 0.5);
}

// Square Wave 
float f_periodic_1(float x)
{
    return  floor(x) - floor(x - 0.5);
}

// The Bouncing Ball (Parabolic Arches)
float f_periodic_2(float x)
{
    return 4 * fract(x) * (1 - fract(x));
}

float f_periodic_3(float x)
{
    return exp(-30 * ((fract(x + 0.5) - 0.5) * (fract(x + 0.5) - 0.5)));
}

float f_periodic_4(float x)
{
    return abs(0.7 * cos(2 * PI * x) + 0.3 * cos(6 * PI * x)) * (-1.0) + 1.0;
}

float f_periodic_5(float x)
{ 
    return 1.0 - abs(round(10 * fract(x)) / 10 - 0.5) * 2.0;
}

float f_periodic_6(float x)
{
    return sqrt(4 * fract(x) * (1 - fract(x)));
}

float f_periodic_7(float x)
{
    return sin(5 * PI * fract(x)) * (1 - fract(x));
}

// 1) Raised-cosine (Hann) arch — smooth & band-limited-ish
float f_periodic_8(float x)
{
    return 0.5 - 0.5 * cos(TAU * x);               // 0 at integers, 1 at half-integers
}

float f_periodic_9(float x)
{
    return pow(2.0 * abs(fract(x + 0.5) - 0.5), 1.5);
}

float f_periodic_10(float x)
{
    return (abs(1.0 / (1.0 + exp(-6.0 * sin(TAU * x))) - 0.5)) * 2.0 * 2.0 * abs(fract(x + 0.5) - 0.5);
}

float f_periodic_11(float x)
{
    return fract(x) * fract(x) * (3.0 - 2.0 * fract(x)) * 2.0 * abs(fract(x + 0.5) - 0.5) * 1.9;
}

float f_adjust_to_two_pi(float x)
{
    return x * (1.0 / TAU);
}


// wave functions
vec4 wave_1(float x, float y, float t)
{

    float offset_x = 0.0;
    float offset_y = 0.0;
    float wave_color_r = 0.0;
    float wave_color_g = 0.0;
    float wave_color_b = 0.0;
    float offset_x_0 = float(0.20715593) * sin(float(5) * x * TAU + float(0.1389061) + t * float(0.04387028));
    offset_x += offset_x_0;
    wave_color_r += float(0.08718471) * offset_x_0;
    wave_color_g += float(0.10404019) * offset_x_0;
    wave_color_b += float(0.037236504) * offset_x_0;
    float offset_x_1 = float(0.043979187) * sin(float(65) * x * TAU + float(0.26255584) + t * float(0.07876257));
    offset_x += offset_x_1;
    wave_color_r += float(0.06692742) * offset_x_1;
    wave_color_g += float(0.11061345) * offset_x_1;
    wave_color_b += float(0.03177116) * offset_x_1;
    float offset_x_2 = float(0.15698901) * sin(float(48) * x * TAU + float(0.9861861) + t * float(0.020843994));
    offset_x += offset_x_2;
    wave_color_r += float(0.18535261) * offset_x_2;
    wave_color_g += float(0.012480606) * offset_x_2;
    wave_color_b += float(0.19236512) * offset_x_2;
    float offset_x_3 = float(0.031175792) * sin(float(18) * x * TAU + float(0.26552093) + t * float(0.0917245));
    offset_x += offset_x_3;
    wave_color_r += float(0.06320059) * offset_x_3;
    wave_color_g += float(0.17112404) * offset_x_3;
    wave_color_b += float(0.11021918) * offset_x_3;
    float offset_x_4 = float(0.16076235) * sin(float(20) * x * TAU + float(0.5096664) + t * float(0.089321315));
    offset_x += offset_x_4;
    wave_color_r += float(0.023230953) * offset_x_4;
    wave_color_g += float(0.16969512) * offset_x_4;
    wave_color_b += float(0.028994516) * offset_x_4;
    float offset_x_5 = float(0.14718413) * sin(float(40) * x * TAU + float(0.65034485) + t * float(-0.004823774));
    offset_x += offset_x_5;
    wave_color_r += float(0.18882371) * offset_x_5;
    wave_color_g += float(0.073612824) * offset_x_5;
    wave_color_b += float(0.1031881) * offset_x_5;
    float offset_x_6 = float(0.09322583) * sin(float(35) * x * TAU + float(0.49919277) + t * float(-0.043507975));
    offset_x += offset_x_6;
    wave_color_r += float(0.075703815) * offset_x_6;
    wave_color_g += float(0.03297395) * offset_x_6;
    wave_color_b += float(0.16633144) * offset_x_6;
    float offset_x_7 = float(0.013745837) * sin(float(57) * x * TAU + float(0.9234267) + t * float(-0.076351985));
    offset_x += offset_x_7;
    wave_color_r += float(0.06970841) * offset_x_7;
    wave_color_g += float(0.18147571) * offset_x_7;
    wave_color_b += float(0.064742066) * offset_x_7;
    float offset_x_8 = float(0.11998532) * sin(float(34) * x * TAU + float(0.41038448) + t * float(-0.015289819));
    offset_x += offset_x_8;
    wave_color_r += float(0.13310109) * offset_x_8;
    wave_color_g += float(0.019548278) * offset_x_8;
    wave_color_b += float(0.15691778) * offset_x_8;
    float offset_x_9 = float(0.025796592) * sin(float(2) * x * TAU + float(0.2519781) + t * float(0.0785311));
    offset_x += offset_x_9;
    wave_color_r += float(0.07464202) * offset_x_9;
    wave_color_g += float(0.08080213) * offset_x_9;
    wave_color_b += float(0.008938857) * offset_x_9;
    float offset_y_0 = float(0.110325694) * sin(float(7) * y * TAU + float(0.42923763) + t * float(-0.09553705));
    offset_y += offset_y_0;
    wave_color_r += float(0.11659652) * offset_y_0;
    wave_color_g += float(0.14488101) * offset_y_0;
    wave_color_b += float(0.15571225) * offset_y_0;
    float offset_y_1 = float(0.16631682) * sin(float(32) * y * TAU + float(0.8860714) + t * float(-0.008794367));
    offset_y += offset_y_1;
    wave_color_r += float(0.034065418) * offset_y_1;
    wave_color_g += float(0.16088928) * offset_y_1;
    wave_color_b += float(0.13922355) * offset_y_1;
    float offset_y_2 = float(0.005186671) * sin(float(3) * y * TAU + float(0.24068254) + t * float(0.04877795));
    offset_y += offset_y_2;
    wave_color_r += float(0.19662546) * offset_y_2;
    wave_color_g += float(0.002182965) * offset_y_2;
    wave_color_b += float(0.05003717) * offset_y_2;
    float offset_y_3 = float(0.068520665) * sin(float(22) * y * TAU + float(0.04588819) + t * float(-0.066903375));
    offset_y += offset_y_3;
    wave_color_r += float(0.037852716) * offset_y_3;
    wave_color_g += float(0.05847372) * offset_y_3;
    wave_color_b += float(0.13106282) * offset_y_3;
    float offset_y_4 = float(0.10808486) * sin(float(66) * y * TAU + float(0.9950504) + t * float(0.09136288));
    offset_y += offset_y_4;
    wave_color_r += float(0.14337812) * offset_y_4;
    wave_color_g += float(0.12496774) * offset_y_4;
    wave_color_b += float(0.12455368) * offset_y_4;
    float offset_y_5 = float(0.09871009) * sin(float(23) * y * TAU + float(0.56637806) + t * float(0.07010619));
    offset_y += offset_y_5;
    wave_color_r += float(0.0970641) * offset_y_5;
    wave_color_g += float(0.115789086) * offset_y_5;
    wave_color_b += float(0.09211609) * offset_y_5;
    float offset_y_6 = float(0.14448592) * sin(float(50) * y * TAU + float(0.7522833) + t * float(-0.04438728));
    offset_y += offset_y_6;
    wave_color_r += float(0.077621326) * offset_y_6;
    wave_color_g += float(0.14758623) * offset_y_6;
    wave_color_b += float(0.18303841) * offset_y_6;
    float offset_y_7 = float(0.15540992) * sin(float(6) * y * TAU + float(0.2723963) + t * float(-0.09581043));
    offset_y += offset_y_7;
    wave_color_r += float(0.066712216) * offset_y_7;
    wave_color_g += float(0.057569798) * offset_y_7;
    wave_color_b += float(0.017836047) * offset_y_7;
    float offset_y_8 = float(0.09016426) * sin(float(5) * y * TAU + float(0.48555467) + t * float(-0.012820154));
    offset_y += offset_y_8;
    wave_color_r += float(0.096533656) * offset_y_8;
    wave_color_g += float(0.13224086) * offset_y_8;
    wave_color_b += float(0.037406396) * offset_y_8;
    float offset_y_9 = float(0.052795194) * sin(float(68) * y * TAU + float(0.804536) + t * float(0.06592407));
    offset_y += offset_y_9;
    wave_color_r += float(0.07487525) * offset_y_9;
    wave_color_g += float(0.05541928) * offset_y_9;
    wave_color_b += float(0.06901351) * offset_y_9;

    float offset = offset_x + offset_y;
    offset *= 0.1;
    float color_r = wave_color_r * 0.2;
    float color_g = wave_color_g * 0.2;
    float color_b = wave_color_b * 0.2;
    
    return vec4(color_r, color_g, color_b, offset);
}

vec4 wave_0(float x, float y, float t)
{

    float offset_x = 0.0;
    float offset_y = 0.0;
    float wave_color_r = 0.0;
    float wave_color_g = 0.0;
    float wave_color_b = 0.0;
    float offset_x_0 = float(0.018083356) * sin(float(2) * x * TAU + float(0.17355414) + t * float(0.079558656));
    offset_x += offset_x_0;
    wave_color_r += float(0.101380765) * offset_x_0;
    wave_color_g += float(0.069838196) * offset_x_0;
    wave_color_b += float(0.0932496) * offset_x_0;
    float offset_x_1 = float(0.046099935) * sin(float(20) * x * TAU + float(0.94568825) + t * float(-0.054092538));
    offset_x += offset_x_1;
    wave_color_r += float(0.00026399014) * offset_x_1;
    wave_color_g += float(0.12441676) * offset_x_1;
    wave_color_b += float(0.07694869) * offset_x_1;
    float offset_x_2 = float(0.059385415) * sin(float(5) * x * TAU + float(0.254976) + t * float(0.043955445));
    offset_x += offset_x_2;
    wave_color_r += float(0.026854625) * offset_x_2;
    wave_color_g += float(0.16116203) * offset_x_2;
    wave_color_b += float(0.11836807) * offset_x_2;
    float offset_x_3 = float(0.08959239) * sin(float(16) * x * TAU + float(0.7673928) + t * float(-0.015861671));
    offset_x += offset_x_3;
    wave_color_r += float(0.08377262) * offset_x_3;
    wave_color_g += float(0.055806912) * offset_x_3;
    wave_color_b += float(0.008385951) * offset_x_3;
    float offset_x_4 = float(0.0018998529) * sin(float(30) * x * TAU + float(0.7726081) + t * float(-0.06988038));
    offset_x += offset_x_4;
    wave_color_r += float(0.1607175) * offset_x_4;
    wave_color_g += float(0.060363848) * offset_x_4;
    wave_color_b += float(0.001278829) * offset_x_4;
    float offset_x_5 = float(0.119611256) * sin(float(15) * x * TAU + float(0.1253893) + t * float(-0.0867358));
    offset_x += offset_x_5;
    wave_color_r += float(0.1968278) * offset_x_5;
    wave_color_g += float(0.09402631) * offset_x_5;
    wave_color_b += float(0.073490374) * offset_x_5;
    float offset_x_6 = float(0.078181684) * sin(float(18) * x * TAU + float(0.24721366) + t * float(-0.05095563));
    offset_x += offset_x_6;
    wave_color_r += float(0.053577024) * offset_x_6;
    wave_color_g += float(0.11723667) * offset_x_6;
    wave_color_b += float(0.17381297) * offset_x_6;
    float offset_x_7 = float(0.12043605) * sin(float(46) * x * TAU + float(0.5660573) + t * float(-0.02045393));
    offset_x += offset_x_7;
    wave_color_r += float(0.0051145903) * offset_x_7;
    wave_color_g += float(0.15144724) * offset_x_7;
    wave_color_b += float(0.020589566) * offset_x_7;
    float offset_x_8 = float(0.17067286) * sin(float(2) * x * TAU + float(0.18242274) + t * float(-0.0957204));
    offset_x += offset_x_8;
    wave_color_r += float(0.018181343) * offset_x_8;
    wave_color_g += float(0.12562624) * offset_x_8;
    wave_color_b += float(0.19624805) * offset_x_8;
    float offset_x_9 = float(0.29603723) * sin(float(14) * x * TAU + float(0.5857536) + t * float(0.09841025));
    offset_x += offset_x_9;
    wave_color_r += float(0.018390227) * offset_x_9;
    wave_color_g += float(0.040075906) * offset_x_9;
    wave_color_b += float(0.0795906) * offset_x_9;
    float offset_y_0 = float(0.0046067955) * sin(float(24) * y * TAU + float(0.121883534) + t * float(-0.05283642));
    offset_y += offset_y_0;
    wave_color_r += float(0.038017478) * offset_y_0;
    wave_color_g += float(0.19585346) * offset_y_0;
    wave_color_b += float(0.11402328) * offset_y_0;
    float offset_y_1 = float(0.0393524) * sin(float(17) * y * TAU + float(0.7709284) + t * float(-0.041428465));
    offset_y += offset_y_1;
    wave_color_r += float(0.013657632) * offset_y_1;
    wave_color_g += float(0.078391105) * offset_y_1;
    wave_color_b += float(0.07483702) * offset_y_1;
    float offset_y_2 = float(0.120043114) * sin(float(23) * y * TAU + float(0.7010856) + t * float(0.020199906));
    offset_y += offset_y_2;
    wave_color_r += float(0.05453586) * offset_y_2;
    wave_color_g += float(0.04966646) * offset_y_2;
    wave_color_b += float(0.14793922) * offset_y_2;
    float offset_y_3 = float(0.14505993) * sin(float(2) * y * TAU + float(0.12676693) + t * float(-0.05186518));
    offset_y += offset_y_3;
    wave_color_r += float(0.028523168) * offset_y_3;
    wave_color_g += float(0.050628938) * offset_y_3;
    wave_color_b += float(0.01955702) * offset_y_3;
    float offset_y_4 = float(0.15446617) * sin(float(4) * y * TAU + float(0.841945) + t * float(0.061997615));
    offset_y += offset_y_4;
    wave_color_r += float(0.16466753) * offset_y_4;
    wave_color_g += float(0.012211514) * offset_y_4;
    wave_color_b += float(0.17311282) * offset_y_4;
    float offset_y_5 = float(0.023865351) * sin(float(3) * y * TAU + float(0.49486282) + t * float(0.04775562));
    offset_y += offset_y_5;
    wave_color_r += float(0.14736485) * offset_y_5;
    wave_color_g += float(0.102863766) * offset_y_5;
    wave_color_b += float(0.042615633) * offset_y_5;
    float offset_y_6 = float(0.18800332) * sin(float(80) * y * TAU + float(0.20928887) + t * float(-0.01647144));
    offset_y += offset_y_6;
    wave_color_r += float(0.13638374) * offset_y_6;
    wave_color_g += float(0.11294265) * offset_y_6;
    wave_color_b += float(0.06274587) * offset_y_6;
    float offset_y_7 = float(0.15803143) * sin(float(28) * y * TAU + float(0.40810975) + t * float(0.0644114));
    offset_y += offset_y_7;
    wave_color_r += float(0.10393439) * offset_y_7;
    wave_color_g += float(0.1077501) * offset_y_7;
    wave_color_b += float(0.15060659) * offset_y_7;
    float offset_y_8 = float(0.105227254) * sin(float(7) * y * TAU + float(0.73229104) + t * float(0.08059666));
    offset_y += offset_y_8;
    wave_color_r += float(0.15966189) * offset_y_8;
    wave_color_g += float(0.0032296036) * offset_y_8;
    wave_color_b += float(0.1603413) * offset_y_8;
    float offset_y_9 = float(0.061344173) * sin(float(36) * y * TAU + float(0.11837722) + t * float(0.00826099));
    offset_y += offset_y_9;
    wave_color_r += float(0.15325348) * offset_y_9;
    wave_color_g += float(0.15749599) * offset_y_9;
    wave_color_b += float(0.05422124) * offset_y_9;

    float offset = offset_x + offset_y;
    offset *= 0.1;
    float color_r = wave_color_r * 0.2;
    float color_g = wave_color_g * 0.2;
    float color_b = wave_color_b * 0.2;
    
    return vec4(color_r, color_g, color_b, offset);
}

void main()
{
    int id = gl_InstanceID;

    id =  id + (uGrid.x * uGrid.y * uGrid.z) * int(uDrawcallNumber);

    
    // Per-instance randomness
        uint s0 = uSeed + uint(id + 0);
        uint s1 = uSeed + uint(id + 42);
        uint s2 = uSeed + uint(id + 142);
        float rnd_x = rand01(s0);
        float rnd_y = rand01(s1);
        float rnd_z = rand01(s2);
    
        // The instancd cube rotation randomization
        uint s0_rot_x = uSeed + uint(id + 2431);
        uint s1_rot_y = uSeed + uint(id + 4412);
        uint s2_rot_y = uSeed + uint(id + 1234);
        uint s3_rot_angle = uSeed + uint(id + 2332);
        float rnd_cube_rotation_x = rand01(s0_rot_x);
        float rnd_cube_rotation_y = rand01(s1_rot_y);
        float rnd_cube_rotation_z = rand01(s2_rot_y);
        float rnd_cube_rotation_angle = rand01(s3_rot_angle);

    float f_0 = fract(uTime * (1.0/4.0));
    float f_1 = 1.0 - f_0;


    vec4 value_wave_0 = wave_0(rnd_x, rnd_y, uTime);
    vec4 value_wave_1 = wave_1(rnd_x, rnd_y, uTime);
    float w = f_1 * value_wave_0.w + f_0 * value_wave_1.w;

    float color_r = value_wave_0.r * f_1 + value_wave_1.r * f_0;
    float color_g = value_wave_0.g * f_1 + value_wave_1.g * f_0;
    float color_b = value_wave_0.b * f_1 + value_wave_1.b * f_0;

    float radius = 0.2 + w;
    
    // Sphere
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    float px = sphere_position.x;
    float py = sphere_position.y;
    float pz = sphere_position.z;
    
    // Instances Cube Scale
    float scale_cube = 0.0001;
    vec3  pos = vec3(px, pz, py);
    vec3  scale = vec3(scale_cube, scale_cube, scale_cube);
    
    
    // Whole object rotation
    vec3 rotation_axis = vec3(0.0, 1.0, 0.0);
    float rotation_angle = sin(uTime * TAU * (1.0 / 4.0)); // using uTime will not be wise after we will be interpolating between two values
    
    // Whole object scale
    vec3 scale_object = vec3(1.0, 1.0, 1.0);
    
    
    vec4 new_position = vec4(vec3(pos), 1.0);
    
        if (true) {
    
            uint s0_instance_0 = uSeed + uint(uint(u0 * 1000.0f));
            uint s0_instance_1 = uSeed + uint(uint(u0 * 1421.0f));
            float rnd_instance_0 = rand01(s0_instance_0);
            float rnd_instance_1 = rand01(s0_instance_1);
    
            uint s0_instance_x_scale = uSeed + uint(uint(u0 * 14024.0f));
            uint s0_instance_y_scale = uSeed + uint(uint(u0 * 15214.0f));
            uint s0_instance_z_scale = uSeed + uint(uint(u0 * 14215.0f));
            float rnd_instance_scale_x = rand01(s0_instance_x_scale);
            float rnd_instance_scale_y = rand01(s0_instance_y_scale);
            float rnd_instance_scale_z = rand01(s0_instance_z_scale);
    
            // Rotation
            // vec3 axis = normalize(vec3(0.0, 1.0, 1.0));
            vec3 axis = normalize(rotation_axis);
            // float angle = uTime;
            float angle = rotation_angle;
            mat3 R3 = axisAngleToMat3(axis, angle);
            mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
    
            // Translation
            mat4 T = mat4(1.0);
            vec3 offset = vec3(sin(uTime + rnd_instance_0 * 10.0) * 10.0, sin(uTime + rnd_instance_1 * 0.0) * 10.0, 0.0);
            offset = vec3(0.5, 0.5, 0.5);
            T[3] = vec4(offset, 1.0);
    
    
    
            // Scale
            mat4 S = mat4(1.0);
            S[0][0] = scale_object.x;
            S[1][1] = scale_object.y;
            S[2][2] = scale_object.z;
    
            new_position = T * R * S * new_position;
        }
    
    
    
        pos = new_position.xyz;
        
    
    
    
    
    
        
        // Per-instance tint (kept neutral here)
        color_vs = vec3(color_r, color_g, color_b);
    
        // Build TRS
        mat4 T = mat4(1.0); T[3] = vec4(pos, 1.0);
        vec3 axis = normalize(vec3(rnd_cube_rotation_x, rnd_cube_rotation_y, rnd_cube_rotation_z));
        float angle = rnd_cube_rotation_angle;//uTime * 0.0;
        mat3 R3 = axisAngleToMat3(axis, angle);
        mat4 R = mat4(vec4(R3[0], 0.0), vec4(R3[1], 0.0), vec4(R3[2], 0.0), vec4(0, 0, 0, 1));
        mat4 S = mat4(1.0); S[0][0] = scale.x; S[1][1] = scale.y; S[2][2] = scale.z;
    
        mat4 instanceModel = T * R * S;
        mat4 M = model * instanceModel;
    
        // World-space position (for lighting)
        vec4 wp = M * vec4(aPos, 1.0);
        vWorldPos = wp.xyz;
    
        // World-space normal:
        // Fast path (assumes uniform scale): rotate the face normal by model rotation and R3.
        // If you later use non-uniform model scale, switch to normal matrix:
        //   mat3 N = transpose(inverse(mat3(M)));
        //   vNormal = normalize(N * nLocal);
        vec3 nLocal = localCubeFaceNormal(aPos);
        vNormal = normalize(mat3(model) * (R3 * nLocal)); // uniform-scale assumption
    
        // Clip-space position and UV
        gl_Position = projection * view * wp;
        TexCoord = aTexCoord;
    
    
        // World position color
    
        // float world_x = wp.x;
        // float world_y = wp.y;
        // float world_z = wp.z;
        // color_vs = vec3(sin(world_x * 10.0), sin(world_y * 10.0), sin(world_z * 10.0)) * vec3(0.01, 0.01, 0.01);
}
