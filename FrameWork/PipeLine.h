#pragma once
#include "stdafx.h"
#include "Base.h"

class CPipeLine	final : public CBase
{
public:
	static		CPipeLine* Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPipeLine;
		}

		return m_pInstance;
	}

	static	void	Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	enum TRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

private:
	static CPipeLine* m_pInstance;

public:
	void Set_Transform(TRANSFORMSTATE eTransformState, FXMMATRIX TransformMatrix);

	XMMATRIX Get_TransformMatrix(TRANSFORMSTATE eTransformState) const {
		return XMLoadFloat4x4(&m_TransformMatrix[eTransformState]);
	}
	XMFLOAT4X4 Get_TransformFloat4x4(TRANSFORMSTATE eTransformState) const {
		return m_TransformMatrix[eTransformState];
	}
	XMFLOAT4X4 Get_TransformFloat4x4_TP(TRANSFORMSTATE eTransformState) const {
		XMFLOAT4X4		Transform;
		XMStoreFloat4x4(&Transform, XMMatrixTranspose(Get_TransformMatrix(eTransformState)));
		return Transform;
	}

	XMFLOAT4 Get_CamPosition() const {
		return m_vCamPosition;
	}

public:
	void Update();
	
private:
	XMFLOAT4X4				m_TransformMatrix[D3DTS_END];
	XMFLOAT4X4				m_TransformInverseMatrix[D3DTS_END];
	XMFLOAT4					m_vCamPosition;

public:
	virtual void Free() override;
};
