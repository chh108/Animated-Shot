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

	XMFLOAT3 Get_P1Look() { return m_P1f3Look; };
	void Set_P1Look(XMFLOAT3 xmf3Look) { m_P1f3Look = xmf3Look; };

	XMFLOAT3 Get_P1Right() { return m_P1f3Right; };
	void Set_P1Right(XMFLOAT3 xmf3Right) { m_P1f3Right = xmf3Right; };

	XMFLOAT3 Get_P1Up() { return m_P1f3Up; };
	void Set_P1Up(XMFLOAT3 xmf3Up) { m_P1f3Up = xmf3Up; };

	XMFLOAT3 Get_P2Look() { return m_P2f3Look; };
	void Set_P2Look(XMFLOAT3 xmf3Look) { m_P2f3Look = xmf3Look; };

	XMFLOAT3 Get_P2Right() { return m_P2f3Right; };
	void Set_P2Right(XMFLOAT3 xmf3Right) { m_P2f3Right = xmf3Right; };

	XMFLOAT3 Get_P2Up() { return m_P2f3Up; };
	void Set_P2Up(XMFLOAT3 xmf3Up) { m_P2f3Up = xmf3Up; };	

	PLAYERSTATE Get_P1Animation() { return m_Party1Animation; };
	void Set_P1Animation(PLAYERSTATE Animation) { m_Party1Animation = Animation; };

	PLAYERSTATE Get_P2Animation() { return m_Party2Animation; };
	void Set_P2Animation(PLAYERSTATE Animation) { m_Party2Animation = Animation; };

	char Get_P1State() { return m_Party1State; };
	void Set_P1State(char Party1State) { m_Party1State = Party1State; };

	char Get_P2State() { return m_Party2State; };
	void Set_P2State(char Party2State) { m_Party2State = Party2State; };

	bool Get_P1IsMoving() { return m_P1IsMoving; };
	void Set_P1IsMoving(bool P1IsMoving) { m_P1IsMoving = P1IsMoving; };

	bool Get_P2IsMoving() { return m_P2IsMoving; };
	void Set_P2IsMoving(bool P2IsMoving) { m_P2IsMoving = P2IsMoving; };

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

	XMFLOAT3	m_P1f3Right;
	XMFLOAT3	m_P1f3Up;
	XMFLOAT3	m_P1f3Look;

	XMFLOAT3	m_P2f3Right;
	XMFLOAT3	m_P2f3Up;
	XMFLOAT3	m_P2f3Look;

	PLAYERSTATE		m_Party1Animation;
	PLAYERSTATE		m_Party2Animation;

	bool		m_Party1State = false;
	bool		m_Party2State = false;

	bool		m_P1IsMoving = false;
	bool		m_P2IsMoving = false;
};