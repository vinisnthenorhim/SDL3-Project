static const uint3 gl_WorkGroupSize = uint3(64u, 1u, 1u);

cbuffer UniFormBuffer : register(b0, space2)
{
    float _46_deltaTime : packoffset(c0);
    uint _46_entityCount : packoffset(c0.y);
};

RWByteAddressBuffer _60 : register(u1, space1);
ByteAddressBuffer _66 : register(t0, space0);
RWByteAddressBuffer _112 : register(u0, space1);

static uint3 gl_GlobalInvocationID;
struct SPIRV_Cross_Input
{
    uint3 gl_GlobalInvocationID : SV_DispatchThreadID;
};

float4x4 translationMatrix(float3 offset)
{
    return float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(offset.x, offset.y, offset.z, 1.0f));
}

void main_inner()
{
    uint entityIndex = gl_GlobalInvocationID.x;
    if (entityIndex >= _46_entityCount)
    {
        return;
    }
    _60.Store(entityIndex * 16 + 0, asuint(asfloat(_60.Load(entityIndex * 16 + 0)) + (asfloat(_66.Load(entityIndex * 4 + 0)) * _46_deltaTime)));
    _60.Store(entityIndex * 16 + 4, asuint(asfloat(_60.Load(entityIndex * 16 + 4)) + (asfloat(_66.Load((entityIndex + _46_entityCount) * 4 + 0)) * _46_deltaTime)));
    _60.Store(entityIndex * 16 + 8, asuint(asfloat(_60.Load(entityIndex * 16 + 8)) + (asfloat(_66.Load(((entityIndex + _46_entityCount) + _46_entityCount) * 4 + 0)) * _46_deltaTime)));
    float3 param = asfloat(_60.Load3(entityIndex * 16 + 0));
    float4x4 _119 = translationMatrix(param);
    _112.Store4(entityIndex * 64 + 0, asuint(_119[0]));
    _112.Store4(entityIndex * 64 + 16, asuint(_119[1]));
    _112.Store4(entityIndex * 64 + 32, asuint(_119[2]));
    _112.Store4(entityIndex * 64 + 48, asuint(_119[3]));
}

[numthreads(64, 1, 1)]
void main(SPIRV_Cross_Input stage_input)
{
    gl_GlobalInvocationID = stage_input.gl_GlobalInvocationID;
    main_inner();
}
