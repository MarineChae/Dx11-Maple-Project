#pragma once
#include"Packet.h"



BOOL PacketProc_MoveStart(Packet* pack);
BOOL PacketProc_MoveEnd(Packet* pack);
BOOL PacketProc_CreateMyCharacter(Packet* pack);

BOOL PacketProc_CreateOtherCharacter(Packet* pack);


BOOL PacketProc_DisconnectOtherCharacter(Packet* pack);

