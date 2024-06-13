#include "Pch.h"
#include "User.h"
#include"StreamPacket.h"

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
		if (dwTransfer == 0)
		{
			m_bConnected = false;
			return;
		}
		m_sPacket->Put(m_buffer, dwTransfer);
		m_sPacket->GetPacket(std::ref(*this));

	}
	if (myov->flag == MyOV::MODE_SEND)
	{
		int a = 0;
	}
	Recv();
	return;
}

User::User()
{
	m_bConnected = false;
}
User::User(SOCKET sock, SOCKADDR_IN addr)
	:m_Sock(sock)
	, m_Addr(addr)
{
	m_bConnected = false;
}

