#include "stdafx.h"
#include "CMegaGolemBObject.h"


CMegaGolemBObject::CMegaGolemBObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	// 다리 제대로 돌려줘야 함.
	CLoadedModelInfo* pMegaGolemBModel = pModel;
	if (!pMegaGolemBModel) pMegaGolemBModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Mega_Golem_B.bin", NULL);

	SetChild(pMegaGolemBModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pMegaBTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[3]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pMegaBTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pMegaBTexture, 0, 15);

	m_pMegaBMaterial = new CMaterial(1);
	m_pMegaBMaterial->SetTexture(m_pMegaBTexture);

	SetMaterial(0, m_pMegaBMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pMegaGolemBModel);

	strcpy_s(m_pstrFrameName, "MegaGolem_B");

	Rotate(0.0f, 90.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CMegaGolemBObject::CMegaGolemBObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	// 다리 제대로 돌려줘야 함.
	CLoadedModelInfo* pMegaGolemBModel = pModel;
	if (!pMegaGolemBModel) pMegaGolemBModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Mega_Golem_B.bin", NULL);

	SetChild(pMegaGolemBModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pMegaBTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[3]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pMegaBTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pMegaBTexture, 0, 15);

	m_pMegaBMaterial = new CMaterial(1);
	m_pMegaBMaterial->SetTexture(m_pMegaBTexture);

	SetMaterial(0, m_pMegaBMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pMegaGolemBModel);

	strcpy_s(m_pstrFrameName, "MegaGolem_B");

	Rotate(0.0f, 90.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CMegaGolemBObject::~CMegaGolemBObject()
{
}

void CMegaGolemBObject::Update()
{
}
