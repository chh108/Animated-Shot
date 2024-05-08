cbuffer cbCameraInfo : register(b1)
{
    matrix                  gmtxView : packoffset(c0);
    matrix                  gmtxProjection : packoffset(c4);
    float3                  gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
    matrix                  gmtxGameObject : packoffset(c0);
    float4                  gcPixelColor : packoffset(c4);
};

#define MAX_VERTEX_INFLUENCES          4
#define SKINNED_ANIMATION_BONES        256

cbuffer cbBoneOffsets : register(b7)
{
    float4x4 gpmtxBoneOffsets[SKINNED_ANIMATION_BONES];
};

cbuffer cbBoneTransforms : register(b8)
{
    float4x4 gpmtxBoneTransforms[SKINNED_ANIMATION_BONES];
};

Texture2D gTexture : register(t15);
SamplerState gssWrap : register(s0);

struct VS_SKINNED_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
    int4 indices : BONEINDEX;
    float4 weights : BONEWEIGHT;
};

struct VS_SKINNED_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_SKINNED_OUTPUT VSSkinnedAnimation(VS_SKINNED_INPUT input)
{
    VS_SKINNED_OUTPUT output;

    float3 positionW = float3(0.0f, 0.0f, 0.0f);
    matrix mtxVertexToBoneWorld;
    for (int i = 0; i < MAX_VERTEX_INFLUENCES; i++)
    {
        mtxVertexToBoneWorld = mul(gpmtxBoneOffsets[input.indices[i]], gpmtxBoneTransforms[input.indices[i]]);
        positionW += input.weights[i] * mul(float4(input.position, 1.0f), mtxVertexToBoneWorld).xyz;
    }

    output.position = mul(mul(float4(positionW, 1.0f), gmtxView), gmtxProjection);
    //output.uv = input.uv;

    return(output);
}

float4 PSSkinnedAnimation(VS_SKINNED_OUTPUT input) : SV_TARGET
{
    float4 Color = gTexture.Sample(gssWrap,input.uv);

    return (Color);
}