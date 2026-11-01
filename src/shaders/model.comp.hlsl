static const uint3 gl_WorkGroupSize = uint3(64u, 1u, 1u);

cbuffer UniFormBuffer : register(b0, space2)
{
    float _155_deltaTime : packoffset(c0);
    uint _155_entityCount : packoffset(c0.y);
};

RWByteAddressBuffer _168 : register(u1, space1);
ByteAddressBuffer _174 : register(t0, space0);
RWByteAddressBuffer _202 : register(u2, space1);
ByteAddressBuffer _207 : register(t1, space0);
RWByteAddressBuffer _234 : register(u3, space1);
ByteAddressBuffer _239 : register(t2, space0);
RWByteAddressBuffer _266 : register(u0, space1);

static uint3 gl_GlobalInvocationID;
struct SPIRV_Cross_Input
{
    uint3 gl_GlobalInvocationID : SV_DispatchThreadID;
};

float4x4 translationMatrix(float4 offset)
{
    return float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(offset.x, offset.y, offset.z, offset.w));
}

float4x4 scaleMatrix(float4 offset)
{
    return float4x4(float4(offset.x, 0.0f, 0.0f, 0.0f), float4(0.0f, offset.y, 0.0f, 0.0f), float4(0.0f, 0.0f, offset.z, 0.0f), float4(0.0f, 0.0f, 0.0f, offset.w));
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
    uint entityIndex = gl_GlobalInvocationID.x;
    if (entityIndex >= _155_entityCount)
    {
        return;
    }
    _168.Store(entityIndex * 16 + 0, asuint(asfloat(_174.Load(entityIndex * 4 + 0))));
    _168.Store(entityIndex * 16 + 4, asuint(asfloat(_174.Load((entityIndex + _155_entityCount) * 4 + 0))));
    _168.Store(entityIndex * 16 + 8, asuint(asfloat(_174.Load(((entityIndex + _155_entityCount) + _155_entityCount) * 4 + 0))));
    _202.Store(entityIndex * 16 + 0, asuint(asfloat(_207.Load(entityIndex * 4 + 0))));
    _202.Store(entityIndex * 16 + 4, asuint(asfloat(_207.Load((entityIndex + _155_entityCount) * 4 + 0))));
    _202.Store(entityIndex * 16 + 8, asuint(asfloat(_207.Load(((entityIndex + _155_entityCount) + _155_entityCount) * 4 + 0))));
    _234.Store(entityIndex * 16 + 0, asuint(asfloat(_239.Load(entityIndex * 4 + 0))));
    _234.Store(entityIndex * 16 + 4, asuint(asfloat(_239.Load((entityIndex + _155_entityCount) * 4 + 0))));
    _234.Store(entityIndex * 16 + 8, asuint(asfloat(_239.Load(((entityIndex + _155_entityCount) + _155_entityCount) * 4 + 0))));
    float4 param = asfloat(_168.Load4(entityIndex * 16 + 0));
    float4 param_1 = asfloat(_202.Load4(entityIndex * 16 + 0));
    float4 param_2 = asfloat(_234.Load4(entityIndex * 16 + 0));
    float4x4 _285 = mul(rotationMatrix(param_2), mul(scaleMatrix(param_1), translationMatrix(param)));
    _266.Store4(entityIndex * 64 + 0, asuint(_285[0]));
    _266.Store4(entityIndex * 64 + 16, asuint(_285[1]));
    _266.Store4(entityIndex * 64 + 32, asuint(_285[2]));
    _266.Store4(entityIndex * 64 + 48, asuint(_285[3]));
}

[numthreads(64, 1, 1)]
void main(SPIRV_Cross_Input stage_input)
{
    gl_GlobalInvocationID = stage_input.gl_GlobalInvocationID;
    main_inner();
}
