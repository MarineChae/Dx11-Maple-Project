#pragma once
#include "User.h"


struct UserPacket
{
	UPACKET packet;
	

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

