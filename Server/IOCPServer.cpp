#include "Netstd.h"
#include "IOCPServer.h"

bool IOCPServer::Init()
{
	WSADATA wsa;
	//winsock ������ �����Ͽ� �ʱ�ȭ 2.2������ ���
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	//�ʱ�ȭ�� ������ ��� ���α׷� ����
	if (ret != 0)
	{

		return false;
	}

	// SOCKET SockTCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	// SOCKET SockUDP = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
	// Stream �� ������ TCP    DGRAM�� UDP
	// ���� �ڿ� ���� ���� �׳� 0������ �˾Ƽ� ����
	m_hSock = socket(AF_INET, SOCK_STREAM, 0);


	//���� �ּ��� �ּ� �йи�, �׻� AF_INET
	m_SockAddr.sin_family = AF_INET;
	//IPv4 ���� �ּҸ� �����ϴ� IN_ADDR ����ü 
	//INADDR_ANY - �ڵ����� �� ��ǻ�Ϳ� �����ϴ� ��ī�� �� ��밡���� ��ī���� IP�ּҸ� ���
	m_SockAddr.sin_addr.s_addr = htons(INADDR_ANY);
	m_SockAddr.sin_port = htons(m_iPort);

	//���Ͽ� �ּҸ� �Ҵ�
	ret = bind(m_hSock, (SOCKADDR*)&m_SockAddr, sizeof(m_SockAddr));
	if (ret == SOCKET_ERROR)
	{
		ERRORMSG(L"SOCKET BIND ERROR");
		return false;
	}

	//�����û�� �����·�
	//SOMAXCONN - ���� ���� ���� ť�� �ִ� ����. SOMAXCONN���� �����ϸ� ������ ����ϴ� �⺻ ���� �����ڰ� ��α׸� ������ �ִ� ������ ����
	ret = listen(m_hSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		ERRORMSG(L"SOCKET LISTEN ERROR");
		return false;
	}

	std::cout << "Server Is Running!" << std::endl;



	MyThread::Create();

	m_iocpModel.Init();





	return true;
}

bool IOCPServer::ThreadRun()
{


	

	return true;
}
