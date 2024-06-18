#include"Pch.h"
#include "PacketProc.h"




BOOL PacketProc_MoveStart(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> shX;
    *pack >> shY;






    return TRUE;
}

BOOL PacketProc_MoveEnd(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;


    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> shX;
    *pack >> shY;

   

    

    std::wstring s;
    s.push_back(dwSessionID);
    s.push_back(' ');
    s.push_back(byDirection);
    s.push_back(' ');
    s.push_back(shX);
    s.push_back(' ');
    s.push_back(shY);
    s.push_back(' ');
    OutputDebugString(s.c_str());

    return TRUE;
}
