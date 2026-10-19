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
    outColor = float4(0.20000000298023223876953125f, 0.100000001490116119384765625f, 0.4000000059604644775390625f, 1.0f);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    fragUV = stage_input.fragUV;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.outColor = outColor;
    return stage_output;
}
