#pragma once

#include "Component.h"
#include "Function.h"

class CTransform final : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct tagTransformDesc
	{
		float			fSpeedPerSec;
		float			fRotationPerSec;
	}TRANSFORMDESC;


private:
	CTransform(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	XMVECTOR Get_State(STATE eState) {
		return XMLoadFloat4((XMFLOAT4*)&m_WorldMatrix.m[eState][0]);
	}

	XMMATRIX Get_WorldMatrix() const {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	XMFLOAT4X4 Get_WorldFloat4x4() const {
		return m_WorldMatrix;
	}

	XMFLOAT4X4 Get_WorldFloat4x4_TP() const {
		XMFLOAT4X4	WorldMatrix;
		XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(Get_WorldMatrix()));
		return WorldMatrix;
	}

	XMMATRIX Get_WorldMatrixInverse() const {
		return XMMatrixInverse(nullptr, Get_WorldMatrix());
	}

	void Set_State(STATE eState, FXMVECTOR vState);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	void Go_Straight(float fTimeDelta);
	void Go_Backward(float fTimeDelta);
	void Go_Left(float fTimeDelta);
	void Go_Right(float fTimeDelta);
	void Go_Up(float fTimeDelta);
	void Go_Down(float fTimeDelta);

	void Set_Scale(FXMVECTOR vScaleInfo);
	XMFLOAT3 Get_Scale();

	void Turn(FXMVECTOR vAxis, float fTimeDelta);
	void Rotation(FXMVECTOR vAxis, float fRadian);

	void LookAt(FXMVECTOR vAt);
	void LookAt_ForLandObject(FXMVECTOR vAt);
	bool Move(FXMVECTOR vTargetPos, float fTimeDelta, float fLimitDistance = 0.1f);

private:
	XMFLOAT4X4				m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

public:
	static CTransform* Create(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};