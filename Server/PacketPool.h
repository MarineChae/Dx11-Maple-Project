#pragma once
#include"Netstd.h"


class User;
class Packet;


class PacketPool
{
private:
	std::shared_mutex m_pShardMutex;
	list<std::shared_ptr<Packet>> m_lPackList;

public:
	list<std::shared_ptr<Packet>>& GetPacketList() { return m_lPackList; };
	
public:
	void Add(std::shared_ptr<Packet> packet);
	void Process(std::shared_ptr<Packet> packet);
};

