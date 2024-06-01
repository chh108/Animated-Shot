#include "stdafx.h"
#include "CRabbyObject.h"

CRabbyObject::CRabbyObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pRabbyModel = pModel;
	if (!pRabbyModel) pRabbyModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Rabby.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pRabbyModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pRabbyTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[15]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pRabbyTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pRabbyTexture, 0, 15);

	m_pRabbyMaterial = new CMaterial(1);
	m_pRabbyMaterial->SetTexture(m_pRabbyTexture);

	SetMaterial(0, m_pRabbyMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pRabbyModel);

	strcpy_s(m_pstrFrameName, "Rabby");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CRabbyObject::CRabbyObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pRabbyModel = pModel;
	if (!pRabbyModel) pRabbyModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Rabby.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pRabbyModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pRabbyTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[15]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pRabbyTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pRabbyTexture, 0, 15);

	m_pRabbyMaterial = new CMaterial(1);
	m_pRabbyMaterial->SetTexture(m_pRabbyTexture);

	SetMaterial(0, m_pRabbyMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pRabbyModel);

	strcpy_s(m_pstrFrameName, "Rabby");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CRabbyObject::~CRabbyObject()
{
}

void CRabbyObject::Update()
{
}
