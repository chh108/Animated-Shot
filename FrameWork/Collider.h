#pragma once

#include "Component.h"
#include "DebugDraw.h"

class CCollider abstract : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };

public:
	typedef struct tagColliderDesc
	{
		XMFLOAT3			vCenter;
		XMFLOAT3			vSize;
		XMFLOAT3			vRotation;
	}COLLIDERDESC;

protected:
	CCollider(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	TYPE Get_ColliderType() const {
		return m_eColliderType;
	}

public:
	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(FXMMATRIX TransformMatrix) = 0;
	virtual bool Collision(CCollider* pTargetCollider) = 0;


//#ifdef _DEBUG
public:
	virtual HRESULT Render();
//#endif // _DEBUG

protected:
	TYPE					m_eColliderType = TYPE_END;
	COLLIDERDESC			m_ColliderDesc;
	bool					m_isColl = false;
	
protected:
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = nullptr;
	BasicEffect*								m_pEffect = nullptr;	
	ID3D11InputLayout*							m_pInputLayout = nullptr;
	XMFLOAT4									m_vColor = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

protected:
	XMMATRIX Remove_Rotation(FXMMATRIX Matrix);

public:	
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;


};

