#pragma once
#include"pch.h"
class Texture;
class Shader;
struct SpriteData
{
	TVector3 m_vOffset;
	TVector3 m_vScale;
	int iCol = 0;
	int iRow = 0;
	int iMaxImageCount = 0;
	float m_fDelay = 0.0f;
	const Texture* m_pTexture;
	std::vector<UVRect> m_UVList;
};

class DxRenderer
{
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11Buffer> m_pConstantBuffer;
	ComPtr<ID3D11InputLayout> m_pInputLayout;

	std::vector<DWORD> m_vIndexList;
	const Shader* m_pShader = nullptr;
	const Texture* m_pTexture = nullptr;
	ConstantData m_ConstantData;
public:
	//setter
	std::vector<PNCT_VERTEX> m_vVertexList;
	void  SetShader(const Shader* shader) { m_pShader = shader; };
	void  SetTexture(const Texture* texture) { m_pTexture = texture; };
	void  SetVertexList(std::vector<PNCT_VERTEX> vVertexList) { m_vVertexList = vVertexList; };

	void  SetIndexList(std::vector <DWORD> vIndexList) { m_vIndexList = vIndexList; };
	void  SetConstantData(ConstantData& constdata) { m_ConstantData = constdata; };
	const Shader* GetShader() { return m_pShader; };
	const Texture* GetTexture() { return m_pTexture; }


	virtual void  SetPlayerSprite() {};
	//getter
	ConstantData& GetConstantData() { return m_ConstantData; };
	ID3D11Buffer* GetConstantBuffer() { return m_pConstantBuffer.Get(); };
	ComPtr<ID3D11Buffer> GetVertexBuffer() const { return m_pVertexBuffer; }
	std::vector<PNCT_VERTEX> GetVertexList() { return m_vVertexList; }
	PNCT_VERTEX&  GetVertex(int num) { return m_vVertexList[num]; };
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

