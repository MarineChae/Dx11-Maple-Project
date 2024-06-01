#pragma once
#include "Netstd.h"
#include "SessionMgr.h"

class NetBase
{
private:
	CRITICAL_SECTION m_Cs;
	SessionMgr		 m_SessionMgr;
public:
	SessionMgr& GetSessionMgr() { return m_SessionMgr; };

public:
	virtual int  SendPacket(User* user, UPACKET& packet);
	virtual int  RecvPacket(User* user, UPACKET& packet);
	virtual bool Broadcasting(UPACKET packet, int index);
public:
	NetBase();
	virtual ~NetBase();

};

