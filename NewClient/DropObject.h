#pragma once
#include"SpriteObject.h"
class DropObject :public SpriteObject
{

private:
	int m_iPlacedScene;

public:
	virtual bool Frame();


	void SetPlacedScene(int scene) { m_iPlacedScene = scene;}




public:
	DropObject();
	virtual ~DropObject();

};

