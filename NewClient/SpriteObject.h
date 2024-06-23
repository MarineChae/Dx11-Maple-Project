#pragma once
#include"Object.h"



class SpriteObject : public Object
{
private:
	int m_iTexIndex = 0;
	double m_AccumulatedTime = 0.0;
	SpriteData SpriteInfo;
	std::vector<UVRect> m_UVList;

public:
	virtual bool Create(std::wstring FileName, std::wstring ShaderFileName) override;
	virtual void  SetSpriteData(SpriteData data) override;
	bool SetUVData(int iRow, int iCol);

public:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;
	virtual bool Release()override;

};

