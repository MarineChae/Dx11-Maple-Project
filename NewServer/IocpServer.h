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

class IocpServer : public MyThread , public Singleton<IocpServer>
{
	friend class Singleton<IocpServer>;

private:
	SOCKET m_hSock;
	SOCKADDR_IN m_SockAddr;
	int m_iPort = 12345;

private:
	std::shared_ptr<IocpModel>	m_IocpModel;
	AcceptIocp  m_Accepter;

private:
	std::shared_ptr<PacketPool>  m_pPacketPool;
	std::shared_ptr<PacketPool>  m_pBroadcastPacketPool;
	typedef std::function<void(Packet& t)> CallFunction;
	std::map<int, CallFunction> m_fnExecutePacket;

public:

	SOCKET& GetSocket()  { return m_hSock; };
	void SetSocket(SOCKET sock) { m_hSock = sock; };
	SOCKADDR_IN& GetAddr()  { return m_SockAddr; }
	void SetAddr(SOCKADDR_IN addr) { m_SockAddr = addr; };
	const int GetPort() const { return m_iPort; };
	std::shared_ptr<IocpModel> GetIocpModel() { return 	m_IocpModel; }
	std::map<int, CallFunction>& GetExecutePacket() { return m_fnExecutePacket; }

public:
	//packetproc
	typedef std::map<int, CallFunction>::iterator FunctionIterator;
	void AddPacket(Packet& packet, BYTE PacketType);
	int  SendPacket(std::shared_ptr<User> pUser, Packet& packet);

	void ChatMsg(Packet& packet);
	void MoveStartPacket(Packet& packet);

public:
	bool Init();
	bool ThreadRun() override;
	bool Release();
	bool Broadcasting(Packet* packet);

public:
	IocpServer();
	virtual ~IocpServer() {};
};
