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
	virtual bool Create(const Texture* tex, const Shader* shader)override;
	virtual void SetSpriteInfo(std::shared_ptr<SpriteData> info) override { m_pSpriteInfo = info; };
	virtual std::shared_ptr<SpriteData> GetSpriteInfo() { return m_pSpriteInfo; };
	virtual void  SetPlayerSprite() override;
	bool SetUVData(std::vector<UVRect>& rectlist , int iRow, int iCol);

	std::shared_ptr<SpriteData> GetCurrentSpriteInfo() const { return m_pSpriteInfo; };
	const std::shared_ptr<SpriteData> GetSpriteData(int state) const { return m_vSpriteList[state]; };
	const std::vector<std::shared_ptr<SpriteData>> GetSpriteList() const { return m_vSpriteList; };
	bool ChangeSpriteData(std::shared_ptr<SpriteData> data ,MONSTER_STATE state);



	void PushSpriteData(std::shared_ptr<SpriteData> data) { m_vSpriteList.push_back(data); }
	virtual void InitTexIndex() override { m_iTexIndex = 0; };
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

