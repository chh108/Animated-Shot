#include "stdafx.h"
#include "DoranSword.h"
#include "Object.h"
#include "Shader.h"
#include "Scene.h"
#include "Mesh.h"
#include "PlayerManager.h"
#include "CameraManager.h"

CDoranSword::CDoranSword(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature) : CGameObject(1)
{
    // UI �޽� ���� (20x20 ũ��)
    C2DUIMesh* pUIMesh = new C2DUIMesh(pd3dDevice, pd3dCommandList, 0.02f, 0.02f);
    SetMesh(pUIMesh);

    CreateShaderVariables(pd3dDevice, pd3dCommandList);

    // UI �ؽ�ó �ε�
    CTexture* pUITexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
    pUITexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Model/UI/DoranBlade.png", RESOURCE_TEXTURE2D_ARRAY, 10, 0, PNG);

    // UI ���̴� ����
    CUIShader *pUIShader = new CUIShader();
    pUIShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, SHADER_TYPE::UI);
    pUIShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

    // �ؽ�ó�� ���̴� ���ҽ� ��� ����
    CScene::CreateShaderResourceViews(pd3dDevice, pUITexture, 0, 10);

    pUIShader->CreateDepthStencilState();

    // UI�� ���� ���� ����
    CMaterial* pUIMaterial = new CMaterial(1);
    pUIMaterial->SetTexture(pUITexture);
    pUIMaterial->SetShader(pUIShader);

    // ���� ����
    SetMaterial(0, pUIMaterial);
}

CDoranSword::~CDoranSword()
{
}

void CDoranSword::Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera)
{
    D3D12_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    //pd3dCommandList->OMSetDepthStencilState(m_pDepthStencilState, 0);

    // ī�޶� �������� �̿��� UI ��ġ�� ī�޶� ����Ʈ ���� ����
    XMFLOAT3 xmf3Pos = pCamera->GetPosition();
    SetPosition(xmf3Pos.x, xmf3Pos.y - 0.5f, xmf3Pos.z + 2.f);

    // �⺻ GameObject�� ���� �Լ� ȣ��

    //if(CCameraManager::Get_Instance()->Get_Mode() == 1)
    CGameObject::Render(pd3dCommandList, pCamera);
}