#pragma once
#include "Protocol.h"
#include "Packet.h"



void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y);

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y);


void CreateMyCharacter(Packet* Pack, DWORD SessionID, BYTE Direction, short X, short Y, int HP);
