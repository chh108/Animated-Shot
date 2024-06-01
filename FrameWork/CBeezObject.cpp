#include "stdafx.h"
#include "CBeezObject.h"

CBeezObject::CBeezObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pBeezModel = pModel;
	if (!pBeezModel) pBeezModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Beez.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pBeezModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pBeezTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[12]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pBeezTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pBeezTexture, 0, 15);

	m_pBeezMaterial = new CMaterial(1);
	m_pBeezMaterial->SetTexture(m_pBeezTexture);

	SetMaterial(0, m_pBeezMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pBeezModel);

	strcpy_s(m_pstrFrameName, "Beez");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CBeezObject::CBeezObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pBeezModel = pModel;
	if (!pBeezModel) pBeezModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Beez.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pBeezModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pBeezTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pBeezTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pBeezTexture, 0, 15);

	m_pBeezMaterial = new CMaterial(1);
	m_pBeezMaterial->SetTexture(m_pBeezTexture);

	SetMaterial(0, m_pBeezMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pBeezModel);

	strcpy_s(m_pstrFrameName, "Beez");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CBeezObject::~CBeezObject()
{
}

void CBeezObject::Update()
{
}