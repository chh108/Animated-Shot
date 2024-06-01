#pragma once

#include "stdafx.h"
#include "Camera.h"

class CCameraManager
{
private:
	CCameraManager();
	virtual ~CCameraManager();



public:
	static		CCameraManager* Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CCameraManager;
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
	XMFLOAT3 Get_Pos();
	void Set_Pos(XMFLOAT3 fCameraPos);

	int Get_Mode();
	void Set_Mode(int iMode);

private:
	static CCameraManager* m_pInstance;


private:
	XMFLOAT3	m_fCameraPos;
	int			m_iMode;
};

