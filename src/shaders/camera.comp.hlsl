static const uint3 gl_WorkGroupSize = uint3(1u, 1u, 1u);

RWByteAddressBuffer _155 : register(u1, space1);
RWByteAddressBuffer _161 : register(u0, space1);
ByteAddressBuffer _191 : register(t0, space0);
ByteAddressBuffer _195 : register(t1, space0);
cbuffer UniFormBuffer : register(b0, space2)
{
    float _198_deltaTime : packoffset(c0);
    uint _198_follow : packoffset(c0.y);
    uint _198_entityId : packoffset(c0.z);
    uint _198_entityCount : packoffset(c0.w);
};


static uint3 gl_GlobalInvocationID;
struct SPIRV_Cross_Input
{
    uint3 gl_GlobalInvocationID : SV_DispatchThreadID;
};

float4x4 translationMatrix(float4 offset)
{
    return float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(-offset.x, -offset.y, -offset.z, 1.0f));
}

float4x4 scaleMatrix(float4 offset)
{
    return float4x4(float4(offset.x, 0.0f, 0.0f, 0.0f), float4(0.0f, offset.y, 0.0f, 0.0f), float4(0.0f, 0.0f, offset.z, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
}

float4x4 rotationMatrix(float4 rotation)
{
    float4x4 result = float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
    if (rotation.x != 0.0f)
    {
        float c = cos(rotation.x);
        float s = sin(rotation.x);
        result = mul(float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, c, s, 0.0f), float4(0.0f, -s, c, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f)), result);
    }
    if (rotation.y != 0.0f)
    {
        float c_1 = cos(rotation.y);
        float s_1 = sin(rotation.y);
        result = mul(float4x4(float4(c_1, 0.0f, -s_1, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(s_1, 0.0f, c_1, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f)), result);
    }
    if (rotation.z != 0.0f)
    {
        float c_2 = cos(rotation.z);
        float s_2 = sin(rotation.z);
        result = mul(float4x4(float4(c_2, s_2, 0.0f, 0.0f), float4(-s_2, c_2, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f)), result);
    }
    return result;
}

void main_inner()
{
    if (gl_GlobalInvocationID.x != 0u)
    {
        return;
    }
    float4 param = asfloat(_161.Load4(48));
    float4 param_1 = asfloat(_161.Load4(16));
    float4 param_2 = asfloat(_161.Load4(32));
    float4 param_3 = asfloat(_161.Load4(0));
    float4x4 _184 = mul(translationMatrix(param_3), mul(rotationMatrix(param_2), mul(scaleMatrix(param_1), translationMatrix(param))));
    _155.Store4(0, asuint(_184[0]));
    _155.Store4(16, asuint(_184[1]));
    _155.Store4(32, asuint(_184[2]));
    _155.Store4(48, asuint(_184[3]));
}

[numthreads(1, 1, 1)]
void main(SPIRV_Cross_Input stage_input)
{
    gl_GlobalInvocationID = stage_input.gl_GlobalInvocationID;
    main_inner();
}
