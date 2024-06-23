#include "Netstd.h"
#include "IOCPServer.h"
#include"Timer.h"


int main()
{
	
	IOCPServer::GetInstance().Init();
	Timer::GetInstance().Init();


	WaitForSingleObject(IOCPServer::GetInstance().GetKillEvent(), INFINITE);

    closesocket(IOCPServer::GetInstance().GetNetWork().GetSocket());
    //2)윈속 해제
    WSACleanup();


	return 0;

}