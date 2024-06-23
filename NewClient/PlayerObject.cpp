#include "PlayerObject.h"
#include "Input.h"
#include "Timer.h"
#include "ClientNet.h"
#include "MakePacket.h"
bool PlayerObject::Init()
{
    Object::Init();


    return true;
}

bool PlayerObject::Frame()
{
    Object::Frame();
    if (ObejctMgr::GetInstance().GetPlayerObject().get() == this && Input::GetInstance().IsActive())
    {
        InputKey();
        InputAction();

    }

    if (m_vDestination != m_vTransform && ObejctMgr::GetInstance().GetPlayerObject().get() != this)
    {
        
        m_vTransform = m_vTransform.Lerp(m_vTransform, m_vDestination, Timer::GetInstance().GetSecPerFrame());
        m_vTransform = m_vTransform.SmoothStep(m_vTransform, m_vDestination, 0.05f);

    }
  

    return true;
}

bool PlayerObject::Render()
{
    Object::Render();

    return true;
}

bool PlayerObject::Release()
{
    Object::Release();

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


    m_dwActionInput = dwAction;

}

void PlayerObject::InputAction()
{
    m_dwBeforeAction = m_dwCurrentAction;


    if (ACTION_MOVELEFT == m_dwActionInput)
    {
        TVector3 pos = GetTransform();
        pos.x -= 1000 * Timer::GetInstance().GetSecPerFrame();
        SetTransform(pos);
        m_dwCurrentAction = m_dwActionInput;
      
    }
    if (ACTION_MOVERIGHT == m_dwActionInput)
    {
        TVector3 pos = GetTransform();
        pos.x += 1000 * Timer::GetInstance().GetSecPerFrame();
        SetTransform(pos);
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_MOVEUP == m_dwActionInput)
    {
        TVector3 pos = GetTransform();
        pos.y += 1000 * Timer::GetInstance().GetSecPerFrame();
        SetTransform(pos);
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_MOVEDOWN == m_dwActionInput)
    {
        TVector3 pos = GetTransform();
        pos.y -= 1000 * Timer::GetInstance().GetSecPerFrame();
        SetTransform(pos);
        m_dwCurrentAction = m_dwActionInput;
    }
    if (ACTION_STAND == m_dwActionInput)
    {

        m_dwCurrentAction = m_dwActionInput;
    }

    Packet SendPacket;

    switch (m_dwCurrentAction)
    {
    case ACTION_STAND:
        MoveStopPacket(&SendPacket, (BYTE)99, GetObejctID(), (short)GetTransform().x, (short)GetTransform().y);
        OutputDebugString(L"stop\n");
        break;


    case ACTION_MOVELEFT:
    case ACTION_MOVERIGHT: 
    case ACTION_MOVEUP:
    case ACTION_MOVEDOWN:
        MoveStartPacket(&SendPacket, (BYTE)99, GetObejctID(),(short)GetTransform().x, (short)GetTransform().y);
        OutputDebugString(L"start\n");
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


PlayerObject::PlayerObject()
    : Object()
    , m_dwCurrentAction(ACTION_STAND)
    , m_dwBeforeAction(ACTION_STAND)
    , m_iHP(0)
    , m_CurrentiDirection(0)
    , m_BeforeDirection(0)
    , m_bIsPlayable(true)
    , m_dwActionInput(0)
{


}

PlayerObject::~PlayerObject()
{
}
