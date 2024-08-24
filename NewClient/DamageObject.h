#pragma once
#include"Object.h"
class DamageObject
{
private:
	float m_fPrintTime;
	float m_fCurrentTime;
	float m_fAlpha;
	std::vector<const Texture*> m_vTextureList;
	bool  m_bValid;
	int   m_iNumSize;
	std::vector<std::shared_ptr<Object>> m_vDamageList;

public:
	bool Init();
	bool Frame(TVector3 pos , int damage);
	bool Render();
public:
	
	bool  GetIsValid()const { return m_bValid; }
	void  SetIsValid(bool valid);
	void  InitCurrentTime() { m_fCurrentTime = 0.0f; };
public:
	DamageObject();
	~DamageObject();
};

