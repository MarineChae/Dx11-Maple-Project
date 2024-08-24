#include "PlayerObject.h"
#include "Input.h"
#include "Timer.h"
#include "ClientNet.h"
#include"Texture.h"
#include "MakePacket.h"
#include "Collider.h"
#include "Collision.h"
#include"SKill.h"
#include"Camera.h"

std::vector<std::shared_ptr<Packet>> SendPacketList;

bool PlayerObject::Init()
{
    SpriteObject::Init();
    GetCollider()->Init();
    m_bIsFalling = false;
    
    return true;
}

bool PlayerObject::Frame()
{
    SpriteObject::Frame();


   if (GetDestination() != GetTransform())//&& ObejctMgr::GetInstance().GetPlayerObject().get() != this)
   {
       auto tr = GetTransform();
       auto des = GetDestination();
       SetTransform(GetTransform().Lerp(GetTransform(), GetDestination(), static_cast<float>(Timer::GetInstance().GetSecPerFrame())));
       SetTransform(GetTransform().SmoothStep(GetTransform(), GetDestination(), 0.08f));
      
   }
   if (GetJumping())
   {
       if (GetTransform().y - m_vBeforePos.y > fabs(200.0f))
       {
           m_vBeforePos = TVector3::Zero;
           SetJumping(false);
           SetFalling(true);
       }
   }


    if (ObejctMgr::GetInstance().GetPlayerObject().get() == this && Input::GetInstance().IsActive())
    {
        InputKey();
        InputAction();

    }
    if (ObejctMgr::GetInstance().GetPlayerObject().get() == this)
    {
        PacketSendProc();
    }

   
    //for (auto& obj : ObejctMgr::GetInstance().GetObjectList())
    //{
    //    if (obj != nullptr && ObejctMgr::GetInstance().GetPlayerObject() != obj)
    //    {
    //        obj->m_vRotate.z += Timer::GetInstance().GetSecPerFrame();
    //        obj->GetCollider()->m_vRotate.z = obj->m_vRotate.z;
    //    }
    //       
    //}
    //구조정리
    GetCollider()->SetTransform(GetTransform());
    GetCollider()->Frame();

    for (auto& skill : m_vSkillList)
    {
        skill.second->Frame();
        skill.second->GetCollider()->SetTransform(skill.second->GetTransform());
        skill.second->GetCollider()->Frame();
    }

    return true;
}

bool PlayerObject::Render()
{
    SpriteObject::Render();

    for (auto& skill : m_vSkillList)
    {
        skill.second->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
        skill.second->Render();
        skill.second->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
        skill.second->GetCollider()->Render();
    }

  
    return true;
}

bool PlayerObject::Release()
{
    SpriteObject::Release();

    return true;
}
void PlayerObject::InputKey()
{
    DWORD dwAction = ACTION_STAND;

    if (Input::GetInstance().GetKeyState('W') >= KEY_PUSH && m_bOnLope)
    {
        dwAction = ACTION_MOVEUP;
    }
    else if (Input::GetInstance().GetKeyState('D') >= KEY_PUSH)
    {
        dwAction = ACTION_MOVERIGHT;
    }
    else if (Input::GetInstance().GetKeyState('A') >= KEY_PUSH)
    {
        dwAction = ACTION_MOVELEFT;
    }
    else if (Input::GetInstance().GetKeyState('S') >= KEY_PUSH && m_bOnLope)
    {
        dwAction = ACTION_MOVEDOWN;
    }
    else
    {
        dwAction = ACTION_STAND;
    }
    if (m_bIsFalling)
    {
        dwAction += ACTION_FALL;
    }
    else if (Input::GetInstance().GetKeyState(VK_SPACE) >= KEY_PUSH || m_bIsJump)
    {
        m_vBeforePos = GetTransform();
        dwAction += ACTION_JUMP;
    }

    if (Input::GetInstance().GetKeyState('Z') >= KEY_PUSH)
    {

        m_pActivateSkill = SkillMgr::GetInstance().GetSkill("00001");
        m_vSkillList.insert({ m_pActivateSkill->GetSkillName(), m_pActivateSkill });
        if(GetDirection()>0)
            m_pActivateSkill->SetTransform(GetTransform() + (m_pActivateSkill->GetOffset() * 1));
        else
        {
            TVector3 temp{ m_pActivateSkill->GetOffset().x * -1 ,m_pActivateSkill->GetOffset().y ,1 };
            m_pActivateSkill->SetTransform(GetTransform() + temp);

        }
        ChangeState(PLAYER_STATE::PS_ATTACK);
        m_pActivateSkill->SetDirection(GetDirection());
        m_pActivateSkill->SetEnable(true);
        dwAction = ACTION_ATTACK;
    }




    m_dwActionInput = dwAction;

}

