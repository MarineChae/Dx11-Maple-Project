#include "Object.h"
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

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetPlayerObject(dwSessionID);

    if (obj == nullptr)
        return FALSE;


    return TRUE;
}

BOOL PacketProc_MoveEnd(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;


    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> shX;
    *pack >> shY;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetPlayerObject(dwSessionID);

    if (obj == nullptr)
        return FALSE;



    return TRUE;
}
