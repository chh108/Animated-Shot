#pragma once
#include "Object.h"
#include "Scene.h"

class CTurnipaObject : public CGameObject
{
private:
	CTexture* m_pTurnipaTexture;
	CMaterial* m_pTurnipaMaterial;
public:
	CTurnipaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks);
	CTurnipaObject(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature, CLoadedModelInfo* pModel, int nAnimationTracks,void* pArg);
	virtual ~CTurnipaObject();

	virtual void Update();
};