#include "Object.h"
#include "PacketProc.h"
#include"PlayerObject.h"
#include"MonsterObject.h"
#include"Collider.h"
#include"texture.h"
#include"Scene.h"
#include"SaveLoader.h"
BOOL PacketProc_MoveStart(std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr)//|| obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    //obj->SetTransform((TVector3(fX, fY, 1)));
    obj->SetDestination(TVector3(fX, fY, 0.0f));
    obj->SetDirection(byDirection);
    obj->ChangeState(state);
    obj->SetFalling(isFalling);
    obj->SetJumping(isJump);
    
    return TRUE;
}

BOOL PacketProc_MoveEnd(std::shared_ptr<Packet> pack)
{ 
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state;
    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr )//|| obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;
    //obj->SetTransform((TVector3(fX, fY, 1)));
    obj->SetDestination(TVector3(fX, fY, 0));
    obj->SetDirection(byDirection);
    obj->ChangeState(state);
    obj->SetFalling(isFalling);
    obj->SetJumping(isJump);
    return TRUE;
}

BOOL PacketProc_SceneChange(std::shared_ptr<Packet> pack)
{


    DWORD dwSessionID;
    BYTE Scenenum;

    *pack >> dwSessionID;
    *pack >> Scenenum;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr || obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;


    obj->SetCurrentScene((SceneNum)Scenenum);


    return 0;
}

BOOL PacketProc_CreateMonster(std::shared_ptr<Packet> pack)
{
    char name[80];
    int namelen;
    int ID;
    BYTE Direction;
    float X;
    float Y;
    int HP;
    BYTE CurrentScene;

    *pack >> namelen;
    pack->GetData(name, namelen);
    *pack >> ID;
    *pack >> Direction;
    *pack >> X;
    *pack >> Y;
    *pack >> HP;
    *pack >> CurrentScene;


    std::wstring ws(name, &name[namelen]);

    std::shared_ptr<MonsterObject> obj = std::make_shared<MonsterObject>();


    SaveLoadMgr::GetInstance().GetSaveLoader().LoadMonsterData(obj,wtm(ws));
    obj->SetTransform({X,Y,0 });
    obj->GetCollider()->SetTransform(obj->GetTransform());
    obj->GetCollider()->SetScale(obj->GetScale());
    obj->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
    //id관련 추가해야함
    SceneMgr::GetInstance().GetCurrentScene()->PushMonster(obj);


    return 0;
}

BOOL PacketProc_UpdateMonster(std::shared_ptr<Packet> pack)
{
    int ID;
    BYTE byDirection;
    float fX;
    float fY;
    int HP;
    BYTE CurrentScene;

    *pack >> ID;
    *pack >> byDirection;
    *pack >> fX;
    *pack >> fY;
    *pack >> HP;
    *pack >> CurrentScene;

    auto list = SceneMgr::GetInstance().GetCurrentScene()->GetMonsterList();
    auto& monster = list[ID];
    monster->SetDirection(byDirection);
    monster->SetTransform({ fX,fY,0 });
    monster->SetCurrentScene(CurrentScene);

    return 0;
}

BOOL PacketProc_CreateMyCharacter(std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY;
    int  HP;
    BYTE CurrentScene;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> fX;
    *pack >> fY;
    *pack >> HP;
    *pack >> CurrentScene;


    std::shared_ptr<Object> player = std::make_shared<PlayerObject>();
    player->Init();
    player->SetPlayerSprite();
    player->SetTransform(TVector3(fX,fY,0));
    player->SetDestination(TVector3(fX, fY, 0));
    player->SetRenderState(true);
    player->SetObejctID(dwSessionID);
    player->SetCurrentScene((SceneNum)CurrentScene);
    ObejctMgr::GetInstance().PushObject(player, dwSessionID);
    ObejctMgr::GetInstance().SetPlayerObject(player);
   

    return 0;
}

BOOL PacketProc_CreateOtherCharacter(std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY;
    int  HP;
    BYTE CurrentScene;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> fX;
    *pack >> fY;
    *pack >> HP;
    *pack >> CurrentScene;

      if (ObejctMgr::GetInstance().GetPlayerObject()->GetObejctID() == dwSessionID)
        return 0;


    std::shared_ptr<Object> other = std::make_shared<PlayerObject>();

    other->Init();
    other->SetPlayerSprite();
    other->SetTransform(TVector3(fX, fY, 0));
    other->SetDestination(TVector3(fX, fY, 0));
    other->SetRenderState(true);
    other->SetObejctID(dwSessionID);
    other->SetCurrentScene((SceneNum)CurrentScene);
    ObejctMgr::GetInstance().PushObject(other, dwSessionID);



    return 0;
}

BOOL PacketProc_DisconnectOtherCharacter(std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    *pack >> dwSessionID;

    ObejctMgr::GetInstance().DisconnectCharacter(dwSessionID);

    return 0;
}
