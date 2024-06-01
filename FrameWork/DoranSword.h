#pragma once

#include "Mesh.h"
#include "Object.h"
#include "stdafx.h"

class CDoranSword : public CGameObject
{
public:
	CDoranSword(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CDoranSword();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera = NULL);
};