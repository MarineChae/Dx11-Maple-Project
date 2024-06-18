#pragma once
#include"User.h"
#include"Singleton.h"


class SessionMgr : public Singleton<SessionMgr>
{
	friend class Singleton<SessionMgr>;
private:
	std::list<std::shared_ptr<User>> m_vUserList;

public:
	std::list<std::shared_ptr<User>>& GetUserList()  { return m_vUserList; };
	
	void PushUser(std::shared_ptr<User> user) { m_vUserList.push_back(user); };


};

