#pragma once
#include"Netstd.h"


class User;
class Packet;


class PacketPool 
{
private:
	std::shared_mutex m_pShardMutex;
	list<std::pair<Packet*,int>> m_lPackList;

public:
	list<std::pair<Packet*, int>>& GetPacketList() { return m_lPackList; };
	
public:
	void Add(Packet* packet,int CurrentScene);
	void Process(Packet* packet);
};

