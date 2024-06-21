#include "stdafx.h"
#include "CameraManager.h"

CCameraManager* CCameraManager::m_pInstance = nullptr;


CCameraManager::CCameraManager()
{
	m_iMode = 3;
}

CCameraManager::~CCameraManager()
{
}

XMFLOAT3 CCameraManager::Get_Pos()
{
	return m_fCameraPos;
}

void CCameraManager::Set_Pos(XMFLOAT3 fCameraPos)
{
	m_fCameraPos = fCameraPos;
}

int CCameraManager::Get_Mode()
{
	return m_iMode;
}

void CCameraManager::Set_Mode(int iMode)
{
	m_iMode = iMode;
}



