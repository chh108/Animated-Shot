#include "DBConnection.h"
#include "stdafx.h"
class CDBConnectionPool
{
public:
	CDBConnectionPool();
	~CDBConnectionPool();

public:
	bool Connect(int connectCount, const WCHAR* connectString);
	void Clear();
	CDBConnection* Pop();
	void Push(CDBConnection* connection);

private:
	SQLHENV m_hEnv = SQL_NULL_HANDLE;
	std::vector<CDBConnection*> m_vConnects;
};