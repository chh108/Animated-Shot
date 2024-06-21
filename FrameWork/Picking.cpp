#include "stdafx.h"
//}
//#include "Picking.h"
//#include "Transform.h"
//#include "GameFramework.h"
//#include "stdafx.h"
//
//
//CPicking* CPicking::m_pInstance = nullptr;
//
//CPicking::CPicking()
//{
//}
//
//
//HRESULT CPicking::Initialize(HWND hWnd, unsigned int iWinCX, unsigned int iWinCY, ID3D12Device* pDevice, ID3D11DeviceContext* pContext)
//{
//	m_pDevice = pDevice;
//	m_pContext = pContext;
//
//	Safe_AddRef(m_pDevice);
//	Safe_AddRef(m_pContext);
//
//	m_hWnd = hWnd;
//
//	m_iWinCX = iWinCX;
//
//	m_iWinCY = iWinCY;
//
//	return S_OK;
//}
//
//void CPicking::Tick()
//{
//	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
//
//	POINT			ptMouse;
//
//	GetCursorPos(&ptMouse);
//
//	ScreenToClient(m_hWnd, &ptMouse);
//
//	XMFLOAT4			vMousePos;
//
//	vMousePos.x = float(ptMouse.x / (m_iWinCX * 0.5f) - 1);
//	vMousePos.y = float(ptMouse.y / (m_iWinCY * -0.5f) + 1);
//	vMousePos.z = 0.f;
//	vMousePos.w = 1.f;
//
//	
//	XMFLOAT4X4		ProjMatrixInv;
//	XMMATRIX		ViewInverse;
//	XMVECTOR		VectorMouse;
//
//	ViewInverse = XMMatrixInverse(nullptr, XMLoadFloat4x4(&pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ)));
//
//	VectorMouse = XMLoadFloat4(&vMousePos);
//
//	VectorMouse = XMVector3TransformCoord(VectorMouse, ViewInverse);
//
//	XMVECTOR Rayvector;
//	m_vRayPos = XMFLOAT3(0.f, 0.f, 0.f);
//	Rayvector = VectorMouse - XMLoadFloat3(&m_vRayPos);
//	XMStoreFloat3(&m_vRayDir, Rayvector);
//
//
//	XMVECTOR RayPosvector;
//	XMFLOAT4X4	ViewMatrix = pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);
//
//	RayPosvector = XMLoadFloat3(&m_vRayPos);
//	RayPosvector = XMVector3TransformCoord(RayPosvector, XMMatrixInverse(nullptr, XMLoadFloat4x4(&ViewMatrix)));
//
//	XMStoreFloat3(&m_vRayPos, RayPosvector);
//	XMStoreFloat3(&m_vRayDir, XMVector3TransformNormal(Rayvector, XMMatrixInverse(nullptr, XMLoadFloat4x4(&ViewMatrix))));
//
//	RELEASE_INSTANCE(CGameInstance);
//
//
//}
//
//void CPicking::Compute_LocalRayInfo(XMFLOAT3* pRayDir, XMFLOAT3* pRayPos, CTransform* pTransform)
//{
//	XMFLOAT4X4		WorldMatrixInv;
//	XMStoreFloat4x4(&WorldMatrixInv ,pTransform->Get_WorldMatrixInverse());
//
//	//D3DXVec3TransformCoord(pRayPos, &m_vRayPos, &WorldMatrixInv);
//	//D3DXVec3TransformNormal(pRayDir, &m_vRayDir, &WorldMatrixInv);
//
//
//	XMVECTOR 	RayPosvector = XMLoadFloat3(&m_vRayPos);
//	RayPosvector = XMVector3TransformCoord(RayPosvector, pTransform->Get_WorldMatrixInverse());
//	XMStoreFloat3(pRayPos, RayPosvector);
//
//	XMVECTOR RayDirvector = XMLoadFloat3(&m_vRayDir);
//	RayDirvector = XMVector3TransformNormal(RayDirvector, pTransform->Get_WorldMatrixInverse());
//	XMStoreFloat3(pRayDir, RayDirvector);
//}
//
//
//void CPicking::Free()
//{
//	Safe_Release(m_pDevice);
//	Safe_Release(m_pContext);
