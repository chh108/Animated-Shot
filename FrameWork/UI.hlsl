cbuffer cbPerObject : register(b0)
{
    matrix mtxWorldViewProj; // 월드 뷰 프로젝션 매트릭스
    float4 pixelColor; // UI 색상
};

// 텍스처와 샘플러
Texture2D uiTexture : register(t15);
SamplerState uiSampler : register(s0);


// 입력 정점 구조체
struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

// 출력 정점 구조체
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// 버텍스 쉐이더
VS_OUTPUT VSUI(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0), mtxWorldViewProj); // 월드-뷰-프로젝션 변환
    output.texcoord = input.texcoord;
    return output;
}

// 픽셀 쉐이더
float4 PSUI(VS_OUTPUT input) : SV_TARGET
{
    float4 texColor = uiTexture.Sample(uiSampler, input.texcoord);
    return texColor; // 텍스처 색상과 픽셀 색상 결합
}