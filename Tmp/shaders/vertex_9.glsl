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
vec4 wave_0(float x, float y, float t)
{

    float offset_x = 0.0;
    float offset_y = 0.0;
    float wave_color_r = 0.0;
    float wave_color_g = 0.0;
    float wave_color_b = 0.0;
    float offset_x_0 = float(0.119025126) * sin(float(32) * x * TAU + float(0.9411141) + t * float(0.047663845));
    offset_x += offset_x_0;
    wave_color_r += float(0.18027337) * offset_x_0;
    wave_color_g += float(0.077821404) * offset_x_0;
    wave_color_b += float(0.13954882) * offset_x_0;
    float offset_x_1 = float(0.018885724) * sin(float(27) * x * TAU + float(0.1835462) + t * float(0.07420721));
    offset_x += offset_x_1;
    wave_color_r += float(0.1088631) * offset_x_1;
    wave_color_g += float(0.048374668) * offset_x_1;
    wave_color_b += float(0.019752659) * offset_x_1;
    float offset_x_2 = float(0.125064) * sin(float(1) * x * TAU + float(0.19987348) + t * float(0.00089714525));
    offset_x += offset_x_2;
    wave_color_r += float(0.19052191) * offset_x_2;
    wave_color_g += float(0.12191341) * offset_x_2;
    wave_color_b += float(0.010953431) * offset_x_2;
    float offset_x_3 = float(0.06075142) * sin(float(6) * x * TAU + float(0.20698854) + t * float(-0.029283678));
    offset_x += offset_x_3;
    wave_color_r += float(0.014405523) * offset_x_3;
    wave_color_g += float(0.14636636) * offset_x_3;
    wave_color_b += float(0.06863007) * offset_x_3;
    float offset_x_4 = float(0.14452188) * sin(float(25) * x * TAU + float(0.5551876) + t * float(0.023660565));
    offset_x += offset_x_4;
    wave_color_r += float(0.01750093) * offset_x_4;
    wave_color_g += float(0.13085459) * offset_x_4;
    wave_color_b += float(0.017291944) * offset_x_4;
    float offset_x_5 = float(0.050281577) * sin(float(4) * x * TAU + float(0.42051014) + t * float(0.013786805));
    offset_x += offset_x_5;
    wave_color_r += float(0.04157161) * offset_x_5;
    wave_color_g += float(0.066621736) * offset_x_5;
    wave_color_b += float(0.011742979) * offset_x_5;
    float offset_x_6 = float(0.10050865) * sin(float(8) * x * TAU + float(0.9383967) + t * float(-0.08372109));
    offset_x += offset_x_6;
    wave_color_r += float(0.03140997) * offset_x_6;
    wave_color_g += float(0.1074543) * offset_x_6;
    wave_color_b += float(0.032025713) * offset_x_6;
    float offset_x_7 = float(0.1683078) * sin(float(23) * x * TAU + float(0.14452918) + t * float(-0.07834285));
    offset_x += offset_x_7;
    wave_color_r += float(0.14819263) * offset_x_7;
    wave_color_g += float(0.15536526) * offset_x_7;
    wave_color_b += float(0.028185198) * offset_x_7;
    float offset_x_8 = float(0.1343927) * sin(float(82) * x * TAU + float(0.9204118) + t * float(-0.009452706));
    offset_x += offset_x_8;
    wave_color_r += float(0.11820547) * offset_x_8;
    wave_color_g += float(0.121626414) * offset_x_8;
    wave_color_b += float(0.04552283) * offset_x_8;
    float offset_x_9 = float(0.07826108) * sin(float(47) * x * TAU + float(0.73191863) + t * float(0.09811781));
    offset_x += offset_x_9;
    wave_color_r += float(0.14808148) * offset_x_9;
    wave_color_g += float(0.023601891) * offset_x_9;
    wave_color_b += float(0.09026785) * offset_x_9;
    float offset_y_0 = float(0.08654879) * sin(float(66) * y * TAU + float(0.68452454) + t * float(0.089673124));
    offset_y += offset_y_0;
    wave_color_r += float(0.11841171) * offset_y_0;
    wave_color_g += float(0.14288318) * offset_y_0;
    wave_color_b += float(0.17222649) * offset_y_0;
    float offset_y_1 = float(0.116420224) * sin(float(52) * y * TAU + float(0.9643515) + t * float(0.09861995));
    offset_y += offset_y_1;
    wave_color_r += float(0.10807209) * offset_y_1;
    wave_color_g += float(0.13520515) * offset_y_1;
    wave_color_b += float(0.06834249) * offset_y_1;
    float offset_y_2 = float(0.084116586) * sin(float(2) * y * TAU + float(0.88282204) + t * float(0.0022833706));
    offset_y += offset_y_2;
    wave_color_r += float(0.069396764) * offset_y_2;
    wave_color_g += float(0.13782607) * offset_y_2;
    wave_color_b += float(0.10720973) * offset_y_2;
    float offset_y_3 = float(0.12146635) * sin(float(18) * y * TAU + float(0.7421482) + t * float(0.038549937));
    offset_y += offset_y_3;
    wave_color_r += float(0.10336618) * offset_y_3;
    wave_color_g += float(0.008327081) * offset_y_3;
    wave_color_b += float(0.099787354) * offset_y_3;
    float offset_y_4 = float(0.09327181) * sin(float(66) * y * TAU + float(0.7473014) + t * float(-0.07848674));
    offset_y += offset_y_4;
    wave_color_r += float(0.055347633) * offset_y_4;
    wave_color_g += float(0.13480519) * offset_y_4;
    wave_color_b += float(0.14358473) * offset_y_4;
    float offset_y_5 = float(0.14317894) * sin(float(5) * y * TAU + float(0.9638128) + t * float(0.01673534));
    offset_y += offset_y_5;
    wave_color_r += float(0.12115466) * offset_y_5;
    wave_color_g += float(0.09066616) * offset_y_5;
    wave_color_b += float(0.15381001) * offset_y_5;
    float offset_y_6 = float(0.035537712) * sin(float(36) * y * TAU + float(0.74239755) + t * float(-0.038419567));
    offset_y += offset_y_6;
    wave_color_r += float(0.034081463) * offset_y_6;
    wave_color_g += float(0.13094176) * offset_y_6;
    wave_color_b += float(0.03940688) * offset_y_6;
    float offset_y_7 = float(0.05093936) * sin(float(3) * y * TAU + float(0.5200592) + t * float(0.042049684));
    offset_y += offset_y_7;
    wave_color_r += float(0.17531143) * offset_y_7;
    wave_color_g += float(0.014925677) * offset_y_7;
    wave_color_b += float(0.074513786) * offset_y_7;
    float offset_y_8 = float(0.1400065) * sin(float(16) * y * TAU + float(0.44676822) + t * float(-0.051175512));
    offset_y += offset_y_8;
    wave_color_r += float(0.1014984) * offset_y_8;
    wave_color_g += float(0.13258474) * offset_y_8;
    wave_color_b += float(0.032955643) * offset_y_8;
    float offset_y_9 = float(0.12851377) * sin(float(49) * y * TAU + float(0.211874) + t * float(-0.03474648));
    offset_y += offset_y_9;
    wave_color_r += float(0.11335967) * offset_y_9;
    wave_color_g += float(0.07183497) * offset_y_9;
    wave_color_b += float(0.10816284) * offset_y_9;

    float offset = offset_x + offset_y;
    offset *= 0.1;
    float color_r = wave_color_r * 0.2;
    float color_g = wave_color_g * 0.2;
    float color_b = wave_color_b * 0.2;
    
    return vec4(color_r, color_g, color_b, offset);
}

