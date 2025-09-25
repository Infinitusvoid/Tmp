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
    float offset_x_0 = float(0.012778097) * sin(float(31) * x * TAU + float(0.524483) + t * float(-0.06399971));
    offset_x += offset_x_0;
    wave_color_r += float(0.04270252) * offset_x_0;
    wave_color_g += float(0.0010801449) * offset_x_0;
    wave_color_b += float(0.13334215) * offset_x_0;
    float offset_x_1 = float(0.014200368) * sin(float(45) * x * TAU + float(0.9724575) + t * float(-0.042817395));
    offset_x += offset_x_1;
    wave_color_r += float(0.10711795) * offset_x_1;
    wave_color_g += float(0.024563381) * offset_x_1;
    wave_color_b += float(0.114302896) * offset_x_1;
    float offset_x_2 = float(0.10971392) * sin(float(3) * x * TAU + float(0.5133404) + t * float(-0.02888459));
    offset_x += offset_x_2;
    wave_color_r += float(0.06828469) * offset_x_2;
    wave_color_g += float(0.082310446) * offset_x_2;
    wave_color_b += float(0.0017231532) * offset_x_2;
    float offset_x_3 = float(0.114688076) * sin(float(48) * x * TAU + float(0.44082743) + t * float(0.00061043503));
    offset_x += offset_x_3;
    wave_color_r += float(0.14115278) * offset_x_3;
    wave_color_g += float(0.033383496) * offset_x_3;
    wave_color_b += float(0.026335653) * offset_x_3;
    float offset_x_4 = float(0.120478354) * sin(float(11) * x * TAU + float(0.36609414) + t * float(0.0148490425));
    offset_x += offset_x_4;
    wave_color_r += float(0.14773674) * offset_x_4;
    wave_color_g += float(0.009346447) * offset_x_4;
    wave_color_b += float(0.11614696) * offset_x_4;
    float offset_x_5 = float(0.104372084) * sin(float(3) * x * TAU + float(0.8242502) + t * float(0.026388943));
    offset_x += offset_x_5;
    wave_color_r += float(0.04847436) * offset_x_5;
    wave_color_g += float(0.16251554) * offset_x_5;
    wave_color_b += float(0.16229448) * offset_x_5;
    float offset_x_6 = float(0.06043671) * sin(float(16) * x * TAU + float(0.5670009) + t * float(-0.09805677));
    offset_x += offset_x_6;
    wave_color_r += float(0.081886366) * offset_x_6;
    wave_color_g += float(0.112196565) * offset_x_6;
    wave_color_b += float(0.12605818) * offset_x_6;
    float offset_x_7 = float(0.16666976) * sin(float(1) * x * TAU + float(0.576927) + t * float(0.009928739));
    offset_x += offset_x_7;
    wave_color_r += float(0.1315263) * offset_x_7;
    wave_color_g += float(0.19421843) * offset_x_7;
    wave_color_b += float(0.021957558) * offset_x_7;
    float offset_x_8 = float(0.1683971) * sin(float(20) * x * TAU + float(0.4461656) + t * float(-0.09990283));
    offset_x += offset_x_8;
    wave_color_r += float(0.101383) * offset_x_8;
    wave_color_g += float(0.08724828) * offset_x_8;
    wave_color_b += float(0.11244266) * offset_x_8;
    float offset_x_9 = float(0.12826553) * sin(float(6) * x * TAU + float(0.61458546) + t * float(-0.07909818));
    offset_x += offset_x_9;
    wave_color_r += float(0.12973529) * offset_x_9;
    wave_color_g += float(0.12284289) * offset_x_9;
    wave_color_b += float(0.13180166) * offset_x_9;
    float offset_y_0 = float(0.074730046) * sin(float(32) * y * TAU + float(0.63303024) + t * float(-0.06259));
    offset_y += offset_y_0;
    wave_color_r += float(0.027931422) * offset_y_0;
    wave_color_g += float(0.19975266) * offset_y_0;
    wave_color_b += float(0.11355514) * offset_y_0;
    float offset_y_1 = float(0.11646373) * sin(float(33) * y * TAU + float(0.9693648) + t * float(-0.09881241));
    offset_y += offset_y_1;
    wave_color_r += float(0.05272411) * offset_y_1;
    wave_color_g += float(0.12014939) * offset_y_1;
    wave_color_b += float(0.03601151) * offset_y_1;
    float offset_y_2 = float(0.13467328) * sin(float(35) * y * TAU + float(0.24766079) + t * float(-0.06777708));
    offset_y += offset_y_2;
    wave_color_r += float(0.021168077) * offset_y_2;
    wave_color_g += float(0.014698607) * offset_y_2;
    wave_color_b += float(0.14400183) * offset_y_2;
    float offset_y_3 = float(0.105044246) * sin(float(34) * y * TAU + float(0.88587874) + t * float(-0.07940321));
    offset_y += offset_y_3;
    wave_color_r += float(0.076958574) * offset_y_3;
    wave_color_g += float(0.030058792) * offset_y_3;
    wave_color_b += float(0.10041819) * offset_y_3;
    float offset_y_4 = float(0.08778061) * sin(float(7) * y * TAU + float(0.061568066) + t * float(0.071983434));
    offset_y += offset_y_4;
    wave_color_r += float(0.08548813) * offset_y_4;
    wave_color_g += float(0.017401032) * offset_y_4;
    wave_color_b += float(0.097569734) * offset_y_4;
    float offset_y_5 = float(0.008223933) * sin(float(17) * y * TAU + float(0.16938393) + t * float(0.0929458));
    offset_y += offset_y_5;
    wave_color_r += float(0.19244526) * offset_y_5;
    wave_color_g += float(0.01670704) * offset_y_5;
    wave_color_b += float(0.043303493) * offset_y_5;
    float offset_y_6 = float(0.16708143) * sin(float(37) * y * TAU + float(0.6276205) + t * float(0.060103573));
    offset_y += offset_y_6;
    wave_color_r += float(0.14348935) * offset_y_6;
    wave_color_g += float(0.12910502) * offset_y_6;
    wave_color_b += float(0.11594179) * offset_y_6;
    float offset_y_7 = float(0.08663478) * sin(float(15) * y * TAU + float(0.6647134) + t * float(-0.08853333));
    offset_y += offset_y_7;
    wave_color_r += float(0.078612626) * offset_y_7;
    wave_color_g += float(0.11186199) * offset_y_7;
    wave_color_b += float(0.13799632) * offset_y_7;
    float offset_y_8 = float(0.06973902) * sin(float(75) * y * TAU + float(0.35836285) + t * float(0.015282547));
    offset_y += offset_y_8;
    wave_color_r += float(0.13679773) * offset_y_8;
    wave_color_g += float(0.039031424) * offset_y_8;
    wave_color_b += float(0.16479976) * offset_y_8;
    float offset_y_9 = float(0.1496288) * sin(float(2) * y * TAU + float(0.801817) + t * float(-0.07837234));
    offset_y += offset_y_9;
    wave_color_r += float(0.0878753) * offset_y_9;
    wave_color_g += float(0.14616762) * offset_y_9;
    wave_color_b += float(0.04640228) * offset_y_9;

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
    float offset_x_0 = float(0.05171597) * sin(float(2) * x * TAU + float(0.80077904) + t * float(0.09467678));
    offset_x += offset_x_0;
    wave_color_r += float(0.083799005) * offset_x_0;
    wave_color_g += float(0.05462548) * offset_x_0;
    wave_color_b += float(0.105312005) * offset_x_0;
    float offset_x_1 = float(0.14942794) * sin(float(18) * x * TAU + float(0.50938445) + t * float(-0.006153983));
    offset_x += offset_x_1;
    wave_color_r += float(0.15762433) * offset_x_1;
    wave_color_g += float(0.14443831) * offset_x_1;
    wave_color_b += float(0.12584145) * offset_x_1;
    float offset_x_2 = float(0.1453175) * sin(float(9) * x * TAU + float(0.72796184) + t * float(0.032013617));
    offset_x += offset_x_2;
    wave_color_r += float(0.19352263) * offset_x_2;
    wave_color_g += float(0.03507634) * offset_x_2;
    wave_color_b += float(0.1264111) * offset_x_2;
    float offset_x_3 = float(0.06570341) * sin(float(92) * x * TAU + float(0.98934466) + t * float(-0.0728822));
    offset_x += offset_x_3;
    wave_color_r += float(0.15489538) * offset_x_3;
    wave_color_g += float(0.13463989) * offset_x_3;
    wave_color_b += float(0.040921018) * offset_x_3;
    float offset_x_4 = float(0.15209118) * sin(float(36) * x * TAU + float(0.41258228) + t * float(-0.048014082));
    offset_x += offset_x_4;
    wave_color_r += float(0.011503516) * offset_x_4;
    wave_color_g += float(0.055926275) * offset_x_4;
    wave_color_b += float(0.07071839) * offset_x_4;
    float offset_x_5 = float(0.042738486) * sin(float(51) * x * TAU + float(0.8129589) + t * float(-0.041882288));
    offset_x += offset_x_5;
    wave_color_r += float(0.07882216) * offset_x_5;
    wave_color_g += float(0.13609129) * offset_x_5;
    wave_color_b += float(0.16499975) * offset_x_5;
    float offset_x_6 = float(0.13326433) * sin(float(43) * x * TAU + float(0.69191587) + t * float(-0.08740582));
    offset_x += offset_x_6;
    wave_color_r += float(0.14236195) * offset_x_6;
    wave_color_g += float(0.10160728) * offset_x_6;
    wave_color_b += float(0.073809445) * offset_x_6;
    float offset_x_7 = float(0.16194247) * sin(float(11) * x * TAU + float(0.41171503) + t * float(0.08853121));
    offset_x += offset_x_7;
    wave_color_r += float(0.084913984) * offset_x_7;
    wave_color_g += float(0.14810012) * offset_x_7;
    wave_color_b += float(0.021479398) * offset_x_7;
    float offset_x_8 = float(0.06567919) * sin(float(26) * x * TAU + float(0.72997874) + t * float(-0.09380128));
    offset_x += offset_x_8;
    wave_color_r += float(0.05550939) * offset_x_8;
    wave_color_g += float(0.1620394) * offset_x_8;
    wave_color_b += float(0.116847955) * offset_x_8;
    float offset_x_9 = float(0.03211949) * sin(float(37) * x * TAU + float(0.74324965) + t * float(-0.016761566));
    offset_x += offset_x_9;
    wave_color_r += float(0.0019444991) * offset_x_9;
    wave_color_g += float(0.027455514) * offset_x_9;
    wave_color_b += float(0.10948175) * offset_x_9;
    float offset_y_0 = float(0.143509) * sin(float(58) * y * TAU + float(0.82509935) + t * float(0.08572768));
    offset_y += offset_y_0;
    wave_color_r += float(0.16696268) * offset_y_0;
    wave_color_g += float(0.12006556) * offset_y_0;
    wave_color_b += float(0.020317173) * offset_y_0;
    float offset_y_1 = float(0.063723855) * sin(float(35) * y * TAU + float(0.59354764) + t * float(0.099096775));
    offset_y += offset_y_1;
    wave_color_r += float(0.048950948) * offset_y_1;
    wave_color_g += float(0.10174682) * offset_y_1;
    wave_color_b += float(0.024873476) * offset_y_1;
    float offset_y_2 = float(0.045360237) * sin(float(13) * y * TAU + float(0.089080796) + t * float(-0.06198126));
    offset_y += offset_y_2;
    wave_color_r += float(0.06687498) * offset_y_2;
    wave_color_g += float(0.15623322) * offset_y_2;
    wave_color_b += float(0.02989379) * offset_y_2;
    float offset_y_3 = float(0.11731043) * sin(float(37) * y * TAU + float(0.10756271) + t * float(0.020546425));
    offset_y += offset_y_3;
    wave_color_r += float(0.10139193) * offset_y_3;
    wave_color_g += float(0.13407066) * offset_y_3;
    wave_color_b += float(0.115029834) * offset_y_3;
    float offset_y_4 = float(0.108381845) * sin(float(1) * y * TAU + float(0.9398411) + t * float(-0.027564133));
    offset_y += offset_y_4;
    wave_color_r += float(0.07492949) * offset_y_4;
    wave_color_g += float(0.07577913) * offset_y_4;
    wave_color_b += float(0.08846928) * offset_y_4;
    float offset_y_5 = float(0.12458263) * sin(float(27) * y * TAU + float(0.4943187) + t * float(-0.065299354));
    offset_y += offset_y_5;
    wave_color_r += float(0.094833836) * offset_y_5;
    wave_color_g += float(0.04314588) * offset_y_5;
    wave_color_b += float(0.17651024) * offset_y_5;
    float offset_y_6 = float(0.108246535) * sin(float(51) * y * TAU + float(0.4923365) + t * float(-0.070039466));
    offset_y += offset_y_6;
    wave_color_r += float(0.010248011) * offset_y_6;
    wave_color_g += float(0.07863857) * offset_y_6;
    wave_color_b += float(0.13120417) * offset_y_6;
    float offset_y_7 = float(0.11328353) * sin(float(38) * y * TAU + float(0.78302014) + t * float(-0.05290112));
    offset_y += offset_y_7;
    wave_color_r += float(0.16077782) * offset_y_7;
    wave_color_g += float(0.023641145) * offset_y_7;
    wave_color_b += float(0.18161836) * offset_y_7;
    float offset_y_8 = float(0.1260955) * sin(float(24) * y * TAU + float(0.38116035) + t * float(0.023542201));
    offset_y += offset_y_8;
    wave_color_r += float(0.033802472) * offset_y_8;
    wave_color_g += float(0.17930599) * offset_y_8;
    wave_color_b += float(0.0918625) * offset_y_8;
    float offset_y_9 = float(0.04950636) * sin(float(31) * y * TAU + float(0.5734605) + t * float(-0.097365044));
    offset_y += offset_y_9;
    wave_color_r += float(0.16933309) * offset_y_9;
    wave_color_g += float(0.08737306) * offset_y_9;
    wave_color_b += float(0.042785652) * offset_y_9;

    float offset = offset_x + offset_y;
    offset *= 0.1;
    float color_r = wave_color_r * 0.2;
    float color_g = wave_color_g * 0.2;
    float color_b = wave_color_b * 0.2;
    
    return vec4(color_r, color_g, color_b, offset);
}

