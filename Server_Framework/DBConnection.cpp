#include "DBConnection.h"
#include "User.h"

thread_local SQLHANDLE CDBConnection::m_henv = SQL_NULL_HANDLE;
thread_local SQLHANDLE CDBConnection::m_hdbc = SQL_NULL_HANDLE;
thread_local SQLHANDLE CDBConnection::m_hstmt = SQL_NULL_HANDLE;
thread_local SQLRETURN CDBConnection::m_retcode;

void CDBConnection::display_error(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER iError;
	WCHAR wszMessage[1000];
	WCHAR wszState[SQL_SQLSTATE_SIZE + 1];
	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

bool CDBConnection::Connect()
{
	setlocale(LC_ALL, "korean");

	m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO) 
		return false;

	m_retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO)
		return false;

	m_retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO)
		return false;

	SQLSetConnectAttr(m_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
	m_retcode = SQLConnect(m_hdbc, (SQLWCHAR*)L"AnimatedShot_DB", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	
	m_retcode = SQLExecDirect(m_hstmt, (SQLWCHAR*)L"SELECT user_id, user_name, user_type, user_level, user_hp, user_exp, user_posX, user_posY, user_posZ FROM [Animated_Shot].[dbo].[user_table]", SQL_NTS);
	//m_retcode = SQLExecDirect(m_hstmt, 
		//(SQLWCHAR*)L"SELECT user_id, user_name, user_type, user_level, user_hp, user_exp, user_posX, user_posY, user_posZ, user_collisionX, user_collisionY, user_collisionZ, user_collisiontype FROM user_table", SQL_NTS);
	if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO)
		return true;
	else {
		display_error(m_hstmt, SQL_HANDLE_STMT, m_retcode);
		return false;
	}
}


void CDBConnection::Clear()
{
	if (m_hstmt != SQL_NULL_HANDLE)
	{
		SQLCancel(m_hstmt);
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		m_hstmt = SQL_NULL_HANDLE;
	}
	if (m_hdbc != SQL_NULL_HANDLE)
	{
		SQLDisconnect(m_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
	}
	SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
}

void CDBConnection::Fetch()
{
	for (int i = 0; ; i++) {
		m_retcode = SQLFetch(m_hstmt);
		if (m_retcode == SQL_ERROR)
			std::cout << "Error" << std::endl;
		else if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO)
			PrintInfo(i);
		else
			break;
	}
	SQLFreeStmt(m_hstmt, SQL_CLOSE);
}

bool CDBConnection::BindCol()
{
	m_retcode = SQLBindCol(m_hstmt, 1, SQL_INTEGER, &dId, 0, &cbId);
	m_retcode = SQLBindCol(m_hstmt, 2, SQL_C_WCHAR, szName, sizeof(szName), &cbName);
	m_retcode = SQLBindCol(m_hstmt, 3, SQL_C_SHORT, &dType, 0, &cbType);
	m_retcode = SQLBindCol(m_hstmt, 4, SQL_C_SHORT, &dLevel, 0, &cbLevel);
	m_retcode = SQLBindCol(m_hstmt, 5, SQL_C_SHORT, &dHp, 0, &cbHp);
	m_retcode = SQLBindCol(m_hstmt, 6, SQL_C_SHORT, &dExp, 0, &cbExp);
	m_retcode = SQLBindCol(m_hstmt, 7, SQL_C_FLOAT, &dPosX, sizeof(SQLFLOAT), &cbPosX);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO) 
		std::cout << "PosX error" << std::endl;
	m_retcode = SQLBindCol(m_hstmt, 8, SQL_C_FLOAT, &dPosY, sizeof(SQLFLOAT), &cbPosY);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO)
		std::cout << "PosY error" << std::endl;
	m_retcode = SQLBindCol(m_hstmt, 9, SQL_C_FLOAT, &dPosZ, sizeof(SQLFLOAT), &cbPosZ);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO)
		std::cout << "PosZ error" << std::endl;
	//m_retcode = SQLBindCol(m_hstmt, 10, SQL_C_FLOAT, &dColPosX, 60, &cbColPosX);
	//m_retcode = SQLBindCol(m_hstmt, 11, SQL_C_FLOAT, &dColPosY, 60, &cbColPosY);
	//m_retcode = SQLBindCol(m_hstmt, 12, SQL_C_FLOAT, &dColPosZ, 60, &cbColPosZ);
	//m_retcode = SQLBindCol(m_hstmt, 13, SQL_C_SHORT, &dColType, 60, &cbColType);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO)
		return false;
	return true;
}

void CDBConnection::PrintInfo(int num)
{
	wprintf(L"%d: %6d %20s %3d %3d %3d %3d %f %f %f\n", 
		num + 1, dId, szName, dType, dLevel, dHp, dExp, dPosX, dPosY, dPosZ/*, dColPosX, dColPosY, dColPosZ, dColType*/);
}

bool CDBConnection::Login(TCHAR* Name, CUser& user)
{
	std::lock_guard<std::mutex> lock(user.m_p_lock);
	m_retcode = SQLExecDirect(m_hstmt, (SQLWCHAR*)L"SELECT user_id, user_name, user_type, user_level, user_hp, user_exp, user_posX, user_posY, user_posZ FROM [Animated_Shot].[dbo].[user_table]", SQL_NTS);
	//m_retcode = SQLExecDirect(m_hstmt, 
		//(SQLWCHAR*)L"SELECT user_id, user_name, user_type, user_level, user_hp, user_exp, user_posX, user_posY, user_posZ, user_collisionX, user_collisionY, user_collisionZ, user_collisiontype FROM user_table", SQL_NTS);
	if (m_retcode != SQL_SUCCESS && m_retcode != SQL_SUCCESS_WITH_INFO)
		return false;
	if (!BindCol())
		return false;

	while ((m_retcode = SQLFetch(m_hstmt)) != SQL_NO_DATA)
	{
		bool nameMatch = true;
		for (int i = 0; i < NAME_SIZE; ++i)
			if (szName[i] == ' ')
				szName[i] = '\0';

		if (std::_tcscmp(Name, szName) == 0)
		{
			user.m_eType = dType;
			user.m_iHp = dHp;
			user.m_iExp = dExp;
			user.x = dPosX;
			user.y = 314;
			user.z = dPosZ;
			user.m_iLevel = dLevel;
			SQLFreeStmt(m_hstmt, SQL_CLOSE);
			return true;
		}
	}
	SQLFreeStmt(m_hstmt, SQL_CLOSE);
	return false;
}