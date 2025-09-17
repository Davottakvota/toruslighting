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
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

cbuffer params : register(b1)
{
    float r, g, b;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float3 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

static float3 lightvector = float3(0, -0.55, -1);

float hash11(uint n)
{
    // integer hash copied from Hugo Elias
    n = (n << 13U) ^ n;
    n = n * (n * n * 15731U + 789221U) + 1376312589U;
    return float(n & uint(0x7fffffffU)) / float(0x7fffffff);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float pi = 3.141519;

    float3 h = 2 * (dot(lightvector, input.vnorm) * input.vnorm) - lightvector;
    
    float3 I = 0.05 * float3(1, 1, 1) + 0.25 * max(dot(lightvector, input.vnorm), 0) * float3(64, 224, 255) * 0.0061 + 0.001 * max(pow(dot(h, input.vnorm), 52), 0) * float3(1, 1, 1);
    
    I -= float3(0.9, 0, 1) * hash11(int(200*input.vnorm[0]))*0.15;

    return float4(I, 1.);

}