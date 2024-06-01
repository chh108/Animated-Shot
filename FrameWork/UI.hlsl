cbuffer cbPerObject : register(b0)
{
    matrix mtxWorldViewProj; // ���� �� �������� ��Ʈ����
    float4 pixelColor; // UI ����
};

// �ؽ�ó�� ���÷�
Texture2D uiTexture : register(t15);
SamplerState uiSampler : register(s0);


// �Է� ���� ����ü
struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

// ��� ���� ����ü
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// ���ؽ� ���̴�
VS_OUTPUT VSUI(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0), mtxWorldViewProj); // ����-��-�������� ��ȯ
    output.texcoord = input.texcoord;
    return output;
}

// �ȼ� ���̴�
float4 PSUI(VS_OUTPUT input) : SV_TARGET
{
    float4 texColor = uiTexture.Sample(uiSampler, input.texcoord);
    return texColor; // �ؽ�ó ����� �ȼ� ���� ����
}