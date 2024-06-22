#include "stdafx.h"
#include "Network.h"
#include "PlayerManager.h"
#include "PartyManager.h"

CNetwork* CNetwork::m_pInstance = nullptr;

void CNetwork::SetSocket()
{
	m_clsocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, 0);
}

void CNetwork::SocketInit()
{
	unsigned long noblock = 1;
	int nRet = ioctlsocket(m_clsocket, FIONBIO, &noblock);

	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(PORT_NUM);
}

void CNetwork::ConnectSocket()
{
	while (true)
	{
		if (::connect(m_clsocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			if (::WSAGetLastError() == WSAEISCONN)
				break;
			break;
		} 
	}
}

void CNetwork::SetSendPacket(int packet_id)
{
	WSABUF wsabuf;
	DWORD sent_size;
	switch (packet_id) {
	case CS_LOGIN:
		CS_LOGIN_PACKET login_packet;
		login_packet.type = CS_LOGIN;
		login_packet.size = sizeof(login_packet);
		wcscpy_s(login_packet.name, m_ID);
		wsabuf.buf = reinterpret_cast<char*>(&login_packet);
		wsabuf.len = login_packet.size;

		while (true) {
			if (WSASend(m_clsocket, &wsabuf, 1, &sent_size, 0, 0, 0) == SOCKET_ERROR) {
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;
			}
			else
				break;
		}
		break;

	case CS_MOVE:
		CS_MOVE_PACKET move_packet;
		move_packet.type = CS_MOVE;
		move_packet.direction = CPlayerManager::Get_Instance()->Get_MoveKey();

		/*move_packet.f3Look[0] = CPlayerManager::Get_Instance()->Get_xmf3Look().x;
		move_packet.f3Look[1] = CPlayerManager::Get_Instance()->Get_xmf3Look().y;
		move_packet.f3Look[2] = CPlayerManager::Get_Instance()->Get_xmf3Look().z;

		move_packet.f3Up[0] = CPlayerManager::Get_Instance()->Get_xmf3Up().x;
		move_packet.f3Up[1] = CPlayerManager::Get_Instance()->Get_xmf3Up().y;
		move_packet.f3Up[2] = CPlayerManager::Get_Instance()->Get_xmf3Up().z;

		move_packet.f3Right[0] = CPlayerManager::Get_Instance()->Get_xmf3Right().x;
		move_packet.f3Right[1] = CPlayerManager::Get_Instance()->Get_xmf3Right().y;
		move_packet.f3Right[2] = CPlayerManager::Get_Instance()->Get_xmf3Right().z;*/

		move_packet.x = CPlayerManager::Get_Instance()->Get_Pos().x;
		move_packet.y = CPlayerManager::Get_Instance()->Get_Pos().y;
		move_packet.z = CPlayerManager::Get_Instance()->Get_Pos().z;

		move_packet.size = sizeof(move_packet);
		wsabuf.buf = reinterpret_cast<char*>(&move_packet);
		wsabuf.len = move_packet.size;

		while (true) {
			if (WSASend(m_clsocket, &wsabuf, 1, &sent_size, 0, 0, 0) == SOCKET_ERROR) {
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;
			}
			else
				break;
		}
		break;
	case CS_CAMERA:
		CS_CAMERA_PACKET camera_packet;
		camera_packet.type = CS_CAMERA;
		camera_packet.size = sizeof(camera_packet);

		camera_packet.f3Look = CPlayerManager::Get_Instance()->Get_xmf3Look();
		camera_packet.f3Right = CPlayerManager::Get_Instance()->Get_xmf3Right();
		camera_packet.f3Up = CPlayerManager::Get_Instance()->Get_xmf3Up();

		wsabuf.buf = reinterpret_cast<char*>(&camera_packet);
		wsabuf.len = camera_packet.size;

		while (true) {
			if (WSASend(m_clsocket, &wsabuf, 1, &sent_size, 0, 0, 0) == SOCKET_ERROR) {
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;
			}
			else
				break;
		}
		break;
	case CS_ANIMATION:
		CS_ANIMATION_PACKET animation_packet;
		animation_packet.type = CS_ANIMATION;
		animation_packet.size = sizeof(animation_packet);

		animation_packet.animation = CPlayerManager::Get_Instance()->Get_Animation();

		wsabuf.buf = reinterpret_cast<char*>(&animation_packet);
		wsabuf.len = animation_packet.size;

		while (true) {
			if (WSASend(m_clsocket, &wsabuf, 1, &sent_size, 0, 0, 0) == SOCKET_ERROR) {
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;
			}
			else
				break;
		}
		break;
	}
}

void CNetwork::RecvPacket()
{
	DWORD recv_flag = 0;
	DWORD recv_byte;
	WSABUF wsabuf;
	char send_buf[BUF_SIZE];
	wsabuf.len = BUF_SIZE;
	wsabuf.buf = send_buf;
	int ret = WSARecv(m_clsocket, &wsabuf, 1, &recv_byte, &recv_flag, 0, 0);

	if (ret == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK)
		{
			printf("WSARecv failed with error: %d\n", err);
			return;
		}
	}
	else
	{
		if (recv_byte > 0)
		{
			ProcessPacket(send_buf);
		}
	}
}


