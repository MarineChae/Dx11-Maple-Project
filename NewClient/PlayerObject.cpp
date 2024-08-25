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
#include"Scene.h"
#include"DamageIndicator.h"
#include"UI.h"
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

    if (nullptr != m_pActivateSkill && !m_pActivateSkill->GetEnable())
    {
        m_pActivateSkill->SetCanHit(true);
        m_pActivateSkill = nullptr;
        ChangeState(PS_STAND);
    }
   

   if (GetDestination() != GetTransform())//&& ObejctMgr::GetInstance().GetPlayerObject().get() != this)
   {
       auto tr = GetTransform();
       auto des = GetDestination();
       SetTransform(GetTransform().Lerp(GetTransform(), GetDestination(), static_cast<float>(Timer::GetInstance().GetSecPerFrame())));
       SetTransform(GetTransform().SmoothStep(GetTransform(), GetDestination(), 0.10f));
      
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
        SetProgressBar();
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
    if (m_pActivateSkill != nullptr)
    {
        m_pActivateSkill->Frame();
        m_pActivateSkill->GetCollider()->SetTransform(m_pActivateSkill->GetTransform());
        m_pActivateSkill->GetCollider()->Frame();

        if (m_pActivateSkill->GetCanHit())
        {
            for (auto& monster : SceneMgr::GetInstance().GetCurrentScene()->GetMonsterList())
            {
                auto coll = monster->GetCollider();
                if (!monster->GetIsDead())
                {
                    if (Collider::CheckOBBCollision(m_pActivateSkill->GetCollider(), coll))
                    {
                        monster->SetIsHit(true);
                        if (ObejctMgr::GetInstance().GetPlayerObject().get() == this)
                        {
                            std::shared_ptr<Packet> pack = std::make_shared<Packet>();
                            MonsterGetDamagePacket(pack, GetObejctID(), monster->GetID(), 1000000, (BYTE)GetCurrentScene());
                            NetSendPacket(pack);

                        }

                    }

                }

            }
        }

        m_pActivateSkill->SetCanHit(false);
    }

    return true;
}

bool PlayerObject::Render()
{
    SpriteObject::Render();


    if (m_pActivateSkill != nullptr)
    {
        m_pActivateSkill->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
        m_pActivateSkill->Render();
        m_pActivateSkill->GetCollider()->SetMatrix(nullptr, &CameraMgr::GetInstance().GetCamera().GetView(), &CameraMgr::GetInstance().GetCamera().GetProjection());
        m_pActivateSkill->GetCollider()->Render();
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

    if (Input::GetInstance().GetKeyState(VK_UP) >= KEY_PUSH && m_bOnLope)
    {
        dwAction = ACTION_MOVEUP;
    }
    else if (Input::GetInstance().GetKeyState(VK_RIGHT) >= KEY_PUSH)
    {
        dwAction = ACTION_MOVERIGHT;
    }
    else if (Input::GetInstance().GetKeyState(VK_LEFT) >= KEY_PUSH)
    {
        dwAction = ACTION_MOVELEFT;
    }
    else if (Input::GetInstance().GetKeyState(VK_DOWN) >= KEY_PUSH && m_bOnLope)
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
         ChangeState(PLAYER_STATE::PS_ATTACK);

        dwAction = ACTION_ATTACK;
    }
    if(nullptr != m_pActivateSkill && m_pActivateSkill->GetEnable())
        dwAction = ACTION_ATTACK;


    m_dwActionInput = dwAction;

}

void PlayerObject::PacketSendProc()
{
    static auto temp = SkillMgr::GetInstance().GetSkill("00001");
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
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, (BYTE)m_bOnLope, (BYTE)2);
        ChangeState(PLAYER_STATE::PS_JUMP);
        break;
    
    case ACTION_ATTACK:
        AttackPacket(SendPacket, GetObejctID(),
            GetTransform().x,
            GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling, (BYTE)m_bIsJump, temp->GetSkillName().data(), temp->GetSkillNum().data());

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
    fist->m_fDelay = 0.065f;
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

void PlayerObject::SetProgressBar()
{
    ///hp ui 값
    float HPinterval = static_cast<float> (m_iHP) / m_iMaxHP;
    auto hpBar = UIMgr::GetInstance().GetUI()->GetUIObject(UI_DESC::HP);
    hpBar->SetScale({ 171 * HPinterval,13,1 });
    float movebar = (171 * HPinterval);
    hpBar->SetTransform({ -158 + movebar , -700.0f, 1 });

    float MPinterval = static_cast<float> (m_iHP) / m_iMaxHP;
    auto mpBar = UIMgr::GetInstance().GetUI()->GetUIObject(UI_DESC::MP);
    mpBar->SetScale({ 171 * MPinterval,13,1 });
    movebar = (171 * MPinterval);
    mpBar->SetTransform({ -158 + movebar , -730.0f, 1 });
}

void PlayerObject::InsertSkill(std::shared_ptr<Skill> skill)
{
    m_vSkillList.insert({ skill->GetSkillNum() ,skill });
}

void PlayerObject::SetActivateSkill(std::shared_ptr<Skill> skill)
{
    m_pActivateSkill = skill;
}

std::shared_ptr<Skill> PlayerObject::FindSkillMap(std::string num)
{
    auto it = m_vSkillList.find(num);
    if (it != m_vSkillList.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }

     
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
    , m_PlayerState(PS_STAND)
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


void ObejctMgr::DisconnectCharacter(DWORD SessionID)
{
    m_lObjectList[SessionID].reset();
}
