#include "MakePacket.h"



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
