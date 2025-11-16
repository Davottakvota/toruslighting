struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

static float3 rect[6] = { float3(0,0,0), float3(1,0,0), float3(0,1,0), float3(1,0,0), float3(0,1,0), float3(1,1,0) };

//Plane
VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    
    output.pos = float4(rect[vID]*2 - 1,1);
    output.uv = float4(rect[vID],1);
    output.uv.y = 1-output.uv.y;

    return output;
}