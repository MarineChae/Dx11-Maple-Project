#include "Netstd.h"
#include "MyThread.h"

void MyThread::Create()
{
	if (GetStartState() == true) return;

	SetStartState(true);
								//������ ��ƾ�� �����ּ� ,������ ��ƾ�� ����� ����
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
