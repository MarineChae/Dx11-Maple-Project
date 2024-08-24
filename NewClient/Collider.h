#pragma once
#include "Object.h"


struct CollisionBufferData {
	TVector4 targetColor;
	TVector2 targetPosition;
	float padding[2]; // 16����Ʈ ���߱� ���� �е�
};

class Collider : public Object
{
private:
	int			   m_iWidth;
	int			   m_iHeight;
	int			   m_iDepth;
	TVector3	   m_Point[4];
	TVector3	   m_Origin[4];
	TVector3	   m_Axis[3];
	COLLISION_TYPE m_CollisionType;
	TVector3       m_vCollisionPoint;
	bool		   m_bEnable;
public:
	//getter
	int			   GetHeight() const { return m_iHeight; }
	int			   GetWidth() const { return m_iWidth; }
	TVector3	   GetAxis(int num) const { return m_Axis[num]; };
	COLLISION_TYPE GetCollisionType() const { return m_CollisionType; };
	TVector3       GetCollisionPoint()const  override { return m_vCollisionPoint; };
	bool		   GetEnable() const { return m_bEnable; };
	//setter
	virtual void SetScale(TVector3 scale) override;
	void		 SetColliderHeight(int height) { m_iHeight = height; };
	void		 SetCollisionBox();
	void		 SetCollsionType(COLLISION_TYPE type) { m_CollisionType = type; };
	void         SetCollisionPoint();
	void	     SetEnable(bool enable) { m_bEnable = enable; };
public:
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render() override;
public:
	virtual bool CreateVertexData()override;
	virtual bool PreRender()override;
	static bool CheckOBBCollision(std::shared_ptr<Collider> coll1, std::shared_ptr<Collider> coll2);
public:
	Collider();
	virtual ~Collider();
};

