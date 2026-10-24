cbuffer Projection : register(b0, space1)
{
    row_major float4x4 _25_projection : packoffset(c0);
};

ByteAddressBuffer _32 : register(t1, space0);
ByteAddressBuffer _39 : register(t0, space0);
ByteAddressBuffer _60 : register(t3, space0);
ByteAddressBuffer _71 : register(t2, space0);

static float4 gl_Position;
static int gl_InstanceIndex;
static float3 position;
static float2 fragUV;
static float2 uv;

struct SPIRV_Cross_Input
{
    float3 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    uint gl_InstanceIndex : SV_InstanceID;
};

struct SPIRV_Cross_Output
{
    float2 fragUV : TEXCOORD0;
    float4 gl_Position : SV_Position;
};

void main_inner()
{
    uint entityIndex = uint(gl_InstanceIndex);
    float4x4 _34 = asfloat(uint4x4(_32.Load4(0), _32.Load4(16), _32.Load4(32), _32.Load4(48)));
    float4x4 _42 = asfloat(uint4x4(_39.Load4(entityIndex * 64 + 0), _39.Load4(entityIndex * 64 + 16), _39.Load4(entityIndex * 64 + 32), _39.Load4(entityIndex * 64 + 48)));
    gl_Position = mul(float4(position, 1.0f), mul(_42, mul(_34, _25_projection)));
    uint tex = uint(int(_60.Load(entityIndex * 4 + 0)));
    float4 rect = asfloat(_71.Load4(tex * 16 + 0));
    fragUV = rect.xy + (uv * rect.zw);
}

SPIRV_Cross_Output main(SPIRV_Cross_Input stage_input)
{
    gl_InstanceIndex = int(stage_input.gl_InstanceIndex);
    position = stage_input.position;
    uv = stage_input.uv;
    main_inner();
    SPIRV_Cross_Output stage_output;
    stage_output.gl_Position = gl_Position;
    stage_output.fragUV = fragUV;
    return stage_output;
}
