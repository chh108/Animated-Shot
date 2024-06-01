#include "stdafx.h"
#include "CDevilTreeObject.h"

CDevilTreeObject::CDevilTreeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pDevilTreeModel = pModel;
	if (!pDevilTreeModel) pDevilTreeModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/DevilTree.bin", NULL);

	SetChild(pDevilTreeModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pDevilTreeTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[13]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pDevilTreeTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pDevilTreeTexture, 0, 15);

	m_pDevilTreeMaterial = new CMaterial(1);
	m_pDevilTreeMaterial->SetTexture(m_pDevilTreeTexture);

	SetMaterial(0, m_pDevilTreeMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pDevilTreeModel);

	strcpy_s(m_pstrFrameName, "DevilTree");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CDevilTreeObject::CDevilTreeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pDevilTreeModel = pModel;
	if (!pDevilTreeModel) pDevilTreeModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/DevilTree.bin", NULL);

	SetChild(pDevilTreeModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pDevilTreeTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[13]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pDevilTreeTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pDevilTreeTexture, 0, 15);

	m_pDevilTreeMaterial = new CMaterial(1);
	m_pDevilTreeMaterial->SetTexture(m_pDevilTreeTexture);

	SetMaterial(0, m_pDevilTreeMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pDevilTreeModel);

	strcpy_s(m_pstrFrameName, "DevilTree");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CDevilTreeObject::~CDevilTreeObject()
{
}

void CDevilTreeObject::Update()
{
}
