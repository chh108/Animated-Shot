#pragma once
#include "Object.h"
#include "Scene.h"

class CDryadObject : public CGameObject
{
private:
	CTexture* m_pDryadTexture;
	CMaterial* m_pDryadMaterial;
public:
	CDryadObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CDryadObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks,void* pArg);
	virtual ~CDryadObject();

	virtual void Update();
};