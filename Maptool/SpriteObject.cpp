#include "SpriteObject.h"
#include "Timer.h"
#include "Device.h"
#include"Texture.h"
bool SpriteObject::Create(std::wstring FileName, std::wstring ShaderFileName)
{
    Object::Create(FileName,ShaderFileName);
    SetScale(m_pSpriteInfo->m_vScale);
    SetUVData(m_pSpriteInfo->m_UVList, m_pSpriteInfo->iRow, m_pSpriteInfo->iCol);
    m_pSpriteInfo->m_pTexture = TextureMgr::GetInstance().Load(FileName);
    m_vSpriteList.push_back(m_pSpriteInfo);
    return false;
}

void SpriteObject::SetPlayerSprite()
{





}

bool SpriteObject::SetUVData(std::vector<UVRect>& rectlist, int iRow, int iCol)
{

    UVRect rect;
    TVector2 uv = { 0,0 };

    float OffSet = 1.0f / iCol;

    float fOffsetX = 1.0f / iCol;
    float fOffsetY = 1.0f / iRow;
    for (int row = 0; row < iRow; row++)
    {
        uv.y = fOffsetY * row;
        for (int column = 0; column < iCol; column++)
        {
            uv.x = fOffsetX * column;
            rect.m_vMin = uv;
            rect.m_vMax.x = uv.x + fOffsetX;
            rect.m_vMax.y = uv.y + fOffsetY;
            rectlist.push_back(rect);
        }
    }



    return true;
}



bool SpriteObject::Init()
{
    Object::Init();
    return true;
}

bool SpriteObject::Frame()
{
    Object::Frame();

    m_AccumulatedTime += Timer::GetInstance().GetSecPerFrame();

    if (m_AccumulatedTime >= m_pSpriteInfo->m_fDelay)
    {
        m_iTexIndex++;
        if (m_iTexIndex >= m_pSpriteInfo->m_UVList.size())
        {

            m_iTexIndex = 0;
        }
        m_AccumulatedTime -= m_pSpriteInfo->m_fDelay;
    }




    return true;
}

bool SpriteObject::Render()
{
    Object::PreRender();

    UVRect uv = m_pSpriteInfo->m_UVList[m_iTexIndex];
    if (m_byDirection)
    {
        GetVertex(0).Tex.x = uv.m_vMax.x;
        GetVertex(0).Tex.y = uv.m_vMin.y;
        GetVertex(1).Tex = uv.m_vMin;
        GetVertex(2).Tex = uv.m_vMax;
        GetVertex(3).Tex = GetVertex(2).Tex;
        GetVertex(4).Tex = GetVertex(1).Tex;
        GetVertex(5).Tex.x = uv.m_vMin.x;
        GetVertex(5).Tex.y = uv.m_vMax.y;
    }
    else
    {
        GetVertex(0).Tex = uv.m_vMin;
        GetVertex(1).Tex.x = uv.m_vMax.x;
        GetVertex(1).Tex.y = uv.m_vMin.y;
        GetVertex(2).Tex.x = uv.m_vMin.x;
        GetVertex(2).Tex.y = uv.m_vMax.y;
        GetVertex(3).Tex = GetVertex(2).Tex;
        GetVertex(4).Tex = GetVertex(1).Tex;
        GetVertex(5).Tex = uv.m_vMax;
    }
   
    Device::GetContext()->UpdateSubresource(GetVertexBuffer().Get(), 0, nullptr, &GetVertexList().at(0), 0, 0);


    Object::PostRender();
    return true;
}

bool SpriteObject::Release()
{
    Object::Release();
    return true;
}

SpriteObject::SpriteObject()
    :Object()
    , m_iTexIndex(0)
    , m_AccumulatedTime(0)
    , m_byDirection(0)
    , m_pSpriteInfo()
    , m_vSpriteList()
{

}

SpriteObject::~SpriteObject()
{
    m_vSpriteList.clear();
}
