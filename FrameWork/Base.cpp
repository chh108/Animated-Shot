#include "stdafx.h"
#include "Base.h"


unsigned long CBase::AddRef()
{
	return ++m_dwRefCnt;	
}

unsigned long CBase::Release()
{
	if (0 == m_dwRefCnt)
	{
		int a = 10;

		Free();
	
		delete this;

		return 0;
	}
	else
		return m_dwRefCnt--;
}
