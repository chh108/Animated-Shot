#include "stdafx.h"
#include "CDryadObject.h"

CDryadObject::CDryadObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pDryadModel = pModel;
	if (!pDryadModel) pDryadModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Dryad.bin", NULL);

	SetChild(pDryadModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pDryadTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pDryadTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pDryadTexture, 0, 15);

	m_pDryadMaterial = new CMaterial(1);
	m_pDryadMaterial->SetTexture(m_pDryadTexture);

	SetMaterial(0, m_pDryadMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pDryadModel);

	strcpy_s(m_pstrFrameName, "Dryad");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CDryadObject::CDryadObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pDryadModel = pModel;
	if (!pDryadModel) pDryadModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Dryad.bin", NULL);

	SetChild(pDryadModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pDryadTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pDryadTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pDryadTexture, 0, 15);

	m_pDryadMaterial = new CMaterial(1);
	m_pDryadMaterial->SetTexture(m_pDryadTexture);

	SetMaterial(0, m_pDryadMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pDryadModel);

	strcpy_s(m_pstrFrameName, "Dryad");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CDryadObject::~CDryadObject()
{
}

void CDryadObject::Update()
{
}
