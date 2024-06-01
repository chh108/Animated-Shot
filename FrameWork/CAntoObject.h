#pragma once
#include "Object.h"
#include "Scene.h"

class CAntoObject : public CGameObject
{
private:
	CTexture* m_pAntoTexture;
	CMaterial* m_pAntoMaterial;
public:
	CAntoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CAntoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks,void* pArg);
	virtual ~CAntoObject();

	virtual void Update();
};