#include"Netstd.h"
#include "PacketProc.h"
#include"IOCPServer.h"
#include "Packet.h"
#include "PlayerData.h"
#include"MakePacket.h"
#include"ServerScene.h"
#include"MonsterData.h"
BOOL PacketProc_MoveStart(DWORD Sessionid, std::shared_ptr<Packet> pack)
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
    std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

 
    player->SetIsFalling(isFalling);
    if (player->GetIsJumping() != isJump)
    {
        player->SetBeforePos(player->GetPos());
    }
    player->SetIsJumping(isJump);
    player->SetIsMove(true);
    player->SetAction(state);
    player->SetDirection(byDirection);

    return 0; 
}

BOOL PacketProc_MoveEnd(DWORD Sessionid, std::shared_ptr<Packet> pack)
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
    
    
    player->SetIsFalling(isFalling);
    player->SetIsMove(false);
    player->SetAction(state);
    player->SetDirection(byDirection);

    if (player->GetIsJumping() != isJump)
    {
        player->SetBeforePos(player->GetPos());
    }
    player->SetIsJumping(isJump);
    return 0;
}

BOOL PacketProc_SceneChange(DWORD Sessionid, std::shared_ptr<Packet> pack)
{

    DWORD dwSessionID;
    BYTE Scenenum;

    *pack >> dwSessionID;
    *pack >> Scenenum;

    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);

    if (player == nullptr)
        return FALSE;
    std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();
    auto beforeScenenum = player->GetCurrentScene();
    player->SetCurrentScene((SceneNum)Scenenum);
    player->SetPos({ 0,0,0 });
    SceneChangePacket(SendPack, dwSessionID, Scenenum);

    //현재씬과 이전씬에서 플레이어 목록삭제 및 추가
    auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(beforeScenenum);
    auto curScene = ServerSceneMgr::GetInstance().InsertScene(Scenenum);

    BeforeScene->DeleteScenePlayer(player);
    curScene->AddScenePlayer(player);

    IOCPServer::GetInstance().AddPacket(SendPack, beforeScenenum);
    IOCPServer::GetInstance().AddPacket(SendPack, Scenenum);

    int iId = 0;
    for (auto& monster : curScene->GetSceneMonsterList())
    {
        std::shared_ptr<Packet> pack = std::make_shared<Packet>();
        CreateMonster(pack, iId++,monster->GetName(), 0, monster->GetPos().x,monster->GetPos().y, 100, Scenenum);
        IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[dwSessionID].get(), pack);
    }


    return 0;
}
