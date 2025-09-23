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
    float offset_x_0 = float(0.10024158) * sin(float(9) * x * TAU + float(0.6029766) + t * float(-0.063265175));
    offset_x += offset_x_0;
    wave_color_r += float(0.079203665) * offset_x_0;
    wave_color_g += float(0.08642169) * offset_x_0;
    wave_color_b += float(0.09882287) * offset_x_0;
    float offset_x_1 = float(0.11566137) * sin(float(23) * x * TAU + float(0.1176091) + t * float(-0.038725637));
    offset_x += offset_x_1;
    wave_color_r += float(0.16360222) * offset_x_1;
    wave_color_g += float(0.061239876) * offset_x_1;
    wave_color_b += float(0.11184193) * offset_x_1;
    float offset_x_2 = float(0.056682825) * sin(float(40) * x * TAU + float(0.44272116) + t * float(-0.037358172));
    offset_x += offset_x_2;
    wave_color_r += float(0.163714) * offset_x_2;
    wave_color_g += float(0.15194838) * offset_x_2;
    wave_color_b += float(0.12097094) * offset_x_2;
    float offset_x_3 = float(0.16673097) * sin(float(30) * x * TAU + float(0.14481393) + t * float(0.032491792));
    offset_x += offset_x_3;
    wave_color_r += float(0.14496413) * offset_x_3;
    wave_color_g += float(0.09528882) * offset_x_3;
    wave_color_b += float(0.14597984) * offset_x_3;
    float offset_x_4 = float(0.12632188) * sin(float(12) * x * TAU + float(0.43072405) + t * float(-0.056126844));
    offset_x += offset_x_4;
    wave_color_r += float(0.16102077) * offset_x_4;
    wave_color_g += float(0.039224427) * offset_x_4;
    wave_color_b += float(0.1585192) * offset_x_4;
    float offset_x_5 = float(0.109605916) * sin(float(5) * x * TAU + float(0.18167448) + t * float(-0.038089104));
    offset_x += offset_x_5;
    wave_color_r += float(0.051227782) * offset_x_5;
    wave_color_g += float(0.11824822) * offset_x_5;
    wave_color_b += float(0.08958237) * offset_x_5;
    float offset_x_6 = float(0.05076279) * sin(float(33) * x * TAU + float(0.4310411) + t * float(-0.09272056));
    offset_x += offset_x_6;
    wave_color_r += float(0.092878416) * offset_x_6;
    wave_color_g += float(0.09029143) * offset_x_6;
    wave_color_b += float(0.033880837) * offset_x_6;
    float offset_x_7 = float(0.12577246) * sin(float(31) * x * TAU + float(0.66534424) + t * float(0.09390348));
    offset_x += offset_x_7;
    wave_color_r += float(0.019137012) * offset_x_7;
    wave_color_g += float(0.1197485) * offset_x_7;
    wave_color_b += float(0.015558114) * offset_x_7;
    float offset_x_8 = float(0.13168892) * sin(float(61) * x * TAU + float(0.95908934) + t * float(0.08533071));
    offset_x += offset_x_8;
    wave_color_r += float(0.122902244) * offset_x_8;
    wave_color_g += float(0.14141035) * offset_x_8;
    wave_color_b += float(0.16031542) * offset_x_8;
    float offset_x_9 = float(0.016531324) * sin(float(4) * x * TAU + float(0.68509823) + t * float(0.0600245));
    offset_x += offset_x_9;
    wave_color_r += float(0.0013496709) * offset_x_9;
    wave_color_g += float(0.09617831) * offset_x_9;
    wave_color_b += float(0.06452855) * offset_x_9;
    float offset_y_0 = float(0.13558966) * sin(float(15) * y * TAU + float(0.54901785) + t * float(-0.089689955));
    offset_y += offset_y_0;
    wave_color_r += float(0.101933554) * offset_y_0;
    wave_color_g += float(0.14620557) * offset_y_0;
    wave_color_b += float(0.15448573) * offset_y_0;
    float offset_y_1 = float(0.053537533) * sin(float(16) * y * TAU + float(0.4793457) + t * float(-0.017620187));
    offset_y += offset_y_1;
    wave_color_r += float(0.03292334) * offset_y_1;
    wave_color_g += float(0.055127207) * offset_y_1;
    wave_color_b += float(0.0065427325) * offset_y_1;
    float offset_y_2 = float(0.067731194) * sin(float(30) * y * TAU + float(0.37175882) + t * float(0.009178817));
    offset_y += offset_y_2;
    wave_color_r += float(0.104346044) * offset_y_2;
    wave_color_g += float(0.021911241) * offset_y_2;
    wave_color_b += float(0.15954192) * offset_y_2;
    float offset_y_3 = float(0.19126332) * sin(float(13) * y * TAU + float(0.09582117) + t * float(-0.05386864));
    offset_y += offset_y_3;
    wave_color_r += float(0.11020057) * offset_y_3;
    wave_color_g += float(0.117394365) * offset_y_3;
    wave_color_b += float(0.0346832) * offset_y_3;
    float offset_y_4 = float(0.112795606) * sin(float(69) * y * TAU + float(0.14405958) + t * float(0.026755046));
    offset_y += offset_y_4;
    wave_color_r += float(0.01754452) * offset_y_4;
    wave_color_g += float(0.06303229) * offset_y_4;
    wave_color_b += float(0.13457559) * offset_y_4;
    float offset_y_5 = float(0.020289432) * sin(float(49) * y * TAU + float(0.76507306) + t * float(-0.019660693));
    offset_y += offset_y_5;
    wave_color_r += float(0.024026947) * offset_y_5;
    wave_color_g += float(0.16705734) * offset_y_5;
    wave_color_b += float(0.11687416) * offset_y_5;
    float offset_y_6 = float(0.08002004) * sin(float(7) * y * TAU + float(0.88541025) + t * float(0.09184466));
    offset_y += offset_y_6;
    wave_color_r += float(0.094598934) * offset_y_6;
    wave_color_g += float(0.16738181) * offset_y_6;
    wave_color_b += float(0.13969055) * offset_y_6;
    float offset_y_7 = float(0.14672266) * sin(float(76) * y * TAU + float(0.064793676) + t * float(0.0011932135));
    offset_y += offset_y_7;
    wave_color_r += float(0.17438123) * offset_y_7;
    wave_color_g += float(0.065709) * offset_y_7;
    wave_color_b += float(0.06071889) * offset_y_7;
    float offset_y_8 = float(0.043776844) * sin(float(7) * y * TAU + float(0.06955728) + t * float(-0.004200256));
    offset_y += offset_y_8;
    wave_color_r += float(0.16575573) * offset_y_8;
    wave_color_g += float(0.14020795) * offset_y_8;
    wave_color_b += float(0.055988483) * offset_y_8;
    float offset_y_9 = float(0.14827368) * sin(float(17) * y * TAU + float(0.9103612) + t * float(0.093521394));
    offset_y += offset_y_9;
    wave_color_r += float(0.17428905) * offset_y_9;
    wave_color_g += float(0.05597316) * offset_y_9;
    wave_color_b += float(0.13689871) * offset_y_9;

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
    float offset_x_0 = float(0.015645195) * sin(float(60) * x * TAU + float(0.50252867) + t * float(-0.047855817));
    offset_x += offset_x_0;
    wave_color_r += float(0.09938139) * offset_x_0;
    wave_color_g += float(0.078788355) * offset_x_0;
    wave_color_b += float(0.079854175) * offset_x_0;
    float offset_x_1 = float(0.087498285) * sin(float(26) * x * TAU + float(0.39752412) + t * float(0.030685544));
    offset_x += offset_x_1;
    wave_color_r += float(0.101387314) * offset_x_1;
    wave_color_g += float(0.049620453) * offset_x_1;
    wave_color_b += float(0.11136246) * offset_x_1;
    float offset_x_2 = float(0.023674605) * sin(float(6) * x * TAU + float(0.5825201) + t * float(0.0518262));
    offset_x += offset_x_2;
    wave_color_r += float(0.07232806) * offset_x_2;
    wave_color_g += float(0.07592959) * offset_x_2;
    wave_color_b += float(0.17941639) * offset_x_2;
    float offset_x_3 = float(0.1672557) * sin(float(23) * x * TAU + float(0.58164555) + t * float(-0.018082429));
    offset_x += offset_x_3;
    wave_color_r += float(0.07544111) * offset_x_3;
    wave_color_g += float(0.18745865) * offset_x_3;
    wave_color_b += float(0.085092515) * offset_x_3;
    float offset_x_4 = float(0.06083169) * sin(float(37) * x * TAU + float(0.8956209) + t * float(-0.06240519));
    offset_x += offset_x_4;
    wave_color_r += float(0.1607152) * offset_x_4;
    wave_color_g += float(0.040489644) * offset_x_4;
    wave_color_b += float(0.11649624) * offset_x_4;
    float offset_x_5 = float(0.0016994675) * sin(float(40) * x * TAU + float(0.09438479) + t * float(-0.04143418));
    offset_x += offset_x_5;
    wave_color_r += float(0.030072086) * offset_x_5;
    wave_color_g += float(0.1956005) * offset_x_5;
    wave_color_b += float(0.18394731) * offset_x_5;
    float offset_x_6 = float(0.26556873) * sin(float(26) * x * TAU + float(0.044915214) + t * float(0.020541787));
    offset_x += offset_x_6;
    wave_color_r += float(0.10729618) * offset_x_6;
    wave_color_g += float(0.18822275) * offset_x_6;
    wave_color_b += float(0.006341928) * offset_x_6;
    float offset_x_7 = float(0.21496001) * sin(float(58) * x * TAU + float(0.37649113) + t * float(0.039659955));
    offset_x += offset_x_7;
    wave_color_r += float(0.16543047) * offset_x_7;
    wave_color_g += float(0.116409145) * offset_x_7;
    wave_color_b += float(0.07857617) * offset_x_7;
    float offset_x_8 = float(0.052871242) * sin(float(10) * x * TAU + float(0.4656099) + t * float(-0.032805197));
    offset_x += offset_x_8;
    wave_color_r += float(0.10449552) * offset_x_8;
    wave_color_g += float(0.025833309) * offset_x_8;
    wave_color_b += float(0.024279764) * offset_x_8;
    float offset_x_9 = float(0.1099951) * sin(float(4) * x * TAU + float(0.57003474) + t * float(0.061971463));
    offset_x += offset_x_9;
    wave_color_r += float(0.08345272) * offset_x_9;
    wave_color_g += float(0.041647613) * offset_x_9;
    wave_color_b += float(0.07603892) * offset_x_9;
    float offset_y_0 = float(0.24629216) * sin(float(26) * y * TAU + float(0.68459797) + t * float(-0.044031274));
    offset_y += offset_y_0;
    wave_color_r += float(0.18974875) * offset_y_0;
    wave_color_g += float(0.17224224) * offset_y_0;
    wave_color_b += float(0.015099603) * offset_y_0;
    float offset_y_1 = float(0.07191326) * sin(float(39) * y * TAU + float(0.34011987) + t * float(0.022656118));
    offset_y += offset_y_1;
    wave_color_r += float(0.18579838) * offset_y_1;
    wave_color_g += float(0.056775235) * offset_y_1;
    wave_color_b += float(0.13392752) * offset_y_1;
    float offset_y_2 = float(0.041231412) * sin(float(78) * y * TAU + float(0.15296571) + t * float(0.07883316));
    offset_y += offset_y_2;
    wave_color_r += float(0.1243949) * offset_y_2;
    wave_color_g += float(0.15351734) * offset_y_2;
    wave_color_b += float(0.14043963) * offset_y_2;
    float offset_y_3 = float(0.039401412) * sin(float(36) * y * TAU + float(0.9713816) + t * float(-0.018644392));
    offset_y += offset_y_3;
    wave_color_r += float(0.03479976) * offset_y_3;
    wave_color_g += float(0.067093804) * offset_y_3;
    wave_color_b += float(0.07615768) * offset_y_3;
    float offset_y_4 = float(0.034915723) * sin(float(2) * y * TAU + float(0.566686) + t * float(-0.036445897));
    offset_y += offset_y_4;
    wave_color_r += float(0.09237217) * offset_y_4;
    wave_color_g += float(0.028160568) * offset_y_4;
    wave_color_b += float(0.13130066) * offset_y_4;
    float offset_y_5 = float(0.21903268) * sin(float(65) * y * TAU + float(0.09824261) + t * float(-0.037894838));
    offset_y += offset_y_5;
    wave_color_r += float(0.12822215) * offset_y_5;
    wave_color_g += float(0.15906747) * offset_y_5;
    wave_color_b += float(0.025365332) * offset_y_5;
    float offset_y_6 = float(0.03676894) * sin(float(59) * y * TAU + float(0.73378754) + t * float(-0.0040481924));
    offset_y += offset_y_6;
    wave_color_r += float(0.09235003) * offset_y_6;
    wave_color_g += float(0.036921564) * offset_y_6;
    wave_color_b += float(0.09589837) * offset_y_6;
    float offset_y_7 = float(0.102192275) * sin(float(40) * y * TAU + float(0.34511366) + t * float(-0.05697458));
    offset_y += offset_y_7;
    wave_color_r += float(0.09266753) * offset_y_7;
    wave_color_g += float(0.17116888) * offset_y_7;
    wave_color_b += float(0.06963937) * offset_y_7;
    float offset_y_8 = float(0.03779913) * sin(float(45) * y * TAU + float(0.88666135) + t * float(-0.08180492));
    offset_y += offset_y_8;
    wave_color_r += float(0.056786776) * offset_y_8;
    wave_color_g += float(0.058805745) * offset_y_8;
    wave_color_b += float(0.15693024) * offset_y_8;
    float offset_y_9 = float(0.17045303) * sin(float(50) * y * TAU + float(0.03128892) + t * float(-0.06414047));
    offset_y += offset_y_9;
    wave_color_r += float(0.002859672) * offset_y_9;
    wave_color_g += float(0.08369287) * offset_y_9;
    wave_color_b += float(0.014347242) * offset_y_9;

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
