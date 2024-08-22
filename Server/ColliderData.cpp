#include "Netstd.h"
#include "ColliderData.h"

void ColliderData::Init()
{
    m_Axis[0] = { 1,0,0 };
    m_Axis[1] = { 0,1,0 };
}

void ColliderData::Update()
{


   m_vCollisionPoint = { m_vPos.x,m_vPos.y - m_fHeight,0 };
   
   
}
