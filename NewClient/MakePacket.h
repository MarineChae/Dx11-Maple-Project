#pragma once
#include "Protocol.h"
#include "Packet.h"




void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void SceneChangePacket(Packet* pack, DWORD SessionID, BYTE SceneNum);


void CharacterStatePacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y);