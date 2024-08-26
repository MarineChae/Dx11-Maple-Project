#include "Netstd.h"
#include "ColliderData.h"
#include"Collision.h"
void ColliderData::Init()
{
    m_Axis[0] = { 1,0,0 };
    m_Axis[1] = { 0,1,0 };
    m_Origin[0] = { -1.0f,1.0f,0.0f };
    m_Origin[1] = { 1.0f,1.0f,0.0f };
    m_Origin[2] = { 1.0f ,-1.0f,0.0f };
    m_Origin[3] = { -1.0f, -1.0f,0.0f };
}

void ColliderData::Update()
{


   m_vCollisionPoint = { m_vPos.x,m_vPos.y - m_fHeight,0 };
   
   
}

bool ColliderData::CheckOBBCollision(ColliderData coll1)
{
    std::vector<TVector3> axis = { GetAxis(0),
                                   GetAxis(1),
                                   coll1.GetAxis(0),
                                   coll1.GetAxis(1) };
    for (int i = 0; i < 4; ++i)
    {
        axis[i].Normalize();
        if (!Collision::OBBCollision2D(*this, coll1, axis[i]))
        {
            return false;
        }
    }

    return true;
}

ColliderData::ColliderData()
    : m_fWidth(46.0f)
    , m_fHeight(68.0f)
    , m_Axis()
    , m_Origin()
    , m_vCollisionPoint()
    , m_vPos()
{
    Init();
};


ColliderData::~ColliderData()
{
}