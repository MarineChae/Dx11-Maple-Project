#pragma once
#include"User.h"

class SessionMgr
{
private:
	std::list<User*> m_vUserList;

public:
	std::list<User*>& GetUserList() { return m_vUserList; }

public:
	SessionMgr()
		:m_vUserList()
	{
	}

};

