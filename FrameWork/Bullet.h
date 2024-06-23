#pragma once
// 0608
#define BULLET 10

#include "Mesh.h"
#include "Object.h"
#include "stdafx.h"


class CBullet : public CGameObject
{
public:
	CBullet(ID3D12Device* pd3dDevice, ID3D12GraphicsCommandList* pd3dCommandList, ID3D12RootSignature* pd3dGraphicsRootSignature);
	virtual ~CBullet();

public:
	virtual void AnimateBullet(float fElapsedTime);

	float		m_fElapsedTime = 0.0f;
	float		m_fDuringTime = 3.0f;

	float		m_fMovingSpeed = 150.0f;

	XMFLOAT3		m_xmf3MovingDir = XMFLOAT3(0.0f, 0.0f, 1.0f);

	void Reset();
};