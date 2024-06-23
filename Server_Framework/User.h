#pragma once
#include "Session.h"
#include "stdafx.h"

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
	void send_fail_move_packet(int c_id, CUser& cl);
	void send_camera_packet(int c_id, CUser& cl);
	void send_animation_packet(int c_id, CUser& cl);
	void send_remove_packet(int c_id, CUser& cl);

	DirectX::XMFLOAT3 Compute_Min();
	DirectX::XMFLOAT3 Compute_Max();

	bool Collision_AABB(CUser* pTargetUser);
public:
	char				m_cName[NAME_SIZE];
	float				x, y, z;
	int					m_iHp;
	int					m_iExp;
	int					m_iLevel;
	S_STATE				m_eState;
	int					m_eType;
	int					m_animation;

	DirectX::XMFLOAT3	m_f3Right;
	DirectX::XMFLOAT3	m_f3Look;
	DirectX::XMFLOAT3	m_f3Up;
};