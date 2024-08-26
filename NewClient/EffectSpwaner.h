#pragma once
#include"Singleton.h"

class SpriteObject;
class EffectSpwaner : public Singleton<EffectSpwaner>
{

	std::vector<SpriteObject> m_vEffctList;

	std::vector<std::pair<bool,float>> m_vEffctDelayList;
public:
	void SpawnEffect(std::shared_ptr<SpriteObject> eff, TVector3 pos,float delay);
	void Frame();
	void Render();
	void Init();

public:

	EffectSpwaner()
		: m_vEffctList(100)
		, m_vEffctDelayList(100)
	{
		Init();
	}


};