vec4 wave_1(float x, float y, float t)
{

    float offset_x = 0.0;
    float offset_y = 0.0;
    float wave_color_r = 0.0;
    float wave_color_g = 0.0;
    float wave_color_b = 0.0;
    float offset_x_0 = float(0.12522122) * sin(float(36) * x * TAU + float(0.3864992) + t * float(0.03196125));
    offset_x += offset_x_0;
    wave_color_r += float(0.06448614) * offset_x_0;
    wave_color_g += float(0.06290623) * offset_x_0;
    wave_color_b += float(0.07763197) * offset_x_0;
    float offset_x_1 = float(0.03655061) * sin(float(12) * x * TAU + float(0.1551666) + t * float(0.039162718));
    offset_x += offset_x_1;
    wave_color_r += float(0.028439334) * offset_x_1;
    wave_color_g += float(0.08750715) * offset_x_1;
    wave_color_b += float(0.015464984) * offset_x_1;
    float offset_x_2 = float(0.04372264) * sin(float(11) * x * TAU + float(0.48618966) + t * float(0.033741057));
    offset_x += offset_x_2;
    wave_color_r += float(0.08163561) * offset_x_2;
    wave_color_g += float(0.12978478) * offset_x_2;
    wave_color_b += float(0.17569247) * offset_x_2;
    float offset_x_3 = float(0.17262095) * sin(float(3) * x * TAU + float(0.95826256) + t * float(-0.07736735));
    offset_x += offset_x_3;
    wave_color_r += float(0.14578293) * offset_x_3;
    wave_color_g += float(0.07883454) * offset_x_3;
    wave_color_b += float(0.19101368) * offset_x_3;
    float offset_x_4 = float(0.09060625) * sin(float(13) * x * TAU + float(0.15124378) + t * float(-0.054546215));
    offset_x += offset_x_4;
    wave_color_r += float(0.10129551) * offset_x_4;
    wave_color_g += float(0.13444631) * offset_x_4;
    wave_color_b += float(0.025169367) * offset_x_4;
    float offset_x_5 = float(0.08597138) * sin(float(14) * x * TAU + float(0.3204092) + t * float(0.05973836));
    offset_x += offset_x_5;
    wave_color_r += float(0.021491084) * offset_x_5;
    wave_color_g += float(0.10973142) * offset_x_5;
    wave_color_b += float(0.048495777) * offset_x_5;
    float offset_x_6 = float(0.050243024) * sin(float(39) * x * TAU + float(0.05551181) + t * float(-0.08234664));
    offset_x += offset_x_6;
    wave_color_r += float(0.040728338) * offset_x_6;
    wave_color_g += float(0.118144855) * offset_x_6;
    wave_color_b += float(0.08480989) * offset_x_6;
    float offset_x_7 = float(0.14801578) * sin(float(15) * x * TAU + float(0.61610913) + t * float(-0.012398809));
    offset_x += offset_x_7;
    wave_color_r += float(0.0676952) * offset_x_7;
    wave_color_g += float(0.09085572) * offset_x_7;
    wave_color_b += float(0.01327773) * offset_x_7;
    float offset_x_8 = float(0.10028063) * sin(float(26) * x * TAU + float(0.7477301) + t * float(-0.071382545));
    offset_x += offset_x_8;
    wave_color_r += float(0.17940767) * offset_x_8;
    wave_color_g += float(0.1157497) * offset_x_8;
    wave_color_b += float(0.03903959) * offset_x_8;
    float offset_x_9 = float(0.14676753) * sin(float(6) * x * TAU + float(0.07159084) + t * float(-0.079864524));
    offset_x += offset_x_9;
    wave_color_r += float(0.1721166) * offset_x_9;
    wave_color_g += float(0.07203936) * offset_x_9;
    wave_color_b += float(0.16765395) * offset_x_9;
    float offset_y_0 = float(0.120297216) * sin(float(60) * y * TAU + float(0.82088107) + t * float(0.06246469));
    offset_y += offset_y_0;
    wave_color_r += float(0.088411875) * offset_y_0;
    wave_color_g += float(0.17149422) * offset_y_0;
    wave_color_b += float(0.13034895) * offset_y_0;
    float offset_y_1 = float(0.19799183) * sin(float(27) * y * TAU + float(0.75988847) + t * float(0.055937074));
    offset_y += offset_y_1;
    wave_color_r += float(0.065998934) * offset_y_1;
    wave_color_g += float(0.050353628) * offset_y_1;
    wave_color_b += float(0.18508564) * offset_y_1;
    float offset_y_2 = float(0.18143234) * sin(float(6) * y * TAU + float(0.73150307) + t * float(0.03652797));
    offset_y += offset_y_2;
    wave_color_r += float(0.1530775) * offset_y_2;
    wave_color_g += float(0.17038283) * offset_y_2;
    wave_color_b += float(0.08260409) * offset_y_2;
    float offset_y_3 = float(0.03797189) * sin(float(59) * y * TAU + float(0.9674357) + t * float(-0.012999505));
    offset_y += offset_y_3;
    wave_color_r += float(0.03625636) * offset_y_3;
    wave_color_g += float(0.13704485) * offset_y_3;
    wave_color_b += float(0.090627484) * offset_y_3;
    float offset_y_4 = float(0.0804335) * sin(float(16) * y * TAU + float(0.41561252) + t * float(-0.060997676));
    offset_y += offset_y_4;
    wave_color_r += float(0.13048077) * offset_y_4;
    wave_color_g += float(0.0936379) * offset_y_4;
    wave_color_b += float(0.03635682) * offset_y_4;
    float offset_y_5 = float(0.095344864) * sin(float(40) * y * TAU + float(0.8170117) + t * float(0.06713017));
    offset_y += offset_y_5;
    wave_color_r += float(0.04444915) * offset_y_5;
    wave_color_g += float(0.07986948) * offset_y_5;
    wave_color_b += float(0.05750844) * offset_y_5;
    float offset_y_6 = float(0.10439326) * sin(float(4) * y * TAU + float(0.7228258) + t * float(-0.09939529));
    offset_y += offset_y_6;
    wave_color_r += float(0.15583757) * offset_y_6;
    wave_color_g += float(0.07417324) * offset_y_6;
    wave_color_b += float(0.16459061) * offset_y_6;
    float offset_y_7 = float(0.12238744) * sin(float(2) * y * TAU + float(0.3178871) + t * float(-0.06431328));
    offset_y += offset_y_7;
    wave_color_r += float(0.1481769) * offset_y_7;
    wave_color_g += float(0.055690546) * offset_y_7;
    wave_color_b += float(0.015795529) * offset_y_7;
    float offset_y_8 = float(0.0020335717) * sin(float(10) * y * TAU + float(0.114559) + t * float(-0.07337934));
    offset_y += offset_y_8;
    wave_color_r += float(0.11701559) * offset_y_8;
    wave_color_g += float(0.09704678) * offset_y_8;
    wave_color_b += float(0.014382111) * offset_y_8;
    float offset_y_9 = float(0.05771411) * sin(float(41) * y * TAU + float(0.85242593) + t * float(0.068991445));
    offset_y += offset_y_9;
    wave_color_r += float(0.060295355) * offset_y_9;
    wave_color_g += float(0.07030659) * offset_y_9;
    wave_color_b += float(0.08283485) * offset_y_9;

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
