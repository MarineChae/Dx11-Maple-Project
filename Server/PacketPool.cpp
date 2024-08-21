#include "Netstd.h"
#include "PacketPool.h"
#include "Packet.h"



void PacketPool::Add(std::shared_ptr<Packet> packet,int CurrentScene)
{
	{
		std::lock_guard<std::shared_mutex>lock(m_pShardMutex);
		m_lPackList.push_back(std::make_pair(packet, CurrentScene));
	}
}

void PacketPool::Process(std::shared_ptr<Packet> packet)
{
}

PacketPool PacketPool::operator=(PacketPool& Packet)
{
	{
		std::lock_guard<std::shared_mutex>lock(m_pShardMutex);
		m_lPackList = Packet.m_lPackList;
		Packet.m_lPackList.clear();

	}

	return *this;
}

PacketPool::PacketPool(const PacketPool& pool)
{

}



