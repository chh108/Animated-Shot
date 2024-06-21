#include "Monster.h"
#include "GameServer.h"

CMonster::CMonster()
{
	m_eState = M_IDLE;
}

CMonster::~CMonster()
{
}

bool CMonster::m_bMPCollision()
{
	return false;
}

bool CMonster::m_bMMCollision()
{
	return false;
}

bool CMonster::m_bOMCollision()
{
	return false;
}

bool CMonster::Attack()
{
	//switch (m_eAttack)
	//{
	//case ATTACK_ON_SIGHT:
	//	if (/*CGameServer::can_see()*/)
	//	{
	//		if (m_eState != M_ATTACK)
	//		{
	//			m_eState = M_ATTACK;
	//			return true;
	//		}
	//		return false;
	//	}
	//	else
	//	{
	//		if (m_eState != M_IDLE)
	//		{
	//			m_eState = M_IDLE;
	//			return true;
	//		}
	//		return false;
	//	}
	//	break;
	//case MID_LEVEL_DEFEND:		//맨 마지막 구현

	//	break;
	//case RETALIATE_WHEN_ATTACK:
	//	if (/*공격 충돌이 있을시*/)
	//		break;
	//}
	//return 0;
}
