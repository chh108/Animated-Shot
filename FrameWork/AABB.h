#pragma once

#include "Collider.h"

class CAABB final : public CCollider
{
protected:
	CAABB(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	CAABB(const CAABB& rhs);
	virtual ~CAABB() = default;

public:
	BoundingBox Get_Collider() {
		return *m_pAABB;
	}
	

public:
	virtual HRESULT Initialize_Prototype(CCollider::TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX TransformMatrix);
	virtual bool Collision(CCollider* pTargetCollider);

public:
	bool Collision_AABB(CCollider* pTargetCollider);

public:
	virtual HRESULT Render();

private:
	BoundingBox*			m_pOriginal_AABB = nullptr;
	BoundingBox*			m_pAABB = nullptr;

private:
	XMFLOAT3 Compute_Min();
	XMFLOAT3 Compute_Max();


public:
	static CAABB* Create(ID3D12Device* pDevice, ID3D11DeviceContext* pContext, CCollider::TYPE eColliderType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};
