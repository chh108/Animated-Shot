#pragma once
#include "Session.h"

enum M_State { M_IDLE, M_ATTACK, M_DEFEND };

enum M_Attack {
	ATTACK_ON_SIGHT,		 // ���Ͱ� �÷��̾ �߰����� �� ����
	MID_LEVEL_DEFEND,		// �߱� ���Ͱ� ���ݹ޾��� �� �ϱ� ���Ͱ� ���/����
	RETALIATE_WHEN_ATTACK	//�¾�����
};
class CMonster : public CSession
{
public:
	CMonster();
	~CMonster();

public:
	bool m_bMPCollision();
	bool m_bMMCollision();
	bool m_bOMCollision();

	bool can_see(CSession& player);
	bool Attack();

private:
	float x, y, z;
	int m_iHp;
	M_State m_eState;

	M_Attack m_eAttack;
	short m_sStage;
	short m_sDefend;
	short m_sAttack;
	short m_sSize;
};