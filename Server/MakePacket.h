#pragma once
#include "Protocol.h"
#include "Packet.h"


void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void SceneChangePacket(Packet* pack, DWORD SessionID, BYTE SceneNum);

void CreateMyCharacter(Packet* pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene);
void CreateOtherCharacter(Packet* pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP ,BYTE CurrentScene);
void CreateMonster(Packet* pack,char* name, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene);
void DisConnectCharacter(Packet* pack, DWORD SessionID);
