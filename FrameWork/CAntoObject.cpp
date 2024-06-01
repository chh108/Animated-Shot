#include "stdafx.h"
#include "CAntoObject.h"

CAntoObject::CAntoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pAntoModel = pModel;
	if (!pAntoModel) pAntoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Anto.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pAntoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pAntoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pAntoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pAntoTexture, 0, 15);

	m_pAntoMaterial = new CMaterial(1);
	m_pAntoMaterial->SetTexture(m_pAntoTexture);

	SetMaterial(0, m_pAntoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pAntoModel);

	strcpy_s(m_pstrFrameName, "Anto");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}


CAntoObject::CAntoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pAntoModel = pModel;
	if (!pAntoModel) pAntoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Anto.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pAntoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pAntoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pAntoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pAntoTexture, 0, 15);

	m_pAntoMaterial = new CMaterial(1);
	m_pAntoMaterial->SetTexture(m_pAntoTexture);

	SetMaterial(0, m_pAntoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pAntoModel);

	strcpy_s(m_pstrFrameName, "Anto");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CAntoObject::~CAntoObject()
{
}

void CAntoObject::Update()
{
}