#include "Netstd.h"
#include "User.h"
#include "IOCPServer.h"
#include "MakePacket.h"
#include "PlayerData.h"
#include "Packet.h"
#include"PacketProc.h"
#include"StreamPacket.h"
std::mutex m;

int SessionMgr::m_iSessionCount = 0;

void User::Close()
{
	std::shared_ptr<Packet> pack = std::make_shared<Packet>();
	DisConnectCharacter(pack, m_dwSessionID);
	
	for (auto& otherplayer : PlayerDataMgr::GetInstance().GetPlayerList())
	{
		if (otherplayer == nullptr)
		{
			continue;
		}
		else if (otherplayer->GetSessionID() == m_dwSessionID)
		{
			PlayerDataMgr::GetInstance().DeletePlayerData(m_dwSessionID);
			break;
		}

	}

	IOCPServer::GetInstance().AddPacket(pack);
	m_bConnected = false;
	closesocket(m_UserSock);

}

void User::bind(HANDLE iocp)
{
	m_bConnected = true;
	//기존의 iocp서버에 연동
	HANDLE io = CreateIoCompletionPort((HANDLE)m_UserSock, iocp, (ULONG_PTR)this, 0);
}

void User::Recv()
{

	DWORD dwTransfer;
	DWORD dwFlag = 0;
	MyOV* myov = new MyOV(MyOV::MODE_RECV);
	m_wsaRecvBuffer.buf = m_buffer;
	m_wsaRecvBuffer.len = sizeof(m_buffer);
	int iret = WSARecv(m_UserSock, &m_wsaRecvBuffer, 1, &dwTransfer, &dwFlag, (LPOVERLAPPED)myov, NULL);
	if (iret == SOCKET_ERROR)
	{
		int i = WSAGetLastError();
		int a = i;
	}


 }

void User::Dispatch(DWORD dwTransfer, OVERLAPPED* ov)
{  
	MyOV* myov = (MyOV*)ov;

	if (myov->flag == MyOV::MODE_RECV)
	{
		Recv();
		if (dwTransfer == 0)
		{
			m_bConnected = false;
			return;
		}

		m_pStreamPacket->Put(m_buffer, dwTransfer);

		Packet pack;
		ParsePacket(pack);

	/*	Packet pack;

		
		BYTE end;
		m_StreamPacket.Peek((char*)&hd, PACKET_HEADER_SIZE);
		m_StreamPacket.RemoveData(PACKET_HEADER_SIZE);
		OutputDebugString(L"send\n");
		m_StreamPacket.Get(pack.GetBufferPointer(), hd.PacketSize);

		m_StreamPacket.Get((char*)&end, 1);

		pack.MoveWritePos(hd.PacketSize);

	

		Packet spack;


		MoveStopPacket(&spack, 1,1, 444, 444);

		IOCPServer::GetInstance().AddPacket(spack);*/
	}
	if (myov->flag == MyOV::MODE_SEND)
	{
		

	}
	
	
}

void User::ParsePacket(Packet& pack)
{
	PACKET_HEADER hd;
	BYTE end;
	m_pStreamPacket->Peek((char*)&hd, PACKET_HEADER_SIZE);
	m_pStreamPacket->RemoveData(PACKET_HEADER_SIZE);
	//OutputDebugString(L"send\n");
	m_pStreamPacket->Get(pack.GetBufferPointer(), hd.PacketSize);
				  
	m_pStreamPacket->Get((char*)&end, 1);

	pack.MoveWritePos(hd.PacketSize);


	
	PacketProc(m_dwSessionID, hd.PacketType, &pack);

}

BOOL User::PacketProc(DWORD SessionId, BYTE PacketType, Packet* pack)
{

	switch (PacketType)
	{
	case PACKET_CS_MOVE_START:
		return PacketProc_MoveStart(SessionId, pack);
		break;

	case PACKET_CS_MOVE_END:
		return PacketProc_MoveEnd(SessionId, pack);
		break;
	}

	return FALSE;
}

User::User() 
	:m_dwSessionID(0)
	,m_UserSock()
	, m_UserAddr()
	, m_bConnected(false)
	, m_buffer()
	, m_wsaRecvBuffer()
	, m_wsaSendBuffer()
	, m_pStreamPacket(std::make_shared<StreamPacket>())
{
	 
}

User::User(SOCKET sock, SOCKADDR_IN Addr)
	:m_dwSessionID(0)
	,m_UserSock(sock)
	,m_UserAddr(Addr)
	,m_bConnected(false)
	,m_buffer()
	,m_wsaRecvBuffer()
	,m_wsaSendBuffer()
	, m_pStreamPacket(std::make_shared<StreamPacket>())

{

}

bool SessionMgr::ConnectUser(std::shared_ptr<User> user)
{
	m.lock();
	if (m_vUserList.size() <= MAX_USER_SIZE)
	{
		m_vUserList[m_iSessionCount] = user;
		user->SetSessionId(m_iSessionCount++);

		short x = rand() % 500;
		short y = rand() % 500;

		std::shared_ptr<Packet> pack = std::make_shared<Packet>();
		CreateMyCharacter(pack, user->GetSessionId(),0,x,y,100);
		IOCPServer::GetInstance().SendPacket(user.get(), pack);


		std::shared_ptr<Packet> pack2 = std::make_shared<Packet>();
 		CreateOtherCharacter(pack2, user->GetSessionId(), 0, x, y, 100);
		IOCPServer::GetInstance().AddPacket(pack2);

		std::shared_ptr<PlayerData> data = std::make_shared<PlayerData>();
		data->Init(true, user->GetSessionId(),0,0,x,y,100);
 		PlayerDataMgr::GetInstance().PushPlayerData(user->GetSessionId(),data);

		
 		for (auto& otherplayer : PlayerDataMgr::GetInstance().GetPlayerList())
		{
			if (otherplayer == nullptr)
			{
				continue;
			}
			else if (otherplayer->GetSessionID() != user->GetSessionId())
			{
				std::shared_ptr<Packet> playerpack = std::make_shared<Packet>();
				CreateOtherCharacter(playerpack,
					otherplayer->GetSessionID(),
					otherplayer->GetDirection(),
					otherplayer->GetXPos(),
					otherplayer->GetYPos(),
					otherplayer->GetHP());

				OutputDebugString(L"other character");
				Sleep(50);
				IOCPServer::GetInstance().SendPacket(user.get(), playerpack);
			}

		}


		m.unlock();
		return true;
	}
	else
	{
		for (auto& useriter : m_vUserList)
		{
			if (!useriter->IsConnected())
			{
				useriter = user;
				m.unlock();
				return true;
			}
		}
	}
	m.unlock();
	return false;
}
