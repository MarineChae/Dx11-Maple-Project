#include "pch.h"
#include "MakePacket.h"

void MoveStartPacket(std::shared_ptr<Packet> pack, BYTE direction,DWORD SessionID ,float X, float Y,
					PLAYER_STATE state, BYTE isFalling, BYTE isJump,BYTE onLope, BYTE lopeup)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 17 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_START;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << SessionID;
	*pack << X;
	*pack << Y;
	*pack << isFalling;
	*pack << isJump;
	*pack << onLope;
	*pack << lopeup;
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void MoveStopPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y,
					PLAYER_STATE state, BYTE isFalling,BYTE isJump,BYTE onLope,BYTE lopeup)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 17 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_END;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << SessionID;
	*pack << X;
	*pack << Y;
	*pack << isFalling;
	*pack << isJump;
	*pack << onLope;
	*pack << lopeup;
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void AttackPacket(std::shared_ptr<Packet> pack, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump,char* skillname)
{
	int namelen = strlen(skillname);
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 18 + sizeof(PLAYER_STATE) + namelen;
	PacketHeader.PacketType = PACKET_CS_CHRACTER_ATTACK;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << namelen;//4
	pack->PutData(skillname, namelen);
	*pack << SessionID;//4
	*pack << X;//4
	*pack << Y;//4
	*pack << isFalling;//1
	*pack << isJump;//1
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void SceneChangePacket(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE SceneNum)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 5;
	PacketHeader.PacketType = PACKET_CS_SCENE_CHANGE;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << SessionID;
	*pack << SceneNum;
	*pack << (BYTE)NETWORK_PACKET_END;
}

