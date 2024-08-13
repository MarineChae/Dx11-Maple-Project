#include"Netstd.h"
#include "PacketProc.h"
#include"IOCPServer.h"
#include "Packet.h"
#include "PlayerData.h"
#include"MakePacket.h"
BOOL PacketProc_MoveStart(DWORD Sessionid, Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> shX;
    *pack >> shY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;


    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);
    if (player == nullptr)
        return FALSE;
    Packet* SendPack = new Packet;

    player->SetXPos(shX);
    player->SetYPos(shY);
    MoveStartPacket(SendPack, byDirection, Sessionid, player->GetXPos(), player->GetYPos(), state, isFalling, isJump);
    
    IOCPServer::GetInstance().AddPacket(SendPack);

    return 0; 
}

BOOL PacketProc_MoveEnd(DWORD Sessionid, Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY; 
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> byDirection;
    *pack >> dwSessionID; 
    *pack >> shX;
    *pack >> shY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);

    if (player == nullptr)
        return FALSE;
    Packet* SendPack = new Packet;

    player->SetXPos(shX);
    player->SetYPos(shY);
    MoveStopPacket(SendPack, byDirection, Sessionid, player->GetXPos(), player->GetYPos(), state, isFalling, isJump);

    IOCPServer::GetInstance().AddPacket(SendPack);


    return 0;
}

BOOL PacketProc_SceneChange(DWORD Sessionid, Packet* pack)
{

    DWORD dwSessionID;
    BYTE Scenenum;

    *pack >> dwSessionID;
    *pack >> Scenenum;

    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);

    if (player == nullptr)
        return FALSE;
    Packet* SendPack = new Packet;
    player->SetCurrentScene((SceneNum)Scenenum);

    SceneChangePacket(SendPack, dwSessionID, Scenenum);

    IOCPServer::GetInstance().AddPacket(SendPack);

    return 0;
}
