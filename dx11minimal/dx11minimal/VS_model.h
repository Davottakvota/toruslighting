cbuffer camera : register(b3)
{
    float4x4 world[2];
    float4x4 view[2];
    float4x4 proj[2];
    int k1;
    int k2;
    int poly;
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
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

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 worldPos = mul(float4(input.Pos, 1.0), model);
    output.wpos = worldPos;
    output.vpos = mul(worldPos, view[0]);
    output.pos = mul(output.vpos, proj[0]);
    output.lpos = mul(worldPos, view[1]);
    output.lpos = mul(output.lpos, proj[1]);
    output.vnorm = normalize(mul(input.Normal, (float3x3)model));
    output.uv = input.TexCoord;

    return output;
}