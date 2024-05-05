cbuffer cbCameraInfo : register(b1)
{
	matrix					gmtxView : packoffset(c0);
	matrix					gmtxProjection : packoffset(c4);
	float3					gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix					gmtxWorldViewProjection : packoffset(c0);
	float4					gcPixelColor : packoffset(c4); 
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

Texture2D gTexture : register(t14);

SamplerState gssClamp : register(s1);

struct VS_TEXTURE_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_TEXTURE_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

VS_TEXTURE_OUTPUT VSTexture(VS_TEXTURE_INPUT input)
{
    VS_TEXTURE_OUTPUT output;

    // 정점 위치 계산
    output.position = mul(float4(input.position, 1.0f), gmtxWorldViewProjection);

    output.color = input.color;
    output.uv = input.uv;

    return (output);
}

float4 PSTexture(VS_TEXTURE_OUTPUT input) : SV_TARGET
{
    // 텍스처에서 픽셀 색상을 샘플링
    float4 texColor = gTexture.Sample(gSampler, input.uv);

    return (texColor);
}