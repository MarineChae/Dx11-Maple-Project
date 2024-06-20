#include "Netstd.h"

#include "IOCPServer.h"
#include "Packet.h"

std::mutex m1;


bool AcceptIocp::ThreadRun()
{
	if (m_pServer == nullptr)return false;

	SOCKADDR_IN clientaddr;
	int addlen = sizeof(clientaddr);
	SOCKET clientsock = accept(m_pServer->GetNetWork().GetSocket(), (SOCKADDR*)&clientaddr, &addlen);
	if (clientsock == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if (iError != WSAEWOULDBLOCK)
		{
			
			return false;
		}
	}
	else
	{

		std::shared_ptr<User> user = std::make_shared<User>(clientsock, clientaddr);
 		user->bind(m_pServer->GetIocpModel().GetIocpHandle());
		user->Recv();

		for (int iSize = 0; iSize < MAX_USER_SIZE; ++iSize)
		{
			if (SessionMgr::GetInstance().ConnectUser(user))
			{
				break;
			}
			else
				printf("Client Connect Failed IP: %s Port:%d\n", inet_ntoa(user->GetUserAddr().sin_addr), ntohs(user->GetUserAddr().sin_port));

		}
		printf("Client Connect IP: %s Port:%d\n", inet_ntoa(user->GetUserAddr().sin_addr), ntohs(user->GetUserAddr().sin_port));

	}

	return true;
}

void IOCPServer::AddPacket(Packet& packet)
{
	m_BroadcastPacketPool.Add(packet);

}

void IOCPServer::ChatMsg(Packet& packet)
{
	m_BroadcastPacketPool.Add(packet);
}

int IOCPServer::SendPacket(User* pUser, Packet* packet)
{
	char* SendBuffer = packet->GetBufferPointer();
	pUser->GetSendBuffer().buf = SendBuffer;
	pUser->GetSendBuffer().len = packet->GetDataSize();

	MyOV* ov = new MyOV(MyOV::MODE_SEND);

	int iSendByte = 0;
	int iTotalByte = 0;
	DWORD dwSendByte;

	int iRet = WSASend(pUser->GetUserSock(), &pUser->GetSendBuffer(), 1, &dwSendByte, 0, (LPOVERLAPPED)ov, NULL);

	if (iRet == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			return -1;
		}
	}


	return packet->GetDataSize();
}

bool IOCPServer::Broadcasting(Packet* packet)
{
	for (auto& iterSend : SessionMgr::GetInstance().GetUserList())
	{
		if (iterSend->IsConnected() == false) continue;

		int iSendByte = SendPacket(iterSend.get(), packet);

		if (iSendByte == SOCKET_ERROR)
		{

			int err = GetLastError();
			ERRORMSG(L"BroadCasting");
			iterSend->SetConnect(false);
			continue;
		}
	}
	return true;
}

bool IOCPServer::Broadcasting(Packet* packet, std::shared_ptr<User> pUser)
{

	for (auto& iterSend : SessionMgr::GetInstance().GetUserList())
	{
		if (iterSend == nullptr) continue;

		if (iterSend->IsConnected() == false || iterSend == pUser) continue;

		int iSendByte = SendPacket(iterSend.get(), packet);

		if (iSendByte == SOCKET_ERROR)
		{

			int err = GetLastError();
			ERRORMSG(L"BroadCasting");
			iterSend->SetConnect(false);
			continue;
		}
	}

	return true;
}



bool IOCPServer::Init()
{
	WSADATA wsa;
	//winsock 버전을 기입하여 초기화 2.2버전을 사용
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	//초기화에 실패한 경우 프로그램 종료
	if (ret != 0)
	{

		return false;
	}

	// SOCKET SockTCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	// SOCKET SockUDP = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
	// Stream 은 무조건 TCP    DGRAM은 UDP
	// 굳이 뒤에 넣지 말고 그냥 0넣으면 알아서 적용
	m_NetworkBase.SetSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

	SOCKADDR_IN SockAddr;
	//전송 주소의 주소 패밀리, 항상 AF_INET
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = htons(INADDR_ANY);
	//IPv4 전송 주소를 포함하는 IN_ADDR 구조체 
	//INADDR_ANY - 자동으로 이 컴퓨터에 존재하는 랜카드 중 사용가능한 랜카드의 IP주소를 사용
	SockAddr.sin_port = htons(m_NetworkBase.GetPort());
	m_NetworkBase.SetSockAddr(SockAddr);



	//소켓에 주소를 할당
	ret = bind(m_NetworkBase.GetSocket(), (SOCKADDR*)&m_NetworkBase.GetSockAddr(), sizeof(m_NetworkBase.GetSockAddr()));
	if (ret == SOCKET_ERROR)
	{
		ERRORMSG(L"SOCKET BIND ERROR");
		return false;
	}

	//연결요청을 대기상태로
	//SOMAXCONN - 보류 중인 연결 큐의 최대 길이. SOMAXCONN으로 설정하면 소켓을 담당하는 기본 서비스 공급자가 백로그를 적절한 최대 값으로 설정
	ret = listen(m_NetworkBase.GetSocket(), SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		ERRORMSG(L"SOCKET LISTEN ERROR");
		return false;
	}

	std::cout << "Server Is Running!" << std::endl;

	m_iocpModel.Init();
	m_AcceptIocp.SetServer(this);

	MyThread::Create();



	return true;
}

bool IOCPServer::ThreadRun()
{
	for (auto& data : m_BroadcastPacketPool.GetPacketList())
	{
		if (!Broadcasting(data))
		{

		}
	}
	m_BroadcastPacketPool.GetPacketList().clear();

	for (std::vector<std::shared_ptr<User>>::iterator iterSend = SessionMgr::GetInstance().GetUserList().begin();
		iterSend != SessionMgr::GetInstance().GetUserList().end();)
	{
		if (*iterSend == nullptr) continue;
		std::shared_ptr<User> pUser = *iterSend;
		if (pUser->IsConnected() == false)
		{
			pUser->Close();
			iterSend = SessionMgr::GetInstance().GetUserList().erase(iterSend);
		}
		else
		{
			iterSend++;
		}

	}


	return true;
}
