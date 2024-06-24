#include "Object.h"
#include"Shader.h"
#include"Device.h"
#include"Protocol.h"
#include"Texture.h"
#include"Collider.h"
std::shared_ptr<Collider> Object::GetCollider() const
{
    return m_pCollider;
}
bool Object::CreateVertexData()
{
    std::vector<PNCT_VERTEX> v;
    v.resize(6);

    v[0].Pos = { -1.0f,1.0f,0.0f };
    v[1].Pos = { 1.0f,1.0f,0.0f };
    v[2].Pos = { -1.0f, -1.0f,0.0f };
    v[3].Pos = { -1.0f, -1.0f,0.0f };
    v[4].Pos = { 1.0f,1.0f,0.0f };
    v[5].Pos = { 1.0f ,-1.0f,0.0f };

    v[0].Tex = { 0.0f,0.0f };
    v[1].Tex = { 1.0f,0.0f };
    v[2].Tex = { 0.0f,1.0f };
    v[3].Tex = { 0.0f,1.0f };
    v[4].Tex = { 1.0f,0.0f };
    v[5].Tex = { 1.0f,1.0f };

    SetVertexList(v);
   
    return true;
}

bool Object::CreateIndexData()
{
    return false;
}

void Object::UpdataMatrix()
{
}

bool Object::Create(std::wstring FileName, std::wstring ShaderFileName)
{
    CreateConstantBuffer();
    CreateVertexData();
    CreateVertexBuffer();
    CreateIndexBuffer();
    SetShader(ShaderMgr::GetInstance().Load(ShaderFileName));
    SetTexture(TextureMgr::GetInstance().Load(FileName));

    CreateInputLayout();

 
    return true;
}

void Object::SetMatrix(TMatrix* WolrdMatrix, TMatrix* ViewMatrix, TMatrix* ProjMatrix)
{
    if (WolrdMatrix != nullptr)
    {
        m_WolrdMatrix = *WolrdMatrix;
    }
    if (ViewMatrix != nullptr)
    {
        m_ViewMatrix = *ViewMatrix;
    }
    if (ProjMatrix != nullptr)
    {
        m_ProjMatrix = *ProjMatrix;
    }

    GetConstantData().WolrdMatrix = m_WolrdMatrix.Transpose();
    GetConstantData().ViewMatrix = m_ViewMatrix.Transpose();
    GetConstantData().ProjMatrix = m_ProjMatrix.Transpose();

    Device::GetContext()->UpdateSubresource(GetConstantBuffer(), 0, nullptr, &GetConstantData(), 0, 0);
}

void Object::SetScale(TVector3 scale)
{
    m_vScale = scale;
}

void Object::SetMatrix()
{
    TMatrix matrixtrans;
    matrixtrans.Translation(m_vTransform);
    TMatrix matrixSCale;
    matrixSCale = matrixSCale.CreateScale(m_vScale);
    TMatrix matrixRotate;
    matrixRotate = matrixRotate.CreateRotationZ(m_vRotate.z);

    m_WolrdMatrix = matrixSCale * matrixRotate * matrixtrans;

}

bool Object::Init()
{
    m_pCollider = std::make_shared<Collider>();
    return false;
}

bool Object::Frame()
{
    SetMatrix();


    return true;
}

bool Object::Render()
{
    PreRender();
    PostRender();
    return true;
}

bool Object::Release()
{
    return false;
}

Object::Object()
    :m_WolrdMatrix()
    , m_ViewMatrix()
    , m_ProjMatrix()
    , m_vScale(1, 1, 1)
    , m_vRotate(0, 0, 0)
    , m_vTransform(0, 0, 0)
    , m_dwObjectID(1)
    , m_bRender(false)
{
   
}



void ObejctMgr::DisconnectCharacter(DWORD SessionID)
{
    m_lObjectList[SessionID].reset();
}
