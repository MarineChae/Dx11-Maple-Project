#include "Shader.h"

bool Shader::Load(ID3D11Device* pDevice, std::wstring FileName)
{
    CreateVertexShader(pDevice, FileName);
    CreatePixelShader(pDevice, FileName);
    return true;
}

bool Shader::CreatePixelShader(ID3D11Device* pDevice, std::wstring FileName)
{
    ID3DBlob* ErrpBb;
    UINT flags = 0;
#ifdef _DEBUG
    flags = D3DCOMPILE_DEBUG;
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    HRESULT ht = D3DCompileFromFile(FileName.c_str(), NULL, NULL, "PS", "ps_5_0",
        flags, 0, &m_pBb, &ErrpBb);

    if (ErrpBb != nullptr)
    {
        LPCSTR errorText = "nullptr";
        errorText = (LPCSTR)ErrpBb->GetBufferPointer();

        MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);
    }
    pDevice->CreatePixelShader(m_pBb->GetBufferPointer(),
        m_pBb->GetBufferSize(),
        NULL,
        m_pPixelShader.GetAddressOf());


    if (ErrpBb)ErrpBb->Release();

    return  true;
}

bool Shader::CreateVertexShader(ID3D11Device* pDevice, std::wstring FileName)
{
    UINT flags = 0;
#ifdef _DEBUG
    flags = D3DCOMPILE_DEBUG;
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ID3DBlob* ErrvBb;
    HRESULT hr = D3DCompileFromFile(FileName.c_str(), NULL, NULL, "VS", "vs_5_0",
        flags, 0, m_vBb.GetAddressOf(), &ErrvBb);



    if (ErrvBb != nullptr)
    {
        LPCSTR errorText = "nullptr";
        errorText = (LPCSTR)ErrvBb->GetBufferPointer();

        MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);
    }


    pDevice->CreateVertexShader(m_vBb->GetBufferPointer(),
        m_vBb->GetBufferSize(),
        NULL,
        m_VertexShader.GetAddressOf());


    if (ErrvBb)ErrvBb->Release();
    return true;
}

bool Shader::Release()
{
    return false;
}

const Shader* ShaderMgr::Load(std::wstring FileName)
{
    std::size_t found = FileName.find_last_of(L"/");
    std::wstring path = FileName.substr(0, found + 1);
    std::wstring Key = FileName.substr(found + 1);
    const Shader* data = GetPtr(Key);

    if (data != nullptr)
    {
        return data;

    }
    Shader* NewData = new Shader;
    if (NewData->Load(Device::GetDevice(), FileName))
    {
        m_List.insert(std::make_pair(Key, NewData));
        return NewData;
    }
    delete NewData;

    return nullptr;
}

const Shader* ShaderMgr::GetPtr(std::wstring FileName)
{
    auto iter = m_List.find(FileName);
    if (m_List.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}

bool ShaderMgr::Get(std::wstring FileName, Shader& texture)
{
    auto iter = m_List.find(FileName);
    if (m_List.end() == iter)
    {
        return false;
    }
    texture = *(iter->second);

    return true;
}

bool ShaderMgr::Release()
{
    for (auto& data : m_List)
    {
        data.second->Release();
        delete data.second;

    }
    m_List.clear();
    return true;
}

ShaderMgr::ShaderMgr()
{
}

ShaderMgr::~ShaderMgr()
{
    Release();
}
