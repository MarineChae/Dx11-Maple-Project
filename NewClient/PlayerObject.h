#pragma once
#include"SpriteObject.h"


class Skill;
class PlayerObject :public SpriteObject
{

private:
	DWORD	     m_dwCurrentAction;
	DWORD	     m_dwBeforeAction;
	int		     m_iMaxHP;
	int		     m_iHP;
	int		     m_CurrentiDirection;
	int		     m_BeforeDirection;
	bool	     m_bIsPlayable;
	DWORD	     m_dwActionInput;
	PLAYER_STATE m_PlayerState;
	TVector3     m_pMovePow;
	int			 m_CurrentScene;
	bool		 m_bOnLope;
	std::map<std::string ,std::shared_ptr<Skill>> m_vSkillList;
	std::shared_ptr<Skill>	m_pActivateSkill;

public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	bool	     m_bIsFalling;
	bool         m_bIsJump;
	TVector3     m_vBeforePos;
public:
	void SetMaxHp(int hp) { m_iMaxHP = hp; };
	int  GetMaxHp()	const{ return m_iMaxHP; };
	void SetHp(int hp) { m_iHP = hp; };
	int  GetHp() const { return m_iHP; };
	void InputAction();
	void InputKey();
	void PacketSendProc();
	void ChangeState(PLAYER_STATE state);
	void SetProgressBar();
	PLAYER_STATE GetPlayerState() const { return m_PlayerState; };
	void InsertSkill(std::shared_ptr<Skill> skill); 
	void SetActivateSkill(std::shared_ptr<Skill> skill);
	std::shared_ptr<Skill> FindSkillMap(std::string num);
	std::map<std::string, std::shared_ptr<Skill>> GetSkillList() { return m_vSkillList; }
public:
	virtual void  SetPlayerSprite() override;
	virtual void SetState(PLAYER_STATE state) override;
	virtual void SetFalling(bool state) { m_bIsFalling = state; };
	virtual bool GetJumping() { return m_bIsJump; };
	virtual void SetJumping(bool state) { m_bIsJump = state; };
	virtual bool GetFalling() { return m_bIsFalling; };
	virtual void SetCurrentScene(int  currentScene) { m_CurrentScene = currentScene; };
	virtual int GetCurrentScene() const { return m_CurrentScene; };
	virtual void SetOnLope(bool onLope) { m_bOnLope = onLope; };
	virtual bool GetOnLope() { return m_bOnLope; }
	virtual void SetBeforePos(TVector3 before) { m_vBeforePos = before; };
public:
	PlayerObject();
	virtual ~PlayerObject();

};



class ObejctMgr : public Singleton<ObejctMgr>
{
	friend class Singleton<ObejctMgr>;

private:
	std::vector<std::shared_ptr<PlayerObject>> m_lObjectList;
	std::shared_ptr<PlayerObject>			   m_pPlayerObject;
public:
	std::vector<std::shared_ptr<PlayerObject>> GetObjectList() { return m_lObjectList; };
	void                    PushObject(std::shared_ptr<PlayerObject> obj, DWORD sessionId) { m_lObjectList[sessionId] = obj; };
	std::shared_ptr<PlayerObject> GetOtherObject(DWORD SessionID) { return m_lObjectList[SessionID]; };
	void					DisconnectCharacter(DWORD SessionID);
	std::shared_ptr<PlayerObject> GetPlayerObject() { return m_pPlayerObject; };
	void					SetPlayerObject(std::shared_ptr<PlayerObject> obj) { m_pPlayerObject = obj; };

public:
	ObejctMgr()
		:m_lObjectList()
		, m_pPlayerObject()
	{
		m_lObjectList.resize(MAX_USER_SIZE);
	}
};