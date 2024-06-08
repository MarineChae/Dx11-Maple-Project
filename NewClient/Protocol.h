#pragma once
#include <Windows.h>

#define PACKET_HEADER_SIZE 4
#define PACKET_MSG_BUFFER_SIZE 2048
#define PACKET_MAX_SIZE (PACKET_HEADER_SIZE+PACKET_MSG_BUFFER_SIZE)
#define NETWORK_PACKET_CODE	((BYTE)0x89)

#define NETWORK_PACKET_END	((BYTE)0x79)

#define	PACKET_CS_MOVE_START 10


#define NETWORK_WSABUFF_SIZE 512


#pragma pack(push,1)

//��Ŷ ����� � ��Ŷ���� Ȯ��
//��Ŷ ��� -> ������
//��Ŷ ����� � ��Ŷ���� Ȯ��

typedef struct
{
	BYTE	PacketCode;			// ��Ŷ�ڵ� 0x89 ����.
	BYTE	PacketSize;			// ��Ŷ ������.
	BYTE	PacketType;			// ��ŶŸ��.
	BYTE	PacketTemp;			// ������.

}PACKET_HEADER;


typedef struct
{
	PACKET_HEADER PacketHeader;
	char msg[2048];
}UPACKET;

typedef struct
{
	char szName[13];
	int iID;
	char szMsg[1024];

}CHAT_MSG;


//https://zelon.tistory.com/424 
//��������
//bad pointer ���� ��Ȳ������ pragma pack �� ������ �� �ִٴ� �� ���� ���ܾ߰ڴ�. 
#pragma pack (pop)


#define PACKET_CHAT_MSG		1000
#define PACKET_GAME_START	2000
#define PACKET_GMAE_END		3000

