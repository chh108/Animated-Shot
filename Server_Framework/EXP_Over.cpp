#include "EXP_Over.h"

CEXP_Over::CEXP_Over()
{
	m_wsabuf.len = BUF_SIZE;
	m_wsabuf.buf = m_cbuf;
	m_eop = C_RECV;
	ZeroMemory(&m_over, sizeof(m_over));
}

CEXP_Over::CEXP_Over(unsigned char* packet)
{
	m_wsabuf.len = packet[0];
	m_wsabuf.buf = m_cbuf;
	ZeroMemory(&m_over, sizeof(m_over));
	m_eop = C_SEND;
	memcpy(m_cbuf, packet, packet[0]);
}