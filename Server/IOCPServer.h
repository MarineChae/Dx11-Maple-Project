#pragma once
#include "Netstd.h"
#include "MyThread.h"
#include "IocpModel.h"
class IOCPServer : public Singleton<IOCPServer>, public MyThread
{
	friend class Singleton<IOCPServer>;

protected:

	SOCKET m_hSock;
	SOCKADDR_IN m_SockAddr;
	int m_iPort = 12345;
public:

	SOCKET GetSocket() const { return m_hSock; };
	SOCKADDR_IN GetSockerAddr() const { return m_SockAddr; };
	int GetPortNumber() { return m_iPort = 12345; };

protected:
	IocpModel m_iocpModel;


public:

	bool Init();

	virtual bool ThreadRun() override;


};

