#pragma once
#include "Object.h"
#include "Scene.h"

class CRabbyObject : public CGameObject
{
private:
	CTexture* m_pRabbyTexture;
	CMaterial* m_pRabbyMaterial;
public:
	CRabbyObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CRabbyObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks,void* pArg);
	virtual ~CRabbyObject();

	virtual void Update();
};