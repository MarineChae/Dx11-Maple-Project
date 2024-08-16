#include "Netstd.h"
#include "PacketPool.h"
#include "Packet.h"



void PacketPool::Add(Packet* packet,int CurrentScene)
{
	{
		std::lock_guard<std::shared_mutex>lock(m_pShardMutex);
		m_lPackList.push_back(std::make_pair(packet, CurrentScene));
	}
}

void PacketPool::Process(Packet* packet)
{
}
