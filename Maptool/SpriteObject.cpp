#include "SpriteObject.h"
#include "Timer.h"
#include "Device.h"
#include"Texture.h"
#include"Collider.h"
bool SpriteObject::Create(std::wstring FileName, std::wstring ShaderFileName)
{
    Object::Create(FileName,ShaderFileName);
    if (m_pSpriteInfo != nullptr)
    {
        //m_pSpriteInfo->m_vScale = { static_cast<float>(GetTexture()->GetWidth()),static_cast<float>(GetTexture()->GetHeight()),1 };
        SetScale({ static_cast<float>(GetTexture()->GetWidth()),static_cast<float>(GetTexture()->GetHeight()),1 });
        SetUVData(m_pSpriteInfo->m_UVList, m_pSpriteInfo->iRow, m_pSpriteInfo->iCol);
        m_pSpriteInfo->m_pTexture = TextureMgr::GetInstance().Load(FileName);
        m_vSpriteList.push_back(m_pSpriteInfo);
    }
    else
    {
        m_pSpriteInfo = std::make_shared<SpriteData>();
        m_pSpriteInfo->m_vScale = { static_cast<float>(GetTexture()->GetWidth()),static_cast<float>(GetTexture()->GetHeight()),1 };
    }

    return false;
}

bool SpriteObject::Create(const Texture* tex, const Shader* shader)
{
    Object::Create(tex, shader);
    if (m_pSpriteInfo != nullptr)
    {
        //m_pSpriteInfo->m_vScale = { static_cast<float>(GetTexture()->GetWidth()),static_cast<float>(GetTexture()->GetHeight()),1 };
        SetScale(m_pSpriteInfo->m_vScale);
        SetUVData(m_pSpriteInfo->m_UVList, m_pSpriteInfo->iRow, m_pSpriteInfo->iCol);
        m_pSpriteInfo->m_pTexture = tex;
        m_vSpriteList.push_back(m_pSpriteInfo);
    }
    else
    {
        m_pSpriteInfo = std::make_shared<SpriteData>();
        m_pSpriteInfo->m_vScale = { static_cast<float>(GetTexture()->GetWidth()),static_cast<float>(GetTexture()->GetHeight()),1 };
    }

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

bool SpriteObject::ChangeSpriteData(std::shared_ptr<SpriteData> data, MONSTER_STATE state)
{
    if (m_vSpriteList.size() - 1 >= state)
    {

        m_vSpriteList[state] = data;

    }


    return false;
}



bool SpriteObject::Init()
{
    Object::Init();
    GetCollider()->Init();
    return true;
}

bool SpriteObject::Frame()
{
    Object::Frame();

    m_AccumulatedTime += Timer::GetInstance().GetSecPerFrame();

    if (m_AccumulatedTime >= m_pSpriteInfo->m_fDelay)
    {
        m_iTexIndex++;
        if (m_iTexIndex >= m_pSpriteInfo->iMaxImageCount)
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
