#include "Netstd.h"
#include "IOCPServer.h"
#include "Packet.h"
#include "Timer.h"
#include"PlayerData.h"
#include"ObjectPool.h"
#include"ServerScene.h"
#include"PacketPool.h"
std::mutex m1;
std::mutex broadMutex;

bool AcceptIocp::ThreadRun()
{
	if (m_pServer == nullptr)return false;
	std::shared_ptr<Packet> pack = std::make_shared<Packet>();
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



void IOCPServer::AddPacket(std::shared_ptr<Packet> packet, int currentScene)
{

	m_BroadcastPacketPool.Add(packet, currentScene);

}

void IOCPServer::ChatMsg(Packet& packet)
{
	
}

int IOCPServer::SendPacket(User* pUser, std::shared_ptr<Packet> packet)
{
	if (pUser == nullptr)
		return -1;
	char* SendBuffer = packet->GetBufferPointer();
	pUser->GetSendBuffer().buf = SendBuffer;
	pUser->GetSendBuffer().len = packet->GetDataSize();

	//std::shared_ptr<MyOV> ov = std::make_shared<MyOV>(MyOV::MODE_SEND);//new MyOV(MyOV::MODE_SEND);
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

bool IOCPServer::Broadcasting(std::shared_ptr<Packet> packet)
{
	for (auto& iterSend : SessionMgr::GetInstance().GetUserList())
	{
		if (iterSend == nullptr) continue;
		if (iterSend->IsConnected() == false) continue;
		if (PlayerDataMgr::GetInstance().GetPlayerData(iterSend->GetSessionId()) == nullptr) continue;


		int curScene = PlayerDataMgr::GetInstance().GetPlayerData(iterSend->GetSessionId())->GetCurrentScene();


		int iSendByte = SendPacket(iterSend.get(), packet);


		if (iSendByte == SOCKET_ERROR)
		{

			int err = GetLastError();
			//ERRORMSG(L"BroadCasting");
			//iterSend->SetConnect(false);
			continue;
		}
	}
	return true;
}

bool IOCPServer::Broadcasting(std::pair<std::shared_ptr<Packet>,int> packet)
{
	for (auto& iterSend : SessionMgr::GetInstance().GetUserList())
	{
		if (iterSend == nullptr) continue;
		if (iterSend->IsConnected() == false) continue;
		if (PlayerDataMgr::GetInstance().GetPlayerData(iterSend->GetSessionId()) == nullptr) continue;


		int curScene = PlayerDataMgr::GetInstance().GetPlayerData(iterSend->GetSessionId())->GetCurrentScene();

		if (packet.second != curScene && packet.second != -1)
			continue;

 		int iSendByte = SendPacket(iterSend.get(), packet.first);


		if (iSendByte == SOCKET_ERROR)
		{ 

			int err = GetLastError();
			//ERRORMSG(L"BroadCasting");
			//iterSend->SetConnect(false);
			continue;
		}
	}
	return true;
}

bool IOCPServer::Broadcasting(std::pair<std::shared_ptr<Packet>, int> packet, std::shared_ptr<User> pUser)
{

	for (auto& iterSend : SessionMgr::GetInstance().GetUserList())
	{
		if (iterSend == nullptr) continue;

		if (iterSend->IsConnected() == false || iterSend == pUser) continue;

		int curScene = PlayerDataMgr::GetInstance().GetPlayerData(iterSend->GetSessionId())->GetCurrentScene();

		if (packet.second != curScene && packet.second != -1)
			continue;

		int iSendByte = SendPacket(iterSend.get(), packet.first);

		if (iSendByte == SOCKET_ERROR)
		{

			int err = GetLastError();
			//ERRORMSG(L"BroadCasting");
			//iterSend->SetConnect(false);
			continue;
		}
	}

	return true;
}
bool IOCPServer::Broadcasting(std::shared_ptr<Packet>packet , std::shared_ptr<User> pUser)
{

	for (auto& iterSend : SessionMgr::GetInstance().GetUserList())
	{
		if (iterSend == nullptr) continue;

		if (iterSend->IsConnected() == false || iterSend == pUser) continue;

		int curScene = PlayerDataMgr::GetInstance().GetPlayerData(iterSend->GetSessionId())->GetCurrentScene();

		int iSendByte = SendPacket(iterSend.get(), packet);

		if (iSendByte == SOCKET_ERROR)
		{

			int err = GetLastError();
			//ERRORMSG(L"BroadCasting");
			//iterSend->SetConnect(false);
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
		//ERRORMSG(L"SOCKET BIND ERROR");
		return false;
	}

	//연결요청을 대기상태로
	//SOMAXCONN - 보류 중인 연결 큐의 최대 길이. SOMAXCONN으로 설정하면 소켓을 담당하는 기본 서비스 공급자가 백로그를 적절한 최대 값으로 설정
	ret = listen(m_NetworkBase.GetSocket(), SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		//ERRORMSG(L"SOCKET LISTEN ERROR");
		return false;
	}

	std::cout << "Server Is Running!" << std::endl;

	m_iocpModel.Init();
	m_AcceptIocp.SetServer(this);
	m_BroadCastEvent = CreateEvent(0, TRUE, FALSE, 0);
	MyThread::Create();



	return true;
}

bool IOCPServer::ThreadRun()
{
	Timer::GetInstance().Frame();
	static double threadtimer=0;
	threadtimer += Timer::GetInstance().GetSecPerFrame();
	if (threadtimer <= 0.0625)
	{

		return true;
	}

	threadtimer = 0;

	for (auto& scene : ServerSceneMgr::GetInstance().GetSceneList())
	{
		scene.second->Update();
	}


	for (auto& data : m_BroadcastPacketPool.GetPacketList())
	{
		
		if (!Broadcasting(data))
		{
			
		}

		//OutputDebugString(L"send\n");
	}

	m_BroadcastPacketPool.GetPacketList().clear();

	for (std::vector<std::shared_ptr<User>>::iterator iterSend = SessionMgr::GetInstance().GetUserList().begin();
		iterSend != SessionMgr::GetInstance().GetUserList().end();)
	{
		if (*iterSend == nullptr)
		{
			iterSend++;
			continue;
		}
		std::shared_ptr<User> pUser = *iterSend;
		if (iterSend->get()->IsConnected() == false)
		{
			iterSend->get()->Close();
			iterSend = SessionMgr::GetInstance().GetUserList().erase(iterSend);
		}
		else
		{
			iterSend++;
		}

	}


	return true;
}
