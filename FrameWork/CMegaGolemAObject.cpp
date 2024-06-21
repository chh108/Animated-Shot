#include "stdafx.h"
#include "CMegaGolemAObject.h"


CMegaGolemAObject::CMegaGolemAObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	// 다리 제대로 돌려줘야 함.
	CLoadedModelInfo* pMegaGolemAModel = pModel;
	if (!pMegaGolemAModel) pMegaGolemAModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Mega_Golem_A.bin", NULL);

	SetChild(pMegaGolemAModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pMegaATexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[2]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pMegaATexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pMegaATexture, 0, 15);

	m_pMegaAMaterial = new CMaterial(1);
	m_pMegaAMaterial->SetTexture(m_pMegaATexture);

	SetMaterial(0, m_pMegaAMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pMegaGolemAModel);

	strcpy_s(m_pstrFrameName, "MegaGolem_A");

	Rotate(0.0f, 90.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CMegaGolemAObject::CMegaGolemAObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	// 다리 제대로 돌려줘야 함.
	CLoadedModelInfo* pMegaGolemAModel = pModel;
	if (!pMegaGolemAModel) pMegaGolemAModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Mega_Golem_A.bin", NULL);

	SetChild(pMegaGolemAModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pMegaATexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[2]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pMegaATexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pMegaATexture, 0, 15);

	m_pMegaAMaterial = new CMaterial(1);
	m_pMegaAMaterial->SetTexture(m_pMegaATexture);

	SetMaterial(0, m_pMegaAMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pMegaGolemAModel);

	strcpy_s(m_pstrFrameName, "MegaGolem_A");

	Rotate(0.0f, 90.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CMegaGolemAObject::~CMegaGolemAObject()
{
}

void CMegaGolemAObject::Update()
{
}
