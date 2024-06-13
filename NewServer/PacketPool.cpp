#include "Pch.h"
#include "PacketPool.h"

void PacketPool::Add(Packet& packet)
{
	{
		std::lock_guard<std::shared_mutex>lock(m_pSharedMutex);
		m_PackList.push_back(&packet);
	}
}

void PacketPool::Process(Packet& packet)
{
	{
		std::shared_lock<std::shared_mutex>lock(m_pSharedMutex);



	}
}
