#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_WORKER_THREAD 2
#define ERRORMSG(LPCWSTR) MessageBox(NULL, LPCWSTR, L"ERROR", MB_OK);

#include <iostream>
#include <WinSock2.h>
#include <process.h>
#include <list>
#include <functional>
#include <map>
#include <shared_mutex>
#include <vector>
#include<memory>
#include<array>
#include "Protocol.h"

using std::list;



