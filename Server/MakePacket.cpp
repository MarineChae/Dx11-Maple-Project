#include"Netstd.h"
#include "MakePacket.h"
#include "PlayerData.h"

void MoveStartPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state, BYTE isFalling)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 10 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_START;
	PacketHeader.PacketTemp = 0;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << SessionID;
	*pack << X;
	*pack << Y;
	*pack << isFalling;
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, PLAYER_STATE state,BYTE isFalling)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 10 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_END;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << SessionID;
	*pack << X;
	*pack << Y;
	*pack << isFalling;
	*pack << state;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void CreateMyCharacter(Packet* pack, DWORD dwSessionID, BYTE Direction, short X, short Y, int HP)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 13;
	PacketHeader.PacketType = PACKET_CS_CREATE_MY_CHARACTER;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << dwSessionID;
	*pack << Direction;
	*pack << X;
	*pack << Y;
	*pack << HP;
	*pack << (BYTE)NETWORK_PACKET_END;


}

void CreateOtherCharacter(Packet* pack, DWORD SessionID, BYTE Direction, short X, short Y, int HP)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 13;
	PacketHeader.PacketType = PACKET_CS_CREATE_OTHER_CHARACTER;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << SessionID;
	*pack << Direction;
	*pack << X;
	*pack << Y;
	*pack << HP;
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

