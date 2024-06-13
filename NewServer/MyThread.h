#pragma once

class MyThread
{
public:
	unsigned int m_hThread;
	unsigned int m_iThreadID;
	bool m_bStarted;
public:
	void Create();
	virtual bool ThreadRun() { return true; };
	static unsigned int WINAPI HandlerRunner(LPVOID param);
public:
	MyThread();
	virtual ~MyThread();

};

