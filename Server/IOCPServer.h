#pragma once
#include "Netstd.h"
#include "MyThread.h"
#include "IocpModel.h"
#include "User.h"
#include "PacketPool.h"


class IOCPServer;
class AcceptIocp : public MyThread
{
	IOCPServer* m_pServer;

public:
	void SetServer(IOCPServer* pServer)
	{
		m_pServer = pServer;
		MyThread::Create();
	}

	virtual bool ThreadRun() override;
};




class IOCPServer : public Singleton<IOCPServer>, public MyThread
{
	friend class Singleton<IOCPServer>;

protected:

	SOCKET		m_hSock;
	SOCKADDR_IN m_SockAddr;
	int			m_iPort = 12345;
	list<User*> m_lUserList;
public:

	SOCKET		 GetSocket() const { return m_hSock; };
	SOCKADDR_IN  GetSockerAddr() const { return m_SockAddr; };
	int			 GetPortNumber() const { return m_iPort; };
	IocpModel    GetIocpModel() { return m_iocpModel; };
	HANDLE		 GetKillEvent() { return m_iocpModel.GetKillEvent(); };

protected:
	IocpModel	 m_iocpModel;
	AcceptIocp	 m_AcceptIocp;
	PacketPool   m_PacketPool;
	PacketPool   m_BroadcastPacketPool;
public:

	virtual void AddPacket(UserPacket& packet) {};
	virtual void ChatMsg(UserPacket& packet);
	virtual int  SendPacket(User* pUser, UserPacket& packet);
	void		 PushUser(User* user) { m_lUserList.push_back(user); };
	bool		 Broadcasting(UserPacket packet);

public:
	bool		 Init();
	virtual bool ThreadRun() override;

};

