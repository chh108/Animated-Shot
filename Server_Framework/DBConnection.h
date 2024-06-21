#pragma once
#include "User.h"
#include "stdafx.h"

class CDBConnection
{
public:
	void			display_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);

	bool			Connect();
	void			Clear();

	void			Fetch();
	bool			BindCol();
	void			PrintInfo(int num);

public:
	bool			Login(TCHAR* Name, CUser& user);

private:
	static thread_local SQLHENV			m_henv;
	static thread_local SQLHDBC			m_hdbc;
	static thread_local SQLRETURN		m_retcode;
	static thread_local SQLHSTMT		m_hstmt;


	SQLWCHAR		szName[50];
	SQLINTEGER		dId;     //int
	SQLSMALLINT		dType, dLevel, dHp, dExp, dColType; //short
	SQLFLOAT		dPosX = 0.0f, dPosY = 0.0f, dPosZ = 0.0f, dColPosX, dColPosY, dColPosZ; //float
	SQLLEN			cbName = 0, cbType = 0, cbLevel = 0, cbId = 0,
		cbHp = 0, cbExp = 0, cbPosX = 0, cbPosY = 0, cbPosZ = 0,
		cbColPosX = 0, cbColPosY = 0, cbColPosZ = 0, cbColType =0;

};

