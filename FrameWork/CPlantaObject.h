#pragma once
#include "Object.h"
#include "Scene.h"

class CPlantaObject : public CGameObject
{
private:
	CTexture* m_pPlantaTexture;
	CMaterial* m_pPlantaMaterial;
public:
	CPlantaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CPlantaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks,void* pArg);
	virtual ~CPlantaObject();

	virtual void Update();
};