#include "Texture.h"


const Texture* TextureMgr::Load(std::wstring FileName)
{
    std::size_t found = FileName.find_last_of(L"/");
    std::wstring path = FileName.substr(0, found + 1);
    std::wstring Key = FileName.substr(found + 1);
    const Texture* data = GetPtr(Key);

    if (data != nullptr)
    {
        return data;

    }
    Texture* NewData = new Texture;
    if (NewData->LoadTextureFile(Device::GetDevice(), FileName))
    {
        NewData->m_csName = Key;
        m_List.insert(std::make_pair(Key, NewData));
        return NewData;
    }
    delete NewData;

    return nullptr;
}

const Texture* TextureMgr::GetPtr(std::wstring FileName)
{
    auto iter = m_List.find(FileName);
    if (m_List.end() == iter)
    {
        return nullptr;
    }

    return iter->second;
}

bool TextureMgr::Get(std::wstring FileName, Texture& texture)
{
    auto iter = m_List.find(FileName);
    if (m_List.end() == iter)
    {
        return false;
    }
    texture = *(iter->second);

    return true;
}

bool TextureMgr::Release()
{
    for (auto& data : m_List)
    {
        data.second->Release();
        delete data.second;

    }
    m_List.clear();
    return true;
}

bool Texture::LoadTextureFile(ID3D11Device* pDevice, std::wstring FileName)
{
    auto ImageObj = std::make_unique<DirectX::ScratchImage>();
    DirectX::TexMetadata mData;
    HRESULT hr = DirectX::GetMetadataFromDDSFile(FileName.c_str(), DirectX::DDS_FLAGS_NONE, mData);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromDDSFile(FileName.c_str(), DirectX::DDS_FLAGS_NONE, &mData, *ImageObj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(pDevice, ImageObj->GetImages(),
                ImageObj->GetImageCount(),
                mData, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0, CREATETEX_DEFAULT, &m_pTextureSRV);

            if (SUCCEEDED(hr))
            {

                return true;
            }
        }
    }
    hr = DirectX::GetMetadataFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS_NONE, mData);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromWICFile(FileName.c_str(), DirectX::WIC_FLAGS_NONE, &mData, *ImageObj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(pDevice, ImageObj->GetImages(),
                ImageObj->GetImageCount(),
                mData, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0, CREATETEX_DEFAULT, &m_pTextureSRV);

            if (SUCCEEDED(hr))
            {
                return true;
            }
        }
    }

    hr = DirectX::GetMetadataFromTGAFile(FileName.c_str(), DirectX::TGA_FLAGS_NONE, mData);
    if (SUCCEEDED(hr))
    {
        hr = DirectX::LoadFromTGAFile(FileName.c_str(), DirectX::TGA_FLAGS_NONE, &mData, *ImageObj);
        if (SUCCEEDED(hr))
        {
            hr = DirectX::CreateShaderResourceViewEx(pDevice, ImageObj->GetImages(),
                ImageObj->GetImageCount(),
                mData, D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0, CREATETEX_DEFAULT, &m_pTextureSRV);

            if (SUCCEEDED(hr))
            {
                return true;
            }
        }
    }



    return false;
}

bool Texture::Release()
{
    if (m_pTextureSRV)m_pTextureSRV->Release();
    m_pTextureSRV = nullptr;
    return true;
}
