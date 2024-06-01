#pragma once
#include "Object.h"
#include "Scene.h"

class CCactusoObject : public CGameObject
{
private:
	CTexture* m_pCactusoTexture;
	CMaterial* m_pCactusoMaterial;
public:
	CCactusoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CCactusoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	virtual ~CCactusoObject();

	virtual void Update();
};

