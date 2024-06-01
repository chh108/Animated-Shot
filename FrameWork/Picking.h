//#pragma once
//
//#include "Function.h"
//#include "Base.h"
//
//class CPicking final : public CBase
//{
//public:												
//	static CPicking* Get_Instance();
//	static unsigned long Destroy_Instance();				
//private:												
//	static CPicking* m_pInstance;
//public:
//	CPicking();
//	virtual ~CPicking() = default;
//
//public:
//	HRESULT Initialize(HWND hWnd, unsigned int iWinCX, unsigned int iWinCY, ID3D12Device* pDevice, ID3D11DeviceContext * pContext);
//	void Tick(); /* 월드스페이스 상의 마우스레이, 레이포스를 구한다. */
//	void Compute_LocalRayInfo(XMFLOAT3* pRayDir, XMFLOAT3* pRayPos, class CTransform* pTransform);
//	XMFLOAT3 Get_RayDir() { return m_vRayDir; }
//	XMFLOAT3 Get_RayPos() { return m_vRayPos; }
//
//private:
//	ID3D12Device*			m_pDevice = nullptr;
//	ID3D11DeviceContext*	m_pContext = nullptr;
//	CTransform*				m_pTransformCom = nullptr;
//	HWND				m_hWnd;
//	unsigned int				m_iWinCX, m_iWinCY;
//
//	XMFLOAT3				m_vRayDir;
//	XMFLOAT3				m_vRayPos;
//public:
//	virtual void Free() override;
//};
