#pragma once
#include"DxRenderer.h"

class Object : public DxRenderer
{


private:
	TMatrix m_WolrdMatrix;
	TMatrix m_ViewMatrix;
	TMatrix m_ProjMatrix;

private:
	TVector3 m_vScale;
	TVector3 m_vRotate;
	TVector3 m_vTransform;


public:
	TVector3 GetTransform() const { return m_vTransform; };

public:
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual void UpdataMatrix();
	virtual bool Create(std::wstring FileName, std::wstring ShaderFileName);
	virtual void SetMatrix(TMatrix* WolrdMatrix, TMatrix* ViewMatrix, TMatrix* ProjMatrix);
	virtual void SetScale(TVector3 scale);
public:


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	Object()
	{
		m_vTransform = TVector3(0, 0, 0);
		m_vScale = TVector3(1, 1, 1);
		m_vRotate = TVector3(0, 0, 0);
	}

};

