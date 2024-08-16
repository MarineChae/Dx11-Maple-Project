#include "Netstd.h"
#include "IOCPServer.h"
#include"Timer.h"
bool isClose = false;
BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_BREAK_EVENT:
		isClose = true;
	}
	return FALSE;
}


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