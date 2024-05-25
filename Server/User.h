#pragma once
#include "ObjectPool.h"
#include"PacketPool.h"


struct UserPacket;
struct MyOV : public ObjectPool<MyOV>
{
	enum {MODE_RECV = 0 , MODE_SEND =1};
	OVERLAPPED ov;
	int flag;

	MyOV(int value) :flag(value)
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));

	}
	MyOV()
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		flag = MyOV::MODE_RECV;
	}
};



class User
{

private:
	bool				  m_bConnected;
	SOCKET				  m_UserSock;
	SOCKADDR_IN			  m_UserAddr;
	list<UserPacket>      m_lPacketList;
	char				  m_buffer[4096];
	WSABUF				  m_wsaRecvBuffer;
	WSABUF				  m_wsaSendBuffer;
public:
	bool			  IsConnected() const { return m_bConnected; };
	void			  SetConnect(bool connect) { m_bConnected = connect; };
	SOCKET			  GetUserSock() const { return m_UserSock; };
	SOCKADDR_IN		  GetUserAddr() const { return m_UserAddr; };
	WSABUF&			  GetSendBuffer() { return m_wsaSendBuffer; };
	WSABUF&			  GetRecvBuffer() { return m_wsaRecvBuffer; };
	list<UserPacket>& GetPacketList() { return m_lPacketList; };

public:
	void Close();
	void bind(HANDLE iocp);
	void Recv();
	void Dispatch(DWORD dwTransfer, OVERLAPPED* ov);


public:
	User();
	User(SOCKET sock, SOCKADDR_IN Addr);

};

