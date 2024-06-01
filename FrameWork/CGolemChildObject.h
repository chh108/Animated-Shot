#pragma once
#include "Object.h"
#include "Scene.h"

class CGolemChildObject : public CGameObject
{
private:
	CTexture* m_pGolemChildTexture;
	CMaterial* m_pGolemChildMaterial;
public:
	CGolemChildObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	CGolemChildObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	virtual ~CGolemChildObject();

	virtual void Update();
};