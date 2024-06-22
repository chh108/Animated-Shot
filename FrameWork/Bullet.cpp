#include "stdafx.h"
#include "Bullet.h"
#include "Object.h"
#include "Shader.h"
#include "Scene.h"
#include "Mesh.h"

CBullet::CBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature) : CGameObject(1)
{
	CCubeMesh* pCubeMesh = new CCubeMesh(pd3dDevice, pd3dCommandList, 5.0f, 5.0f, 2.0f);
	SetMesh(pCubeMesh);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CTexture* pCubeTexture = new CTexture(1, RESOURCE_TEXTURE_CUBE, 0, 1);
	pCubeTexture->LoadTextureFromFile(pd3dDevice, pd3dCommandList, L"Skybox/SkyBox_0.dds", RESOURCE_TEXTURE_CUBE, 10, 0, DDS);

	CSkyBoxShader* pCubeShader = new CSkyBoxShader();
	pCubeShader->CreateShader(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, SHADER_TYPE::SkyBox);
	pCubeShader->CreateShaderVariables(pd3dDevice, pd3dCommandList);

	CScene::CreateShaderResourceViews(pd3dDevice, pCubeTexture, 0, 10);

	CMaterial* pCubeMaterial = new CMaterial(1);
	pCubeMaterial->SetTexture(pCubeTexture);
	pCubeMaterial->SetShader(pCubeShader);

	SetMaterial(0, pCubeMaterial);

	m_xmOOBB_Object = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_xmOOBB_Parent = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CBullet::~CBullet()
{
}

void CBullet::AnimateBullet(float fElapsedTime)
{
	if (m_bBullet)
	{
		m_fElapsedTime += fElapsedTime;
		if (m_fDuringTime > m_fElapsedTime)
		{
			float fDistance = m_fMovingSpeed * fElapsedTime;
			XMFLOAT3 xmf3Position = GetPosition();
			XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDir, fDistance, false);
			xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
			SetPosition(xmf3Position);
		}
		else
		{
			Reset();
		}
	}
	UpdateTransform();
	UpdateBoundingBox(); // 0608 ¼öÁ¤ Áß
}

void CBullet::Reset()
{
	m_fElapsedTime = 0.0f;
	m_xmf3MovingDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_bBullet = false;
}