#include <Windows.h>
#include "StreamPacket.h"

void StreamPacket::Init(int iBufferSize)
{
    if (NULL != m_pBuffer)
        delete[] m_pBuffer;

    if (0 >= iBufferSize)return;
    m_iBufferSize = iBufferSize;
    m_iReadPos = 0;
    m_iWritePos = 0;
    m_pBuffer = new char[iBufferSize];

}

int StreamPacket::Put(char* pData, int iSize)
{
    int iWrite;

    //남은 공간이 보낼 데이터보다 적을경우
    if (GetFreeSize() < iSize)
    {
        return 0;
    }

    if (0 >= iSize)
        return 0;

    if (m_iReadPos <= m_iWritePos)
    {
        //쓰기 위치가 읽기보다 뒤에있는경우 
        iWrite = m_iBufferSize - m_iWritePos;

        //쓰기공간이 충분한경우
        if (iWrite >= iSize)
        {
            //패킷버퍼에 쓰기 위치로부터 데이터의 사이즈만큼 복사
            memcpy(m_pBuffer + m_iWritePos, pData, iSize);
            //쓰기위치 이동
            m_iWritePos += iSize;
        }
        //쓰기공간이 부족한 경우 두 번에 나누어 복사 아마 부족할 일은 없을듯
        else
        {
            memcpy(m_pBuffer + m_iWritePos, pData, iWrite);
            memcpy(m_pBuffer , pData + iWrite, iSize - iWrite);
            m_iWritePos = iSize - iWrite;
        }

    }
    else
    {
        //패킷버퍼에 쓰기 위치로부터 데이터의 사이즈만큼 복사
        memcpy(m_pBuffer + m_iWritePos, pData, iSize);
        //쓰기위치 이동
        m_iWritePos += iSize;
    }


    m_iWritePos = m_iWritePos == m_iBufferSize ? 0 : m_iWritePos;


    return 0;
}

int StreamPacket::Get(char* pData, int iSize)
{
    return 0;
}

int StreamPacket::Peek(char* pData, int iSize)
{
    int iRead;

    if (GetUseSize() < iSize)
        iSize = GetUseSize();

    if (0 >= iSize)
        return 0;

    if (m_iReadPos <= m_iWritePos)
    {
        memcpy(pData, m_pBuffer + m_iReadPos, iSize);
    }
    else
    {
        iRead = m_iBufferSize - m_iReadPos;
        if (iRead >= iSize)
        {
            memcpy(pData, m_pBuffer + m_iReadPos, iSize);
        }
        else
        {
            memcpy(pData, m_pBuffer + m_iReadPos, iRead);
            memcpy(pData + iRead, m_pBuffer, iSize - iRead);
        }

    }



    return iSize;
}

void StreamPacket::RemoveData(int iSize)
{
    int iRead;

    if (GetUseSize() < iSize)
        iSize = GetUseSize();

    if (0 >= iSize)
        return;

    if (m_iReadPos <= m_iWritePos)
    {
        m_iReadPos += iSize;
    }
    else
    {
        iRead = m_iBufferSize - m_iReadPos;

        if (iRead >= iSize)
        {
            m_iReadPos += iSize;
        }
        else
        {

            m_iReadPos = iSize - iRead;
        }


    }
    m_iReadPos = m_iReadPos == m_iBufferSize ? 0 : m_iReadPos;
}

int StreamPacket::GetUseSize()
{
    if (m_iReadPos <= m_iWritePos)
    {
        return m_iWritePos - m_iReadPos;
    }
    else
    {
        return m_iBufferSize - m_iReadPos + m_iWritePos;
    }
}

int StreamPacket::GetFreeSize()
{
    return m_iBufferSize - (GetUseSize() - eBUFFER_BLANK);
}

StreamPacket::StreamPacket()
    :m_pBuffer(NULL)
    , m_iBufferSize(0)
    , m_iReadPos(0)
    ,m_iWritePos(0)
{
    Init(eBUFFER_DEFAULT);
}

StreamPacket::StreamPacket(int BuffSize)
    :m_pBuffer(NULL)
    , m_iBufferSize(0)
    , m_iReadPos(0)
    , m_iWritePos(0)
{
    Init(BuffSize);
}

StreamPacket::~StreamPacket()
{
}
