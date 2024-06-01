#pragma once
#include "Object.h"
#include "Scene.h"

class CRatoObject : public CGameObject
{
private:
	CTexture* m_pRatoTexture;
	CMaterial* m_pRatoMaterial;
public:
	CRatoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	CRatoObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	virtual ~CRatoObject();
	

	virtual void Update();
};