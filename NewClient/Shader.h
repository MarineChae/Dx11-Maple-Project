#pragma once
#include"pch.h"
#include"Device.h"
class Shader
{
private:
	ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
	ComPtr<ID3D11PixelShader>  m_pPixelShader = nullptr;
	ComPtr<ID3DBlob>		   m_vBb = nullptr;
	ComPtr<ID3DBlob>		   m_pBb = nullptr;

public:
	
	ID3D11VertexShader* GetVertexShader() const { return m_VertexShader.Get(); } 
	ID3D11PixelShader*  GetPixelShader() const { return m_pPixelShader.Get(); }
	ID3DBlob*			GetBlob() const { return m_vBb.Get(); }

public:
	bool Load(ID3D11Device* pDevice, std::wstring FileName);
	bool CreatePixelShader(ID3D11Device* pDevice, std::wstring FileName);
	bool CreateVertexShader(ID3D11Device* pDevice, std::wstring FileName);
	bool Release();
	bool Apply(ID3D11DeviceContext* pImmediateContext, int iSlot) const
	{
		Device::GetContext()->VSSetShader(m_VertexShader.Get(), NULL, 0); //¡§¡°Ω¶¿Ã¥ı º≥¡§
		Device::GetContext()->PSSetShader(m_pPixelShader.Get(), NULL, 0); //«»ºøΩ¶¿Ã¥ı º≥¡§
		return true;
	}


};


class ShaderMgr
{
private:
	std::map <std::wstring, Shader*> m_List;

public:

	const Shader* Load(std::wstring FileName);
	const Shader* GetPtr(std::wstring FileName);
	bool		  Get(std::wstring FileName, Shader& texture);
	bool		  Release();
	static ShaderMgr& GetInstance()
	{
		static ShaderMgr Shader;
		return Shader;

	}

private:
	ShaderMgr();
public:

	~ShaderMgr();

};
