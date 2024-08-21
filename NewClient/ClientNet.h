#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>

class Packet;
class StreamPacket;

extern SOCKET m_SOCK;
extern short m_PortNum;
extern StreamPacket NetSendQ;



BOOL connentNetWork();
BOOL NetworkProc(WPARAM wParam, LPARAM lParam);
BOOL networkClean();
BOOL NetSendPacket(std::shared_ptr<Packet> packet);
DWORD NetCompleteRecvPacket();
BOOL PacketProc(BYTE byPacketType, std::shared_ptr<Packet> pack);

BOOL NetSendEvent();
BOOL NetRecvEvent();