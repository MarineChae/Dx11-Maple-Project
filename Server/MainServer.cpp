#include "Netstd.h"
#include "IOCPServer.h"



int main()
{
	
	IOCPServer::GetInstance().Init();

	while (1)
	{


		

		if (WaitForSingleObject(IOCPServer::GetInstance().GetKillEvent(), 0) != WAIT_TIMEOUT)
		{
			break;
		}
	}

    closesocket(IOCPServer::GetInstance().GetNetWork().GetSocket());
    //2)윈속 해제
    WSACleanup();


	return 0;

}