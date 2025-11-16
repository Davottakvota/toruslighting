Texture2D Postprocess : register(t0);
sampler Sampler : register(s0);  // Comparison sampler

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

//Plane
float4 PS(VS_OUTPUT input) : SV_Target
{

    float uvr = Postprocess.SampleLevel(Sampler,input.uv+float2(0.01,0.01),0).x;
    float uvg = Postprocess.SampleLevel(Sampler,input.uv-float2(0,0.01),0).y;
    float uvb = Postprocess.SampleLevel(Sampler,input.uv+float2(-0.01,0.01),0).z;
    
    float4 uv = float4(uvr,uvg,uvb,1);
    return uv;
}