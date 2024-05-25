#include "Netstd.h"
#include "IocpModel.h"

DWORD WINAPI WorkerThread(LPVOID param)
{
	while (1)
	{
		

	}

	return 0;

}






bool IocpModel::Init()
{
	m_hKillEvent = CreateEvent(0, TRUE, FALSE, 0);
	//INVALID_HANDLE_VALUE를 지정하면 함수가 파일 핸들과 연결하지 않고 I/O 완료 포트를 만듭니다.
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	DWORD dwThreadID;
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; ++iThread)
	{
		m_hWorkerThread[iThread] = CreateThread(0, 0, WorkerThread, this, 0, &dwThreadID);

	}


	return false;
}
