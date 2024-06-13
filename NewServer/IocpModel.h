#pragma once

class IocpModel
{
public:

	HANDLE m_hKillEvent;
	HANDLE m_hIocp;
	HANDLE m_hWorkerThread[MAX_WORKER_THERAD];

public:
	virtual bool Init();
};

