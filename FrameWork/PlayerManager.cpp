#include "stdafx.h"
#include "PlayerManager.h"

CPlayerManager* CPlayerManager::m_pInstance = nullptr;


CPlayerManager::CPlayerManager()
{
	m_fPlayerPos.x = 0.f;
	m_fPlayerPos.y = 0.f;
	m_fPlayerPos.z = 0.f;
}

CPlayerManager::~CPlayerManager()
{
}

XMFLOAT3 CPlayerManager::Get_Pos()
{
	return m_fPlayerPos;
}

void CPlayerManager::Set_Pos(XMFLOAT3 fPlayerPos)
{
	m_fPlayerPos = fPlayerPos;
}

XMFLOAT3 CPlayerManager::Get_Shift()
{
	return m_fShiftPos;
}
void CPlayerManager::Set_Shift(XMFLOAT3 fPlayerPos)
{
	m_fShiftPos = fPlayerPos;
}

void CPlayerManager::Set_Type(char type)
{
	m_cType = type;
}

char CPlayerManager::Get_Type()
{
	return m_cType;
}

