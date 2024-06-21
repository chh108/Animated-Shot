#include "stdafx.h"
#include "CTurnipaObject.h"

CTurnipaObject::CTurnipaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pTurnipaModel = pModel;
	if (!pTurnipaModel) pTurnipaModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Turnipa.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pTurnipaModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTurnipaTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[14]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pTurnipaTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTurnipaTexture, 0, 15);

	m_pTurnipaMaterial = new CMaterial(1);
	m_pTurnipaMaterial->SetTexture(m_pTurnipaTexture);

	SetMaterial(0, m_pTurnipaMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pTurnipaModel);

	strcpy_s(m_pstrFrameName, "Turnipa");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CTurnipaObject::CTurnipaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pTurnipaModel = pModel;
	if (!pTurnipaModel) pTurnipaModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Turnipa.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pTurnipaModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pTurnipaTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[14]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pTurnipaTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pTurnipaTexture, 0, 15);

	m_pTurnipaMaterial = new CMaterial(1);
	m_pTurnipaMaterial->SetTexture(m_pTurnipaTexture);

	SetMaterial(0, m_pTurnipaMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pTurnipaModel);

	strcpy_s(m_pstrFrameName, "Turnipa");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CTurnipaObject::~CTurnipaObject()
{
}

void CTurnipaObject::Update()
{
}
