#include "Netstd.h"
#include "PacketPool.h"
#include "Packet.h"

void PacketPool::Add(Packet& packet)
{
	{
		std::lock_guard<std::shared_mutex>lock(m_pShardMutex);
		m_lPackList.push_back(&packet);
	}
}

void PacketPool::Process(Packet& packet)
{
}
