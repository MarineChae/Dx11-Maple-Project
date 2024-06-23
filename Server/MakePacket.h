#pragma once
#include "Protocol.h"
#include "Packet.h"



void MoveStartPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, short X, short Y);

void MoveStopPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, short X, short Y);


void CreateMyCharacter(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE Direction, short X, short Y, int HP);
void CreateOtherCharacter(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE Direction, short X, short Y, int HP);
void DisConnectCharacter(std::shared_ptr<Packet> pack, DWORD SessionID);
