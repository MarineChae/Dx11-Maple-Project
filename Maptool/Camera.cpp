#include "Camera.h"
#include"MyWindow.h"
bool Camera::CreateCamera(TVector3 pos, TVector2 size)
{
    m_vCameraPos = pos;

    return true;
}

bool Camera::Init()
{
    return true;
}

bool Camera::Frame()
{


    m_ViewMatrix._41 = -m_vCameraPos.x;
    m_ViewMatrix._42 = -m_vCameraPos.y;
    m_ViewMatrix._43 = -m_vCameraPos.z;
    m_ProjectionMatrix._11 = 1.0f / ((float)MyWindow::GetWindowStyle().m_dwWindowWidth) *Zoom;
    m_ProjectionMatrix._22 = 1.0f / ((float)MyWindow::GetWindowStyle().m_dwWindowHeight) *Zoom;
    return true;
}

bool Camera::Render()
{
    return true;
}

bool Camera::Release()
{
    return true;
}
