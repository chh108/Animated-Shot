#include "stdafx.h"
#include "PipeLine.h"

CPipeLine* CPipeLine::m_pInstance = nullptr;

CPipeLine::CPipeLine()
{
}

void CPipeLine::Set_Transform(TRANSFORMSTATE eTransformState, FXMMATRIX TransformMatrix)
{
	XMStoreFloat4x4(&m_TransformMatrix[eTransformState], TransformMatrix);
}

void CPipeLine::Update()
{
	for (unsigned int i = 0; i < D3DTS_END; ++i)	
		XMStoreFloat4x4(&m_TransformInverseMatrix[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrix[i])));	

	memcpy(&m_vCamPosition, &m_TransformInverseMatrix[D3DTS_VIEW].m[3][0], sizeof(XMFLOAT4));
}

void CPipeLine::Free()
{

}
