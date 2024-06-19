#pragma once
#include "Netstd.h"
#include "MyThread.h"
#include "IocpModel.h"
#include "PacketPool.h"
#include "User.h"
#include "Network.h"
#include"Singleton.h"

class IOCPServer;
class Packet;
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




class IOCPServer :  public Singleton<IOCPServer> ,public MyThread
{
	friend class Singleton<IOCPServer>;
private:
	Network		 m_NetworkBase;
	IocpModel	 m_iocpModel;
	AcceptIocp	 m_AcceptIocp;
	PacketPool   m_PacketPool;
	PacketPool   m_BroadcastPacketPool;
	typedef std::function<void(UserPacket& t)> CallFunction;
	typedef std::map<int, CallFunction>::iterator FunctionIterator;
	std::map<int, CallFunction> m_fnExecutePacket;
public:
	Network&	 GetNetWork()	{ return m_NetworkBase; };
	IocpModel&   GetIocpModel() { return m_iocpModel; };
	HANDLE&		 GetKillEvent() { return m_iocpModel.GetKillEvent(); };

public:

	virtual void AddPacket(Packet& packet);
	virtual void ChatMsg(Packet& packet);
	virtual int  SendPacket(User* pUser, Packet* packet);
	bool		 Broadcasting(Packet* packet);
	bool		 Broadcasting(Packet* packet, std::shared_ptr<User> pUser);

public:
	bool		 Init();
	virtual bool ThreadRun() override;

};

