#pragma once
#include "NetBase.h"

class Network :public NetBase
{
private:
	SOCKET		m_hSock;
	SOCKADDR_IN m_SockAddr;
	int			m_iPort = 12345;

public:
	int			 GetPort() const  { return m_iPort; }
	SOCKET&		 GetSocket()  { return m_hSock; }
	SOCKADDR_IN& GetSockAddr() { return  m_SockAddr; }
	void		 SetSocket(SOCKET sock) { m_hSock = sock; }
	void		 SetSockAddr(SOCKADDR_IN addr) { m_SockAddr = addr; }

public:
	virtual bool Init();
	virtual bool Run();
	virtual bool Release();
public:
	virtual ~Network();
};

