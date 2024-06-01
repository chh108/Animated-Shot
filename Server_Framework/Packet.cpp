#include "Packet.h"
#include "DBConnection.h"

XMFLOAT3 Multiply(XMFLOAT3 xmfloat3, float value)
{
	return XMFLOAT3(xmfloat3.x * value, xmfloat3.y * value, xmfloat3.z * value);
}

XMFLOAT3 Add(XMFLOAT3 xmfloat1, XMFLOAT3 xmfloat2)
{
	return XMFLOAT3(xmfloat1.x + xmfloat2.x, xmfloat1.y + xmfloat2.y, xmfloat1.z + xmfloat2.z);
}

void CPacket::process_packet(int c_id, char* packet, std::array<CUser, MAX_USER>& clients, CDBConnection* DB)
{
	switch (packet[1]) {
	case CS_LOGIN:
	{
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		std::wcout << p->name << std::endl;
		//if (DB->Login(p->name, clients[c_id]))
		{
			std::cout << "로그인 성공" << std::endl;
			/*strcpy_s(clients[c_id].m_cName, p->name);*/
			clients[c_id].send_login_info_packet();
			std::this_thread::sleep_for(std::chrono::seconds(1));
			{
				std::lock_guard<std::mutex> ll{ clients[c_id].m_s_lock };
				clients[c_id].m_eState = ST_INGAME;
			}
			for (auto& pl : clients) {
				{
					std::lock_guard<std::mutex> ll{ pl.m_s_lock };
					if (ST_INGAME != pl.m_eState) continue;
				}
				if (pl.m_cId == c_id) continue;
				pl.send_add_player_packet(c_id, clients[c_id]);
				clients[c_id].send_add_player_packet(pl.m_cId, pl);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		/*else
		{
			std::cout << "로그인 실패" << std::endl;
			clients[c_id].send_login_fail_packet();
		}*/
		break;
	}
	case CS_MOVE:
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);

		std::lock_guard<std::mutex> ll{ clients[c_id].m_p_lock };
		if (p->direction)
		{
			float SetSpeed = 0.3f;
			float fDistance = 22.5f;

			XMFLOAT3  xmf3Look = XMFLOAT3(p->f3Look[0], p->f3Look[1], p->f3Look[2]);
			XMFLOAT3  xmf3Up = XMFLOAT3(p->f3Up[0], p->f3Up[1], p->f3Up[2]);
			XMFLOAT3  xmf3Right = XMFLOAT3(p->f3Right[0], p->f3Right[1], p->f3Right[2]);


			XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
			if (p->direction & DIR_FORWARD) xmf3Shift = Add(xmf3Shift, Multiply(xmf3Look, +fDistance));
			if (p->direction & DIR_BACKWARD)xmf3Shift = Add(xmf3Shift, Multiply(xmf3Look, -fDistance));
#ifdef _WITH_LEFT_HAND_COORDINATES
			if (p->direction & DIR_RIGHT) xmf3Shift = xmf3Shift + p->m_xmf3Right * +fDistance;
			if (p->direction & DIR_LEFT) xmf3Shift = xmf3Shift + p->m_xmf3Right * -fDistance;
#else
			if (p->direction & DIR_RIGHT) xmf3Shift = Add(xmf3Shift, Multiply(xmf3Right, -fDistance));
			if (p->direction & DIR_LEFT) xmf3Shift = Add(xmf3Shift, Multiply(xmf3Right, +fDistance));
#endif
			if (p->direction & DIR_UP) xmf3Shift = Add(xmf3Shift, Multiply(xmf3Up, +fDistance));
			if (p->direction & DIR_DOWN) xmf3Shift = Add(xmf3Shift, Multiply(xmf3Up, -fDistance));

			xmf3Shift = Multiply(xmf3Shift, SetSpeed);

			clients[c_id].x += xmf3Shift.x;
			clients[c_id].y += xmf3Shift.y;
			clients[c_id].z += xmf3Shift.z;
		}

		std::wcout << c_id << ", " <<
			p->f3Look[0] << ", " <<
			p->f3Look[1] << ", " <<
			p->f3Look[2] << std::endl;

		for (auto& pl : clients) {
			{
				std::lock_guard<std::mutex> ll{ pl.m_s_lock };
				if (pl.m_eState != ST_INGAME)continue;
			}
			if (pl.m_cId == c_id)continue;
			pl.send_move_packet(c_id, clients[c_id]);
		}
		break;
	}
}