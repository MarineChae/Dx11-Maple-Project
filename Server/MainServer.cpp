#include "Netstd.h"
#include "IOCPServer.h"



int main()
{
	
	IOCPServer::GetInstance().Init();




	WaitForSingleObject(IOCPServer::GetInstance().GetKillEvent(), INFINITE);


    closesocket(IOCPServer::GetInstance().GetNetWork().GetSocket());
    //2)���� ����
    WSACleanup();


	return 0;

}