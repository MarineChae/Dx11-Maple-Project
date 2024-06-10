#include "MakePacket.h"
#include "PlayerData.h"


void MoveStartPacket(Packet* pack, BYTE direction, short X, short Y)
{
	PACKET_HEADER PacketHeader;

	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 5;
	PacketHeader.PacketType = PACKET_CS_MOVE_START;
	PacketHeader.PacketTemp = 0;

	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << X;
	*pack << Y;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void MoveStopPacket(Packet* pack, BYTE direction, short X, short Y)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 5;
	PacketHeader.PacketType = PACKET_CS_MOVE_END;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);

	*pack << direction;
	*pack << X;
	*pack << Y;
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

