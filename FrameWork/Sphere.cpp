#include "stdafx.h"
#include "Sphere.h"
#include "AABB.h"
#include "OBB.h"


CSphere::CSphere(ID3D12Device* pDevice, ID3D11DeviceContext * pContext)
	: CCollider(pDevice, pContext)
{

}

CSphere::CSphere(const CSphere & rhs)
	: CCollider(rhs)
{

}

HRESULT CSphere::Initialize_Prototype(CCollider::TYPE eColliderType)
{
	if (FAILED(__super::Initialize_Prototype(eColliderType)))
		return E_FAIL;

	m_eColliderType = CCollider::TYPE_SPHERE;

	return S_OK;
}

HRESULT CSphere::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pOriginal_Sphere = new BoundingSphere(m_ColliderDesc.vCenter, m_ColliderDesc.vSize.x);

	m_pSphere = new BoundingSphere(*m_pOriginal_Sphere);

	return S_OK;
}

void CSphere::Update(FXMMATRIX TransformMatrix)
{
	m_pOriginal_Sphere->Transform(*m_pSphere, TransformMatrix);
}

bool CSphere::Collision(CCollider * pTargetCollider)
{
	CCollider::TYPE		eType = pTargetCollider->Get_ColliderType();

	m_isColl = false;

	switch (eType)
	{
	case CCollider::TYPE_AABB:
		m_isColl = m_pSphere->Intersects(((CAABB*)pTargetCollider)->Get_Collider());
		break;

	case CCollider::TYPE_OBB:
		m_isColl = m_pSphere->Intersects(((COBB*)pTargetCollider)->Get_Collider());
		break;

	case CCollider::TYPE_SPHERE:
		m_isColl = m_pSphere->Intersects(((CSphere*)pTargetCollider)->Get_Collider());
		break;
	}

	return m_isColl;
}

HRESULT CSphere::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pBatch->Begin();	

	DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));

	m_pBatch->End();


	return S_OK;
}

CSphere * CSphere::Create(ID3D12Device* pDevice, ID3D11DeviceContext * pContext, CCollider::TYPE eColliderType)
{
	CSphere*			pInstance = new CSphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MessageBox(nullptr, TEXT("Failed To Cloned : CSphere"), L"System Message", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CSphere::Clone(void * pArg)
{
	CSphere*			pInstance = new CSphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MessageBox(nullptr, TEXT("Failed To Cloned : CSphere"), L"System Message", MB_OK);
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CSphere::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginal_Sphere);
	Safe_Delete(m_pSphere);

}
