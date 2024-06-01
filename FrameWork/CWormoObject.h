#pragma once
#include "Object.h"
#include "Scene.h"


class CWormoObject : public CGameObject
{
private:
	CTexture* m_pWormoTexture;
	CMaterial* m_pWormoMaterial;
public:
	CWormoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CWormoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	virtual ~CWormoObject();

	virtual void Update();
};