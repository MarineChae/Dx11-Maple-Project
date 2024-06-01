#pragma once
#include"Netstd.h"
#include "User.h"

class User;
struct UserPacket
{
	UPACKET packet;
	User* m_pUser;

};


class PacketPool
{
private:
	std::shared_mutex m_pShardMutex;
	list<UserPacket*> m_lPackList;

public:
	list<UserPacket*>& GetPacketList() { return m_lPackList; };

public:
	void Add(UserPacket& packet);
	void Process(UserPacket& packet);
};

