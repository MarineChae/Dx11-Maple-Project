#include"Netstd.h"
#include "PacketProc.h"
#include"IOCPServer.h"
#include "Packet.h"
#include "PlayerData.h"
#include"MakePacket.h"
#include"ServerScene.h"
#include"MonsterData.h"
#include"ObjectData.h"
BOOL PacketProc_MoveStart(DWORD Sessionid, std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY;
    int iHP;
    BYTE isFalling;
    BYTE isJump;
    BYTE onLope;
    BYTE lopeUp;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> iHP;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> onLope;
    *pack >> lopeUp;
    *pack >> state;


    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);
    if (player == nullptr)
        return FALSE;
    std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

 
    player->SetIsFalling(isFalling);

    if (player->GetIsJumping() != isJump && !isFalling)
    {
        player->SetBeforePos(player->GetPos());
    }
    player->SetIsJumping(isJump);
    player->SetIsMove(true);
    player->SetAction(state);
    player->SetDirection(byDirection);
    player->SetOnLope(onLope);
    player->SetHP(iHP);

 
    player->SetLopeUp(lopeUp);
    std::cout << "[" << dwSessionID << "]" << "Recv MoveStartPacket" << "\n";
    return 0; 
}

BOOL PacketProc_MoveEnd(DWORD Sessionid, std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY; 
    int iHP;
    BYTE isFalling;
    BYTE isJump;
    BYTE onLope;
    BYTE lopeUp;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> byDirection;
    *pack >> dwSessionID; 
    *pack >> fX;
    *pack >> fY;
    *pack >> iHP;
    *pack >> isFalling;
     *pack >> isJump;
    *pack >> onLope;
    *pack >> lopeUp;
    *pack >> state;

    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);

    if (player == nullptr)
        return FALSE;

 
    player->SetIsFalling(isFalling);
    player->SetIsMove(false);
    player->SetAction(state);
    player->SetDirection(byDirection);

    if (player->GetIsJumping() != isJump && !isFalling)
    {
        player->SetBeforePos(player->GetPos());
    }
    player->SetIsJumping(isJump);
    player->SetOnLope(onLope);
    player->SetHP(iHP);
   

    player->SetLopeUp(lopeUp);
    std::cout << "[" << dwSessionID << "]" << "Recv MoveEndPacket" << "\n";
    return 0;
}

BOOL PacketProc_Attack(DWORD Sessionid, std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    int namelen;
    char name[80]{0,};
    int SkillNumLen;
    char SkillNum[80]{ 0, };
    float fX;
    float fY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> namelen;
    pack->GetData(name, namelen);
    *pack >> SkillNumLen;
    pack->GetData(SkillNum, SkillNumLen);
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
    if (player->GetIsJumping() != isJump && !isFalling)
    {
        player->SetBeforePos(player->GetPos());
    }
    player->SetIsJumping(isJump);
    player->SetActiveSkillName(name);


    std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

    AttackPacket(SendPack, dwSessionID, player->GetPos().x, player->GetPos().y, state, player->GetIsFalling(), player->GetIsJumping(), name, SkillNum);

    IOCPServer::GetInstance().Broadcasting({ SendPack,player->GetCurrentScene()});
    std::cout << "[" << dwSessionID << "]" << "Recv PlayerAttackPacket" << "\n";
    return 0;
}

BOOL PacketProc_MonsterGetDamage(DWORD Sessionid, std::shared_ptr<Packet> pack)
{
    DWORD sessionID;
    int monsterid;
    int damage;
    BYTE currentScene;


    *pack >> sessionID;
    *pack >> monsterid;
    *pack >> damage;
    *pack >> currentScene;

    auto scene = ServerSceneMgr::GetInstance().InsertScene(currentScene);

    auto monster = scene->GetMonsterData(monsterid);

     monster->SetHP(monster->GetHP() - damage);


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
    player->SetPos({ 0,-300,0 });
    SceneChangePacket(SendPack, dwSessionID, Scenenum);

    IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[dwSessionID].get(), SendPack);

    //현재씬과 이전씬에서 플레이어 목록삭제 및 추가
    auto BeforeScene = ServerSceneMgr::GetInstance().InsertScene(beforeScenenum);
    auto curScene = ServerSceneMgr::GetInstance().InsertScene(Scenenum);

    BeforeScene->DeleteScenePlayer(player);
    curScene->AddScenePlayer(player);

    IOCPServer::GetInstance().Broadcasting({ SendPack,Scenenum }, SessionMgr::GetInstance().GetUserList()[dwSessionID]);
    IOCPServer::GetInstance().Broadcasting({ SendPack,beforeScenenum }, SessionMgr::GetInstance().GetUserList()[dwSessionID]);
    //IOCPServer::GetInstance().AddPacket(SendPack, beforeScenenum);
    //IOCPServer::GetInstance().AddPacket(SendPack, Scenenum);

    int iId = 0;
    for (auto& monster : curScene->GetSceneMonsterList())
    {
        std::shared_ptr<Packet> pack = std::make_shared<Packet>();
        CreateMonster(pack, iId++,monster->GetName(), 0, monster->GetCollisionData().GetPos().x, monster->GetCollisionData().GetPos().y, monster->GetMaxHP(), Scenenum);
        IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[dwSessionID].get(), pack);
    }
    for (auto& obj : curScene->GetSceneObjectList())
    {
        std::shared_ptr<Packet> pack = std::make_shared<Packet>();
        SpawnObjectPacket(pack, obj->GetPosX(), obj->GetPosY(), obj->GetRotate(), obj->GetName(), obj->GetObjectType(), Scenenum);
        IOCPServer::GetInstance().SendPacket(SessionMgr::GetInstance().GetUserList()[dwSessionID].get(), pack);
    }

    std::cout << "[" << dwSessionID << "]" << "Recv PlayerSceneChange" << "\n";
    return 0;
}
 