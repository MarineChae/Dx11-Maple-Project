#include "Object.h"
#include "PacketProc.h"
#include"PlayerObject.h"



BOOL PacketProc_MoveStart(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;


    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> shX;
    *pack >> shY;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr|| obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    obj->SetDestination(TVector3(shX, shY, 1));


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

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr || obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    obj->SetDestination(TVector3(shX, shY, 1));




    return TRUE;
}

BOOL PacketProc_CreateMyCharacter(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;
    int  HP;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> shX;
    *pack >> shY;
    *pack >> HP;



    std::shared_ptr<Object> player = std::make_shared<PlayerObject>();
    
    player->SetScale(TVector3(100.0f, 100.0f, 1.0f));
    player->Create(L"", L"../Shader/Defalutshader.hlsl");
    player->SetTransform(TVector3(shX,shY,1));
    player->SetDestination(TVector3(shX, shY, 1));
    player->SetRenderState(true);
    player->SetObejctID(dwSessionID);

    ObejctMgr::GetInstance().PushObject(player, dwSessionID);
    ObejctMgr::GetInstance().SetPlayerObject(player);
   

    return 0;
}

BOOL PacketProc_CreateOtherCharacter(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;
    int  HP;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> shX;
    *pack >> shY;
    *pack >> HP;

    if (ObejctMgr::GetInstance().GetPlayerObject()->GetObejctID() == dwSessionID)
        return 0;


    std::shared_ptr<Object> other = std::make_shared<PlayerObject>();

    other->SetScale(TVector3(55, 55, 1.0f));
    other->Create(L"", L"../Shader/Defalutshader.hlsl");
    other->SetTransform(TVector3(shX, shY, 1));
    other->SetDestination(TVector3(shX, shY, 1));
    other->SetRenderState(true);
    other->SetObejctID(dwSessionID);
    ObejctMgr::GetInstance().PushObject(other, dwSessionID);


    return 0;
}

BOOL PacketProc_DisconnectOtherCharacter(Packet* pack)
{
    DWORD dwSessionID;
    *pack >> dwSessionID;

    ObejctMgr::GetInstance().DisconnectCharacter(dwSessionID);

    return 0;
}
