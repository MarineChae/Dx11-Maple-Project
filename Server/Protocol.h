#pragma once
#include <Windows.h>

#define PACKET_HEADER_SIZE 4
#define PACKET_MSG_BUFFER_SIZE 2048
#define PACKET_MAX_SIZE (PACKET_HEADER_SIZE+PACKET_MSG_BUFFER_SIZE)
#define NETWORK_PACKET_CODE	((BYTE)0x89)
#define NETWORK_PACKET_END	((BYTE)0x79)
#define MAX_USER_SIZE 63
#define NETWORK_WSABUFF_SIZE 512

////////////클라이언트 서버 전송 패킷/////////
#define	PACKET_CS_CREATE_MY_CHARACTER 1
#define	PACKET_CS_CREATE_OTHER_CHARACTER 2
#define	PACKET_CS_CREATE_MONSTER 3
#define	PACKET_CS_UPDATE_MONSTER_STATE 4


#define	PACKET_CS_MOVE_START 10
#define	PACKET_CS_MOVE_END 11
#define	PACKET_CS_SCENE_CHANGE 12



#define	PACKET_CS_CHRACTER_ATTACK 40
#define	PACKET_CS_MONSTER_GET_DAMAGE 100
#define	PACKET_CS_DISCONNECT_CHARACTER 255;
//////////////////////////////////////////



///////////////캐릭터 액션값//////////////////



#define ACTION_MOVE		0


#define ACTION_STAND	255





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


//https://zelon.tistory.com/424 
//주의하자
//bad pointer 오류 상황에서는 pragma pack 도 연관될 수 있다는 걸 깊이 새겨야겠다. 
#pragma pack (pop)


#define PACKET_CHAT_MSG		1000
#define PACKET_GAME_START	2000
#define PACKET_GMAE_END		3000

