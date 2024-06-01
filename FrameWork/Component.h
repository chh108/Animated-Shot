#pragma once

#include "Base.h"

/*  ���� ������Ʈ���� �θ� �Ǵ� Ŭ������. */

class CComponent  abstract : public CBase
{
protected:
	CComponent(ID3D12Device* pDevice, ID3D11DeviceContext* pContext);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
#ifdef _DEBUG
public:
	virtual HRESULT Render() { return S_OK; }
#endif // _DEBUG

protected:
	ID3D12Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	bool					m_isCloned = false;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;

};
