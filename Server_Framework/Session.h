#pragma once
#include "EXP_Over.h"

enum S_STATE { ST_FREE, ST_ALLOC, ST_INGAME };
class CSession
{
public:
	CSession();
	~CSession();

public:
	void do_recv();
	void do_send(void* packet);

public:
	SOCKET				m_sClient;
	int					m_iPrev_remain;
	CEXP_Over			m_pExp_over;
	int					m_cId;
	std::mutex			m_s_lock;
	std::mutex			m_p_lock;
};