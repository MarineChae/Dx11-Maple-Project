#pragma once
#include "Protocol.h"
#include "Packet.h"




void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state);

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state);

void CharacterStatePacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y);