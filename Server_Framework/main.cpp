#include "stdafx.h"
#include "GameServer.h"
#include "Session.h"
#include "Packet.h"
#include "DBConnection.h"

HANDLE g_hIocp;
std::array<CUser, MAX_USER> clients;
CEXP_Over ex_over;
SOCKET g_s_socket, g_c_socket;
CDBConnection* g_DB = new CDBConnection;

void worker(SOCKET server)
{
	while (true)
	{
		DWORD num_bytes;
		ULONG_PTR key;
		LPOVERLAPPED over = nullptr;
		BOOL ret = GetQueuedCompletionStatus(g_hIocp, &num_bytes, &key, &over, INFINITE);
		CEXP_Over* ex_over = reinterpret_cast<CEXP_Over*>(over);

		if (FALSE == ret)
		{
			if (ex_over->m_eop == C_ACCEPT)
			{
				std::cout << "Accept_Error";
				exit(-1);
			}
			else
			{
				std::cout << "GQCS Error on client " << key << "\n";
				for (auto& pl : clients) {
					{
						std::lock_guard<std::mutex> ll{ pl.m_s_lock };
						if (pl.m_eState != ST_INGAME)continue;
					}
					if (pl.m_cId == key)continue;
					pl.send_remove_packet(key, clients[key]);
				}
				CGameServer::DisConnect(static_cast<int>(key));
				if (ex_over->m_eop == C_SEND) delete ex_over;
				return;
			}
		}

		switch (ex_over->m_eop) {
		case C_ACCEPT:
		{
			int cl_id = 0;
			for (int i = 0; i < MAX_USER; ++i) {
				if (clients[i].m_eState == ST_FREE) {
					cl_id = i;
					break;
				}
			}
			if (cl_id != -1)
			{
				clients[cl_id].m_eState = ST_INGAME;
				clients[cl_id].x = 0;
				clients[cl_id].y = 0;
				clients[cl_id].z = 0;
				clients[cl_id].m_cId = cl_id;
				clients[cl_id].m_cName[0] = 0;
				clients[cl_id].m_iPrev_remain = 0;
				clients[cl_id].m_sClient = g_c_socket;
				CGameServer::RegisterSOCKETHANDLE(g_hIocp, g_c_socket, cl_id);
				clients[cl_id].do_recv();
				g_c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			}
			else
			{
				std::cout << "Max user exceeded.\n";
				closesocket(g_c_socket);
			}
			ZeroMemory(&ex_over->m_over, sizeof(OVERLAPPED));
			AcceptEx(server, g_c_socket, ex_over->m_cbuf,
				0, sizeof(SOCKADDR) + 16, sizeof(SOCKADDR) + 16, 0, &ex_over->m_over);
			break;
		}
		case C_RECV:
		{
			int remain_data = num_bytes + clients[key].m_iPrev_remain;
			char* p = ex_over->m_cbuf;
			while (remain_data > 0) {
				int packet_size = p[0];
				if (packet_size <= remain_data) {
					CPacket::process_packet(static_cast<int>(key), 
						p, clients, g_DB);
					p = p + packet_size;
					remain_data = remain_data - packet_size;
				}
				else break;
			}
			clients[key].m_iPrev_remain = remain_data;
			if (remain_data > 0)
				memcpy(ex_over->m_cbuf, p, remain_data);	//¶¯±â±â
			clients[key].do_recv();
			break;
		}
		case C_SEND:
		{
			delete ex_over;
			break;
		}
		}
	}
}

int main()
{
	//if (g_DB->Connect()) {
	//	g_DB->BindCol();
	//	g_DB->Fetch();
	//}
	CGameServer::Init();
	SOCKET sv_socket = CGameServer::CreateSocket();
	CGameServer::BindAnyAddress(sv_socket);
	CGameServer::Listen(sv_socket);

	g_hIocp = CGameServer::CreateHandle();
	CGameServer::RegisterSOCKETHANDLE(g_hIocp, sv_socket, 9999);
	//CreateIoCompletionPort(reinterpret_cast<HANDLE>(sv_socket), g_hIocp, 9999, 0);

	g_c_socket = CGameServer::CreateSocket();
	ex_over.m_eop = C_ACCEPT;
	int res = AcceptEx(sv_socket, g_c_socket, ex_over.m_cbuf,
		0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		nullptr, &ex_over.m_over);

	std::vector<std::thread>worker_threads;
	int num_threads = std::thread::hardware_concurrency();
	for (int i = 0; i < num_threads; ++i)
		worker_threads.emplace_back(worker, sv_socket);

	for (auto& w_th : worker_threads)
		w_th.join();

	g_DB->Clear();
	CGameServer::Clear();
}