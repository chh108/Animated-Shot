#include "stdafx.h"
#include "CCatusoObject.h"


CCactusoObject::CCactusoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pCactusoModel = pModel;
	if (!pCactusoModel) pCactusoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Cactuso.bin", NULL);

	SetChild(pCactusoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pCactusoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[4]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pCactusoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pCactusoTexture, 0, 15);

	m_pCactusoMaterial = new CMaterial(1);
	m_pCactusoMaterial->SetTexture(m_pCactusoTexture);

	SetMaterial(0, m_pCactusoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pCactusoModel);

	strcpy_s(m_pstrFrameName, "Cactuso");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CCactusoObject::CCactusoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pCactusoModel = pModel;
	if (!pCactusoModel) pCactusoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Cactuso.bin", NULL);

	SetChild(pCactusoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pCactusoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[4]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pCactusoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pCactusoTexture, 0, 15);

	m_pCactusoMaterial = new CMaterial(1);
	m_pCactusoMaterial->SetTexture(m_pCactusoTexture);

	SetMaterial(0, m_pCactusoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pCactusoModel);

	strcpy_s(m_pstrFrameName, "Cactuso");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CCactusoObject::~CCactusoObject()
{
}

void CCactusoObject::Update()
{
}