void CNetwork::ProcessPacket(char* packet)
{
	switch (packet[1]) {
	case SC_LOGIN_FAIL:
	{	
		m_bLoginPacket = true;
		m_bLoginFail = true;
		break;
	}

	case SC_LOGIN_SUCESS_INFO:		//내 정보 받기
	{
		SC_LOGIN_SUCESS_INFO_PACKET* p = reinterpret_cast<SC_LOGIN_SUCESS_INFO_PACKET*>(packet);
		CPlayerManager::Get_Instance()->Set_Pos(XMFLOAT3(p->x, p->y, p->z));
		CPlayerManager::Get_Instance()->Set_Type(p->chartype);
		CPlayerManager::Get_Instance()->Set_Login(true);
		m_bLoginPacket = true;
		break;
	}

	case SC_ADD_PLAYER:
	{
		SC_ADD_PLAYER_PACKET* p = reinterpret_cast<SC_ADD_PLAYER_PACKET*>(packet);
		if (CPartyManager::Get_Instance()->Get_Party1Type() == -1)
		{
			CPartyManager::Get_Instance()->Set_Party1Name(p->name);
			CPartyManager::Get_Instance()->Set_Party1Pos(XMFLOAT3(p->x, p->y, p->z));
			CPartyManager::Get_Instance()->Set_Party1Type(p->chartype);
			CPartyManager::Get_Instance()->Set_Party1Id(p->id);

			m_bAddParty1 = true;
			CPartyManager::Get_Instance()->Set_P1State(true);
		}
		else if (CPartyManager::Get_Instance()->Get_Party1Type() != -1)
		{
			CPartyManager::Get_Instance()->Set_Party2Name(p->name);
			CPartyManager::Get_Instance()->Set_Party2Pos(XMFLOAT3(p->x, p->y, p->z));
			CPartyManager::Get_Instance()->Set_Party2Type(p->chartype);
			CPartyManager::Get_Instance()->Set_Party2Id(p->id);

			m_bAddParty2 = true;
			CPartyManager::Get_Instance()->Set_P2State(true);
		}
		break;
	}

	case SC_MOVE_PLAYER:
	{
		SC_MOVE_PLAYER_PACKET* p = reinterpret_cast<SC_MOVE_PLAYER_PACKET*>(packet);
		if (p->id == CPartyManager::Get_Instance()->Get_Party1Id())
		{
			CPartyManager::Get_Instance()->Set_Party1Pos(XMFLOAT3(p->x, p->y, p->z));
		}
		else if (p->id == CPartyManager::Get_Instance()->Get_Party2Id())
		{
			CPartyManager::Get_Instance()->Set_Party2Pos(XMFLOAT3(p->x, p->y, p->z));
		}
		break;
	}
	case SC_MOVE_FAIL_PLAYER:
	{
		SC_MOVE_FAIL_PLAYER_PACKET* p = reinterpret_cast<SC_MOVE_FAIL_PLAYER_PACKET*>(packet);
		CPlayerManager::Get_Instance()->Set_Coll(true);
		break;
	}
	case SC_CAMERA_PLAYER:
	{
		SC_CAMERA_PLAYER_PACKET* p = reinterpret_cast<SC_CAMERA_PLAYER_PACKET*>(packet);
		if (p->id == CPartyManager::Get_Instance()->Get_Party1Id())
		{
			CPartyManager::Get_Instance()->Set_P1Look(p->f3Look);
			CPartyManager::Get_Instance()->Set_P1Right(p->f3Right);
			CPartyManager::Get_Instance()->Set_P1Up(p->f3Up);
		}
		else if (p->id == CPartyManager::Get_Instance()->Get_Party2Id())
		{
			CPartyManager::Get_Instance()->Set_P2Look(p->f3Look);
			CPartyManager::Get_Instance()->Set_P2Right(p->f3Right);
			CPartyManager::Get_Instance()->Set_P2Up(p->f3Up);
		}
		break;
	}

	case SC_ANIMATION_PLAYER:
	{
		SC_ANIMATION_PLAYER_PACKET* p = reinterpret_cast<SC_ANIMATION_PLAYER_PACKET*>(packet);
		if (p->id == CPartyManager::Get_Instance()->Get_Party1Id())
		{
			CPartyManager::Get_Instance()->Set_P1Animation((PLAYERSTATE)p->animation);
		}
		else if (p->id == CPartyManager::Get_Instance()->Get_Party2Id())
		{
			CPartyManager::Get_Instance()->Set_P2Animation((PLAYERSTATE)p->animation);
		}
		break;
	}

	case SC_REMOVE_PLAYER:
	{
		SC_REMOVE_PLAYER_PACKET* p = reinterpret_cast<SC_REMOVE_PLAYER_PACKET*>(packet);
		if (p->id == CPartyManager::Get_Instance()->Get_Party1Id())
		{
			CPartyManager::Get_Instance()->Set_P1State(false);
		}
		else if (p->id == CPartyManager::Get_Instance()->Get_Party2Id())
		{
			CPartyManager::Get_Instance()->Set_P2State(false);
		}
		break;
	}
	}
}