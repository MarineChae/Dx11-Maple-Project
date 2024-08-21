#pragma once
#include"Netstd.h"


class User;
class Packet;


class PacketPool 
{
private:
	std::shared_mutex m_pShardMutex;
	list<std::pair<std::shared_ptr<Packet>,int>> m_lPackList;

public:
	list<std::pair<std::shared_ptr<Packet>, int>>& GetPacketList() { return m_lPackList; };
	
public:

	void Add(std::shared_ptr<Packet> packet,int CurrentScene);
	void Process(std::shared_ptr<Packet> packet);

	PacketPool operator = (PacketPool& Packet);

public:
	PacketPool() {};
	PacketPool(const PacketPool& pool);
};

