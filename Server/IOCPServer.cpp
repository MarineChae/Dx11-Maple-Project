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
	//winsock ������ �����Ͽ� �ʱ�ȭ 2.2������ ���
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	//�ʱ�ȭ�� ������ ��� ���α׷� ����
	if (ret != 0)
	{

		return false;
	}

	// SOCKET SockTCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	// SOCKET SockUDP = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
	// Stream �� ������ TCP    DGRAM�� UDP
	// ���� �ڿ� ���� ���� �׳� 0������ �˾Ƽ� ����
	m_NetworkBase.SetSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

	SOCKADDR_IN SockAddr;
	//���� �ּ��� �ּ� �йи�, �׻� AF_INET
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = htons(INADDR_ANY);
	//IPv4 ���� �ּҸ� �����ϴ� IN_ADDR ����ü 
	//INADDR_ANY - �ڵ����� �� ��ǻ�Ϳ� �����ϴ� ��ī�� �� ��밡���� ��ī���� IP�ּҸ� ���
	SockAddr.sin_port = htons(m_NetworkBase.GetPort());
	m_NetworkBase.SetSockAddr(SockAddr);



	//���Ͽ� �ּҸ� �Ҵ�
	ret = bind(m_NetworkBase.GetSocket(), (SOCKADDR*)&m_NetworkBase.GetSockAddr(), sizeof(m_NetworkBase.GetSockAddr()));
	if (ret == SOCKET_ERROR)
	{
		ERRORMSG(L"SOCKET BIND ERROR");
		return false;
	}

	//�����û�� �����·�
	//SOMAXCONN - ���� ���� ���� ť�� �ִ� ����. SOMAXCONN���� �����ϸ� ������ ����ϴ� �⺻ ���� �����ڰ� ��α׸� ������ �ִ� ������ ����
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