void PlayerObject::PacketSendProc()
{
    std::shared_ptr<Packet> SendPacket = std::make_shared<Packet>();

    switch (m_dwCurrentAction)
    {

    case ACTION_STAND:

        MoveStopPacket(SendPacket, GetDirection(), GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(),(BYTE)m_bIsFalling,(BYTE)m_bIsJump,(BYTE)m_bOnLope, (BYTE)2);
        ChangeState(PLAYER_STATE::PS_STAND);

        break;


    case ACTION_MOVELEFT:
    case ACTION_MOVERIGHT:
        MoveStartPacket(SendPacket, GetDirection(), GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(),(BYTE)m_bIsFalling, (BYTE)m_bIsJump,(BYTE)m_bOnLope, (BYTE)2);
        ChangeState(PLAYER_STATE::PS_WALK);

        break;
    case ACTION_MOVEUP:
        MoveStartPacket(SendPacket, GetDirection(), GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, (BYTE)m_bOnLope,(BYTE)1);
        ChangeState(PLAYER_STATE::PS_ONLOPE);
        break;
    case ACTION_MOVEDOWN:
        MoveStartPacket(SendPacket, GetDirection(), GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, (BYTE)m_bOnLope, (BYTE)0);
        ChangeState(PLAYER_STATE::PS_ONLOPE);
        break;
    case ACTION_STANDJUMP:
        MoveStopPacket(SendPacket, GetDirection(), GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, (BYTE)m_bOnLope, (BYTE)2);
        ChangeState(PLAYER_STATE::PS_JUMP);

        break;
    case ACTION_MOVELEFT_JUMP:
    case ACTION_MOVERIGHT_JUMP:
    case ACTION_MOVELEFT_FALL:
    case ACTION_MOVERIGHT_FALL:
        MoveStartPacket(SendPacket, GetDirection(), GetObejctID(),
            (short)GetTransform().x,
            (short)GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, (BYTE)m_bOnLope, (BYTE)2);
        ChangeState(PLAYER_STATE::PS_JUMP);
        break;
    
    case ACTION_ATTACK:
        MoveStopPacket(SendPacket, GetDirection(), GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, (BYTE)m_bOnLope, (BYTE)2);
        ChangeState(PLAYER_STATE::PS_ATTACK);
        break;
    }
    if (m_bOnLope)
    {
        ChangeState(PLAYER_STATE::PS_ONLOPE);
    }

    static double sendtime = 0;
    sendtime += Timer::GetInstance().GetSecPerFrame();

    if (sendtime >= 0.0625)
    {
        NetSendPacket(SendPacket);
        sendtime -= 0.0625;
    }

}

