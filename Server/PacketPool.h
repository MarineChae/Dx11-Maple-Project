#pragma once
#include"Netstd.h"
#include "User.h"

class User;
class Packet;


class PacketPool
{
private:
	std::shared_mutex m_pShardMutex;
	list<Packet*> m_lPackList;

public:
	list<Packet*>& GetPacketList() { return m_lPackList; };

public:
	void Add(Packet& packet);
	void Process(Packet& packet);
};

