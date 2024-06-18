#pragma once
#include "ObjectPool.h"
#include"PacketPool.h"
#include "StreamPacket.h"
#include"Singleton.h"



struct UserPacket;
struct MyOV : public ObjectPool<MyOV>
{
	enum { MODE_RECV = 0, MODE_SEND = 1, };
	OVERLAPPED ov;
	int flag;

	MyOV(int value) : flag(value)
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
	DWORD				  m_dwSessionID;
	bool				  m_bConnected;
	SOCKET				  m_UserSock;
	SOCKADDR_IN			  m_UserAddr;
	char				  m_buffer[2048];
	WSABUF				  m_wsaRecvBuffer;
	WSABUF				  m_wsaSendBuffer;
	StreamPacket		  m_StreamPacket;

public:
	DWORD			  GetSessionId()const { return m_dwSessionID; };
	void			  SetSessionId(DWORD iD) { m_dwSessionID = iD; };
	bool			  IsConnected() const { return m_bConnected; };
	void			  SetConnect(bool connect) { m_bConnected = connect; };
	SOCKET			  GetUserSock() const { return m_UserSock; };
	SOCKADDR_IN		  GetUserAddr() const { return m_UserAddr; };
	WSABUF&			  GetSendBuffer() { return m_wsaSendBuffer; };
	WSABUF&			  GetRecvBuffer() { return m_wsaRecvBuffer; };
	StreamPacket	  GetStreamPacket()const  { return m_StreamPacket; }
public:
	void Close();
	void bind(HANDLE iocp);
	void Recv();
	void Dispatch(DWORD dwTransfer, OVERLAPPED* ov);


public:
	User();
	User(SOCKET sock, SOCKADDR_IN Addr);

};


class SessionMgr :public Singleton<SessionMgr>
{

private:
	std::list<std::shared_ptr<User>> m_vUserList;

public:
	std::list<std::shared_ptr<User>>& GetUserList() { return m_vUserList; }
	//std::shared_ptr<User>  GetUser(DWORD SessionID) { return m_vUserList[SessionID]; }

	bool  ConnectUser(std::shared_ptr<User> user);

public:
	SessionMgr()
		:m_vUserList()
	{
		
	}
	~SessionMgr()
	{
		m_vUserList.clear();
	}
};

