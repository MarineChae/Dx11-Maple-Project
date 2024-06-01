#pragma once
#include"pch.h"
class Shader;
class DxRenderer
{
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pConstantBuffer;
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	std::vector<PNCT_VERTEX> m_vVertexList;
	std::vector<DWORD> m_vIndexList;
	const Shader* m_pShader = nullptr;

public:
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	virtual bool CreateConstantBuffer();
	virtual bool CreateInputLayout();

public:
	virtual bool PreRender();
	virtual bool Render();
	virtual bool PostRender();
	virtual bool Release();
};

