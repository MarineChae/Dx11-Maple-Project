#pragma once
#include"Packet.h"



BOOL PacketProc_MoveStart(std::shared_ptr<Packet> pack);
BOOL PacketProc_MoveEnd(std::shared_ptr<Packet> pack);
BOOL PacketProc_Attack(std::shared_ptr<Packet> pack);
BOOL PacketProc_SceneChange(std::shared_ptr<Packet> pack);
BOOL PacketProc_CreateMonster(std::shared_ptr<Packet> pack);
BOOL PacketProc_CreateObject(std::shared_ptr<Packet> pack);
BOOL PacketProc_UpdateMonster(std::shared_ptr<Packet> pack);

BOOL PacketProc_PlayerGetDamage(std::shared_ptr<Packet> pack);



BOOL PacketProc_CreateMyCharacter(std::shared_ptr<Packet> pack);

BOOL PacketProc_CreateOtherCharacter(std::shared_ptr<Packet> pack);


BOOL PacketProc_DisconnectOtherCharacter(std::shared_ptr<Packet> pack);

