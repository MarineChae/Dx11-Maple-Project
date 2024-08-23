#pragma once
#include"DxRenderer.h"

enum SceneNum
{
	Lobby,
	BossRoom1,
	BossRoom2,
	BossRoom3,
};
enum ObejctType
{
	Defalut,
	Portal
};


class Collider;
#define MAX_USER_SIZE 63

class Object : public DxRenderer
{


private:
	TMatrix m_WolrdMatrix;
	TMatrix m_ViewMatrix;
	TMatrix m_ProjMatrix;

private:
	TVector3 m_vScale;
	TVector3 m_vTransform;
	TVector3 m_vDestination;
private:

	DWORD m_dwObjectID;
	bool  m_bRender;
	std::shared_ptr<Collider> m_pCollider;
	ObejctType m_ObjectType;
public:
	//getter
	DWORD	 GetObejctID() const { return m_dwObjectID; }
	TVector3 GetTransform() const { return m_vTransform; };
	TVector3 GetDestination() const { return m_vDestination; };
	TMatrix& GetWorldMat() { return m_WolrdMatrix; }
	TMatrix& GetViewMat() { return m_ViewMatrix; }
	TMatrix& GetProjectionMat() { return m_ProjMatrix; }
	std::shared_ptr<Collider> GetCollider() const;
	ObejctType	GetObjectType() const { return m_ObjectType; };

	//setter
	void		 SetObejctID(DWORD id) { m_dwObjectID = id; }
	virtual void SetTransform(TVector3 transform) { m_vTransform = transform; };
	void		 SetDestination(TVector3 Destination) { m_vDestination = Destination; };
	virtual void SetState(PLAYER_STATE state) {};
	virtual void SetDirection(BYTE dir) { };
	void SetObejctType(ObejctType type) { m_ObjectType = type; };
	TVector3 m_vRotate;

	//virtual
	virtual void ChangeState(PLAYER_STATE state) {};
	virtual TVector3 GetCollisionPoint() const { return TVector3(); };
	virtual void SetFalling(bool state) {};
	virtual bool GetFalling() { return false; };
	virtual void SetJumping(bool state) {};
	virtual bool GetJumping() { return false; };
	virtual void SetCurrentScene(int  currentScene) {};
	virtual int GetCurrentScene() const { return -1; };
	virtual std::shared_ptr<SpriteData> GetSpriteInfo() { return nullptr; };
	virtual int GetNextSceneNum()const { return -1; }
	virtual void SetNextSceneNum(int num) {}
	virtual void SetOnLope(bool onLope){}
	virtual bool GetOnLope() { return false; }
	virtual void SetBeforePos(TVector3 before) {};
	virtual bool GetIsDead() { return false; }
	virtual void SetIsDead(bool dead) {};

public:
	virtual bool CreateVertexData();
	virtual bool CreateIndexData();
	virtual void UpdataMatrix();
	virtual bool Create(std::wstring FileName, std::wstring ShaderFileName);
	virtual bool Create(const Texture* tex, const Shader* shader);
	virtual void SetMatrix(TMatrix* WolrdMatrix, TMatrix* ViewMatrix, TMatrix* ProjMatrix);
	virtual void SetScale(TVector3 scale);
	virtual TVector3 GetScale() const { return m_vScale; };
	virtual void SetMatrix();
	virtual void SetRenderState(bool state) {m_bRender = state;}

public:


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	Object();
	virtual~Object() {};

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