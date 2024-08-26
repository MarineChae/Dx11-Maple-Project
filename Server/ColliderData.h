#pragma once

class ColliderData
{
private:

	float m_fWidth = 46.0f;
	float m_fHeight = 68.0f;
	TVector3	   m_Axis[3];
	TVector3	   m_Origin[4];
	TVector3       m_vCollisionPoint;
	TVector3	   m_vPos;
public:

	TVector3 GetPos() const { return m_vPos; };
	TVector3 GetCollisionPoint() const { return m_vCollisionPoint; };
	TVector3 GetAxis(int num)const { return m_Axis[num]; };
	float	 GetWidth()const { return m_fWidth; };
	float	 GetHeight()const { return m_fHeight; };
	bool CheckOBBCollision(ColliderData coll1);
	void SetWidth(float width) { m_fWidth = width; };
	void SetHeight(float height) { m_fHeight = height; };
	void SetPos(TVector3 pos) { m_vPos = pos; };
	void Init();
	void Update();

public:
	ColliderData();

	~ColliderData();
};

