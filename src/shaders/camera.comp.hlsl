static const uint3 gl_WorkGroupSize = uint3(1u, 1u, 1u);

cbuffer UniFormBuffer : register(b0, space2)
{
    float _154_deltaTime : packoffset(c0);
    uint _154_follow : packoffset(c0.y);
    uint _154_entityId : packoffset(c0.z);
};

RWByteAddressBuffer _166 : register(u0, space1);
ByteAddressBuffer _171 : register(t1, space0);
ByteAddressBuffer _194 : register(t0, space0);
RWByteAddressBuffer _309 : register(u1, space1);

static uint3 gl_GlobalInvocationID;
struct SPIRV_Cross_Input
{
    uint3 gl_GlobalInvocationID : SV_DispatchThreadID;
};

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

float4x4 translationMatrix(float4 offset)
{
    return float4x4(float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f), float4(0.0f, 0.0f, 1.0f, 0.0f), float4(-offset.x, -offset.y, -offset.z, 1.0f));
}

float4x4 scaleMatrix(float4 offset)
{
    return float4x4(float4(offset.x, 0.0f, 0.0f, 0.0f), float4(0.0f, offset.y, 0.0f, 0.0f), float4(0.0f, 0.0f, offset.z, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
}

void main_inner()
{
    if (gl_GlobalInvocationID.x != 0u)
    {
        return;
    }
    if (_154_follow != 0u)
    {
        _166.Store4(0, asuint(asfloat(_171.Load4(_154_entityId * 16 + 0))));
        _166.Store(16, asuint(0.60000002384185791015625f));
        _166.Store(20, asuint(0.60000002384185791015625f));
        _166.Store(24, asuint(0.60000002384185791015625f));
        _166.Store(28, asuint(1.0f));
        _166.Store(32, asuint(0.0f));
        _166.Store(36, asuint(0.0f));
        _166.Store(40, asuint(0.0f));
        _166.Store(44, asuint(0.0f));
        _166.Store(48, asuint(asfloat(_194.Load(36))));
        _166.Store(52, asuint(asfloat(_194.Load(40))));
        _166.Store(56, asuint(asfloat(_194.Load(44))));
        _166.Store(60, asuint(1.0f));
    }
    else
    {
        if (asfloat(_194.Load(12)) != 0.0f)
        {
            _166.Store(16, asuint(asfloat(_166.Load(16)) * asfloat(_194.Load(12))));
            _166.Store(20, asuint(asfloat(_166.Load(20)) * asfloat(_194.Load(16))));
            _166.Store(24, asuint(asfloat(_166.Load(24)) * asfloat(_194.Load(20))));
            _166.Store(28, asuint(1.0f));
        }
        float4 newPos = float4(asfloat(_194.Load(0)) * _154_deltaTime, asfloat(_194.Load(4)) * _154_deltaTime, asfloat(_194.Load(8)) * _154_deltaTime, 1.0f);
        float4 param = asfloat(_166.Load4(32));
        float4x4 oldRotate = rotationMatrix(param);
        _166.Store4(0, asuint(asfloat(_166.Load4(0)) + mul(newPos, transpose(oldRotate))));
        _166.Store(32, asuint(asfloat(_166.Load(32)) + (asfloat(_194.Load(24)) * _154_deltaTime)));
        _166.Store(36, asuint(asfloat(_166.Load(36)) + (asfloat(_194.Load(28)) * _154_deltaTime)));
        _166.Store(40, asuint(asfloat(_166.Load(40)) + (asfloat(_194.Load(32)) * _154_deltaTime)));
        _166.Store(44, asuint(0.0f));
        _166.Store(48, asuint(asfloat(_194.Load(36))));
        _166.Store(52, asuint(asfloat(_194.Load(40))));
        _166.Store(56, asuint(asfloat(_194.Load(44))));
        _166.Store(60, asuint(1.0f));
    }
    float4 param_1 = asfloat(_166.Load4(48));
    float4 param_2 = asfloat(_166.Load4(16));
    float4 param_3 = asfloat(_166.Load4(32));
    float4 param_4 = asfloat(_166.Load4(0));
    float4x4 _328 = mul(translationMatrix(param_4), mul(rotationMatrix(param_3), mul(scaleMatrix(param_2), translationMatrix(param_1))));
    _309.Store4(0, asuint(_328[0]));
    _309.Store4(16, asuint(_328[1]));
    _309.Store4(32, asuint(_328[2]));
    _309.Store4(48, asuint(_328[3]));
}

[numthreads(1, 1, 1)]
void main(SPIRV_Cross_Input stage_input)
{
    gl_GlobalInvocationID = stage_input.gl_GlobalInvocationID;
    main_inner();
}
