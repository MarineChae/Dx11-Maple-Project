#pragma once
#include"DxRenderer.h"

#define MAX_USER_SIZE 63

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
	TVector3 m_vDestination;
private:
	DWORD m_dwObjectID;
	bool  m_bRender;

public:
	DWORD GetObejctID() const  {return m_dwObjectID;}
	void SetObejctID(DWORD id) { m_dwObjectID = id; }
	void SetTransform(TVector3 transform) { m_vTransform = transform; };
	void SetDestination(TVector3 Destination) { m_vDestination = Destination; };
	TVector3 GetTransform() const { return m_vTransform; };
	TVector3 GetDestination() const { return m_vDestination; };
public:
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual void UpdataMatrix();
	virtual bool Create(std::wstring FileName, std::wstring ShaderFileName);
	virtual void SetMatrix(TMatrix* WolrdMatrix, TMatrix* ViewMatrix, TMatrix* ProjMatrix);
	virtual void SetScale(TVector3 scale);
	virtual void SetMatrix();
	virtual void SetRenderState(bool state) {m_bRender = state;}
public:


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	Object()
		:m_WolrdMatrix()
		, m_ViewMatrix()
		, m_ProjMatrix()
		, m_vScale(1, 1, 1)
		, m_vRotate(0, 0, 0)
		, m_vTransform(0, 0, 0)
		, m_dwObjectID(1)
		, m_bRender(false)
	{

	}

};



class ObejctMgr : public Singleton<ObejctMgr>
{
	friend class Singleton<ObejctMgr>;

private:
	std::vector<std::shared_ptr<Object>> m_lObjectList;
	std::shared_ptr<Object>			   m_pPlayerObject;
public:
	std::vector<std::shared_ptr<Object>> GetObjectList() { return m_lObjectList; };
	void                    PushObject(std::shared_ptr<Object> obj,DWORD sessionId) { m_lObjectList[sessionId] = obj; };
	std::shared_ptr<Object> GetOtherObject(DWORD SessionID) { return m_lObjectList[SessionID];};
	void					DisconnectCharacter(DWORD SessionID);
	std::shared_ptr<Object> GetPlayerObject() { return m_pPlayerObject; };
	void					SetPlayerObject(std::shared_ptr<Object> obj) { m_pPlayerObject = obj; };

public:
	ObejctMgr()
		:m_lObjectList()
		, m_pPlayerObject()
	{
		m_lObjectList.resize(MAX_USER_SIZE);
	}
};