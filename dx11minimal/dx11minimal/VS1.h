//cbuffer integerconstants : register(b6)
//{
//    int IntConst[32];
//};

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
    int poly;
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

//static float3 rect[6] = { float3(0,0,0), float3(1,0,0), float3(0,1,0), float3(1,0,0), float3(0,1,0), float3(1,1,0) };

//Plane
VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    const int vcnt = poly;
    const int vcnt2 = vcnt * vcnt;

    int vers = vID / 6;
    float strtx = 2.0*float(vers % vcnt) / float(vcnt2) - 1.0 / float(vcnt);
    float strty = 2.0*float(vers / vcnt) / float(vcnt2) - 1.0 / float(vcnt);

    float2 quad[6];
    quad[0] = float2(strtx, strty);
    quad[1] = float2(strtx + 2.0 / float(vcnt2), strty);
    quad[2] = float2(strtx, strty + 2.0 / float(vcnt2));
    quad[3] = float2(strtx + 2.0 / float(vcnt2), strty);
    quad[4] = float2(strtx, strty + 2.0 / float(vcnt2));
    quad[5] = float2(strtx + 2.0 / float(vcnt2), strty + 2.0 / float(vcnt2));
    float2 p = vcnt*quad[vID%6];

    float4 pos = float4(100*p, 0, 1)+float4(0,5,5,0);

    output.lpos = mul(pos, mul(view[1], proj[1]));

    output.vnorm = float3(0,0,-1);

    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = float2(1, -1) * p / 2. + .5;

    return output;
}