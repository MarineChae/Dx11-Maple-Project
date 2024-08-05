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


#define	PACKET_CS_MOVE_START 10
#define	PACKET_CS_MOVE_END 11


#define	PACKET_CS_DISCONNECT_CHARACTER 255
//////////////////////////////////////////



///////////////캐릭터 액션값//////////////////



#define ACTION_MOVELEFT		0
#define ACTION_MOVERIGHT	1
#define ACTION_MOVEUP		2
#define ACTION_MOVEDOWN		3

#define ACTION_STAND			10
#define ACTION_FALL				11
#define ACTION_MOVELEFT_FALL	ACTION_FALL+ACTION_MOVELEFT
#define ACTION_MOVERIGHT_FALL	ACTION_FALL+ACTION_MOVERIGHT
#define ACTION_JUMP				100


#define ACTION_STANDJUMP		ACTION_JUMP+ACTION_STAND
#define ACTION_MOVELEFT_JUMP	ACTION_JUMP+ACTION_MOVELEFT
#define ACTION_MOVERIGHT_JUMP	ACTION_JUMP+ACTION_MOVERIGHT


////////////////////////////////////////////


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

