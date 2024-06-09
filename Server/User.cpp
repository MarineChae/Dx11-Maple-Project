#include "Netstd.h"
#include "User.h"
#include "IOCPServer.h"
#include "Packet.h"
void User::Close()
{
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
		if (dwTransfer == 0)
		{
			m_bConnected = false;
			return;
		}

		m_StreamPacket.Put(m_buffer, dwTransfer);

		Packet pack;

		PACKET_HEADER hd;
		BYTE end;
		m_StreamPacket.Peek((char*)&hd,PACKET_HEADER_SIZE);
		m_StreamPacket.RemoveData(PACKET_HEADER_SIZE);

		m_StreamPacket.Get(pack.GetBufferPointer(), hd.PacketSize);

		m_StreamPacket.Get((char*)&end , 1);

		pack.MoveWritePos(hd.PacketSize);


		BYTE dir=0;
		short sx=0;
		short sy=0;

		pack >> dir;
		pack >> sx;
		pack >> sy;
		std::cout << (int)dir << " " << sx << " " << sy << " " << "\n";

	}
	if (myov->flag == MyOV::MODE_SEND)
	{


	}
	Recv();
	
}

User::User() 
	:m_UserSock()
	, m_UserAddr()
	, m_bConnected(true)
	, m_buffer()
	, m_wsaRecvBuffer()
	, m_wsaSendBuffer()
{
	 
}

User::User(SOCKET sock, SOCKADDR_IN Addr)
	:m_UserSock(sock)
	,m_UserAddr(Addr)
	,m_bConnected(true)
	,m_buffer()
	,m_wsaRecvBuffer()
	,m_wsaSendBuffer()

{

}
