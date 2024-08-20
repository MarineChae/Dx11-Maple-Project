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
    float fX;
    float fY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;


    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);
    if (player == nullptr)
        return FALSE;
    Packet* SendPack = new Packet;

    player->SetPos({fX,fY,0});

    MoveStartPacket(SendPack, byDirection, Sessionid, player->GetPos().x,player->GetPos().y, state, isFalling, isJump);
    
    IOCPServer::GetInstance().AddPacket(SendPack, player->GetCurrentScene());

    return 0; 
}

BOOL PacketProc_MoveEnd(DWORD Sessionid, Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY; 
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> byDirection;
    *pack >> dwSessionID; 
    *pack >> fX;
    *pack >> fY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);

    if (player == nullptr)
        return FALSE;
    Packet* SendPack = new Packet;

    player->SetPos({ fX,fY,0 });

    MoveStopPacket(SendPack, byDirection, Sessionid, player->GetPos().x,player->GetPos().y, state, isFalling, isJump);

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
    IOCPServer::GetInstance().AddPacket(SendPack, Scenenum);

    for (auto& monster : curScene->GetSceneMonsterList())
    {
        Packet* pack = new Packet;
        CreateMonster(pack, monster->GetName(), 0, monster->GetPos().x,monster->GetPos().y, 100, Scenenum);
        IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[dwSessionID].get(), pack);
    }


    return 0;
}
