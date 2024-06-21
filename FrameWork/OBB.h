#pragma once

#include "Collider.h"
#include "Function.h"

class COBB final : public CCollider
{
public:
	typedef struct tagOBBDesc
	{
		XMFLOAT3		vCenter;
		XMFLOAT3		vCenterAxis[3];
		XMFLOAT3		vAlignAxis[3];
	}OBBDESC;

protected:
	COBB(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	COBB(const COBB& rhs);
	virtual ~COBB() = default;

public:
	BoundingOrientedBox Get_Collider() {
		return *m_pOBB;
	}

public:
	virtual HRESULT Initialize_Prototype(CCollider::TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX TransformMatrix);
	virtual bool Collision(CCollider* pTargetCollider);

public:
	bool Collision_OBB(CCollider* pTargetCollider);

public:
	virtual HRESULT Render();


private:
	BoundingOrientedBox*	m_pOBB = nullptr;
	BoundingOrientedBox*	m_pOriginal_OBB = nullptr;

private:
	OBBDESC Compute_OBBDesc();
	

public:
	static COBB* Create(ID3D12Device* pDevice, ID3D11DeviceContext* pContext, CCollider::TYPE eColliderType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;


};