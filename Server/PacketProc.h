#pragma once


class Packet;
BOOL PacketProc_MoveStart(DWORD Sessionid ,Packet* pack);
BOOL PacketProc_MoveEnd(DWORD Sessionid, Packet* pack);
BOOL PacketProc_SceneChange(DWORD Sessionid, Packet* pack);