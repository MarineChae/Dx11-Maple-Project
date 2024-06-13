#include "Pch.h"
#include "MyThread.h"


void MyThread::Create()
{
	if (m_bStarted == true)return;
	m_bStarted = true;
	m_hThread = _beginthreadex(NULL, 0, HandlerRunner, (LPVOID)this, 0, &m_iThreadID);
}

unsigned int __stdcall MyThread::HandlerRunner(LPVOID param)
{
	MyThread* pThread = (MyThread*)param;
	while (pThread->m_bStarted)
	{
		if (!pThread->ThreadRun())
		{
			break;
		}

	}

	return 0;
}

MyThread::MyThread()
	:m_bStarted(false)
{
}

MyThread::~MyThread()
{
	CloseHandle((HANDLE)m_hThread);
}

