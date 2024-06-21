#pragma once
#include "Session.h"

enum M_State { M_IDLE, M_ATTACK, M_DEFEND };

enum M_Attack {
	ATTACK_ON_SIGHT,		 // 몬스터가 플레이어를 발견했을 때 공격
	MID_LEVEL_DEFEND,		// 중급 몬스터가 공격받았을 때 하급 몬스터가 방어/지원
	RETALIATE_WHEN_ATTACK	//맞았을때
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