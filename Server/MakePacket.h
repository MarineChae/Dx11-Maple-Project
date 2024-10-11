#pragma once
#include "Protocol.h"
#include "Packet.h"

class MonsterData;

void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, float X, float Y, int HP, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, float X, float Y, int HP, PLAYER_STATE state, BYTE isFalling, BYTE isJump);

void PlayerGetDamage(Packet* pack, DWORD SessionID, float damage);


void AttackPacket(Packet* pack, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump, char* skillname, char* skillNum);

void SpawnObjectPacket(Packet* pack, float X, float Y, float rotate, char* ObjectName, OBJECT_TYPE objectType, BYTE CurrentScene);

void SceneChangePacket(Packet* pack, DWORD SessionID, BYTE SceneNum);

void MonsterStateUpdatePacket(Packet* pack, MonsterData monster);

void CreateMyCharacter(Packet* pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene);
void CreateOtherCharacter(Packet* pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP ,BYTE CurrentScene);
void CreateMonster(Packet* pack, int ID, char* name, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene);
void DisConnectCharacter(Packet* pack, DWORD SessionID);