void PlayerObject::SetPlayerSprite()
{
    std::shared_ptr<SpriteData> stand = std::make_shared<SpriteData>();
    stand->iCol = 3;
    stand->iRow = 1;
    stand->iMaxImageCount = 3;
    stand->m_fDelay = 0.5f;
    stand->m_vScale = { 46,68,1 };
    SetSpriteInfo(stand);
    Create(L"../resource/Player/PStand.png", L"../Shader/Defalutshader.hlsl");

    std::shared_ptr<SpriteData> walk = std::make_shared<SpriteData>();
    walk->iCol = 4;
    walk->iRow = 1;
    walk->iMaxImageCount = 4;
    walk->m_fDelay = 0.18f;
    walk->m_vScale = { 46,68,1 };
    SetSpriteInfo(walk);
    Create(L"../resource/Player/PWalk.png", L"../Shader/Defalutshader.hlsl");

    std::shared_ptr<SpriteData> jump = std::make_shared<SpriteData>();
    jump->iCol = 1;
    jump->iRow = 1;
    jump->iMaxImageCount = 1;
    jump->m_fDelay = 0.18f;
    jump->m_vScale = { 46,68,1 };
    SetSpriteInfo(jump);
    Create(L"../resource/Player/PJump.png", L"../Shader/Defalutshader.hlsl");

    std::shared_ptr<SpriteData> lope = std::make_shared<SpriteData>();
    lope->iCol = 2;
    lope->iRow = 1;
    lope->iMaxImageCount = 2;
    lope->m_fDelay = 0.25f;
    lope->m_vScale = { 46,68,1 }; 
    SetSpriteInfo(lope);
    Create(L"../resource/Player/POnLope.png", L"../Shader/Defalutshader.hlsl");

    std::shared_ptr<SpriteData> fist = std::make_shared<SpriteData>();
    fist->iCol = 5;
    fist->iRow = 2;
    fist->iMaxImageCount = 10;
    fist->m_fDelay = 0.07f;
    fist->m_vScale = { 78,68,1 };
    SetSpriteInfo(fist);
    Create(L"../resource/Player/PFistEnrange.png", L"../Shader/Defalutshader.hlsl");




    GetCollider()->SetTransform(GetTransform());
    GetCollider()->SetScale(TVector3(46, 68, 1));
    GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
}

void PlayerObject::ChangeState(PLAYER_STATE state)
{
    if (m_PlayerState == state)
        return;
    if (m_pActivateSkill!=nullptr && m_pActivateSkill->GetEnable())
        return;

    m_PlayerState = state;
    InitTexIndex();
    SetSpriteInfo(GetSpriteData(state));
    SetScale(GetCurrentSpriteInfo()->m_vScale);
    SetTexture(GetCurrentSpriteInfo()->m_pTexture);
}

void PlayerObject::testfunc()
{
    int a = 0;
}

void PlayerObject::SetState(PLAYER_STATE state)
{
    m_PlayerState = state;
}

void PlayerObject::InputAction()
{
    m_dwBeforeAction = m_dwCurrentAction;


    if (ACTION_MOVELEFT == m_dwActionInput)
    {
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(0);
    }
    if (ACTION_MOVERIGHT == m_dwActionInput)
    {
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(1);
    }
    if (ACTION_MOVEUP == m_dwActionInput)
    {
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_MOVEDOWN == m_dwActionInput)
    {
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_STAND == m_dwActionInput)
    {
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_STANDJUMP == m_dwActionInput)
    {
        m_vBeforePos = GetTransform();
        m_bIsJump = true;
        m_bIsFalling = false;
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_MOVELEFT_JUMP == m_dwActionInput)
    {
        m_bIsJump = true;
        m_bIsFalling = false;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(0);
    }
    if (ACTION_MOVERIGHT_JUMP == m_dwActionInput)
    {
        m_bIsJump = true;
        m_bIsFalling = false;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(1);
    }
    if (ACTION_MOVELEFT_FALL == m_dwActionInput)
    {
          m_bIsFalling = true;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(0);
    }
    if (ACTION_MOVERIGHT_FALL == m_dwActionInput)
    {
        m_bIsFalling = true;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(1);
    }
    if (ACTION_ATTACK == m_dwActionInput)
    {
        m_dwCurrentAction = m_dwActionInput;



    }


}


PlayerObject::PlayerObject()
    : SpriteObject()
    , m_dwCurrentAction(ACTION_STAND)
    , m_dwBeforeAction(ACTION_STAND)
    , m_iHP(0)
    , m_CurrentiDirection(0)
    , m_BeforeDirection(0)
    , m_bIsPlayable(true)
    , m_dwActionInput(0)
    , m_PlayerState(PS_DEFAULT)
    , m_bIsFalling(true)
    , m_bIsJump(false)
    , m_vBeforePos()
    , m_CurrentScene(SceneNum::Lobby)
    , m_bOnLope(false)
{


}

PlayerObject::~PlayerObject()
{
}
