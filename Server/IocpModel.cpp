#include "Netstd.h"
#include "IocpModel.h"
#include"Timer.h"
std::mutex m2;
DWORD WINAPI WorkerThread(LPVOID param)
{
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* overlap;
	IocpModel* iocp = (IocpModel*)param;
	
	while (1)
	{
		if (WaitForSingleObject(iocp->GetKillEvent(), 0) == WAIT_OBJECT_0)
		{
			break;
		}
		BOOL bRet = GetQueuedCompletionStatus(iocp->GetIocpHandle(), &dwTransfer, &KeyValue, &overlap,INFINITE);
		User* pUser = (User*)KeyValue;
		if (bRet == TRUE)
		{
			
			if (pUser != nullptr)
			{
				
				pUser->Dispatch(dwTransfer,overlap);
				
			}
		}
		else
		{
			DWORD Errmsg = GetLastError();
			if (Errmsg == WAIT_TIMEOUT)
			{

				continue;
			}
			if (Errmsg == ERROR_HANDLE_EOF)
			{
				//ERRORMSG(L"ERROR_HANDLE_EOF");
				SetEvent(iocp->GetKillEvent());
			}
			if (Errmsg == ERROR_NETNAME_DELETED)
			{
				pUser->SetConnect(false);
				//pUser->Close();
				//ERRORMSG(L"ClientHardClosd");
				continue;
			}
			//ERRORMSG(L"ERROR ETC...");
			SetEvent(iocp->GetKillEvent());
			
			break;
		}
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
		//IocpModel을 인자로 넘기는 스레드 생성
	}


	return false;
}
