#include "pch.h"
#include"IocpServer.h"
#include"IocpModel.h"




int main()
{


	IocpServer::GetInstance().Init();


	WaitForSingleObject(IocpServer::GetInstance().GetIocpModel()->m_hKillEvent, INFINITE);

	closesocket(IocpServer::GetInstance().GetSocket());
	//2)���� ����
	WSACleanup();






	return 0;



}