#include "DBConnectionPool.h"

CDBConnectionPool::CDBConnectionPool()
{

}

CDBConnectionPool::~CDBConnectionPool()
{
	Clear();
}

bool CDBConnectionPool::Connect(int connectionCount, const WCHAR* connectionString)
{

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
		return false;

	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3),0) != SQL_SUCCESS)
		return false;

	for (int i = 0; i < connectionCount; i++)
	{
		CDBConnection* connection = new CDBConnection;
		if (connection->Connect(m_hEnv, connectionString) == false)
			return false;

		m_vConnects.push_back(connection);
	}

	return true;
}

void CDBConnectionPool::Clear()
{

	if (m_hEnv != SQL_NULL_HANDLE)
	{
		::SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = SQL_NULL_HANDLE;
	}

	for (CDBConnection* connection : m_vConnects)
		delete(connection);

	m_vConnects.clear();
}

CDBConnection* CDBConnectionPool::Pop()
{

	if (m_vConnects.empty())
		return nullptr;

	CDBConnection* connection = m_vConnects.back();
	m_vConnects.pop_back();
	return connection;
}

void CDBConnectionPool::Push(CDBConnection* connection)
{
	m_vConnects.push_back(connection);
}
