#pragma once
#include "Object.h"
#include "Scene.h"

class CMegaGolemAObject : public CGameObject
{
private:
	CTexture* m_pMegaATexture;
	CMaterial* m_pMegaAMaterial;
public:
	CMegaGolemAObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	CMegaGolemAObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	virtual ~CMegaGolemAObject();

	virtual void Update();
};
