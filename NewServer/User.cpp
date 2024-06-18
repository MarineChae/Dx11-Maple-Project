#include "Pch.h"
#include "User.h"
#include"StreamPacket.h"
#include"IocpServer.h"
#include"Packet.h"
void User::Close()
{
	m_bConnected = false;
	closesocket(m_Sock);
}
void User::bind(HANDLE iocp)
{
	m_bConnected = true;
	HANDLE se = CreateIoCompletionPort((HANDLE)m_Sock, iocp, (ULONG_PTR)this, 0);
}
void User::Recv()
{
	DWORD dwTransfer;
	DWORD dwFlag = 0;
	MyOV* myov = new MyOV(MyOV::MODE_RECV);
	m_wsaRecvBuffer.buf = m_buffer;
	m_wsaRecvBuffer.len = sizeof(m_buffer);
	int iret = WSARecv(m_Sock, &m_wsaRecvBuffer, 1, &dwTransfer, &dwFlag, (LPOVERLAPPED)myov, NULL);
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
		PACKET_HEADER hd;
		ParseStreamPacket(pack, hd);
		IocpServer::GetInstance().AddPacket(pack,hd.PacketType);

	}
	if (myov->flag == MyOV::MODE_SEND)
	{
		int a = 0;
	}
	
	return;
}

bool User::ParseStreamPacket(Packet& pack, PACKET_HEADER& hd)
{

	BYTE end;
	m_pStreamPacket->Peek((char*)&hd, PACKET_HEADER_SIZE);
	m_pStreamPacket->RemoveData(PACKET_HEADER_SIZE);
	OutputDebugString(L"send\n");
	m_pStreamPacket->Get(pack.GetBufferPointer(), hd.PacketSize);
	m_pStreamPacket->Get((char*)&end, 1);

	pack.MoveWritePos(hd.PacketSize);
	pack.SetPacketType(hd.PacketType);
	return true;
}


User::User()
	: m_Sock()
	, m_Addr()
	, m_pStreamPacket()
	, m_bConnected(false)
	, m_buffer()
	, m_wsaRecvBuffer()
	, m_wsaSendBuffer()
{
	m_pStreamPacket = std::make_shared<StreamPacket>();
}

User::User(SOCKET sock, SOCKADDR_IN Addr)
	: m_Sock(sock)
	, m_Addr(Addr)
	, m_pStreamPacket()
	, m_bConnected(false)
	, m_buffer()
	, m_wsaRecvBuffer()
	, m_wsaSendBuffer()

{
	m_pStreamPacket = std::make_shared<StreamPacket>();
}

