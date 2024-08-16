#pragma once
#include "SpriteObject.h"
class PotalObject :public SpriteObject
{
private:
	int m_iNextSceneNum;



public:
	virtual int GetNextSceneNum()const override{ return m_iNextSceneNum; }
	virtual void SetNextSceneNum(int num) override { m_iNextSceneNum = num; }



public:
	PotalObject();
	virtual ~PotalObject();






};

