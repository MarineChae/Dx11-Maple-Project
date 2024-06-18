#include "Pch.h"
#include "IocpServer.h"
#include"Packet.h"
#include"IocpModel.h"
#include"User.h"
#include"PacketPool.h"
#include"SessionMgr.h"
#include"StreamPacket.h"

bool AcceptIocp::ThreadRun()
{
    if (m_pServer == nullptr)return false;

    SOCKADDR_IN clientaddr;
    int addlen = sizeof(clientaddr);
    SOCKET clientsock = accept(m_pServer->GetSocket(), (SOCKADDR*)&clientaddr, &addlen);
    if (clientsock == SOCKET_ERROR)
    {
        int iError = WSAGetLastError();
        if (iError != WSAEWOULDBLOCK)
        {
          // LogErrorA("WSAGetLastError");
            return false;
        }
    }
    else
    {

        std::shared_ptr<User> user = std::make_shared<User>(clientsock, clientaddr);
        user->bind(m_pServer->GetIocpModel()->m_hIocp);
        user->Recv();
        SessionMgr::GetInstance().PushUser(user);
        printf("Client Connect IP: %s Port:%d\n", inet_ntoa(user->m_Addr.sin_addr), ntohs(user->m_Addr.sin_port));
      
    }


    return true;
}

void IocpServer::AddPacket(Packet& packet, BYTE PacketType)
{
    FunctionIterator iter = GetExecutePacket().find(PacketType);
    if (iter != m_fnExecutePacket.end())
    {
        CallFunction call = iter->second;
        call(packet);
    }
}

void IocpServer::ChatMsg(Packet& packet)
{
    m_pBroadcastPacketPool->Add(packet);
}

void IocpServer::MoveStartPacket(Packet& packet)
{
    m_pBroadcastPacketPool->Add(packet);
}

int IocpServer::SendPacket(std::shared_ptr<User> pUser, Packet& packet)
{
    char* SendBuffer = (char*)&packet;
    pUser->m_wsaSendBuffer.buf = (char*)&packet;
    pUser->m_wsaSendBuffer.len = packet.GetDataSize();

    MyOV* ov = new MyOV(MyOV::MODE_SEND);

    int iSendByte = 0;
    int iTotalByte = 0;
    DWORD dwSendByte;
    int iRet = WSASend(pUser->m_Sock, &pUser->m_wsaSendBuffer, 1, &dwSendByte, 0, (LPOVERLAPPED)ov, NULL);
    if (iRet == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        if (err != WSA_IO_PENDING)
        {
            return -1;
        }
    }


    return packet.GetDataSize();
}

bool IocpServer::Init()
{
 

    WSADATA WsaData;
    int iRet = WSAStartup(MAKEWORD(2, 2), &WsaData);
    if (iRet != 0)
    {
        ERRORMSG(L"WSAStartup");
        return false;
    }

    SetSocket(socket(AF_INET, SOCK_STREAM, 0));

    SOCKADDR_IN SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = htons(INADDR_ANY);
    SockAddr.sin_port = htons(GetPort());
    SetAddr(SockAddr);

    int ret = bind(GetSocket(), (SOCKADDR*)&GetAddr(), sizeof(GetAddr()));
    if (ret == SOCKET_ERROR)
    {
        ERRORMSG(L"bind");
        return false;
    }
    ret = listen(GetSocket(), SOMAXCONN);
    if (ret == SOCKET_ERROR)
    {
        ERRORMSG(L"listen");
        return false;
    }

    std::cout << "Server Is Running!" << std::endl;



    m_IocpModel->Init();
    m_Accepter.Set(this);

    MyThread::Create();

    m_fnExecutePacket[PACKET_CHAT_MSG] = std::bind(&IocpServer::ChatMsg, this, std::placeholders::_1);
    m_fnExecutePacket[PACKET_CS_MOVE_START] = std::bind(&IocpServer::MoveStartPacket, this, std::placeholders::_1);
    m_fnExecutePacket[PACKET_CS_MOVE_END] = std::bind(&IocpServer::MoveStartPacket, this, std::placeholders::_1);

    return true;
}

bool IocpServer::ThreadRun()
{
    for (auto& data : m_pBroadcastPacketPool->m_PackList)
    {
        if (!Broadcasting(data))
        {

        }
    }
    m_pBroadcastPacketPool->m_PackList.clear();

    for (std::list<std::shared_ptr<User>>::iterator iterSend = SessionMgr::GetInstance().GetUserList().begin();
        iterSend != SessionMgr::GetInstance().GetUserList().end();)
    {

        std::shared_ptr<User> pUser = *iterSend;
        if (pUser->m_bConnected == false)
        {
            pUser->Close();
            iterSend = SessionMgr::GetInstance().GetUserList().erase(iterSend);
        }
        else
        {
            iterSend++;
        }

    }

    return true;
}

bool IocpServer::Release()
{

    return true;
}

bool IocpServer::Broadcasting(Packet* userpack)
{
    for (std::list<std::shared_ptr<User>>::iterator iterSend = SessionMgr::GetInstance().GetUserList().begin();
        iterSend != SessionMgr::GetInstance().GetUserList().end();
        iterSend++)
    {
        std::shared_ptr<User> pUser = *iterSend;
        if (pUser->m_bConnected == false)continue;
        int iSendByte = SendPacket(pUser, *userpack);
        if (iSendByte == SOCKET_ERROR)
        {
            ERRORMSG(L"SendPacket");
            pUser->m_bConnected = false;
            continue;
        }

    }
    return true;
}

IocpServer::IocpServer()
{
    m_IocpModel = std::make_shared<IocpModel>();
    m_pPacketPool = std::make_shared<PacketPool>();
    m_pBroadcastPacketPool = std::make_shared<PacketPool>();
}

