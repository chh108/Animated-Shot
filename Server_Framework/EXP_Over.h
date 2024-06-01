#pragma once
#include "stdafx.h"

enum C_OP { C_ACCEPT, C_RECV, C_SEND };

class CEXP_Over
{
public:
	CEXP_Over();
	CEXP_Over(unsigned char* packet);

	LPOVERLAPPED GetOverlapped() { return &m_over; };

	char* Get_Buf() { return m_cbuf; };
	
	WSABUF& Get_Wsabuf() { return m_wsabuf; };

public:
	WSAOVERLAPPED m_over;
	char m_cbuf[BUF_SIZE];
	C_OP m_eop;
	WSABUF m_wsabuf;
	SOCKET m_clsocket;

private:
};