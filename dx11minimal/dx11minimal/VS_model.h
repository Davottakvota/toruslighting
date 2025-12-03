// VS_model.h
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

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 norm : NORMAL;
    float2 uv : TEXCOORD0;
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
    VS_OUTPUT output = (VS_OUTPUT)0;

    // Применяем матрицу модели
    float4 worldPos = mul(float4(input.pos, 1.0), model);
    float4 viewPos = mul(worldPos, view[0]);
    output.pos = mul(viewPos, proj[0]);

    output.wpos = worldPos;
    output.vnorm = mul(float4(input.norm, 0.0), model).xyz;
    output.uv = input.uv;

    // Для теней
    output.lpos = mul(worldPos, mul(view[1], proj[1]));

    return output;
}