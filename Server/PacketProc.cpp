#include"Netstd.h"
#include "PacketProc.h"
#include"IOCPServer.h"
#include "Packet.h"
#include "PlayerData.h"
#include"MakePacket.h"
#include"ServerScene.h"
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
    
    IOCPServer::GetInstance().AddPacket(SendPack, player->GetCurrentScene());

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

    IOCPServer::GetInstance().AddPacket(SendPack,player->GetCurrentScene());


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
    auto beforeScenenum = player->GetCurrentScene();
    player->SetCurrentScene((SceneNum)Scenenum);

    SceneChangePacket(SendPack, dwSessionID, Scenenum);

    //현재씬과 이전씬에서 플레이어 목록삭제 및 추가
    auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(beforeScenenum);
    auto curScene = ServerSceneMgr::GetInstance().InsertScene(Scenenum);

    BeforeScene->DeleteScenePlayer(player);
    curScene->AddScenePlayer(player);

    IOCPServer::GetInstance().AddPacket(SendPack, beforeScenenum);
    IOCPServer::GetInstance().AddPacket(SendPack, player->GetCurrentScene());




    return 0;
}
