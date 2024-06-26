#pragma once
#include"MyWindow.h"


class Device : public MyWindow
{

private:
	ComPtr<IDXGISwapChain>			   m_pSwapChain;
	DXGI_SWAP_CHAIN_DESC			   m_SwapChainDesc;
	D3D_FEATURE_LEVEL				   m_pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_DRIVER_TYPE					   m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
	ComPtr <IDXGIFactory>			   m_pFactory = nullptr;
	ComPtr<ID3D11RenderTargetView>     m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView>	   m_pDepthStencilView;
	D3D11_VIEWPORT					   m_ViewPort;
	static ComPtr<ID3D11Device>		   m_pDevice;
	static ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	ComPtr<ID3D11BlendState> m_pBlendState;

public:
	static ID3D11Device* GetDevice() { return m_pDevice.Get(); };
	static ID3D11DeviceContext* GetContext() { return m_pImmediateContext.Get(); };
	ComPtr<ID3D11BlendState> GetBlendState() { return m_pBlendState; }
public:
	bool CreateDevice();
	bool CreateDepthStencilView();
	bool CreateFactory();
	bool CreateSwapChain();
	bool SetRenderTarGetView();
	bool SetViewPort();
	bool CreateBlendState();
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool PreRender();
	virtual bool PostRender();
	virtual bool Render();
	virtual bool Release();




};

