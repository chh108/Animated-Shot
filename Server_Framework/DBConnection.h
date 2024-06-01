#pragma once
#include "User.h"
#include "stdafx.h"

class CDBConnection
{
public:
	void			display_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);

	bool			Connect();
	void			Clear();

	bool			Execute(const WCHAR* query);
	void			Fetch();
	int				GetRowCount();
	void			Unbind();

	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool			BindCol();
	void			PrintInfo(int num);

public:
	bool			Login(TCHAR* Name, CUser& user);

private:
	SQLHENV			m_henv;
	SQLHDBC			m_hdbc = SQL_NULL_HANDLE;
	SQLHSTMT		m_State = SQL_NULL_HANDLE;
	SQLRETURN		m_retcode;
	SQLHSTMT		m_hstmt = 0;


	SQLWCHAR		szName[50];
	SQLINTEGER		dId;     //int
	SQLSMALLINT		dType, dLevel, dHp, dExp, dColType; //short
	SQLFLOAT		dPosX = 0.0f, dPosY = 0.0f, dPosZ = 0.0f, dColPosX, dColPosY, dColPosZ; //float
	SQLLEN			cbName = 0, cbType = 0, cbLevel = 0, cbId = 0,
		cbHp = 0, cbExp = 0, cbPosX = 0, cbPosY = 0, cbPosZ = 0,
		cbColPosX = 0, cbColPosY = 0, cbColPosZ = 0, cbColType =0;

};

