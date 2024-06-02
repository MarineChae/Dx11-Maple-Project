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
	ConstantData m_ConstantData;
public:
	//setter
	void  SetShader(const Shader* shader) { m_pShader = shader; };
	void  SetVertexList(std::vector<PNCT_VERTEX> vVertexList) { m_vVertexList = vVertexList; };
	void  SetIndexList(std::vector <DWORD> vIndexList) { m_vIndexList = vIndexList; };
	void  SetConstantData(ConstantData& constdata) { m_ConstantData = constdata; };
	const Shader* GetShader() { return m_pShader; };
	//getter
	ConstantData& GetConstantData() { return m_ConstantData; };
	ID3D11Buffer* GetConstantBuffer() { return m_pConstantBuffer.Get(); };
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

