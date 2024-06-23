#include"Netstd.h"
#include "PacketProc.h"
#include"IOCPServer.h"
#include "Packet.h"
#include "PlayerData.h"
#include"MakePacket.h"
BOOL PacketProc_MoveStart(DWORD Sessionid, Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;

    *pack >> byDirection;
    *pack >> dwSessionID;
    *pack >> shX;
    *pack >> shY;


    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);
    if (player == nullptr)
        return FALSE;
    std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

    player->SetXPos(shX);
    player->SetYPos(shY);
    MoveStartPacket(SendPack, byDirection, Sessionid, player->GetXPos(), player->GetYPos());
    
    IOCPServer::GetInstance().AddPacket(SendPack);

    return 0;
}

BOOL PacketProc_MoveEnd(DWORD Sessionid, Packet* pack)
{
    DWORD dwSessionID;
    BYTE byDirection;
    short shX;
    short shY;

    *pack >> byDirection;
    *pack >> dwSessionID; 
    *pack >> shX;
    *pack >> shY;


    auto player = PlayerDataMgr::GetInstance().GetPlayerData(Sessionid);

    if (player == nullptr)
        return FALSE;
    std::shared_ptr<Packet> SendPack = std::make_shared<Packet>();

    player->SetXPos(shX);
    player->SetYPos(shY);
    MoveStopPacket(SendPack, byDirection, Sessionid, player->GetXPos(), player->GetYPos());

    IOCPServer::GetInstance().AddPacket(SendPack);


    return 0;
}
