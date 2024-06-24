#pragma once
#include"Object.h"



class SpriteObject : public Object
{
private:
	int	   m_iTexIndex = 0;
	double m_AccumulatedTime = 0.0;
	BYTE   m_byDirection;

	std::shared_ptr<SpriteData> m_pSpriteInfo;
	std::vector<std::shared_ptr<SpriteData>> m_vSpriteList;
public:
	virtual bool Create(std::wstring FileName, std::wstring ShaderFileName) override;
	virtual void  SetPlayerSprite() override;
	bool SetUVData(std::vector<UVRect>& rectlist , int iRow, int iCol);
	void SetSpriteInfo(std::shared_ptr<SpriteData> info) {m_pSpriteInfo= info;}
	std::shared_ptr<SpriteData> GetCurrentSpriteInfo() const { return m_pSpriteInfo; };
	const std::shared_ptr<SpriteData> GetSpriteData(int state) const { return m_vSpriteList[state]; };
	void InitTexIndex() { m_iTexIndex = 0; };
	virtual void SetDirection(BYTE dir) { m_byDirection = dir; };
	BYTE GetDirection()const { return m_byDirection; };
public:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render()override;
	virtual bool Release()override;

public:
	SpriteObject();
	virtual ~SpriteObject();
};

