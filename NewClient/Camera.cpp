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
    m_ProjectionMatrix._11 = 1.0f / ((float)MyWindow::GetWindowStyle().m_dwWindowWidth)* m_fZoomScale;// *Zoom;
    m_ProjectionMatrix._22 = 1.0f / ((float)MyWindow::GetWindowStyle().m_dwWindowHeight)* m_fZoomScale;// *Zoom;
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
void Camera::ControlAngle(float WindowsizeX, float WindowsizeY, float MapSizeX, float MapSizeY)
{
    if (m_vCameraPos.x <= -(MapSizeX)+(WindowsizeX))
    {

        m_vCameraPos.x = -(MapSizeX)+(WindowsizeX);

    }
    if (m_vCameraPos.y <= -(MapSizeY)+(WindowsizeY))
    {

        m_vCameraPos.y = -(MapSizeY)+(WindowsizeY);

    }
    if (m_vCameraPos.x >= (MapSizeX)-(WindowsizeX))
    {

        m_vCameraPos.x = (MapSizeX)-(WindowsizeX);

    }
    if (m_vCameraPos.y >= (MapSizeY)-(WindowsizeY))
    {

        m_vCameraPos.y = (MapSizeY)-(WindowsizeY);

    }
}
