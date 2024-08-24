#include "Collider.h"
#include "Device.h"
#include "Shader.h"
#include "Texture.h"
#include"Input.h"
#include"Collision.h"
//bool Collider::OBBCollision(std::shared_ptr<Collider> other , TVector3 axis) 
//{
//    float projection1 = 0;
//
//    projection1 += std::abs((m_Axis[0] .Dot(axis) * m_iWidth));
//    projection1 += std::abs((m_Axis[1] .Dot(axis) * m_iHeight));
//
//    float projection2 = 0;
//
//    projection2 += std::abs((other->m_Axis[0] .Dot(axis) * other->m_iWidth));
//    projection2 += std::abs((other->m_Axis[1] .Dot(axis) * other->m_iHeight));
//
//    auto a = GetTransform() - other->GetTransform();
//    float distance = std::abs(a.Dot(axis))/2;
//    return distance <= projection1 + projection2;
//
//}

void Collider::SetScale(TVector3 scale)
{
    Object::SetScale(scale);
    m_iWidth = static_cast<int>(scale.x);
    m_iHeight = static_cast<int>(scale.y);


}

void Collider::SetCollisionBox()
{
    TVector3 pos = GetTransform();

    m_Origin[0] = { -1.0f,1.0f,0.0f };
    m_Origin[1] = { 1.0f,1.0f,0.0f };
    m_Origin[2] = { 1.0f ,-1.0f,0.0f };
    m_Origin[3] = { -1.0f, -1.0f,0.0f };
    m_Axis[0] = { 1,0,0 };
    m_Axis[1] = { 0,1,0 };

    for (int i = 0; i < 4; ++i)
    {
        D3DXVec3TransformCoord(&m_Origin[i], &m_Origin[i], &GetWorldMat());
    }
    TMatrix matrixRotate;
    matrixRotate = matrixRotate.CreateRotationZ(m_vRotate.z);
    for (int i = 0; i < 2; ++i)
    {
        D3DXVec3TransformCoord(&m_Axis[i], &m_Axis[i], &matrixRotate);
    }

}

void Collider::SetCollisionPoint()
{
    TVector3 Pos = GetTransform();
    m_vCollisionPoint = { Pos.x,Pos.y - m_iHeight,0 };

}

bool Collider::Init()
{
    Object::Init();
  

    return false;
}

bool Collider::Frame()
{
    Object::Frame();
    SetCollisionPoint();
    SetCollisionBox();

    return true;
}

bool Collider::Render()
{
    PreRender();
    PostRender();
    return false;
}

bool Collider::CreateVertexData()
{
    std::vector<PNCT_VERTEX> v;
    v.resize(8);

    v[0].Pos = { -1.0f,1.0f,0.0f };
    v[1].Pos = { 1.0f,1.0f,0.0f };

    v[2].Pos = { 1.0f ,-1.0f,0.0f };
    v[3].Pos = { -1.0f, -1.0f,0.0f };
    v[4].Pos = { -1.0f,1.0f,0.0f };
    v[5].Pos = { 0.0f,1.0f,0.0f };
    
    v[6].Pos = {  0.0f,0.0f,0.0f };
    v[7].Pos = {  1.0f,0.0f,0.0f };
    
   

    SetVertexList(v);
    return true;
}
bool Collider::PreRender()
{
    Object::PreRender();
    Device::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); // 받아온 데이터를 어떤 방식으로 해석할지
  
    return true;
}

bool Collider::CheckOBBCollision(std::shared_ptr<Collider> coll1, std::shared_ptr<Collider> coll2)
{
    std::vector<TVector3> axis = { coll1->GetAxis(0),
                                   coll1->GetAxis(1),
                                   coll2->GetAxis(0),
                                   coll2->GetAxis(1) };
    for (int i = 0; i < 4; ++i)
    {
        axis[i].Normalize();
        if (!Collision::OBBCollision2D(coll1,coll2,axis[i]))
        {
            return false;
        }
    }

    return true;
}

Collider::Collider()
    :m_iWidth(0)
    ,m_iHeight(0)
    ,m_iDepth(0)
    ,m_Point()
    ,m_Origin()
    ,m_Axis()
    ,m_CollisionType(COLLISION_TYPE::CT_DEFAULT)
    ,m_bEnable(true)
{

}

Collider::~Collider()
{
}
