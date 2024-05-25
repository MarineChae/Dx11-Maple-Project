#pragma once
#include "Netstd.h"

class MyThread
{
protected:
	unsigned int m_iThread;
	unsigned int m_iThreadID;
	bool		 m_bStarted;
	
public:
	unsigned int GetThread() const { return m_iThread; };
	unsigned int GetThreadID() const { return m_iThreadID; };
	bool		 GetStartState() const { return m_bStarted; };

	void SetThread(unsigned int Thread) { m_iThread = Thread; };
	void SetThreadID(unsigned int ThreadID) { m_iThreadID = ThreadID; };
	void SetStartState(bool IsStarted) { m_bStarted = IsStarted; };

public:
	void					   Create();
	virtual bool			   ThreadRun() { return false; };
	static unsigned int WINAPI HandlerRunner(LPVOID param);
	
public:
	MyThread();
	virtual ~MyThread();


};

