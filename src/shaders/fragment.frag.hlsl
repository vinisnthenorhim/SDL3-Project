ByteAddressBuffer _23 : register(t1, space2);
Texture2D<float4> tex : register(t0, space2);
SamplerState _tex_sampler : register(s0, space2);

static float2 fragUV;
static uint objectIndex;
static float4 outColor;

struct SPIRV_Cross_Input
{
    float2 fragUV : TEXCOORD0;
    nointerpolation uint objectIndex : TEXCOORD1;
};

struct SPIRV_Cross_Output
{
    float4 outColor : SV_Target0;
};

void main_inner()
{
    float4 color = tex.Sample(_tex_sampler, fragUV);
    color.w *= asfloat(_23.Load(objectIndex * 4 + 0));
    outColor = color;
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    fragUV = stage_input.fragUV;
    objectIndex = stage_input.objectIndex;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.outColor = outColor;
    return stage_output;
}
