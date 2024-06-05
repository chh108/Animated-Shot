#include "User.h"

CUser::CUser()
{
	m_cName[0] = 0;
	x = y = z = 0;
	m_eState = ST_FREE;
	m_eType = C_DEALER;
}

CUser::~CUser()
{
}

void CUser::send_login_fail_packet()
{
	SC_LOGIN_FAIL_PACKET p;
	p.size = sizeof(SC_LOGIN_FAIL_PACKET);
	p.type = SC_LOGIN_SUCESS_INFO;
	do_send(&p);
}

void CUser::send_login_info_packet()
{
	SC_LOGIN_SUCESS_INFO_PACKET p;
	p.size = sizeof(SC_LOGIN_SUCESS_INFO_PACKET);
	p.type = SC_LOGIN_SUCESS_INFO;
	p.id = m_cId;
	p.chartype = m_cId + 1;
	p.x = x;
	p.y = y;
	p.z = z;
	p.level = m_iLevel;
	p.hp = m_iHp;
	p.exp = m_iExp;
	do_send(&p);
}

void CUser::send_add_player_packet(int c_id, CUser& cl)
{
	SC_ADD_PLAYER_PACKET add_packet;
	add_packet.id = cl.m_cId;
	strcpy_s(add_packet.name, m_cName);
	add_packet.type = SC_ADD_PLAYER;
	add_packet.chartype = cl.m_cId + 1;
	{
		std::lock_guard<std::mutex> ll{ cl.m_p_lock };
		add_packet.x = cl.x;
		add_packet.y = cl.y;
		add_packet.z = cl.z;
	}
	add_packet.hp = cl.m_iHp;
	add_packet.level = cl.m_iLevel;
	add_packet.size = sizeof(add_packet);
	do_send(&add_packet);
}

void CUser::send_move_packet(int c_id, CUser& cl)
{
	SC_MOVE_PLAYER_PACKET p;
	p.id = cl.m_cId;
	p.type = SC_MOVE_PLAYER;
	{
		std::lock_guard<std::mutex> ll{ cl.m_p_lock };
		p.x = cl.x;
		p.y = cl.y;
		p.z = cl.z;
	}
	p.size = sizeof(p);
	do_send(&p);
}

void CUser::send_camera_packet(int c_id, CUser& cl)
{
	SC_CAMERA_PLAYER_PACKET p;
	p.id = cl.m_cId;
	p.type = SC_CAMERA_PLAYER;

	p.f3Right = cl.m_f3Right;
	p.f3Look = cl.m_f3Look;
	p.f3Up = cl.m_f3Up;
	p.size = sizeof(p);

	do_send(&p);
}

void CUser::send_animation_packet(int c_id, CUser& cl)
{
	SC_ANIMATION_PLAYER_PACKET p;
	p.id = cl.m_cId;
	p.type = SC_ANIMATION_PLAYER;

	p.animation = cl.m_animation;
	p.size = sizeof(p);

	do_send(&p);
}

void CUser::send_remove_packet(int c_id, CUser& cl)
{
	SC_REMOVE_PLAYER_PACKET p;
	p.id = cl.m_cId;
	p.type = SC_REMOVE_PLAYER;
	p.size = sizeof(p);
	do_send(&p);
}