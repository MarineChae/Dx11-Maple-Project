#include "Object.h"
#include "PacketProc.h"
#include"PlayerObject.h"
#include"MonsterObject.h"
#include"Collider.h"
#include"texture.h"
#include"Scene.h"
#include"SaveLoader.h"
#include"SKill.h"
#include"DropObject.h"
#include"SoundMgr.h"
#include"MakePacket.h"
extern float MapSizeX;
extern float MapSizeY;
BOOL PacketProc_MoveStart(std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    float fX;
    float fY;
    int iHP;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> iHP;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<PlayerObject> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr)//|| obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    if (obj != ObejctMgr::GetInstance().GetPlayerObject())
    {
        obj->SetHp(iHP);
        obj->ChangeState(state);
    }
    obj->SetDestination(TVector3(fX, fY, 0.0f));
    obj->SetDirection(byDirection);
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
    int iHP;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state;
    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> iHP;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<PlayerObject> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr )//|| obj == ObejctMgr::GetInstance().GetPlayerObject())
        return FALSE;

    if (obj != ObejctMgr::GetInstance().GetPlayerObject())
    {
        obj->SetHp(iHP);
        obj->ChangeState(state);
    }
    obj->SetDestination(TVector3(fX, fY, 0));
    obj->SetDirection(byDirection);
    obj->SetFalling(isFalling);
      obj->SetJumping(isJump);
    return TRUE;
}

BOOL PacketProc_Attack(std::shared_ptr<Packet> pack)
{
    DWORD dwSessionID;
    int namelen;
    char name[80] = { 0, };
    int Skillnumlen;
    char Skillnum[80] = { 0, };
    float fX;
    float fY;
    BYTE isFalling;
    BYTE isJump;
    PLAYER_STATE state = PLAYER_STATE::PS_DEFAULT;

    *pack >> namelen;
    pack->GetData(name, namelen);
    *pack >> Skillnumlen;
    pack->GetData(Skillnum, Skillnumlen);
    *pack >> dwSessionID;
    *pack >> fX;
    *pack >> fY;
    *pack >> isFalling;
    *pack >> isJump;
    *pack >> state;

    std::shared_ptr<PlayerObject> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);
    
    if (obj == nullptr)
        return FALSE;
    if (obj != ObejctMgr::GetInstance().GetPlayerObject())
    {
        obj->ChangeState(state);
    }
    obj->SetDestination(TVector3(fX, fY, 0));
    obj->SetFalling(isFalling);

    obj->SetJumping(isJump);
  
    auto skill = obj->FindSkillMap(Skillnum);
    if (skill == nullptr)
    {
        skill = SkillMgr::GetInstance().LoadSkill(Skillnum);
        obj->InsertSkill(skill);
    }
    skill->SetEnable(true);
    skill->SetDirection(obj->GetDirection());
    //if (obj->GetDirection() > 0)
    //{
    //    skill->SetTransform(obj->GetTransform() + (skill->GetOffset() * 1));

    //}
    //else
    //{
    //    TVector3 temp{ skill->GetOffset().x * -1 , skill->GetOffset().y ,1 };
    //    skill->SetTransform(obj->GetTransform() + temp);

    //}
    obj->SetActivateSkill(skill);

   

    return 0;
}

