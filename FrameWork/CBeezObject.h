#pragma once
#include "Object.h"
#include "Scene.h"

class CBeezObject : public CGameObject
{
private:
	CTexture* m_pBeezTexture;
	CMaterial* m_pBeezMaterial;
public:
	CBeezObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CBeezObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	virtual ~CBeezObject();

	virtual void Update();
};