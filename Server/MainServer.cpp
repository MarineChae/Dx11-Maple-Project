#include "Netstd.h"
#include "IOCPServer.h"



int main()
{
	
	IOCPServer::GetInstance().Init();




	WaitForSingleObject(IOCPServer::GetInstance().GetKillEvent(), INFINITE);


    closesocket(IOCPServer::GetInstance().GetSocket());
    //2)���� ����
    WSACleanup();


	return 0;

}