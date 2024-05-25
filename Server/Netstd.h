#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_WORKER_THREAD 2
#include <iostream>
#include<WinSock2.h>
#include<process.h>


#define ERRORMSG(LPCWSTR) MessageBox(NULL, LPCWSTR, L"ERROR", MB_OK);


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