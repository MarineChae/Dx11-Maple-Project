
#include "PlayerObject.h"
#include "Input.h"
#include "Timer.h"
#include "ClientNet.h"
#include"Texture.h"
#include "MakePacket.h"
#include "Collider.h"
#include "Collision.h"
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


    if (GetDestination() != GetTransform() && ObejctMgr::GetInstance().GetPlayerObject().get() != this)
    {
        SetTransform(GetTransform().Lerp(GetTransform(), GetDestination(), static_cast<float>(Timer::GetInstance().GetSecPerFrame())));
        SetTransform(GetTransform().SmoothStep(GetTransform(), GetDestination(), 0.05f));
       
    }

    if (m_bIsFalling && !m_bIsJump)
    {
        auto pos = GetTransform();
        pos.y -= static_cast<float>(Timer::GetInstance().GetSecPerFrame() * 800);
        SetTransform(pos);
    }
    if (m_bIsJump )
    {
        auto pos = GetTransform();
        pos.y += static_cast<float>(Timer::GetInstance().GetSecPerFrame() * 800);
        SetTransform(pos);
        if (pos.y - m_vBeforePos.y > fabs(200.0f))
        {
            m_bIsJump = false;
            m_bIsFalling = true;
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


    return true;
}

bool PlayerObject::Render()
{
    SpriteObject::Render();


  
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

    if (Input::GetInstance().GetKeyState('W') >= KEY_PUSH)
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
    else if (Input::GetInstance().GetKeyState('S') >= KEY_PUSH)
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
    else if (Input::GetInstance().GetKeyState(VK_SPACE) >= KEY_PUSH && !m_bIsJump)
    {
        dwAction += ACTION_JUMP;
    }
    m_dwActionInput = dwAction;

}

void PlayerObject::PacketSendProc()
{
    Packet SendPacket;

    switch (m_dwCurrentAction)
    {

    case ACTION_STAND:
        MoveStopPacket(&SendPacket, GetDirection(), GetObejctID(),
            (short)GetTransform().x,
            (short)GetTransform().y,
            GetPlayerState(),(BYTE)m_bIsFalling);
        ChangeState(PLAYER_STATE::PS_STAND);
        OutputDebugString(L"stop\n");
        break;


    case ACTION_MOVELEFT:
    case ACTION_MOVERIGHT:
    case ACTION_MOVEUP: 
    case ACTION_MOVEDOWN:
        MoveStartPacket(&SendPacket, GetDirection(), GetObejctID(),
            (short)GetTransform().x,
            (short)GetTransform().y,
            GetPlayerState(),(BYTE)m_bIsFalling);
        ChangeState(PLAYER_STATE::PS_WALK);
        OutputDebugString(L"start\n");
        break;
    case ACTION_STANDJUMP:
    case ACTION_MOVELEFT_JUMP:
    case ACTION_MOVERIGHT_JUMP:
    case ACTION_MOVELEFT_FALL:
    case ACTION_MOVERIGHT_FALL:
        MoveStartPacket(&SendPacket, GetDirection(), GetObejctID(),
            (short)GetTransform().x,
            (short)GetTransform().y,
            GetPlayerState(), (BYTE)m_bIsFalling);
        ChangeState(PLAYER_STATE::PS_JUMP);
        break;
    
    }

    static double sendtime = 0;
    sendtime += Timer::GetInstance().GetSecPerFrame();

    if (sendtime >= 0.0625)
    {
        NetSendPacket(&SendPacket);
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





    GetCollider()->SetTransform(GetTransform());
    GetCollider()->SetScale(TVector3(46, 68, 1));
    GetCollider()->Create(L" ", L"../Shader/LineDebug.hlsl");
}

void PlayerObject::ChangeState(PLAYER_STATE state)
{
    if (m_PlayerState == state)
        return;

    m_PlayerState = state;
    InitTexIndex();
    SetSpriteInfo(GetSpriteData(state));
    SetScale(GetCurrentSpriteInfo()->m_vScale);
    SetTexture(GetCurrentSpriteInfo()->m_pTexture);
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
        m_pMovePow.x = min(m_pMovePow.x, 200);
        m_pMovePow.x -= static_cast<float>(1000 * Timer::GetInstance().GetSecPerFrame());
        m_pMovePow.x = max(m_pMovePow.x, -500);
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(0);
    }
    if (ACTION_MOVERIGHT == m_dwActionInput)
    {
        m_pMovePow.x = max(m_pMovePow.x, -200);
        m_pMovePow.x += static_cast<float>(1000 * Timer::GetInstance().GetSecPerFrame());
        m_pMovePow.x = min(m_pMovePow.x, 500);
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
        m_pMovePow = m_pMovePow.Lerp(m_pMovePow, TVector3::Zero, 0.005f);
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
        m_pMovePow.x = min(m_pMovePow.x, 200);
        m_pMovePow.x -= static_cast<float>(1000 * Timer::GetInstance().GetSecPerFrame());
        m_pMovePow.x = max(m_pMovePow.x, -500);
        m_vBeforePos = GetTransform();
        m_bIsJump = true;
        m_bIsFalling = false;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(0);
    }
    if (ACTION_MOVERIGHT_JUMP == m_dwActionInput)
    {
        m_pMovePow.x = max(m_pMovePow.x, -200);
        m_pMovePow.x += static_cast<float>(1000 * Timer::GetInstance().GetSecPerFrame());
        m_pMovePow.x = min(m_pMovePow.x, 500);
        m_vBeforePos = GetTransform();
        m_bIsJump = true;
        m_bIsFalling = false;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(1);
    }
    if (ACTION_MOVELEFT_FALL == m_dwActionInput)
    {
        m_pMovePow.x = min(m_pMovePow.x, 200);
        m_pMovePow.x -= static_cast<float>(1000 * Timer::GetInstance().GetSecPerFrame());
        m_pMovePow.x = max(m_pMovePow.x, -500);
          m_bIsFalling = true;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(0);
    }
    if (ACTION_MOVERIGHT_FALL == m_dwActionInput)
    {
        m_pMovePow.x = max(m_pMovePow.x, -200);
        m_pMovePow.x += static_cast<float>(1000 * static_cast<float>(Timer::GetInstance().GetSecPerFrame()));
        m_pMovePow.x = min(m_pMovePow.x, 500);
        m_bIsFalling = true;
        m_dwCurrentAction = m_dwActionInput;
        SetDirection(1);
    }

    
    TVector3 pos = GetTransform();
    pos = TVector3::Lerp(pos , pos + m_pMovePow , static_cast<float>(Timer::GetInstance().GetSecPerFrame()));
    SetTransform(pos);
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
{


}

PlayerObject::~PlayerObject()
{
}
