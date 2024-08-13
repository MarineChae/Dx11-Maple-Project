#pragma once
#include"pch.h"
#include"Singleton.h"
class Camera
{

private:
	TVector3 m_vCameraPos;
	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;
	float m_fZoomScale=1.0f;
public:
	TMatrix& GetView() { return m_ViewMatrix; };
	TMatrix& GetProjection() { return m_ProjectionMatrix; };
	void SetCameraPos(TVector3 pos) { m_vCameraPos = pos; }
	void ControlAngle(float WindowsizeX, float WindowsizeY,float MapSizeX ,float MapSizeY);
	void SetZoomScale(float scale) { m_fZoomScale = scale; };
	float GetZoomScale() const { return m_fZoomScale; };
public:
	bool CreateCamera(TVector3 pos, TVector2 size);

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();


};

class CameraMgr : public Singleton<CameraMgr>
{
	friend class Singleton<CameraMgr>;

private:
	Camera m_MainCamera;

public:
	void SetCamera(Camera camera) { m_MainCamera = camera; };
	Camera&  GetCamera() { return m_MainCamera; };

};

