#pragma once
#include "Protocol.h"
#include "Packet.h"



void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state);

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state);


void CreateMyCharacter(Packet* pack, DWORD SessionID, BYTE Direction, short X, short Y, int HP);
void CreateOtherCharacter(Packet* pack, DWORD SessionID, BYTE Direction, short X, short Y, int HP);
void DisConnectCharacter(Packet* pack, DWORD SessionID);
