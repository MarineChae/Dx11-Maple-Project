#pragma once
#include "Object.h"




class Collider : public Object
{
private:
	int      m_iWidth;
	int      m_iHeight;
	int      m_iDepth;
	TVector3 m_Point[4];
	TVector3 m_Origin[4];
	TVector3 m_Axis[3];

public:
	bool OBBCollision(std::shared_ptr<Collider> other, TVector3 axis);
	virtual void SetScale(TVector3 scale) override;
	
	void SetColliderHeight(int height) { m_iHeight = height; };
	void SetCollisionBox();
	TVector3 GetAxis(int num) const  { return m_Axis[num]; };
public:
	virtual bool Init()override;
	virtual bool Frame()override;
public:
	virtual bool CreateVertexData()override;
	virtual bool PreRender()override;

};

