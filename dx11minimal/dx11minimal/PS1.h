struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float3 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
    float4 lpos : TEXCOORD1;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1.0, 0.0, 0.0, 1.0);
}