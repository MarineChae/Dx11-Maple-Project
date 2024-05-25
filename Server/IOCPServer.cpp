#include "Netstd.h"
#include "IOCPServer.h"

bool IOCPServer::Init()
{
	WSADATA wsa;
	//winsock 버전을 기입하여 초기화 2.2버전을 사용
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa);

	//초기화에 실패한 경우 프로그램 종료
	if (ret != 0)
	{

		return false;
	}

	// SOCKET SockTCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	// SOCKET SockUDP = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
	// Stream 은 무조건 TCP    DGRAM은 UDP
	// 굳이 뒤에 넣지 말고 그냥 0넣으면 알아서 적용
	m_hSock = socket(AF_INET, SOCK_STREAM, 0);


	//전송 주소의 주소 패밀리, 항상 AF_INET
	m_SockAddr.sin_family = AF_INET;
	//IPv4 전송 주소를 포함하는 IN_ADDR 구조체 
	//INADDR_ANY - 자동으로 이 컴퓨터에 존재하는 랜카드 중 사용가능한 랜카드의 IP주소를 사용
	m_SockAddr.sin_addr.s_addr = htons(INADDR_ANY);
	m_SockAddr.sin_port = htons(m_iPort);

	//소켓에 주소를 할당
	ret = bind(m_hSock, (SOCKADDR*)&m_SockAddr, sizeof(m_SockAddr));
	if (ret == SOCKET_ERROR)
	{
		ERRORMSG(L"SOCKET BIND ERROR");
		return false;
	}

	//연결요청을 대기상태로
	//SOMAXCONN - 보류 중인 연결 큐의 최대 길이. SOMAXCONN으로 설정하면 소켓을 담당하는 기본 서비스 공급자가 백로그를 적절한 최대 값으로 설정
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
