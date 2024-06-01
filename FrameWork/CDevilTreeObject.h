#pragma once
#include "Object.h"
#include "Scene.h"

class CDevilTreeObject : public CGameObject
{
private:
	CTexture* m_pDevilTreeTexture;
	CMaterial* m_pDevilTreeMaterial;
public:
	CDevilTreeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CDevilTreeObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks,void* pArg);
	virtual ~CDevilTreeObject();

	virtual void Update();
};