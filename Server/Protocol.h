#pragma once
#include <Windows.h>
#include "Packet.h"
#define PACKET_HEADER_SIZE 4
#define PACKET_MSG_BUFFER_SIZE 512
#define PACKET_MAX_SIZE (PACKET_HEADER_SIZE+PACKET_MSG_BUFFER_SIZE)
#define NETWORK_PACKET_CODE	((BYTE)0x89)

#define NETWORK_PACKET_END	((BYTE)0x79)
#pragma pack(push,1)

//패킷 헤더로 어떤 패킷인지 확인


//패킷 헤더 -> 데이터
//패킷 헤더로 어떤 패킷인지 확인

typedef struct
{
	BYTE	PacketCode;			// 패킷코드 0x89 고정.
	BYTE	PacketSize;			// 패킷 사이즈.
	BYTE	PacketType;			// 패킷타입.
	BYTE	PacketTemp;			// 사용안함.

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


struct st_NETWORK_SESSION
{
	BOOL			bConnect;		// 접속여부.

	SOCKET			Socket;			// 현 접속의 TCP 소켓.
	DWORD			dwSessionID;	// 접속자의 고유 세션 ID.

	Packet			NetRecvQ;		// 수신 큐.
	Packet			NetSendQ;		// 송신 큐.

	DWORD			dwTrafficTick;	// 트래픽 체크를 위한 틱.  
	DWORD			dwTrafficCount;	// 1초마다 송신된 트래픽 수.

};




//https://zelon.tistory.com/424 
//주의하자
//bad pointer 오류 상황에서는 pragma pack 도 연관될 수 있다는 걸 깊이 새겨야겠다. 
#pragma pack (pop)


#define PACKET_CHAT_MSG		1000
#define PACKET_GAME_START	2000
#define PACKET_GMAE_END		3000

