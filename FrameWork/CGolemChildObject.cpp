#include "stdafx.h"
#include "CGolemChildObject.h"


CGolemChildObject::CGolemChildObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pGolemChildModel = pModel;
	if (!pGolemChildModel) pGolemChildModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Golem_Child.bin", NULL);

	SetChild(pGolemChildModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pGolemChildTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[7]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pGolemChildTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pGolemChildTexture, 0, 15);

	m_pGolemChildMaterial = new CMaterial(1);
	m_pGolemChildMaterial->SetTexture(m_pGolemChildTexture);

	SetMaterial(0, m_pGolemChildMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pGolemChildModel);

	strcpy_s(m_pstrFrameName, "GolemChild");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CGolemChildObject::CGolemChildObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pGolemChildModel = pModel;
	if (!pGolemChildModel) pGolemChildModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Golem_Child.bin", NULL);

	SetChild(pGolemChildModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pGolemChildTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[7]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pGolemChildTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pGolemChildTexture, 0, 15);

	m_pGolemChildMaterial = new CMaterial(1);
	m_pGolemChildMaterial->SetTexture(m_pGolemChildTexture);

	SetMaterial(0, m_pGolemChildMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pGolemChildModel);

	strcpy_s(m_pstrFrameName, "GolemChild");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CGolemChildObject::~CGolemChildObject()
{
}

void CGolemChildObject::Update()
{
}
