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

    //���� ������ ���� �����ͺ��� �������
    if (GetFreeSize() < iSize)
    {
        return 0;
    }

    if (0 >= iSize)
        return 0;

    if (m_iReadPos <= m_iWritePos)
    {
        //���� ��ġ�� �б⺸�� �ڿ��ִ°�� 
        iWrite = m_iBufferSize - m_iWritePos;

        //��������� ����Ѱ��
        if (iWrite >= iSize)
        {
            //��Ŷ���ۿ� ���� ��ġ�κ��� �������� �����ŭ ����
            memcpy(m_pBuffer + m_iWritePos, pData, iSize);
            //������ġ �̵�
            m_iWritePos += iSize;
        }
        //��������� ������ ��� �� ���� ������ ���� �Ƹ� ������ ���� ������
        else
        {
            memcpy(m_pBuffer + m_iWritePos, pData, iWrite);
            memcpy(m_pBuffer , pData + iWrite, iSize - iWrite);
            m_iWritePos = iSize - iWrite;
        }

    }
    else
    {
        //��Ŷ���ۿ� ���� ��ġ�κ��� �������� �����ŭ ����
        memcpy(m_pBuffer + m_iWritePos, pData, iSize);
        //������ġ �̵�
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
