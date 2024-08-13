#pragma once
#include"SpriteObject.h"


class PlayerObject :public SpriteObject
{

private:
	DWORD	     m_dwCurrentAction;
	DWORD	     m_dwBeforeAction;
	int		     m_iHP;
	int		     m_CurrentiDirection;
	int		     m_BeforeDirection;
	bool	     m_bIsPlayable;
	DWORD	     m_dwActionInput;
	PLAYER_STATE m_PlayerState;
	TVector3     m_pMovePow;
	SceneNum     m_CurrentScene;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	bool	     m_bIsFalling;
	bool         m_bIsJump;
	TVector3     m_vBeforePos;
public:

	void InputAction();
	void InputKey();
	void PacketSendProc();
	void ChangeState(PLAYER_STATE state);
	PLAYER_STATE GetPlayerState() const { return m_PlayerState; };

public:
	virtual void  SetPlayerSprite() override;
	virtual void SetState(PLAYER_STATE state) override;
	virtual void SetFalling(bool state) { m_bIsFalling = state; };
	virtual bool GetJumping() { return m_bIsJump; };
	virtual void SetJumping(bool state) { m_bIsJump = state; };
	virtual bool GetFalling() { return m_bIsFalling; };
	virtual void SetCurrentScene(SceneNum  currentScene) { m_CurrentScene = currentScene; };
	virtual SceneNum GetCurrentScene() const { return m_CurrentScene; };

public:
	PlayerObject();
	virtual ~PlayerObject();

};

