#pragma once
#include"SpriteObject.h"

class PlayerObject :public SpriteObject
{

private:
	DWORD   m_dwCurrentAction;
	DWORD   m_dwBeforeAction;
	int	    m_iHP;
	int	    m_CurrentiDirection;
	int	    m_BeforeDirection;
	bool    m_bIsPlayable;
	DWORD	m_dwActionInput;


public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

public:

	void InputAction();
	void InputKey();

public:
	PlayerObject();
	virtual ~PlayerObject();

};

