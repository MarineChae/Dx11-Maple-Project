#pragma once

class IocpModel
{

private:
	HANDLE m_hKillEvent;
	HANDLE m_hIocp;
	HANDLE m_hWorkerThread[MAX_WORKER_THREAD];

public:
	virtual bool Init();


};

