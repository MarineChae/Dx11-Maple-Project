#pragma once
#define MAX_RECV_STREAM_SIZE (PACKET_MAX_SIZE*10)

class User;
class Packet;
class StreamPacket
{

public:
	StreamPacket();
	virtual ~StreamPacket();


public:
	Packet*  m_pPacketStart;
	PVOID	 m_pWritePos;
	PVOID	 m_pPacket;
	char	m_RecvBuffer[MAX_RECV_STREAM_SIZE];
	//volatail 컴파일러 최적화 방지 
	//멀티 스레딩에 있어서 코드의 순서를 보장,
	//개발의도와 다르게 최적화 되는 것을 방지하기 위해 사용한다.
	long long volatile m_iStartPos;
	long long volatile m_iWritePos;
	long long volatile m_iReadPos;
public:
	bool Put(char* recvBuffer, int iSize, User* user = NULL);
	bool GetPacket(User& user);
	bool ParityCheck();
	bool Reset();


};

