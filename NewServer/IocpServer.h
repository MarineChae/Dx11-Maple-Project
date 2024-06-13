#pragma once
#include"MyThread.h"
#include"Singleton.h"

class IocpModel;
class IocpServer;
class PacketPool;
class Packet;
class User;
class AcceptIocp : public MyThread
{
	IocpServer* m_pServer;
public:
	void Set(IocpServer* pServer)
	{
		m_pServer = pServer;
		MyThread::Create();
	}

	virtual bool ThreadRun()override;
};

class IocpServer : public MyThread
{
public:

	std::shared_ptr<IocpModel>	m_IocpModel;
	AcceptIocp  m_Accepter;
public:
	std::shared_ptr<PacketPool>  m_pPacketPool;
	std::shared_ptr<PacketPool>  m_pBroadcastPacketPool;
	typedef std::function<void(Packet& t)> CallFunction;
	typedef std::map<int, CallFunction>::iterator FunctionIterator;
	std::map<int, CallFunction> m_fnExecutePacket;

	virtual void AddPacket(Packet& packet) {};
	virtual void ChatMsg(Packet& packet);
	virtual int  SendPacket(User* pUser, Packet& packet);
public:
	virtual bool Init();
	virtual bool ThreadRun() override;
	virtual bool Release();
	bool		 Broadcasting(Packet* packet);


};

class ZoneServer : public Singleton<ZoneServer>, public IocpServer
{
	friend class Singleton<ZoneServer>;
public:
	virtual void AddPacket(Packet& packet)override;

};