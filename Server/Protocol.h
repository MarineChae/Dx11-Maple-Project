#pragma once
#include <Windows.h>

#define PACKET_HEADER_SIZE 4
#define PACKET_MSG_BUFFER_SIZE 2048
#define PACKET_MAX_SIZE (PACKET_HEADER_SIZE+PACKET_MSG_BUFFER_SIZE)

#pragma pack(push,1)

//패킷 헤더로 어떤 패킷인지 확인
typedef struct
{
	WORD len;
	WORD type;

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

typedef struct
{
	char szName[13];
	int iID;
	char szMsg[1024];

}CHAT_MSG;

//https://zelon.tistory.com/424 
//주의하자
//bad pointer 오류 상황에서는 pragma pack 도 연관될 수 있다는 걸 깊이 새겨야겠다. 
#pragma pack (pop)


#define PACKET_CHAT_MSG		1000
#define PACKET_GAME_START	2000
#define PACKET_GMAE_END		3000

