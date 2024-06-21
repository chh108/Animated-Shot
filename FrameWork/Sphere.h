#pragma once

#include "Collider.h"
#include "Function.h"

class CSphere final : public CCollider
{
protected:
	CSphere(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	CSphere(const CSphere& rhs);
	virtual ~CSphere() = default;

public:
	BoundingSphere Get_Collider() {
		return *m_pSphere;
	}


public:
	virtual HRESULT Initialize_Prototype(CCollider::TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX TransformMatrix);
	virtual bool Collision(CCollider* pTargetCollider);

public:
	virtual HRESULT Render();

private:
	BoundingSphere*			m_pSphere = nullptr;
	BoundingSphere*			m_pOriginal_Sphere = nullptr;


public:
	static CSphere* Create(ID3D12Device* pDevice, ID3D11DeviceContext* pContext, CCollider::TYPE eColliderType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;


};
