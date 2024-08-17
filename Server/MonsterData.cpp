#include"Netstd.h"
#include "MonsterData.h"



void MonsterData::Create(char* name, DWORD Action, BYTE Direction, short X, short Y, int HP)
{
	strcpy_s(m_csMonsterName,name);
	m_dwAction = Action;
	m_byDirection = Direction;
	m_shX = X;
	m_shY = Y;
	m_iHP = HP;
}

MonsterData::MonsterData()
	:m_csMonsterName()
	,m_bIsDead(false)
	, m_fResponTime(0.0f)
	, m_dwAction(0)
	, m_byDirection(0)
	, m_shX(0)
	, m_shY(0)
	, m_iHP(0)
{

}

MonsterData::~MonsterData()
{
}
