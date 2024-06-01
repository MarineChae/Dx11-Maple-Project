#include "Netstd.h"
#include "Network.h"

bool Network::Init()
{
	WSADATA WsaData;
	int iRet = WSAStartup(MAKEWORD(2, 2), &WsaData);
	if (iRet != 0)
	{
		return false;
	}
	return true;
}

bool Network::Run()
{
    return false;
}

bool Network::Release()
{
	closesocket(m_hSock);
	WSACleanup();
	return true;
}

Network::~Network()
{
    Release();
}
