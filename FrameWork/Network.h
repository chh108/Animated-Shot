#pragma once
#include "stdafx.h"
#include "N_Header.h"


class CNetwork
{
private:
	SOCKET m_clsocket;
	SOCKADDR_IN serverAddr;
	TCHAR m_ID[NAME_SIZE];

	bool m_bLoginFail = false;
	bool m_bLoginPacket = false;

	bool m_bAddParty1 = false;
	bool m_bAddParty2 = false;
public:
	static CNetwork* Get_Instance(void) {
		if (!m_pInstance) 
		{
			m_pInstance = new CNetwork();
		}
		return m_pInstance;
	}
	static	void	Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void SetSocket();
	void SocketInit();
	void ConnectSocket();

	void SetSendPacket(int packet_id);

	void RecvPacket();
	void ProcessPacket(char* packet);
public:
	void SetUserID(TCHAR ID[]) { wcscpy_s(m_ID, NAME_SIZE, ID); };
	TCHAR* GetUserID() { return m_ID; };

	void SetLoginPacket(bool LoginPacket) { m_bLoginPacket = LoginPacket; };
	bool GetLoginPacket() { return  m_bLoginPacket; };

	void SetLoginFail(bool LoginFail) { m_bLoginFail = LoginFail; };
	bool GetLoginFail() { return m_bLoginFail; };

	void SetAddParty1Packet(bool AddParty1) { m_bAddParty1 = AddParty1; };
	bool GetAddParty1Packet() { return m_bAddParty1; };

	void SetAddParty2Packet(bool AddParty2) { m_bAddParty2 = AddParty2; };
	bool GetAddParty2Packet() { return m_bAddParty2; };

private:
	static CNetwork* m_pInstance;
};
