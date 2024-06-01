#pragma once
#include "Object.h"
#include "Scene.h"

class CMegaGolemBObject : public CGameObject
{
private:
	CTexture* m_pMegaBTexture;
	CMaterial* m_pMegaBMaterial;
public:
	CMegaGolemBObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks, void* pArg);
	CMegaGolemBObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	virtual ~CMegaGolemBObject();

	virtual void Update();
};
