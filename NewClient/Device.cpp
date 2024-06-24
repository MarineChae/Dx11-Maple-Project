#include "pch.h"
#include "Device.h"

ComPtr<ID3D11Device>		Device::m_pDevice = nullptr;
ComPtr<ID3D11DeviceContext> Device::m_pImmediateContext = nullptr;


bool Device::CreateDevice()
{
    HRESULT HR = D3D11CreateDevice(
        NULL,
        m_DriverType,
        NULL,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        &m_pFeatureLevels,
        1,
        D3D11_SDK_VERSION,
        m_pDevice.GetAddressOf(),
        NULL,
        m_pImmediateContext.GetAddressOf());
    if (FAILED(HR))
    {
        return false;
    }

    return true;
}

bool Device::CreateDepthStencilView()
{
    HRESULT hr;
    ComPtr<ID3D11Texture2D> DSVTex;

    DXGI_SWAP_CHAIN_DESC SCDesc;
    m_pSwapChain->GetDesc(&SCDesc);

    D3D11_TEXTURE2D_DESC TexDesc = { SCDesc.BufferDesc.Width,
                                     SCDesc.BufferDesc.Height,
                                     1,1,
                                     DXGI_FORMAT_R24G8_TYPELESS,
                                     1,0,
                                     D3D11_USAGE_DEFAULT,D3D11_BIND_DEPTH_STENCIL,
                                     0,0 };

    hr = m_pDevice->CreateTexture2D(&TexDesc, NULL, DSVTex.GetAddressOf());

    D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc;
    ZeroMemory(&DSVDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;


    hr = m_pDevice->CreateDepthStencilView(DSVTex.Get(), &DSVDesc, m_pDepthStencilView.GetAddressOf());



    return true;
}

bool Device::CreateFactory()
{
    HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_pFactory.GetAddressOf());
    if (FAILED(HR))
    {
        return false;
    }

    return true;
}

bool Device::CreateSwapChain()
{
    ZeroMemory(&m_SwapChainDesc, sizeof(m_SwapChainDesc));
    m_SwapChainDesc.BufferDesc.Width = GetWindowStyle().m_dwWindowWidth;
    m_SwapChainDesc.BufferDesc.Height = GetWindowStyle().m_dwWindowHeight;
    m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;//�ֻ��� 60/1 ���� 
    m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//�÷� ������� 
    m_SwapChainDesc.SampleDesc.Count = 1;//��Ƽ������� ī��Ʈ�� ����Ƽ�� �������� �׷����� ���������� �޸𸮸� ���� ���
    m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//������ ���뵵
    m_SwapChainDesc.BufferCount = 1;
    m_SwapChainDesc.OutputWindow = GetHWND();
    m_SwapChainDesc.Windowed = true;

    HRESULT HR = m_pFactory->CreateSwapChain(m_pDevice.Get(), &m_SwapChainDesc, m_pSwapChain.GetAddressOf());
    if (FAILED(HR))
    {
        return false;
    }

    return true;
}

bool Device::SetRenderTarGetView()
{
    ID3D11Texture2D* pBackBuffer;
    HRESULT HR = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    //��ȯ�Ǵ� �������̽� Ÿ�� ����
    if (FAILED(HR))
    {
        return false;
    }
    //����� ���
    HR = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, m_pRenderTargetView.GetAddressOf());
    //�������� ���� 2���� ���� , ������� �ĸ����
    if (FAILED(HR))
    {
        pBackBuffer->Release();
        return false;
    }

    //������ ���۸� ����
    pBackBuffer->Release();
    return true;
}

bool Device::SetViewPort()
{
   m_ViewPort.TopLeftX = 0.0f;
   m_ViewPort.TopLeftY = 0.0f;
   m_ViewPort.Width = static_cast<float>(m_SwapChainDesc.BufferDesc.Width);
   m_ViewPort.Height = static_cast<float>(m_SwapChainDesc.BufferDesc.Height);
   m_ViewPort.MinDepth = 0.0f;
   m_ViewPort.MaxDepth = 1.0f;

    return true;
}

bool Device::CreateBlendState()
{

   D3D11_BLEND_DESC BlendStateDesc;
   ZeroMemory(&BlendStateDesc, sizeof(BlendStateDesc));
   BlendStateDesc.RenderTarget[0].BlendEnable = true;
   
   BlendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
   BlendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
   BlendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
   
   BlendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
   BlendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
   BlendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
   
   BlendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
   m_pDevice->CreateBlendState(&BlendStateDesc, m_pBlendState.GetAddressOf());
   
   
   return true;
   
}

bool Device::Init()
{
    CreateDevice();
    CreateFactory();
    CreateSwapChain();
    SetRenderTarGetView();
    SetViewPort();
    CreateDepthStencilView();
    CreateBlendState();

    return true;
}

bool Device::Frame()
{
    return true;
}

bool Device::PreRender()
{

    float color[4] = { 0.75f,0.35f,0.35f,1 }; //4��° ���İ��� ����
    m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);// color�� ������ ���۸� �����.
    m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), NULL);
    m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

    return true;
}

bool Device::PostRender()
{
    m_pSwapChain->Present(0, 0);
    return true;
}

bool Device::Render()
{
    PreRender();


    PostRender();
    return true;
}

bool Device::Release()
{
    return false;
}
