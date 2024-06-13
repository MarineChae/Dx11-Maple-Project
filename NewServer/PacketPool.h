#pragma once

class Packet;
class PacketPool
{
public:
	std::shared_mutex m_pSharedMutex;
	std::list<Packet*> m_PackList;
	void Add(Packet& packet);
	void Process(Packet& packet);

};



