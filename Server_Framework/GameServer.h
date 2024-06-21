#pragma once
#include "stdafx.h"
#include "EXP_Over.h"
#include "DBConnection.h"

class CGameServer
{
private:
	CGameServer() {};
	~CGameServer() {};
public:
	static void Init();
	static void Clear();

	static void Print_Error(const char* msg, int err_no);

	static SOCKET CreateSocket();
	static void BindAnyAddress(SOCKET socket);
	static void Listen(SOCKET socket);
	/*static void Accept(SOCKET& socket);*/

	static void DisConnect(int id);
	static void CloseSocket(SOCKET& socket);

	static HANDLE CreateHandle();
	static void RegisterSOCKETHANDLE(HANDLE& iocp, SOCKET& socket, int id);
	
	static bool DBInit(CDBConnection* db);
	
	//setsocketopt - X
	bool SetRecvBufferSize(SOCKET socket, int size);
	bool SetSendBufferSize(SOCKET socket, int size);
	bool SetTcpNoDelay(SOCKET socket, bool flag);

	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listensocket);
	
};