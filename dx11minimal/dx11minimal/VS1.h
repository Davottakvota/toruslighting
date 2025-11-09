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
    float4 lpos : TEXCOORD1;
};

static float3 rect[6] = { float3(0,0,0), float3(1,0,0), float3(0,1,0), float3(1,0,0), float3(0,1,0), float3(1,1,0) };

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output;

    float3 p = (rect[vID] + float3(-1, 1, 0));
    float4 pos = float4(p, 1);
    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = float2(1, -1) * p / 2. + .5;

    return output;
}