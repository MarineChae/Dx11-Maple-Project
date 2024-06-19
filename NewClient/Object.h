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
	DWORD m_dwObjectID;


public:
	DWORD GetObejctID() const  {return m_dwObjectID;}
	TVector3 GetTransform() const { return m_vTransform; };
	void SetTransform(TVector3 transform) { m_vTransform = transform; };
public:
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual void UpdataMatrix();
	virtual bool Create(std::wstring FileName, std::wstring ShaderFileName);
	virtual void SetMatrix(TMatrix* WolrdMatrix, TMatrix* ViewMatrix, TMatrix* ProjMatrix);
	virtual void SetScale(TVector3 scale);
	virtual void SetMatrix();
public:


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	Object()
	{
		m_dwObjectID = 1;
		m_vTransform = TVector3(0, 0, 0);
		m_vScale = TVector3(1, 1, 1);
		m_vRotate = TVector3(0, 0, 0);
	}

};



class ObejctMgr : public Singleton<ObejctMgr>
{
	friend class Singleton<ObejctMgr>;

private:
	std::list<std::shared_ptr<Object>> m_lObjectList;
	std::shared_ptr<Object>			   m_pPlayerObject;
public:
	std::list<std::shared_ptr<Object>> GetObjectList() { return m_lObjectList; };
	void                               PushObject(std::shared_ptr<Object> obj) { m_lObjectList.push_back(obj); };
	std::shared_ptr<Object>			   GetPlayerObject(DWORD SessionID);
	std::shared_ptr<Object>			   GetPlayerObject() { return m_pPlayerObject; };
	void SetPlayerObject(std::shared_ptr<Object> obj) { m_pPlayerObject = obj; };


};