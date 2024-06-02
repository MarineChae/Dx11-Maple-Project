#include "Camera.h"
#include"MyWindow.h"
bool Camera::CreateCamera(TVector3 pos, TVector2 size)
{
    m_vCameraPos = pos;

    return false;
}

bool Camera::Init()
{
    return false;
}

bool Camera::Frame()
{


    m_ViewMatrix._41 = -m_vCameraPos.x;
    m_ViewMatrix._42 = -m_vCameraPos.y;
    m_ViewMatrix._43 = -m_vCameraPos.z;
    m_ProjectionMatrix._11 = 1.0f / ((float)MyWindow::GetWindowStyle().m_dwWindowWidth);// *Zoom;
    m_ProjectionMatrix._22 = 1.0f / ((float)MyWindow::GetWindowStyle().m_dwWindowHeight);// *Zoom;
    return false;
}

bool Camera::Render()
{
    return false;
}

bool Camera::Release()
{
    return false;
}
