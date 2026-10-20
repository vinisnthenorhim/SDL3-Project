Texture2D<float4> tex : register(t0, space2);
SamplerState _tex_sampler : register(s0, space2);

static float4 outColor;
static float2 fragUV;

struct SPIRV_Cross_Input
{
    float2 fragUV : TEXCOORD0;
};

struct SPIRV_Cross_Output
{
    float4 outColor : SV_Target0;
};

void main_inner()
{
    outColor = tex.Sample(_tex_sampler, fragUV);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    fragUV = stage_input.fragUV;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.outColor = outColor;
    return stage_output;
}
