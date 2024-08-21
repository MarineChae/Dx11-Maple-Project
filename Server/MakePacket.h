#pragma once
#include "Protocol.h"
#include "Packet.h"

class MonsterData;

void MoveStartPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void MoveStopPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void SceneChangePacket(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE SceneNum);

void MonsterStateUpdatePacket(std::shared_ptr<Packet> pack, MonsterData monster);

void CreateMyCharacter(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene);
void CreateOtherCharacter(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP ,BYTE CurrentScene);
void CreateMonster(std::shared_ptr<Packet> pack, int ID, char* name, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene);
void DisConnectCharacter(std::shared_ptr<Packet> pack, DWORD SessionID);
