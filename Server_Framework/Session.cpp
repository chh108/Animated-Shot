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
	m_pExp_over.m_wsabuf.len = BUF_SIZE - m_iPrev_remain;	//���ۻ����ŭ? ó���ϰ� ���� ��Ŷ ������ ���ۿ� ��������� �װ� ���� �޾ƾߵ�
	m_pExp_over.m_wsabuf.buf = m_pExp_over.m_cbuf + m_iPrev_remain; //���� ����� ������ �߸� ���� �ں��� �޾ƾߵ�
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
	CEXP_Over* data = new CEXP_Over{ reinterpret_cast<unsigned char*>(packet) };	//��Ŷ�� ���缭 ����
	WSASend(m_sClient, &data->Get_Wsabuf(), 1, 0, 0, data->GetOverlapped(), 0);
}