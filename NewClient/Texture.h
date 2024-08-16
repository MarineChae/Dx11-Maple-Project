#pragma once
#include"pch.h"
#include"Device.h"

class Texture
{

public:
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;
	std::wstring   m_csName;
	std::wstring   m_csPath;
private:
	int m_iWidth;
	int m_iHeight;

public:
	const int& GetWidth() const { return m_iWidth; };
	const int& GetHeight() const { return m_iHeight; };
	const std::wstring GetName() const { return m_csName; };
public:
	bool Apply(ID3D11DeviceContext* pImmediateContext, int iSlot) const
	{

		pImmediateContext->PSSetShaderResources(iSlot, 1, &m_pTextureSRV);
		return true;
	}
	bool Apply(ID3D11DeviceContext* pImmediateContext, int iSlot, int num) const
	{

		pImmediateContext->PSSetShaderResources(iSlot, num, &m_pTextureSRV);
		return true;
	}
	bool LoadTextureFile(ID3D11Device* pDevice, std::wstring FileName);

	bool Release();

	const ID3D11ShaderResourceView* GetSRV() const { return m_pTextureSRV; };
	void SetSRV(ID3D11ShaderResourceView* srv) { m_pTextureSRV = srv; };
};



class TextureMgr : public Singleton<TextureMgr>
{
	friend class  Singleton<TextureMgr>;
public:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pImmediateContext;
	std::map < std::wstring, Texture*> m_List;

public:

	bool Set(ID3D11Device* pDevice, ID3D11DeviceContext* pImmediateContext);
	//HRESULT SaveFile(std::wstring name, ID3D11Texture2D* pRes);
	const Texture* Load(std::wstring FileName);
	const Texture* GetPtr(std::wstring FileName);
	bool Get(std::wstring FileName, Texture& texture);
	bool Release();




};


