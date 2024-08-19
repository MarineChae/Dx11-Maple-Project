#include "Object.h"
#include "PacketProc.h"
#include"PlayerObject.h"
#include"MonsterObject.h"
#include"Collider.h"
#include"texture.h"
#include"Scene.h"
#include"SaveLoader.h"
BOOL PacketProc_MoveStart(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> shX;
    *pack >> shY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr|| obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    obj->SetDestination(TVector3(shX, shY, 1));
    obj->SetDirection(byDirection);
    obj->ChangeState(state);
    obj->SetFalling(isFalling);
    obj->SetJumping(isJump);
    
    return TRUE;
}

BOOL PacketProc_MoveEnd(Packet* pack)
{ 
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state;
    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> shX;
    *pack >> shY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<Object> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr || obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    obj->SetDestination(TVector3(shX, shY, 0));
    obj->SetDirection(byDirection);
    obj->ChangeState(state);
    obj->SetFalling(isFalling);
    obj->SetJumping(isJump);
    return TRUE;
}

BOOL PacketProc_SceneChange(Packet* pack)
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

BOOL PacketProc_CreateMonster(Packet* pack)
{
    char name[80];
    int namelen;
    BYTE Direction;
    short X;
    short Y;
    int HP;
    BYTE CurrentScene;

    *pack >> namelen;
    pack->GetData(name, namelen);
    *pack >> Direction;
    *pack >> X;
    *pack >> Y;
    *pack >> HP;
    *pack >> CurrentScene;


    std::wstring ws(name, &name[namelen]);

    std::shared_ptr<MonsterObject> obj = std::make_shared<MonsterObject>();


    SaveLoadMgr::GetInstance().GetSaveLoader().LoadMonsterData(obj,wtm(ws));
    obj->SetTransform({ static_cast<float>(X),static_cast<float>(Y),0 });
    obj->GetCollider()->SetTransform(obj->GetTransform());
    obj->GetCollider()->SetScale(obj->GetScale());
    obj->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");

    SceneMgr::GetInstance().GetCurrentScene()->PushMonster(obj);


    return 0;
}

BOOL PacketProc_CreateMyCharacter(Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;
    int  HP;
    BYTE CurrentScene;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> shX;
    *pack >> shY;
    *pack >> HP;
    *pack >> CurrentScene;


    std::shared_ptr<Object> player = std::make_shared<PlayerObject>();
    player->Init();
    player->SetPlayerSprite();
    player->SetTransform(TVector3(shX,shY,0));
    player->SetDestination(TVector3(shX, shY, 0));
    player->SetRenderState(true);
    player->SetObejctID(dwSessionID);
    player->SetCurrentScene((SceneNum)CurrentScene);
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
    BYTE CurrentScene;

    *pack >> dwSessionID;
    *pack >> byDirection;
    *pack >> shX;
    *pack >> shY;
    *pack >> HP;
    *pack >> CurrentScene;

      if (ObejctMgr::GetInstance().GetPlayerObject()->GetObejctID() == dwSessionID)
        return 0;


    std::shared_ptr<Object> other = std::make_shared<PlayerObject>();

    other->Init();
    other->SetPlayerSprite();
    other->SetTransform(TVector3(shX, shY, 0));
    other->SetDestination(TVector3(shX, shY, 0));
    other->SetRenderState(true);
    other->SetObejctID(dwSessionID);
    other->SetCurrentScene((SceneNum)CurrentScene);
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
