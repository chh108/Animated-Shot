#include "stdafx.h"
#include "CPlantaObject.h"

CPlantaObject::CPlantaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pPlantaModel = pModel;
	if (!pPlantaModel) pPlantaModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Planta_Queen.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pPlantaModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pPlantaTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pPlantaTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pPlantaTexture, 0, 15);

	m_pPlantaMaterial = new CMaterial(1);
	m_pPlantaMaterial->SetTexture(m_pPlantaTexture);

	SetMaterial(0, m_pPlantaMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pPlantaModel);

	strcpy_s(m_pstrFrameName, "Planta");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}


CPlantaObject::CPlantaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pPlantaModel = pModel;
	if (!pPlantaModel) pPlantaModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Planta_Queen.bin", NULL);

	// 몸체 y축, Leg_L 180도

	SetChild(pPlantaModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pPlantaTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[6]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pPlantaTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pPlantaTexture, 0, 15);

	m_pPlantaMaterial = new CMaterial(1);
	m_pPlantaMaterial->SetTexture(m_pPlantaTexture);

	SetMaterial(0, m_pPlantaMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pPlantaModel);

	strcpy_s(m_pstrFrameName, "Planta");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CPlantaObject::~CPlantaObject()
{
}

void CPlantaObject::Update()
{
}
