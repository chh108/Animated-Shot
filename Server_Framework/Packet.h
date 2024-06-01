#pragma once
#include <unordered_map>
#include "User.h"
#include "DBConnection.h"
#include "stdafx.h"

class CPacket
{
public:
	CPacket() {};
	~CPacket() {}

public:
	static void process_packet(int c_id, char* packet, std::array<CUser, MAX_USER>& clients, CDBConnection* DB);
};