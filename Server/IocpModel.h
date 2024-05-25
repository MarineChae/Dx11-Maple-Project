#pragma once

class IocpModel
{

private:
	HANDLE m_hKillEvent;
	HANDLE m_hIocp;
	HANDLE m_hWorkerThread[MAX_WORKER_THREAD];


public:
	HANDLE GetKillEvent() const { return m_hKillEvent; };
	HANDLE GetIocpHandle() const { return m_hIocp; };
	HANDLE GetWorkerThread() const { return m_hWorkerThread[MAX_WORKER_THREAD];};

public:
	virtual bool Init();


};

