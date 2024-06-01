#pragma once
#include "Object.h"
#include "Scene.h"

class CScorpiontoObject : public CGameObject
{
private:
	CTexture* m_pScorpionTexture;
	CMaterial* m_pScorpionMaterial;
public:
	CScorpiontoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	CScorpiontoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	virtual ~CScorpiontoObject();

	virtual void Update();
};