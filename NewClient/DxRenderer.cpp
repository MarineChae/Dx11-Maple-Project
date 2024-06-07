#include "DxRenderer.h"
#include "Device.h"
#include "Shader.h"
bool DxRenderer::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC Desc;
    Desc.ByteWidth = static_cast<UINT>(sizeof(PNCT_VERTEX) * m_vVertexList.size());
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    Desc.CPUAccessFlags = 0;
    Desc.MiscFlags = 0;
    Desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA InitialData;
    InitialData.pSysMem = &m_vVertexList.at(0);

    Device::GetDevice()->CreateBuffer(&Desc, &InitialData, m_pVertexBuffer.GetAddressOf());
    return true;
}

bool DxRenderer::CreateIndexBuffer()
{

    if (m_vIndexList.size() == 0)return false;
    D3D11_BUFFER_DESC Desc;
    Desc.ByteWidth = static_cast<UINT>(sizeof(DWORD) * m_vIndexList.size());//������ ũ��
    Desc.Usage = D3D11_USAGE_DEFAULT;//���۸� ��𿡼� ����������
    Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;//������ ���뵵
    Desc.CPUAccessFlags = 0;//cpu�� ���������� �ʿ������ null
    Desc.MiscFlags = 0;//�߰����� �ɼ�
    Desc.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA pInitialData;
    pInitialData.pSysMem = &m_vIndexList.at(0);

    Device::GetDevice()->CreateBuffer(&Desc, &pInitialData, m_pIndexBuffer.GetAddressOf());
    return true;

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

  
    Device::GetDevice()->CreateBuffer(&Desc, nullptr, m_pConstantBuffer.GetAddressOf());
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

    HRESULT hr = Device::GetDevice()->CreateInputLayout(
        InputElementDescs,
        sizeof(InputElementDescs) / sizeof(D3D11_INPUT_ELEMENT_DESC),
        m_pShader->GetBlob()->GetBufferPointer(),
        m_pShader->GetBlob()->GetBufferSize(),
        m_pInputLayout.GetAddressOf());
    if (hr != NULL)
    {

        std::string message = std::system_category().message(hr);
        MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
    }

	return true;
}

bool DxRenderer::PreRender()
{
    Device::GetContext()->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
    Device::GetContext()->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    Device::GetContext()->IASetInputLayout(m_pInputLayout.Get());      //

    if (m_pShader)
    {
        m_pShader->Apply(Device::GetContext(), 0);
    }
    Device::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �޾ƿ� �����͸� � ������� �ؼ�����

    UINT Stride = sizeof(PNCT_VERTEX);
    UINT offset = 0;
    Device::GetContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &Stride, &offset);//p 159����

    return true;

}

bool DxRenderer::Render()
{
    PreRender();
    PostRender();
	return true;
}

bool DxRenderer::PostRender()
{
    if (m_pIndexBuffer == nullptr)
    {
        Device::GetContext()->Draw(m_vVertexList.size(), 0);
    }
    else
    {
         Device::GetContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
         Device::GetContext()->DrawIndexed(m_vIndexList.size(), 0, 0);
    }

	return true;
}

bool DxRenderer::Release()
{
    m_vVertexList.clear();
    m_vIndexList.clear();
	return true;
}
