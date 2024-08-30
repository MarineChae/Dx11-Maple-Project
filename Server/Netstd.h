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
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include<memory>
#include<array>
#include"tchar.h"
#include<string>
#include "Protocol.h"
#include"TMath.h"
using std::list;



enum PLAYER_STATE
{
    PS_STAND,
    PS_WALK,
    PS_JUMP,
    PS_ONLOPE,
    PS_ATTACK,
    PS_DIE,
    PS_DEFAULT,


};
enum MONSTER_STATE
{
    MS_IDLE,
    MS_WALK,
    MS_HIT,
    MS_DIE,
    MS_ATTACK,
    MS_RESPAWN,
    MS_SKILL1,
    MS_SKILL2,
    MS_SKILL3,
};
enum COLLISION_TYPE
{
    CT_FLOOR,
    CT_LOPE,
    CT_WALL,
    CT_FINALFLOOR,
    CT_DEFAULT,
};
enum OBJECT_TYPE
{
    Defalut,
    Portal,
    FALLING_OBJECT,
    LAZER_OBJECT,
    TRAB_OBJECT,
    COLLIDER,

};
struct Line
{
    TVector3 From;
    TVector3 To;
    COLLISION_TYPE type;
};
#define randstep(fmin,fmax) ((float)fmin + ((float)fmax-(float)fmin) * rand()/RAND_MAX)