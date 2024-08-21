#pragma once


class Packet;
BOOL PacketProc_MoveStart(DWORD Sessionid ,std::shared_ptr<Packet> pack);
BOOL PacketProc_MoveEnd(DWORD Sessionid, std::shared_ptr<Packet> pack);
BOOL PacketProc_SceneChange(DWORD Sessionid, std::shared_ptr<Packet> pack);