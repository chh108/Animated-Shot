#include "Session.h"
#include "GameServer.h"

CSession::CSession()
{
	m_iPrev_remain = 0;
	m_sClient = INVALID_SOCKET;
	//m_pExp_over = nullptr;
	m_cId = -1;
}

CSession::~CSession()
{
	//delete m_pExp_over;
}

void CSession::do_recv()
{
	DWORD recv_flag = 0;
	ZeroMemory(&m_pExp_over.m_over, sizeof(OVERLAPPED));
	m_pExp_over.m_wsabuf.len = BUF_SIZE - m_iPrev_remain;	//버퍼사이즈만큼? 처리하고 남은 패킷 조각이 버퍼에 들어있으면 그거 빼고 받아야됨
	m_pExp_over.m_wsabuf.buf = m_pExp_over.m_cbuf + m_iPrev_remain; //남은 사이즈가 있으면 잘린 조각 뒤부터 받아야됨
	int res = WSARecv(m_sClient, &m_pExp_over.m_wsabuf, 1, 0, &recv_flag,
		&m_pExp_over.m_over, 0);
	if (0 != res) {
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			CGameServer::Print_Error("WSARecv", WSAGetLastError());
	}
}

void CSession::do_send(void* packet)
{
	CEXP_Over* data = new CEXP_Over{ reinterpret_cast<unsigned char*>(packet) };	//패킷에 맞춰서 생성
	WSASend(m_sClient, &data->Get_Wsabuf(), 1, 0, 0, data->GetOverlapped(), 0);
}