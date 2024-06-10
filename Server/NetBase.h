#pragma once
#include "Netstd.h"
#include "User.h"

class NetBase
{
private:
	CRITICAL_SECTION m_Cs;

public:
	virtual int  SendPacket(User* user, UPACKET& packet);
	virtual int  RecvPacket(User* user, UPACKET& packet);
	virtual bool Broadcasting(UPACKET packet, int index);
public:
	NetBase();
	virtual ~NetBase();

};

