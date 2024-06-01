#pragma once
#include "Session.h"

enum CHARACTER_TYPE { C_HEALER, C_DEALER, C_TANKER };
class CUser : public CSession
{
public:
	CUser();
	~CUser();

public:
	void send_login_fail_packet();
	void send_login_info_packet();
	void send_add_player_packet(int c_id, CUser& cl);
	void send_move_packet(int c_id, CUser& cl);

public:
	char				m_cName[NAME_SIZE];
	float				x, y, z;
	int					m_iHp;
	int					m_iExp;
	int					m_iLevel;
	S_STATE				m_eState;
	CHARACTER_TYPE		m_eType;
};

