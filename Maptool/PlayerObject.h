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
	bool	     m_bIsFalling;
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	
public:

	void InputAction();
	void InputKey();
	virtual void  SetPlayerSprite() override;
	void ChangeState(PLAYER_STATE state);
	PLAYER_STATE GetPlayerState() const { return m_PlayerState; };
	virtual void SetState(PLAYER_STATE state) override;
	
public:
	PlayerObject();
	virtual ~PlayerObject();

};

