#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_WORKER_THERAD 2
#include"Protocol.h"
#include <iostream>
#include<string>
#include<list>
#include<vector>
#include<process.h>
#include <algorithm>
#include <codecvt>
#include<shared_mutex>
#include<functional>
#include<map>
#include<memory>

#define ERRORMSG(LPCWSTR) MessageBox(NULL, LPCWSTR, L"ERROR", MB_OK);

