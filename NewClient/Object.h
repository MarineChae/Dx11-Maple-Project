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
	Portal,
	FALLING_OBJECT,
	LAZER_OBJECT,
	BALL_OBJECT,
	COLLIDER,
};


class Collider;
#define MAX_USER_SIZE 10000

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
	OBJECT_STATE m_ObjectState;
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
	OBJECT_STATE	GetObjectState()const { return m_ObjectState; };
	//setter
	void SetWorldMat(TMatrix mat) { m_WolrdMatrix = mat; };
	void		 SetObejctID(DWORD id) { m_dwObjectID = id; }
	virtual void SetTransform(TVector3 transform) { m_vTransform = transform; };
	void		 SetDestination(TVector3 Destination) { m_vDestination = Destination; };
	virtual void SetState(PLAYER_STATE state) {};
	virtual void SetDirection(BYTE dir) { };
	void SetObejctType(ObejctType type) { m_ObjectType = type; };
	TVector3 m_vRotate;
	void SetObjectState(OBJECT_STATE state) { m_ObjectState = state; }
	//virtual
	virtual void ChangeMonsterState(MONSTER_STATE state) {};
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
	virtual TVector3 GetResponPos() const{ return TVector3(); }
	virtual bool GetIsHit() const { return false; };
	virtual void SetIsHit(bool hit) { };
	virtual int GetID() const  { return -1; }
	virtual void SetID(int ID)   {  };
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
	bool GetRenderState()const {return m_bRender;}
	void ChangeObjectState();
public:


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	Object();
	virtual~Object() {};

};

