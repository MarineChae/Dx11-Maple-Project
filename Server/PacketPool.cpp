#include "Netstd.h"
#include "PacketPool.h"

void PacketPool::Add(UserPacket& packet)
{
	{
		std::lock_guard<std::shared_mutex>lock(m_pShardMutex);
		m_lPackList.push_back(&packet);
	}
}

void PacketPool::Process(UserPacket& packet)
{
}
