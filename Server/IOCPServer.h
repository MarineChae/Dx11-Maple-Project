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
	HANDLE       m_BroadCastEvent;
public:
	Network&	 GetNetWork()	{ return m_NetworkBase; };
	IocpModel&   GetIocpModel() { return m_iocpModel; };
	HANDLE&		 GetKillEvent() { return m_iocpModel.GetKillEvent(); };

public:

	void AddPacket(std::shared_ptr<Packet> packet, int currentScene);
	virtual void ChatMsg(Packet& packet);
	virtual int  SendPacket(User* pUser, std::shared_ptr<Packet> packet);
	bool		 Broadcasting(std::shared_ptr<Packet> packet);
	bool		 Broadcasting(std::pair<std::shared_ptr<Packet>, int> packet);
	bool		 Broadcasting(std::pair<std::shared_ptr<Packet>, int> packet, std::shared_ptr<User> pUser);
	bool		 Broadcasting(std::shared_ptr<Packet>packet, std::shared_ptr<User> pUser);
public:
	bool		 Init();
	virtual bool ThreadRun() override;

};