BOOL PacketProc_SceneChange(std::shared_ptr<Packet> pack)
{


    DWORD dwSessionID;
    BYTE Scenenum;

    *pack >> dwSessionID;
    *pack >> Scenenum;
     
    std::shared_ptr<PlayerObject> obj = ObejctMgr::GetInstance().GetOtherObject(dwSessionID);

    if (obj == nullptr)
        return FALSE;


    auto scene = SceneMgr::GetInstance().GetScene(Scenenum);
    if (obj == ObejctMgr::GetInstance().GetPlayerObject())
    {
        SceneMgr::GetInstance().GetCurrentScene()->GetBGM()->SoundStop();
        SceneMgr::GetInstance().SetCurrentScene(scene);
        MapSizeX = scene->GetMap()->GetTexture()->GetWidth();
        MapSizeY = scene->GetMap()->GetTexture()->GetHeight();
        scene->GetBGM()->SoundPlay(true);
    }

    obj->SetTransform({ 0,0,0 });
    obj->SetCurrentScene((SceneNum)Scenenum);
    scene->PushPlayer(obj);

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
    obj->SetResponPos({ X,Y,0 });
    obj->SetMaxHp(HP);
    obj->SetHp(HP);
    obj->GetCollider()->SetTransform(obj->GetTransform());
    obj->GetCollider()->SetScale(obj->GetScale());
    obj->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
    obj->SetID(ID);
    //id관련 추가해야함
    SceneMgr::GetInstance().GetScene(CurrentScene)->PushMonster(obj);


    return 0;
}
BOOL PacketProc_CreateObject(std::shared_ptr<Packet> pack)
{
    char name[80];
    int namelen;
    float X;
    float Y;
    float rotate;
    ObejctType type;
    BYTE CurrentScene;

    *pack >> namelen;
    pack->GetData(name, namelen);
    *pack >> X;
    *pack >> Y;
    *pack >> rotate;
    *pack >> type;
    *pack >> CurrentScene;


    std::wstring ws(name, &name[namelen]);
    std::shared_ptr<SpriteObject> obj;
    if (type == ObejctType::FALLING_OBJECT)
    {
        obj = std::make_shared<DropObject>();
        SceneMgr::GetInstance().GetScene(CurrentScene)->PushInteractObjectList(obj);
    }
    else if (type == ObejctType::LAZER_OBJECT || type == ObejctType::COLLIDER)
    {
        obj = std::make_shared<SpriteObject>();
        SceneMgr::GetInstance().GetScene(CurrentScene)->PushObject(obj);
    }

    obj->m_vRotate.z = rotate;
    obj->SetObejctType(type);
    obj->Init();
    SaveLoadMgr::GetInstance().GetSaveLoader().LoadObjectData(obj, wtm(ws));
    obj->SetTransform({ X,Y,0 });

    obj->SetObjectState(OB_WARNING);
    obj->SetSpriteInfo(obj->GetSpriteData(obj->GetObjectState()));
    obj->SetScale(obj->GetCurrentSpriteInfo()->m_vScale);
    obj->SetTexture(obj->GetCurrentSpriteInfo()->m_pTexture);
    //obj->SetPlacedScene(CurrentScene);

    obj->GetCollider()->SetTransform(obj->GetTransform());
    obj->GetCollider()->SetScale(obj->GetScale());
    obj->GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
    obj->GetCollider()->m_vRotate.z = rotate;
    //id관련 추가해야함



  

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
    BYTE isdead;
    MONSTER_STATE state;
    *pack >> ID;
    *pack >> byDirection;
    *pack >> fX;
    *pack >> fY;
    *pack >> HP;
    *pack >> CurrentScene;
    *pack >> isdead;
    *pack >> state;

    auto list = SceneMgr::GetInstance().GetScene(CurrentScene)->GetMonsterList();
    if (list.empty())
        return -1;
    auto& monster = list[ID];

    monster->SetDestination({ fX,fY,0 });
    monster->SetCurrentScene(CurrentScene);
    if (monster->GetIsDead() && !isdead)
    {
        monster->SetTransform(monster->GetResponPos());
    }
    monster->SetHp(HP);
    monster->SetIsDead(isdead);
    monster->ChangeMonsterState(state);
    monster->SetID(ID);

    monster->SetDirection(byDirection);
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


    std::shared_ptr<PlayerObject> player = std::make_shared<PlayerObject>();
    player->Init();
    player->SetPlayerSprite();
    player->SetTransform(TVector3(fX,fY,0));
    player->SetDestination(TVector3(fX, fY, 0));
    player->SetRenderState(true);
    player->SetObejctID(dwSessionID);
    player->SetCurrentScene((SceneNum)CurrentScene);
    player->SetMaxHp(HP);
    player->SetHp(HP);
    player->ChangeState(PLAYER_STATE::PS_JUMP);
    ObejctMgr::GetInstance().PushObject(player, dwSessionID);
    ObejctMgr::GetInstance().SetPlayerObject(player);

    auto scene = SceneMgr::GetInstance().GetScene(CurrentScene);

    
    SceneMgr::GetInstance().SetCurrentScene(scene);
    MapSizeX = scene->GetMap()->GetTexture()->GetWidth();
    MapSizeY = scene->GetMap()->GetTexture()->GetHeight();
    scene->GetBGM()->SoundPlay(true);
    
    player->SetTransform({ 0,0,0 });
    player->SetCurrentScene((SceneNum)CurrentScene);
    scene->PushPlayer(player); 

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


    std::shared_ptr<PlayerObject> other = std::make_shared<PlayerObject>();

    other->Init();
    other->SetPlayerSprite();
    other->SetTransform(TVector3(fX, fY, 0));
    other->SetDestination(TVector3(fX, fY, 0));
    other->SetRenderState(true);
    other->SetObejctID(dwSessionID);
    other->SetCurrentScene((SceneNum)CurrentScene);
    other->ChangeState(PLAYER_STATE::PS_JUMP);
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
