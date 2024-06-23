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

public:
	XMFLOAT3 Get_Pos();
	void Set_Pos(XMFLOAT3 fPlayerPos);

	XMFLOAT3 Get_Shift();
	void Set_Shift(XMFLOAT3 fPlayerPos);

	char Get_Type();
	void Set_Type(char type);

	DWORD Get_MoveKey() { return m_wMoveKey; };
	void Set_MoveKey(DWORD MoveKey) { m_wMoveKey = MoveKey; };

	XMFLOAT3 Get_xmf3Look() { return m_xmf3Look; };
	void Set_xmf3Look(XMFLOAT3 xmf3Look) { m_xmf3Look = xmf3Look; };

	XMFLOAT3 Get_xmf3Right() { return m_xmf3Right; };
	void Set_xmf3Right(XMFLOAT3 xmf3Right) { m_xmf3Right = xmf3Right; };

	XMFLOAT3 Get_xmf3Up() { return m_xmf3Up; };
	void Set_xmf3Up(XMFLOAT3 xmf3Up) { m_xmf3Up = xmf3Up; };

	char Get_Animation() { return m_cAnimation; };
	void Set_Animation(char Animation) { m_cAnimation = Animation; };

	char Get_Login() { return m_bLogin; };
	void Set_Login(char Login) { m_bLogin = Login; };

	XMFLOAT3 Get_Scale() { return m_fScale; };
	void Set_Scale(XMFLOAT3 fScale) { m_fScale = fScale;};

	bool Get_Coll() { return m_bColl; }
	void Set_Coll(bool Coll) { m_bColl = Coll; }

	int Get_ID() { return m_iId; }
	void Set_ID(bool Id) { m_iId = Id; }

private:
	static CPlayerManager* m_pInstance;

private:
	XMFLOAT3	m_fPlayerPos {0.f, 0.f, 0.f};
	XMFLOAT3	m_fShiftPos;
	char		m_cType;
	DWORD		m_wMoveKey;
	char		m_cAnimation = -1;
	bool		m_bLogin = false;
	bool		m_bColl;
	int			m_iId;
	XMFLOAT3	m_xmf3Look, m_xmf3Right, m_xmf3Up;
	XMFLOAT3	m_fScale;
};

