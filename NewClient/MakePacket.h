#pragma once
#include "Protocol.h"
#include "Packet.h"




void MoveStartPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void MoveStopPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);


void SceneChangePacket(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE SceneNum);


void CharacterStatePacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, short X, short Y);