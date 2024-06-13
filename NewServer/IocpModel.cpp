#include "Pch.h"
#include "User.h"
#include "IocpModel.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
    DWORD dwTransfer;
    ULONG_PTR KeyValue;
    OVERLAPPED* overlap;
    IocpModel* iocp = (IocpModel*)param;
    while (1)
    {
        if (WaitForSingleObject(iocp->m_hKillEvent, 0) == WAIT_OBJECT_0)
        {
            break;
        }
        BOOL bRet = GetQueuedCompletionStatus(iocp->m_hIocp, &dwTransfer, &KeyValue, &overlap, 0);

        if (bRet == TRUE)
        {
            User* pUser = (User*)KeyValue;
            if (pUser != nullptr)
            {
               // pUser->Dispatch(dwTransfer, overlap);
            }
        }
        else
        {
            DWORD dwErr = GetLastError();
            if (dwErr == WAIT_TIMEOUT)
            {

                continue;
            }
            if (dwErr == ERROR_HANDLE_EOF)
            {
                //MessageBox(,"GetLastError");
                SetEvent(iocp->m_hKillEvent);
                break;
            }
           // LogErrorA("GetLastError");
            SetEvent(iocp->m_hKillEvent);
            break;
        }


    }
    return 1;
}


bool IocpModel::Init()
{
    m_hKillEvent = CreateEvent(0, TRUE, FALSE, 0);
    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    DWORD dwThreadID;
    for (int iThread = 0; iThread < MAX_WORKER_THERAD; iThread++)
    {
        m_hWorkerThread[iThread] = ::CreateThread(0, 0, WorkerThread,
            this, 0, &dwThreadID);
    }
    return true;
}
