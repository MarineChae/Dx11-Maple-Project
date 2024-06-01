#include "Netstd.h"
#include "StreamPacket.h"
#include "PacketPool.h"
#include "IOCPServer.h"

bool StreamPacket::Put(char* recvBuffer, int iSize, User* user)
{
    Reset();
    if ((m_iWritePos + iSize) >= MAX_RECV_STREAM_SIZE)
    {
        if (m_iReadPos > 0)
        {
            //memcpy���� �����ѹ�� ������ �޸𸮸� ���ۿ� ���� �� dest�� ������ �޸� �ٿ�����
            memmove(m_RecvBuffer, &m_RecvBuffer[m_iStartPos], m_iReadPos);
        }
        InterlockedExchange64(&m_iStartPos, 0);
        InterlockedExchange64(&m_iWritePos, m_iReadPos);
        InterlockedExchangePointer(&m_pWritePos, (void*)m_RecvBuffer[m_iWritePos]);
    }
    CopyMemory(m_pWritePos, recvBuffer, iSize);
    InterlockedAdd64(&m_iWritePos, iSize);
    InterlockedAdd64(&m_iReadPos, iSize);
    InterlockedExchangePointer(&m_pWritePos, (void*)m_RecvBuffer[m_iWritePos]);

    return true;
}

bool StreamPacket::GetPacket(User& user)
{
    int iCurrentPacket = 0;
    if (m_iReadPos < PACKET_HEADER_SIZE)
    {
        return true;
    }
    InterlockedExchangePointer(&m_pPacket, (void*)&m_RecvBuffer[m_iStartPos]);
    if (ParityCheck() == false)
    {
        return false;
    }
    if (m_iReadPos >= m_pPacketStart->PacketHeader.len)
    {

        do
        {
            UserPacket addpacket = { 0, };
            CopyMemory(&addpacket, m_pPacketStart, m_pPacketStart->PacketHeader.len);
            IOCPServer::GetInstance().AddPacket(addpacket);

            InterlockedAdd64(&m_iReadPos, -m_pPacketStart->PacketHeader.len);
            InterlockedAdd64(&m_iStartPos, m_pPacketStart->PacketHeader.len);
            InterlockedExchangePointer(&m_pPacket, (void*)m_RecvBuffer[m_iStartPos]);

            if (m_iReadPos < PACKET_HEADER_SIZE || m_iReadPos < m_pPacketStart->PacketHeader.len)
            {
                break;
            }
            if (ParityCheck() == false)
            {
                return false;
            }
        } while (m_iReadPos >= m_pPacketStart->PacketHeader.len);


    }


    return true;
}

bool StreamPacket::ParityCheck()
{
    //m_pPacket�� ��䰪�� ������
    m_pPacketStart = (UPACKET*)InterlockedExchangePointer(&m_pPacket, nullptr);
    return true;
}

bool StreamPacket::Reset()
{
    ZeroMemory(m_RecvBuffer, sizeof(char) * MAX_RECV_STREAM_SIZE);
    m_iStartPos = 0;
    m_iReadPos = 0;
    m_iWritePos = 0;
    m_pPacket = nullptr;
    m_pPacketStart = nullptr;
    m_pWritePos = nullptr;
    //m_pPacket �� m_RecvBuffer[m_iStartPos]�ּҷ� ��ȯ �� �ʱ�ȭ
    InterlockedExchangePointer(&m_pPacket, (void*)&m_RecvBuffer[m_iStartPos]);
    m_pPacketStart = (UPACKET*)m_pPacket;
    m_pWritePos = m_pPacket;

    return true;
}

StreamPacket::StreamPacket()
{
    Reset();
}

StreamPacket::~StreamPacket()
{
}
