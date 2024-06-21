#include "stdafx.h"
#include "CWormoObject.h"


CWormoObject::CWormoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks) : CGameObject(1)
{
	CLoadedModelInfo* pWormoModel = pModel;
	if (!pWormoModel) pWormoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Wormo.bin", NULL);

	SetChild(pWormoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pWormoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[1]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pWormoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pWormoTexture, 0, 15);

	m_pWormoMaterial = new CMaterial(1);
	m_pWormoMaterial->SetTexture(m_pWormoTexture);

	SetMaterial(0, m_pWormoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pWormoModel);

	strcpy_s(m_pstrFrameName, "Wormo");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CWormoObject::CWormoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg) : CGameObject(1)
{
	CLoadedModelInfo* pWormoModel = pModel;
	if (!pWormoModel) pWormoModel = CGameObject::LoadGeometryAndAnimationFromFile(pd3dDevice, pd3dCommandList, pd3dGraphicsRootSignature, "Monster/Wormo.bin", NULL);

	SetChild(pWormoModel->m_pModelRootObject, true);

	CreateShaderVariables(pd3dDevice, pd3dCommandList);

	m_pWormoTexture = new CTexture(1, RESOURCE_TEXTURE2D_ARRAY, 0, 1);
	CMaterial* pMaterial = CMaterial::v_Materials[1]; // 데이터 가져오기용
	CTextureProperty TextureProperty = pMaterial->GetTextureProperty(0);
	m_pWormoTexture = TextureProperty.GetTextureFromVec(0);

	CScene::CreateShaderResourceViews(pd3dDevice, m_pWormoTexture, 0, 15);

	m_pWormoMaterial = new CMaterial(1);
	m_pWormoMaterial->SetTexture(m_pWormoTexture);

	SetMaterial(0, m_pWormoMaterial);

	m_pSkinnedAnimationController = new CAnimationController(pd3dDevice, pd3dCommandList, nAnimationTracks, pWormoModel);

	strcpy_s(m_pstrFrameName, "Wormo");

	Rotate(0.0f, 0.0f, 0.0f);
	SetScale(1.5f, 1.5f, 1.5f);
}

CWormoObject::~CWormoObject()
{
}

void CWormoObject::Update()
{
}
