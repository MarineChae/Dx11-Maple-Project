#include"Netstd.h"
#include "MakePacket.h"
#include "PlayerData.h"
#include"MonsterData.h"

void MoveStartPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state, BYTE isFalling, BYTE isJump)
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

void MoveStopPacket(std::shared_ptr<Packet> pack, BYTE direction, DWORD SessionID, float X, float Y, PLAYER_STATE state,BYTE isFalling, BYTE isJump)
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

void CreateMyCharacter(std::shared_ptr<Packet> pack, DWORD dwSessionID, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene)
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

void CreateOtherCharacter(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE Direction, float X, float Y, int HP,BYTE CurrentScene)
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

void CreateMonster(std::shared_ptr<Packet> pack, int ID,char* name, BYTE Direction, float X, float Y, int HP, BYTE CurrentScene)
{
	int namelen = strlen(name);
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 22 + namelen;
	PacketHeader.PacketType = PACKET_CS_CREATE_MONSTER;



	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << namelen;
	pack->PutData(name, namelen);
	*pack << ID;
	*pack << Direction;
	*pack << X;
	*pack << Y;
	*pack << HP;
	*pack << CurrentScene;
	*pack << (BYTE)NETWORK_PACKET_END;
}

void DisConnectCharacter(std::shared_ptr<Packet> pack, DWORD SessionID)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 4;
	PacketHeader.PacketType = PACKET_CS_DISCONNECT_CHARACTER;


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << SessionID;
	*pack << (BYTE)NETWORK_PACKET_END;

}

void SceneChangePacket(std::shared_ptr<Packet> pack, DWORD SessionID, BYTE SceneNum)
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

void MonsterStateUpdatePacket(std::shared_ptr<Packet> pack, MonsterData monster)
{
	PACKET_HEADER PacketHeader;
	PacketHeader.PacketCode = NETWORK_PACKET_CODE;
	PacketHeader.PacketSize = 18;
	PacketHeader.PacketType = PACKET_CS_UPDATE_MONSTER_STATE;

	//위치 스테이트 체력 씬위치
	//추후에 점프 및 추락


	pack->PutData((char*)&PacketHeader, PACKET_HEADER_SIZE);
	*pack << monster.GetId();//4
	*pack << monster.GetDirection();//1
	*pack << monster.GetPos().x;//4
	*pack << monster.GetPos().y;//4
	*pack << monster.GetHP();//4
	*pack << monster.GetCurrentScene();//1
	*pack << (BYTE)NETWORK_PACKET_END;
}

