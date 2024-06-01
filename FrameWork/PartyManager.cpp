#include "stdafx.h"
#include "PartyManager.h"

CPartyManager* CPartyManager::m_pInstance = nullptr;


CPartyManager::CPartyManager()
{
	m_cParty1Type = -1;
	m_cParty2Type = -1;
}

CPartyManager::~CPartyManager()
{
}

XMFLOAT3 CPartyManager::Get_Party1Pos()
{
	return m_fParty1Pos;
}

void CPartyManager::Set_Party1Pos(XMFLOAT3 fPlayerPos)
{
	m_fParty1Pos = fPlayerPos;
}

XMFLOAT3 CPartyManager::Get_Party2Pos()
{
	return m_fParty2Pos;
}

void CPartyManager::Set_Party2Pos(XMFLOAT3 fPlayerPos)
{
	m_fParty2Pos = fPlayerPos;
}

char CPartyManager::Get_Party1Type()
{
	return m_cParty1Type;
}

void CPartyManager::Set_Party1Type(char type)
{
	m_cParty1Type = type;
}

char CPartyManager::Get_Party2Type()
{
	return 	m_cParty2Type;
}

void CPartyManager::Set_Party2Type(char type)
{
	m_cParty2Type = type;
}

short CPartyManager::Get_Party1Id()
{
	return m_sParty1Id;
}

void CPartyManager::Set_Party1Id(short id)
{
	m_sParty1Id = id;
}

short CPartyManager::Get_Party2Id()
{
	return m_sParty2Id;
}

void CPartyManager::Set_Party2Id(short id)
{
	m_sParty2Id = id;
}

char* CPartyManager::Get_Party1Name()
{
	return m_cParty1Name;
}

void CPartyManager::Set_Party1Name(char* Party1Name)
{
	m_cParty1Name = Party1Name;
}

char* CPartyManager::Get_Party2Name()
{
	return m_cParty2Name;
}

void CPartyManager::Set_Party2Name(char* Party2Name)
{
	m_cParty2Name = Party2Name;
}
