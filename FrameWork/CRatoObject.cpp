#include "stdafx.h"
#include "CRatoObject.h"


CRatoObject::CRatoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pRatoModel = pModel;
	if (!pRatoModel) pRatoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Rato.bin", NULL);

	SetChild(pRatoModel->m_pModelRootObject, true);

	// ERROR : root frame & model frame 음수값 양수로 변환되서 저장되는 중.

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pRatoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[0]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pRatoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pRatoTexture, 0, 15);

	m_pRatoMaterial = new CMaterial(1);
	m_pRatoMaterial->SetTexture(m_pRatoTexture);

	SetMaterial(0, m_pRatoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pRatoModel);

	strcpy_s(m_pstrFrameName, "Rato");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CRatoObject::CRatoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pRatoModel = pModel;
	if (!pRatoModel) pRatoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Rato.bin", NULL);

	SetChild(pRatoModel->m_pModelRootObject, true);

	// ERROR : root frame & model frame 음수값 양수로 변환되서 저장되는 중.

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pRatoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[0]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pRatoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pRatoTexture, 0, 15);

	m_pRatoMaterial = new CMaterial(1);
	m_pRatoMaterial->SetTexture(m_pRatoTexture);

	SetMaterial(0, m_pRatoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pRatoModel);

	strcpy_s(m_pstrFrameName, "Rato");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(0.5f, 0.5f, 0.5f);
}

CRatoObject::~CRatoObject()
{
}

void CRatoObject::Update()
{
}
