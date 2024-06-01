#include "Netstd.h"

#include "IOCPServer.h"


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

		User* user = new User(clientsock, clientaddr);
		user->bind(m_pServer->GetIocpModel().GetIocpHandle());
		user->Recv();
		m_pServer->GetNetWork().GetSessionMgr().GetUserList().push_back(user);
		printf("Client Connect IP: %s Port:%d\n", inet_ntoa(user->GetUserAddr().sin_addr), ntohs(user->GetUserAddr().sin_port));

	}

	return true;
}

void IOCPServer::AddPacket(UserPacket& packet)
{


}

void IOCPServer::ChatMsg(UserPacket& packet)
{
	m_BroadcastPacketPool.Add(packet);
}

int IOCPServer::SendPacket(User* pUser, UserPacket& packet)
{
	char* SendBuffer = (char*)&packet;
	pUser->GetSendBuffer().buf = SendBuffer;
	pUser->GetSendBuffer().len = packet.packet.PacketHeader.len;

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


	return packet.packet.PacketHeader.len;
}

bool IOCPServer::Broadcasting(UserPacket packet)
{
	for (auto& iterSend : m_NetworkBase.GetSessionMgr().GetUserList())
	{
		if (iterSend->IsConnected() == false) continue;

		int iSendByte = SendPacket(iterSend, packet);
		if (iSendByte == SOCKET_ERROR)
		{
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
		if (!Broadcasting(*data))
		{

		}

	}
	m_BroadcastPacketPool.GetPacketList().clear();
	for (auto& iterSend : m_NetworkBase.GetSessionMgr().GetUserList())
	{
		if (iterSend->IsConnected() == false) continue;

		for (auto& data : iterSend->GetPacketList())
		{
			if (!SendPacket(iterSend, data))
			{
				iterSend->SetConnect(false);
			}
		}
		iterSend->GetPacketList().clear();

	}



	for (auto iter = m_NetworkBase.GetSessionMgr().GetUserList().begin(); iter != m_NetworkBase.GetSessionMgr().GetUserList().end();)
	{
		auto user = *iter;
		if (!user->IsConnected())
		{
			printf("Client Disconnect IP: %s Port:%d\n", inet_ntoa(user->GetUserAddr().sin_addr), ntohs(user->GetUserAddr().sin_port));
			user->Close();
			m_NetworkBase.GetSessionMgr().GetUserList().erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return true;
}
