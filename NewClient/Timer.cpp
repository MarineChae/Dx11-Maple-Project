#include "Timer.h"

int Timer::GetFPS()
{
    static int FPS = 0;
    if (m_dFramePerSecond >= 1.0)
    {
        m_dFramePerSecond -= 1.0;
        m_iFps = FPS;
        FPS = 0;
    }
    ++FPS;
  
    return m_iFps;
}

bool Timer::Init()
{
    m_dGameTime = 0.0f;
    m_dSecondPerFrame = 0.0f;
    m_dFramePerSecond = 0.0f;
    m_BeforeTime = std::chrono::high_resolution_clock::now();
    return true;
}

bool Timer::Frame()
{
    auto dwCurrentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> SecondPerFrame = dwCurrentTime - m_BeforeTime;
    m_dSecondPerFrame = SecondPerFrame.count();
    m_dGameTime += m_dSecondPerFrame;
    m_dFramePerSecond += m_dSecondPerFrame;
    m_BeforeTime = dwCurrentTime;

    return true;
}

bool Timer::Render()
{
    return false;
}

bool Timer::Release()
{
    return false;
}
