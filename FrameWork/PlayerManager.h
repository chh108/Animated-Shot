#pragma once

#include "stdafx.h"
#include "Player.h"

class CPlayerManager
{
private:
	CPlayerManager();
	virtual ~CPlayerManager();



public:
	static		CPlayerManager* Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPlayerManager;
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

private:
	static CPlayerManager* m_pInstance;

};

