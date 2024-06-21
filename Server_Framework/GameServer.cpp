#include "GameServer.h"

void CGameServer::Init()
{
	std::wcout.imbue(std::locale("korean"));	//오류 한글로 설정

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
}

void CGameServer::Clear()
{
	WSACleanup();
}

void CGameServer::Print_Error(const char* msg, int err_no)
{
	WCHAR* msg_buf = nullptr;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&msg_buf), 0, NULL);
	std::cout << msg;
	std::wcout << L": 에러 : " << msg_buf;
	while (true);
	LocalFree(msg_buf);
}

SOCKET CGameServer::CreateSocket()
{
	return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

void CGameServer::BindAnyAddress(SOCKET socket)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT_NUM);

	int res = bind(socket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	if (res != 0)
	{
		Print_Error("Bind", res);
	}
}

void CGameServer::Listen(SOCKET socket)
{
	int res = listen(socket, SOMAXCONN);
	if (res != 0)
	{
		Print_Error("Listen", res);
	}
}

void CGameServer::DisConnect(int id)
{
}

void CGameServer::CloseSocket(SOCKET& socket)
{
	if (socket != INVALID_SOCKET) 
	{
		closesocket(socket);
		socket = INVALID_SOCKET;
	}
}

HANDLE CGameServer::CreateHandle()
{
	return CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
}

void CGameServer::RegisterSOCKETHANDLE(HANDLE& iocp, SOCKET& socket, int id)
{
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), iocp, id, 0);
}

bool CGameServer::DBInit(CDBConnection* db)
{
	if (!db->Connect()) return false;
	if (!db->BindCol())return false;
	db->Fetch();
	return  true;

}

bool CGameServer::SetRecvBufferSize(SOCKET socket, int size)
{
	return false;
}

bool CGameServer::SetSendBufferSize(SOCKET socket, int size)
{
	return false;
}

bool CGameServer::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return false;
}

bool CGameServer::SetUpdateAcceptSocket(SOCKET socket, SOCKET listensocket)
{
	return setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, 
		reinterpret_cast<char*>(&listensocket),sizeof(SOCKET));
}
