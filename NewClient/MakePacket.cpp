#include "pch.h"
#include "MakePacket.h"

void MoveStartPacket(Packet* pack, BYTE direction,DWORD SessionID ,short X, short Y,
					PLAYER_STATE state, BYTE isFalling, BYTE isJump)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 11 + sizeof(PLAYER_STATE);
	PacketHeader.PacketType = PACKET_CS_MOVE_START;

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

void MoveStopPacket(Packet* pack, BYTE direction, DWORD SessionID, short X, short Y, 
					PLAYER_STATE state, BYTE isFalling,BYTE isJump)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 11 + sizeof(PLAYER_STATE);
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

