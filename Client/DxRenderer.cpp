#include "DxRenderer.h"
#include "Device.h"
#include "Shader.h"
bool DxRenderer::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC Desc;
    Desc.ByteWidth = sizeof(PNCT_VERTEX) * m_vVertexList.size();
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    Desc.CPUAccessFlags = 0;
    Desc.MiscFlags = 0;
    Desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA InitialData;
    InitialData.pSysMem = &m_vVertexList.at(0);

    Device::m_pDevice->CreateBuffer(&Desc, &InitialData, m_pVertexBuffer.GetAddressOf());
    return true;
}

bool DxRenderer::CreateIndexBuffer()
{
	return false;
}

bool DxRenderer::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC Desc;
    Desc.ByteWidth = sizeof(ConstantData);
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = 0;
    Desc.MiscFlags = 0;
    Desc.StructureByteStride = 0;


    Device::m_pDevice->CreateBuffer(&Desc, nullptr, m_pConstantBuffer.GetAddressOf());
	return true;
}

bool DxRenderer::CreateInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC InputElementDescs[] =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
    };

    HRESULT hr = Device::m_pDevice->CreateInputLayout(
        InputElementDescs,
        sizeof(InputElementDescs) / sizeof(D3D11_INPUT_ELEMENT_DESC),
        m_pShader->GetBlob()->GetBufferPointer(),
        m_pShader->GetBlob()->GetBufferSize(),
        m_pInputLayout.GetAddressOf());
    if ((hr))
    {
        LPCSTR errorText = "nullptr";
        errorText = (LPCSTR)hr;

        MessageBoxA(NULL, errorText, "Error", MB_OK | MB_ICONERROR);
    }

	return false;
}

bool DxRenderer::PreRender()
{
	return false;
}

bool DxRenderer::Render()
{
	return false;
}

bool DxRenderer::PostRender()
{
	return false;
}

bool DxRenderer::Release()
{
	return false;
}
