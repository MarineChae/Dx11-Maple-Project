#pragma once
#include "User.h"

class IocpModel
{

private:
	HANDLE m_hKillEvent;
	HANDLE m_hIocp;
	HANDLE m_hWorkerThread[MAX_WORKER_THREAD];


public:
	HANDLE& GetKillEvent()  { return m_hKillEvent; };
	HANDLE& GetIocpHandle()  { return m_hIocp; };
	HANDLE& GetWorkerThread(int num)  { return m_hWorkerThread[num];};

public:
	virtual bool Init();


};

