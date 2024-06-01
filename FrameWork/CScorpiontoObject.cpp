#include "stdafx.h"
#include "CScorpiontoObject.h"


CScorpiontoObject::CScorpiontoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pScorpiontoModel = pModel;
	if (!pScorpiontoModel) pScorpiontoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Scorpionto.bin", NULL);

	SetChild(pScorpiontoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pScorpionTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[5]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pScorpionTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pScorpionTexture, 0, 15);

	m_pScorpionMaterial = new CMaterial(1);
	m_pScorpionMaterial->SetTexture(m_pScorpionTexture);

	SetMaterial(0, m_pScorpionMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pScorpiontoModel);

	strcpy_s(m_pstrFrameName, "Scorpionto");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CScorpiontoObject::CScorpiontoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pScorpiontoModel = pModel;
	if (!pScorpiontoModel) pScorpiontoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Scorpionto.bin", NULL);

	SetChild(pScorpiontoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pScorpionTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[5]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pScorpionTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pScorpionTexture, 0, 15);

	m_pScorpionMaterial = new CMaterial(1);
	m_pScorpionMaterial->SetTexture(m_pScorpionTexture);

	SetMaterial(0, m_pScorpionMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pScorpiontoModel);

	strcpy_s(m_pstrFrameName, "Scorpionto");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CScorpiontoObject::~CScorpiontoObject()
{
}

void CScorpiontoObject::Update()
{
}
