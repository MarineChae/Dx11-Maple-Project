#pragma once
#include"SpriteObject.h"

class BallObject :public SpriteObject
{

private:
	int m_iPlacedScene;
	float m_fLifeTime;
	TVector3 m_vDir;
public:
	virtual bool Frame();


	void SetPlacedScene(int scene) { m_iPlacedScene = scene; }




public:
	BallObject();
	virtual ~BallObject();

};


