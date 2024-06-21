cbuffer cbCameraInfo : register(b1)
{
	matrix					gmtxView : packoffset(c0);
	matrix					gmtxProjection : packoffset(c4);
	float3					gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix					gmtxGameObject : packoffset(c0);
	float4					gcPixelColor : packoffset(c4);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_WIREFRAME_INPUT
{
	float3 position : POSITION;
};

struct VS_WIREFRAME_OUTPUT
{
	float4 position : SV_POSITION;
};

VS_WIREFRAME_OUTPUT VSWireFrame(VS_WIREFRAME_INPUT input)
{
	VS_WIREFRAME_OUTPUT output;

	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxGameObject), gmtxView), gmtxProjection);

	return(output);
}

float4 PSWireFrame(VS_WIREFRAME_OUTPUT input) : SV_TARGET
{
	return(float4(0.0f, 0.0f, 1.0f, 1.0f));
}
