#include "Netstd.h"
#include "User.h"

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
	, m_lPacketList()
	, m_buffer()
	, m_wsaRecvBuffer()
	, m_wsaSendBuffer()
{
	 
}

User::User(SOCKET sock, SOCKADDR_IN Addr)
	:m_UserSock(sock)
	,m_UserAddr(Addr)
	,m_bConnected(true)
	,m_lPacketList()
	,m_buffer()
	,m_wsaRecvBuffer()
	,m_wsaSendBuffer()

{

}
