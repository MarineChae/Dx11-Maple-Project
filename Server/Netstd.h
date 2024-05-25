#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_WORKER_THREAD 2
#define ERRORMSG(LPCWSTR) MessageBox(NULL, LPCWSTR, L"ERROR", MB_OK);

#include <iostream>
#include <WinSock2.h>
#include <process.h>
#include <list>
#include<shared_mutex>

#include "Protocol.h"

using std::list;




template<typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;

	}
private:


};