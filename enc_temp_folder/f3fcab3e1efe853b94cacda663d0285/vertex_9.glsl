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

    // first_wave y 0 
    int first_wave_0_y_frequency = int(1);
    float first_wave_0_y_offset = float(-6.429446);
    float first_wave_0_y_amplitude = float(-0.19555403);
    float first_wave_0_y_t = uTime * float(0.007595502);

    // first_wave x 1 
    int first_wave_1_x_frequency = int(4);
    float first_wave_1_x_offset = float(2.1434891);
    float first_wave_1_x_amplitude = float(-0.09066709);
    float first_wave_1_x_t = uTime * float(-0.004637851);

    // first_wave x 2 
    int first_wave_2_x_frequency = int(1);
    float first_wave_2_x_offset = float(-7.7384877);
    float first_wave_2_x_amplitude = float(0.011834408);
    float first_wave_2_x_t = uTime * float(0.0019855031);

    // first_wave x 3 
    int first_wave_3_x_frequency = int(8);
    float first_wave_3_x_offset = float(9.62723);
    float first_wave_3_x_amplitude = float(-0.07308233);
    float first_wave_3_x_t = uTime * float(-0.00080680224);

    // first_wave y 4 
    int first_wave_4_y_frequency = int(3);
    float first_wave_4_y_offset = float(2.0271134);
    float first_wave_4_y_amplitude = float(0.070809625);
    float first_wave_4_y_t = uTime * float(-0.0004082043);

    // first_wave x 5 
    int first_wave_5_x_frequency = int(4);
    float first_wave_5_x_offset = float(-9.397676);
    float first_wave_5_x_amplitude = float(0.013305905);
    float first_wave_5_x_t = uTime * float(0.00020617683);

    // first_wave y 6 
    int first_wave_6_y_frequency = int(8);
    float first_wave_6_y_offset = float(-4.0616174);
    float first_wave_6_y_amplitude = float(0.047785882);
    float first_wave_6_y_t = uTime * float(0.00017708389);

    // first_wave x 7 
    int first_wave_7_x_frequency = int(4);
    float first_wave_7_x_offset = float(-1.7426795);
    float first_wave_7_x_amplitude = float(-0.0037910454);
    float first_wave_7_x_t = uTime * float(-8.7727756e-05);

    // first_wave x 8 
    int first_wave_8_x_frequency = int(7);
    float first_wave_8_x_offset = float(5.265974);
    float first_wave_8_x_amplitude = float(-0.02037584);
    float first_wave_8_x_t = uTime * float(-0.00013499406);

    // first_wave y 9 
    int first_wave_9_y_frequency = int(5);
    float first_wave_9_y_offset = float(7.7876186);
    float first_wave_9_y_amplitude = float(-0.01677518);
    float first_wave_9_y_t = uTime * float(0.00010216705);

    // first_wave x 10 
    int first_wave_10_x_frequency = int(2);
    float first_wave_10_x_offset = float(-0.026438832);
    float first_wave_10_x_amplitude = float(0.03139499);
    float first_wave_10_x_t = uTime * float(5.232222e-05);

    // first_wave x 11 
    int first_wave_11_x_frequency = int(5);
    float first_wave_11_x_offset = float(7.845676);
    float first_wave_11_x_amplitude = float(0.019963913);
    float first_wave_11_x_t = uTime * float(-5.0102084e-05);

    // first_wave x 12 
    int first_wave_12_x_frequency = int(1);
    float first_wave_12_x_offset = float(-7.9867744);
    float first_wave_12_x_amplitude = float(-0.0035692672);
    float first_wave_12_x_t = uTime * float(-2.1694175e-05);

    // first_wave y 13 
    int first_wave_13_y_frequency = int(2);
    float first_wave_13_y_offset = float(-6.1776233);
    float first_wave_13_y_amplitude = float(0.010459141);
    float first_wave_13_y_t = uTime * float(-3.4640867e-05);

    // first_wave y 14 
    int first_wave_14_y_frequency = int(1);
    float first_wave_14_y_offset = float(8.618688);
    float first_wave_14_y_amplitude = float(0.011182512);
    float first_wave_14_y_t = uTime * float(4.841915e-05);

    // first_wave y 15 
    int first_wave_15_y_frequency = int(6);
    float first_wave_15_y_offset = float(2.3484004);
    float first_wave_15_y_amplitude = float(-0.014811394);
    float first_wave_15_y_t = uTime * float(-1.9066905e-05);

    // first_wave x 16 
    int first_wave_16_x_frequency = int(4);
    float first_wave_16_x_offset = float(-2.712164);
    float first_wave_16_x_amplitude = float(0.0059739454);
    float first_wave_16_x_t = uTime * float(3.336572e-05);

    // first_wave y 17 
    int first_wave_17_y_frequency = int(1);
    float first_wave_17_y_offset = float(-4.210067);
    float first_wave_17_y_amplitude = float(0.015071137);
    float first_wave_17_y_t = uTime * float(-2.797362e-05);

    // first_wave y 18 
    int first_wave_18_y_frequency = int(2);
    float first_wave_18_y_offset = float(-2.598576);
    float first_wave_18_y_amplitude = float(0.018034771);
    float first_wave_18_y_t = uTime * float(-2.8910072e-05);

    // first_wave y 19 
    int first_wave_19_y_frequency = int(8);
    float first_wave_19_y_offset = float(2.0967138);
    float first_wave_19_y_amplitude = float(0.00026314278);
    float first_wave_19_y_t = uTime * float(2.2657516e-05);


    float first_wave = 0.0f;
    first_wave += first_wave_0_y_amplitude * f_periodic_3(f_adjust_to_two_pi(first_wave_0_y_offset + rnd_y * TAU * first_wave_0_y_frequency + first_wave_0_y_t * uTime));
    first_wave += first_wave_1_x_amplitude * f_periodic_5(f_adjust_to_two_pi(first_wave_1_x_offset + rnd_x * TAU * first_wave_1_x_frequency + first_wave_1_x_t * uTime));
    first_wave += first_wave_2_x_amplitude * f_periodic_4(f_adjust_to_two_pi(first_wave_2_x_offset + rnd_x * TAU * first_wave_2_x_frequency + first_wave_2_x_t * uTime));
    first_wave += first_wave_3_x_amplitude * f_periodic_3(f_adjust_to_two_pi(first_wave_3_x_offset + rnd_x * TAU * first_wave_3_x_frequency + first_wave_3_x_t * uTime));
    first_wave += first_wave_4_y_amplitude * f_periodic_5(f_adjust_to_two_pi(first_wave_4_y_offset + rnd_y * TAU * first_wave_4_y_frequency + first_wave_4_y_t * uTime));
    first_wave += first_wave_5_x_amplitude * f_periodic_2(f_adjust_to_two_pi(first_wave_5_x_offset + rnd_x * TAU * first_wave_5_x_frequency + first_wave_5_x_t * uTime));
    first_wave += first_wave_6_y_amplitude * f_periodic_8(f_adjust_to_two_pi(first_wave_6_y_offset + rnd_y * TAU * first_wave_6_y_frequency + first_wave_6_y_t * uTime));
    first_wave += first_wave_7_x_amplitude * f_periodic_9(f_adjust_to_two_pi(first_wave_7_x_offset + rnd_x * TAU * first_wave_7_x_frequency + first_wave_7_x_t * uTime));
    first_wave += first_wave_8_x_amplitude * f_periodic_8(f_adjust_to_two_pi(first_wave_8_x_offset + rnd_x * TAU * first_wave_8_x_frequency + first_wave_8_x_t * uTime));
    first_wave += first_wave_9_y_amplitude * f_periodic_9(f_adjust_to_two_pi(first_wave_9_y_offset + rnd_y * TAU * first_wave_9_y_frequency + first_wave_9_y_t * uTime));
    first_wave += first_wave_10_x_amplitude * f_periodic_7(f_adjust_to_two_pi(first_wave_10_x_offset + rnd_x * TAU * first_wave_10_x_frequency + first_wave_10_x_t * uTime));
    first_wave += first_wave_11_x_amplitude * f_periodic_0(f_adjust_to_two_pi(first_wave_11_x_offset + rnd_x * TAU * first_wave_11_x_frequency + first_wave_11_x_t * uTime));
    first_wave += first_wave_12_x_amplitude * f_periodic_8(f_adjust_to_two_pi(first_wave_12_x_offset + rnd_x * TAU * first_wave_12_x_frequency + first_wave_12_x_t * uTime));
    first_wave += first_wave_13_y_amplitude * f_periodic_7(f_adjust_to_two_pi(first_wave_13_y_offset + rnd_y * TAU * first_wave_13_y_frequency + first_wave_13_y_t * uTime));
    first_wave += first_wave_14_y_amplitude * f_periodic_4(f_adjust_to_two_pi(first_wave_14_y_offset + rnd_y * TAU * first_wave_14_y_frequency + first_wave_14_y_t * uTime));
    first_wave += first_wave_15_y_amplitude * f_periodic_5(f_adjust_to_two_pi(first_wave_15_y_offset + rnd_y * TAU * first_wave_15_y_frequency + first_wave_15_y_t * uTime));
    first_wave += first_wave_16_x_amplitude * f_periodic_6(f_adjust_to_two_pi(first_wave_16_x_offset + rnd_x * TAU * first_wave_16_x_frequency + first_wave_16_x_t * uTime));
    first_wave += first_wave_17_y_amplitude * f_periodic_0(f_adjust_to_two_pi(first_wave_17_y_offset + rnd_y * TAU * first_wave_17_y_frequency + first_wave_17_y_t * uTime));
    first_wave += first_wave_18_y_amplitude * f_periodic_8(f_adjust_to_two_pi(first_wave_18_y_offset + rnd_y * TAU * first_wave_18_y_frequency + first_wave_18_y_t * uTime));
    first_wave += first_wave_19_y_amplitude * f_periodic_6(f_adjust_to_two_pi(first_wave_19_y_offset + rnd_y * TAU * first_wave_19_y_frequency + first_wave_19_y_t * uTime));

    first_wave *= float(0.2);

    // second_wave x 0 
    int second_wave_0_x_frequency = int(4);
    float second_wave_0_x_offset = float(0.13850808);
    float second_wave_0_x_amplitude = float(0.35557005);
    float second_wave_0_x_t = uTime * float(0.008871023);

    // second_wave y 1 
    int second_wave_1_y_frequency = int(7);
    float second_wave_1_y_offset = float(9.277724);
    float second_wave_1_y_amplitude = float(0.16112131);
    float second_wave_1_y_t = uTime * float(0.0016807908);

    // second_wave x 2 
    int second_wave_2_x_frequency = int(1);
    float second_wave_2_x_offset = float(9.907807);
    float second_wave_2_x_amplitude = float(-0.093344085);
    float second_wave_2_x_t = uTime * float(0.00034927513);

    // second_wave y 3 
    int second_wave_3_y_frequency = int(1);
    float second_wave_3_y_offset = float(9.648806);
    float second_wave_3_y_amplitude = float(0.08130495);
    float second_wave_3_y_t = uTime * float(0.00096965826);

    // second_wave x 4 
    int second_wave_4_x_frequency = int(8);
    float second_wave_4_x_offset = float(-2.450344);
    float second_wave_4_x_amplitude = float(-0.03401602);
    float second_wave_4_x_t = uTime * float(-0.00029377552);

    // second_wave y 5 
    int second_wave_5_y_frequency = int(4);
    float second_wave_5_y_offset = float(-3.0654068);
    float second_wave_5_y_amplitude = float(-0.049596056);
    float second_wave_5_y_t = uTime * float(-2.3809993e-05);

    // second_wave y 6 
    int second_wave_6_y_frequency = int(2);
    float second_wave_6_y_offset = float(8.397242);
    float second_wave_6_y_amplitude = float(-0.045632653);
    float second_wave_6_y_t = uTime * float(0.00012399348);

    // second_wave x 7 
    int second_wave_7_x_frequency = int(1);
    float second_wave_7_x_offset = float(-0.66922665);
    float second_wave_7_x_amplitude = float(-0.01922132);
    float second_wave_7_x_t = uTime * float(0.00010021774);

    // second_wave y 8 
    int second_wave_8_y_frequency = int(1);
    float second_wave_8_y_offset = float(-9.060003);
    float second_wave_8_y_amplitude = float(0.027009444);
    float second_wave_8_y_t = uTime * float(-0.0001505892);

    // second_wave x 9 
    int second_wave_9_x_frequency = int(7);
    float second_wave_9_x_offset = float(-3.954175);
    float second_wave_9_x_amplitude = float(-0.021682581);
    float second_wave_9_x_t = uTime * float(-7.38925e-05);

    // second_wave x 10 
    int second_wave_10_x_frequency = int(2);
    float second_wave_10_x_offset = float(-6.5505533);
    float second_wave_10_x_amplitude = float(-0.01895378);
    float second_wave_10_x_t = uTime * float(3.90381e-05);

    // second_wave x 11 
    int second_wave_11_x_frequency = int(1);
    float second_wave_11_x_offset = float(-2.1621437);
    float second_wave_11_x_amplitude = float(-0.011469606);
    float second_wave_11_x_t = uTime * float(7.774841e-05);

    // second_wave x 12 
    int second_wave_12_x_frequency = int(6);
    float second_wave_12_x_offset = float(4.9766693);
    float second_wave_12_x_amplitude = float(0.0062724985);
    float second_wave_12_x_t = uTime * float(-2.7010998e-05);

    // second_wave x 13 
    int second_wave_13_x_frequency = int(6);
    float second_wave_13_x_offset = float(-3.3426948);
    float second_wave_13_x_amplitude = float(0.018954752);
    float second_wave_13_x_t = uTime * float(3.3140026e-05);

    // second_wave x 14 
    int second_wave_14_x_frequency = int(1);
    float second_wave_14_x_offset = float(9.3068905);
    float second_wave_14_x_amplitude = float(0.00031892618);
    float second_wave_14_x_t = uTime * float(3.070723e-05);

    // second_wave x 15 
    int second_wave_15_x_frequency = int(5);
    float second_wave_15_x_offset = float(7.420212);
    float second_wave_15_x_amplitude = float(0.009954155);
    float second_wave_15_x_t = uTime * float(2.6934094e-05);

    // second_wave x 16 
    int second_wave_16_x_frequency = int(6);
    float second_wave_16_x_offset = float(1.045115);
    float second_wave_16_x_amplitude = float(-0.021657104);
    float second_wave_16_x_t = uTime * float(-9.8729615e-06);

    // second_wave x 17 
    int second_wave_17_x_frequency = int(4);
    float second_wave_17_x_offset = float(8.63595);
    float second_wave_17_x_amplitude = float(0.0135559635);
    float second_wave_17_x_t = uTime * float(3.007671e-06);

    // second_wave x 18 
    int second_wave_18_x_frequency = int(1);
    float second_wave_18_x_offset = float(-3.3085763);
    float second_wave_18_x_amplitude = float(-0.01807726);
    float second_wave_18_x_t = uTime * float(-4.647589e-06);

    // second_wave x 19 
    int second_wave_19_x_frequency = int(8);
    float second_wave_19_x_offset = float(-4.8778863);
    float second_wave_19_x_amplitude = float(0.007848372);
    float second_wave_19_x_t = uTime * float(-2.3783443e-05);


    float second_wave = 0.0f;
    second_wave += second_wave_0_x_amplitude * f_periodic_5(f_adjust_to_two_pi(second_wave_0_x_offset + rnd_x * TAU * second_wave_0_x_frequency + second_wave_0_x_t * uTime));
    second_wave += second_wave_1_y_amplitude * f_periodic_0(f_adjust_to_two_pi(second_wave_1_y_offset + rnd_y * TAU * second_wave_1_y_frequency + second_wave_1_y_t * uTime));
    second_wave += second_wave_2_x_amplitude * f_periodic_6(f_adjust_to_two_pi(second_wave_2_x_offset + rnd_x * TAU * second_wave_2_x_frequency + second_wave_2_x_t * uTime));
    second_wave += second_wave_3_y_amplitude * f_periodic_2(f_adjust_to_two_pi(second_wave_3_y_offset + rnd_y * TAU * second_wave_3_y_frequency + second_wave_3_y_t * uTime));
    second_wave += second_wave_4_x_amplitude * f_periodic_0(f_adjust_to_two_pi(second_wave_4_x_offset + rnd_x * TAU * second_wave_4_x_frequency + second_wave_4_x_t * uTime));
    second_wave += second_wave_5_y_amplitude * f_periodic_5(f_adjust_to_two_pi(second_wave_5_y_offset + rnd_y * TAU * second_wave_5_y_frequency + second_wave_5_y_t * uTime));
    second_wave += second_wave_6_y_amplitude * f_periodic_2(f_adjust_to_two_pi(second_wave_6_y_offset + rnd_y * TAU * second_wave_6_y_frequency + second_wave_6_y_t * uTime));
    second_wave += second_wave_7_x_amplitude * f_periodic_5(f_adjust_to_two_pi(second_wave_7_x_offset + rnd_x * TAU * second_wave_7_x_frequency + second_wave_7_x_t * uTime));
    second_wave += second_wave_8_y_amplitude * f_periodic_9(f_adjust_to_two_pi(second_wave_8_y_offset + rnd_y * TAU * second_wave_8_y_frequency + second_wave_8_y_t * uTime));
    second_wave += second_wave_9_x_amplitude * f_periodic_8(f_adjust_to_two_pi(second_wave_9_x_offset + rnd_x * TAU * second_wave_9_x_frequency + second_wave_9_x_t * uTime));
    second_wave += second_wave_10_x_amplitude * f_periodic_4(f_adjust_to_two_pi(second_wave_10_x_offset + rnd_x * TAU * second_wave_10_x_frequency + second_wave_10_x_t * uTime));
    second_wave += second_wave_11_x_amplitude * f_periodic_4(f_adjust_to_two_pi(second_wave_11_x_offset + rnd_x * TAU * second_wave_11_x_frequency + second_wave_11_x_t * uTime));
    second_wave += second_wave_12_x_amplitude * f_periodic_10(f_adjust_to_two_pi(second_wave_12_x_offset + rnd_x * TAU * second_wave_12_x_frequency + second_wave_12_x_t * uTime));
    second_wave += second_wave_13_x_amplitude * f_periodic_10(f_adjust_to_two_pi(second_wave_13_x_offset + rnd_x * TAU * second_wave_13_x_frequency + second_wave_13_x_t * uTime));
    second_wave += second_wave_14_x_amplitude * f_periodic_8(f_adjust_to_two_pi(second_wave_14_x_offset + rnd_x * TAU * second_wave_14_x_frequency + second_wave_14_x_t * uTime));
    second_wave += second_wave_15_x_amplitude * f_periodic_0(f_adjust_to_two_pi(second_wave_15_x_offset + rnd_x * TAU * second_wave_15_x_frequency + second_wave_15_x_t * uTime));
    second_wave += second_wave_16_x_amplitude * f_periodic_9(f_adjust_to_two_pi(second_wave_16_x_offset + rnd_x * TAU * second_wave_16_x_frequency + second_wave_16_x_t * uTime));
    second_wave += second_wave_17_x_amplitude * f_periodic_3(f_adjust_to_two_pi(second_wave_17_x_offset + rnd_x * TAU * second_wave_17_x_frequency + second_wave_17_x_t * uTime));
    second_wave += second_wave_18_x_amplitude * f_periodic_0(f_adjust_to_two_pi(second_wave_18_x_offset + rnd_x * TAU * second_wave_18_x_frequency + second_wave_18_x_t * uTime));
    second_wave += second_wave_19_x_amplitude * f_periodic_1(f_adjust_to_two_pi(second_wave_19_x_offset + rnd_x * TAU * second_wave_19_x_frequency + second_wave_19_x_t * uTime));

    second_wave *= float(0.2);

    float f_0 = fract(uTime * 0.1);
    float f_1 = 1.0 - f_0;

    float w = f_1 * first_wave + f_0 * second_wave;

    float radius = 0.2 + w;
    
    // Sphere
        vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
        float px = sphere_position.x;
        float py = sphere_position.y;
        float pz = sphere_position.z;
    
        float color_r = 0.01;
        float color_g = 0.01;
        float color_b = 0.01;
    
        
        // Instances Cube Scale
        float scale_cube = 0.001;
        vec3  pos = vec3(px, pz, py);
        vec3  scale = vec3(scale_cube, scale_cube, scale_cube);
    
    
        // Whole object rotation
    
        vec3 rotation_axis = vec3(0.0, 1.0, 0.0);
        float rotation_angle = uTime; // using uTime will not be wise after we will be interpolating between two values
    
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
