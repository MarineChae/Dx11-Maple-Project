#pragma once

#include"ObjectPool.h"


class Packet;
class StreamPacket;
struct MyOV : public ObjectPool<MyOV>
{
	enum { MODE_RECV = 0, MODE_SEND = 1, };
	OVERLAPPED ov;
	int flag;
	MyOV(int value) : flag(value)
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
	}
	MyOV()
	{
		ZeroMemory(&ov, sizeof(OVERLAPPED));
		flag = MyOV::MODE_RECV;
	}
};


class User
{
public:
	bool							 m_bConnected;
	SOCKET							 m_Sock;
	SOCKADDR_IN						 m_Addr;
	std::shared_ptr<StreamPacket>	 m_pStreamPacket;
	std::list<Packet*>				 m_lPacketList;
	WSABUF							 m_wsaRecvBuffer;
	WSABUF							 m_wsaSendBuffer;
	char							 m_buffer[4096];

public:
	void Close();
	void bind(HANDLE iocp);
	void Recv();
	void Dispatch(DWORD dwTransfer, OVERLAPPED* ov);
	bool ParseStreamPacket(Packet& pack, PACKET_HEADER& hd);



public:
	User();

	User(SOCKET sock, SOCKADDR_IN addr);


};

