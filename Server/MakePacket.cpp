#include"Netstd.h"
#include "MakePacket.h"
#include "PlayerData.h"


void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 15 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_START;
	PacketHeader.PacketTemp = 0;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << SessionID;
	*pack << X;
	*pack << Y;
	*pack << isFalling;
	*pack << isJump;
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state,BYTE isFalling, BYTE isJump)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 15 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_END;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << SessionID;
	*pack << X;
	*pack << Y;
	*pack << isFalling;
	*pack << isJump;
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void CreateMyCharacter(Packet* pack, DWORD dwSessionID, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 18;
	PacketHeader.PacketType = PACKET_CS_CREATE_MY_CHARACTER;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << dwSessionID;
	*pack << Direction;
	*pack << X;
	*pack << Y;
	*pack << HP;
	*pack << CurrentScene;
	*pack << (BYTE)NETWORK_PACKET_END;


}

void CreateOtherCharacter(Packet* pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP,BYTE CurrentScene)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 18;
	PacketHeader.PacketType = PACKET_CS_CREATE_OTHER_CHARACTER;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << SessionID;
	*pack << Direction;
	*pack << X;
	*pack << Y;
	*pack << HP;
	*pack << CurrentScene;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void CreateMonster(Packet* pack, char* name, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene)
{
	int namelen = strlen(name);
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 18 + namelen;
	PacketHeader.PacketType = PACKET_CS_CREATE_MONSTER;



	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << namelen;
	pack->PutData(name, namelen);
	*pack << Direction;
	*pack << X;
	*pack << Y;
	*pack << HP;
	*pack << CurrentScene;
	*pack << (BYTE)NETWORK_PACKET_END;
}

void DisConnectCharacter(Packet* pack, DWORD SessionID)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 4;
	PacketHeader.PacketType = PACKET_CS_DISCONNECT_CHARACTER;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << SessionID;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void SceneChangePacket(Packet* pack, DWORD SessionID, BYTE SceneNum)
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

