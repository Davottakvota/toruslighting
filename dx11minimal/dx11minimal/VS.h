cbuffer global : register(b5)
{
    float4 gConst[32];
};

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
};

cbuffer camera : register(b3)
{
    float4x4 world[2];
    float4x4 view[2];
    float4x4 proj[2];
    int k1;
    int k2;
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float3 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

float3 rotY(float3 pos, float a)
{
    float3x3 m =
    {
        cos(a), 0, sin(a),
        0, 1, 0,
        -sin(a), 0, cos(a)
    };
    pos = mul(pos, m);
    return pos;
}

float3 rotX(float3 pos, float a)
{
    float3x3 m =
    {
        cos(a), sin(a), 0,
        -sin(a), cos(a), 0,
        0, 0, 1
    };
    pos = mul(pos, m);
    return pos;
}

float3 rotZ(float3 pos, float a)
{
    float3x3 m =
    {
        1, 0, 0,
        0, cos(a), sin(a),
        0,-sin(a), cos(a),
        
    };
    pos = mul(pos, m);
    return pos;
}

float hash11(uint n)
{
    // integer hash copied from Hugo Elias
    n = (n << 13U) ^ n;
    n = n * (n * n * 15731U + 789221U) + 1376312589U;
    return float(n & uint(0x7fffffffU)) / float(0x7fffffff);
}

static float pi = 3.14159265;
static int p0 = 0;
static int q0 = 1;
static float s = 0.35;

float3 f(float t) {
    float r = cos(2 * pi * q0 * t / k1) + 2.0;
    float3 res = float3(r * cos(2 * pi * p0 * t / k1), r * sin(2 * pi * p0 * t / k1), -sin(2 * pi * q0 * t / k1));
    return res;
}

float3 fd(float t) {
    return normalize(f(t + 0.05) - f(t));
}

float3 fdd(float t) {
    return normalize(f(t + 0.05) - 2*f(t) + f(t-0.05));
}

float3 g(float t1, float t2) {
    float3 b1 = s * normalize(cross(fd(t2), fdd(t2)));
    float3 b2 = s * normalize(cross(fd(t2), b1));
    return f(t2) + b1 * cos((2 * t1) * pi / k2) + b2 * sin((2 * t1) * pi / k2);
}


VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    int unum = floor(vID / 6.0);
    float vsss = floor(unum / k2+1);

    float3 quad[6] = { g(unum,vsss), g(unum + 1 - 2 * (vsss % 2),vsss), g(unum + k2,vsss + 1),
        g(unum,vsss), g(unum + 1 - 2 * (vsss % 2),vsss), g(unum - k2,vsss - 1) };

    float3 p = quad[vID%6];

    //float3 quad2[3] = { g(unum,vsss), float3(0,1,1), float3(0,-1,1)};
    //float3 q = quad2[vID%3];

    p = rotZ(p, time[0]*0.025);

    float4 pos = float4(p, 1);
    pos -= float4(2, 0, 0, 0.4);
    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = float2(1, -1) * p / 2. + .5;
    float3 N1 = g(unum, floor(unum / k2) - 1) - g(unum, floor(unum / k2) + 1);
    float3 N2 = g(unum + 1, floor(unum / k2)) - g(unum - 1, floor(unum / k2));

    output.vnorm = rotZ(normalize(cross(N1, N2)), time[0] * 0.025);
    //output.vnorm = pos.xyz;

    return output;
}
