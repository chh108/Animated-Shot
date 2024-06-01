#pragma once

#include "stdafx.h"
#include "Party1.h"
#include "Party2.h"

class CPartyManager
{
private:
	CPartyManager();
	virtual ~CPartyManager();

public:
	static		CPartyManager* Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CPartyManager;
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
	XMFLOAT3 Get_Party1Pos();
	void Set_Party1Pos(XMFLOAT3 fPlayerPos);

	XMFLOAT3 Get_Party2Pos();
	void Set_Party2Pos(XMFLOAT3 fPlayerPos);

	char Get_Party1Type();
	void Set_Party1Type(char type);

	char Get_Party2Type();
	void Set_Party2Type(char type);

	short Get_Party1Id();
	void Set_Party1Id(short id);

	short Get_Party2Id();
	void Set_Party2Id(short id);

	char* Get_Party1Name();
	void Set_Party1Name(char* Paty1Name);

	char* Get_Party2Name();
	void Set_Party2Name(char* Paty2Name);

private:
	static CPartyManager* m_pInstance;


private:
	XMFLOAT3	m_fPlayerPos;

	XMFLOAT3	m_fParty1Pos{ 0.1f, 0.f, 0.f};
	XMFLOAT3	m_fParty2Pos{ 0.1f, 0.f, 0.f };

	XMFLOAT3	m_fShiftPos;
	char		m_cType;

	char		m_cParty1Type;
	char		m_cParty2Type;
	short		m_sParty1Id;
	short		m_sParty2Id;

	char*		m_cParty1Name;
	char*		m_cParty2Name;
};