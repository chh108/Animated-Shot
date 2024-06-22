#pragma once
// 0608
#define BULLET 10

#include "Object.h"


class CBullet : public CGameObject
{
public:
	CBullet();
	virtual ~CBullet();

public:
	virtual void AnimateBullet(float fElapsedTime);

	float		m_fElapsedTime = 0.0f;
	float		m_fDuringTime = 3.0f;

	float		m_fMovingSpeed = 2.0f;

	XMFLOAT3		m_xmf3MovingDir = XMFLOAT3(0.0f, 0.0f, 1.0f);

	void Reset();
};