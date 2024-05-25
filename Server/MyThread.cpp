#include "Netstd.h"
#include "MyThread.h"

void MyThread::Create()
{
	if (GetStartState() == true) return;

	SetStartState(true);
								//스레드 루틴의 시작주소 ,스레드 루틴에 사용할 인자
	SetThread(_beginthreadex(NULL, 0, HandlerRunner, (LPVOID)this, 0, &m_iThreadID));

}

unsigned int __stdcall MyThread::HandlerRunner(LPVOID param)
{
	MyThread* pThread = static_cast<MyThread*>(param);
	
	while (pThread->GetStartState())
	{
		if (!pThread->ThreadRun())
		{
			break;
		}
	}
	return 0;
}

MyThread::MyThread() : m_bStarted(false)
{
}

MyThread::~MyThread()
{
	CloseHandle(reinterpret_cast<HANDLE>(m_iThread));
}
