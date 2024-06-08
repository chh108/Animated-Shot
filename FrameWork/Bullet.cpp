#include "stdafx.h"
#include "Bullet.h"
// 0608
CBullet::CBullet()
{
	m_xmOOBB_Object = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	m_xmOOBB_Parent = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

CBullet::~CBullet()
{
}

void CBullet::AnimateBullet(float fElapsedTime)
{
	if (m_bActive)
	{
		m_fElapsedTime += fElapsedTime;
		if (m_fDuringTime > m_fElapsedTime)
		{
			float fDistance = m_fMovingSpeed * fElapsedTime;
			XMFLOAT3 xmf3Position = GetPosition();
			XMFLOAT3 xmf3Movement = Vector3::ScalarProduct(m_xmf3MovingDir, fDistance, false);
			xmf3Position = Vector3::Add(xmf3Position, xmf3Movement);
			SetPosition(xmf3Position);
		}
		else
		{
			Reset();
		}
	}
	UpdateTransform();
	//UpdateBoundingBox(); ¼öÁ¤ Áß
}

void CBullet::Reset()
{
	m_fElapsedTime = 0.0f;
	m_xmf3MovingDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_bActive = false;
}