vec4 wave(float x, float y, float t)
{
    float f_0 = (0.5 + 0.5 * sin(t * 2.0 + y * 10.0));
    float f_1 = 1.0 - f_0;
    vec4 value_wave_0_0 = wave_0(x, y, uTime);
    vec4 value_wave_0_1 = wave_1(x, y, uTime);
    float w0 = f_1 * value_wave_0_0.w + f_0 * value_wave_0_1.w;

    vec4 value_wave_1_0 = wave_0(x * 14.72, y * 42.42, t);
    vec4 value_wave_1_1 = wave_1(x * 70.43, y * 32.0, t);
    float w1 = f_1 * value_wave_1_0.w + f_0 * value_wave_1_1.w;

    float w = (w0 * w1) + pow(sin(w0 / w1), 4.0) * (0.002 + 0.002 * sin(t + x * y * 10.0));

    vec4 value_wave_0 = value_wave_0_0 + value_wave_1_0;
    vec4 value_wave_1 = value_wave_0_1 + value_wave_1_1;
    
    value_wave_0 *= vec4(pow((0.4 + 0.4 * sin(x * 100.0 + w)), 4.0) * 2.0);
    value_wave_1 *= vec4(pow((0.4 + 0.4 * sin(y * 100.0 + w)), 4.0) * 2.0);
    


    w = pow(w, 0.2) * 0.2;

    

    float color_r = value_wave_0.r * f_1 + value_wave_1.r * f_0;
    float color_g = value_wave_0.g * f_1 + value_wave_1.g * f_0;
    float color_b = value_wave_0.b * f_1 + value_wave_1.b * f_0;

    return vec4(color_r, color_g, color_b, w);
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

    /*
    float f_0 = fract(uTime * (1.0/4.0));
    float f_1 = 1.0 - f_0;
    vec4 value_wave_0 = wave_0(rnd_x, rnd_y, uTime);
    vec4 value_wave_1 = wave_1(rnd_x, rnd_y, uTime);
    float w = f_1 * value_wave_0.w + f_0 * value_wave_1.w;

    float color_r = value_wave_0.r * f_1 + value_wave_1.r * f_0;
    float color_g = value_wave_0.g * f_1 + value_wave_1.g * f_0;
    float color_b = value_wave_0.b * f_1 + value_wave_1.b * f_0;
    */
    vec4 wave = wave(rnd_x, rnd_y, uTime);

    float radius = 0.2 + wave.w;

    float color_r = wave.r;
    float color_g = wave.b;
    float color_b = wave.b;

    
    // Sphere
    vec3 sphere_position = spherical01(radius, rnd_x, rnd_y);
    float px = sphere_position.x;
    float py = sphere_position.y;
    float pz = sphere_position.z;
    
    // Instances Cube Scale
    float scale_cube = 0.001;
    vec3  pos = vec3(px, pz, py);
    vec3  scale = vec3(scale_cube, scale_cube, scale_cube);
    
    
    // Whole object rotation
    vec3 rotation_axis = vec3(0.0, 1.0, 0.0);
    float rotation_angle = uTime + sin((rnd_x * rnd_y * 10.0) + uTime * 0.2); // using uTime will not be wise after we will be interpolating between two values
    
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
