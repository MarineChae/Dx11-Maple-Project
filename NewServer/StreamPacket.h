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
	//volatail �����Ϸ� ����ȭ ���� 
	//��Ƽ �������� �־ �ڵ��� ������ ����,
	//�����ǵ��� �ٸ��� ����ȭ �Ǵ� ���� �����ϱ� ���� ����Ѵ�.
	long long volatile m_iStartPos;
	long long volatile m_iWritePos;
	long long volatile m_iReadPos;
public:
	bool Put(char* recvBuffer, int iSize, User* user = NULL);
	bool GetPacket(User& user);
	bool ParityCheck();
	bool Reset();


};

