#pragma once
#include"pch.h"

class Camera
{

private:
	TVector3 m_vCameraPos;
	TMatrix m_ViewMatrix;
	TMatrix m_ProjectionMatrix;
	float Zoom = 1;
public:
	TMatrix& GetView() { return m_ViewMatrix; };
	TMatrix& GetProjection() { return m_ProjectionMatrix; };
	void SetCameraPos(TVector3 pos) { m_vCameraPos = pos; }
	TVector3 GetCameraPos() { return m_vCameraPos; };
	void AddZoom(float value) { Zoom += value; };
	float GetZoomValue() const { return Zoom; };
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
	void SetCamera(Camera& camera) { m_MainCamera = camera; };
	Camera& GetCamera() { return m_MainCamera; };

};

