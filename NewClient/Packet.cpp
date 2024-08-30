#include"pch.h"
#include<Windows.h>
#include<assert.h>

#include "Packet.h"
#include"Object.h"
Packet Packet::operator=(Packet& Packet)
{
	m_pReadPos = m_pBuffer;
	m_pWritePos = m_pBuffer;

	PutData(Packet.m_pReadPos, Packet.m_iDataSize);

	return *this;
}
Packet& Packet::operator<<(PLAYER_STATE statevalue)
{
	PutData(reinterpret_cast<char*>(&statevalue), sizeof(PLAYER_STATE));
	return *this;
}

Packet& Packet::operator>>(PLAYER_STATE& statevalue)
{
	GetData(reinterpret_cast<char*>(&statevalue), sizeof(PLAYER_STATE));
	return *this;
}
Packet& Packet::operator<<(MONSTER_STATE statevalue)
{
	PutData(reinterpret_cast<char*>(&statevalue), sizeof(MONSTER_STATE));
	return *this;
}

Packet& Packet::operator>>(MONSTER_STATE& statevalue)
{
	GetData(reinterpret_cast<char*>(&statevalue), sizeof(MONSTER_STATE));
	return *this;
}
Packet& Packet::operator<<(ObejctType statevalue)
{
	PutData(reinterpret_cast<char*>(&statevalue), sizeof(ObejctType));
	return *this;
}

Packet& Packet::operator>>(ObejctType& statevalue)
{
	GetData(reinterpret_cast<char*>(&statevalue), sizeof(ObejctType));
	return *this;
}

Packet& Packet::operator<<(BYTE bytevalue)
{
	PutData(reinterpret_cast<char*>(&bytevalue), sizeof(BYTE));
	return *this;
}

Packet& Packet::operator<<(char charvalue)
{
	PutData(&charvalue, sizeof(char));
	return *this;
}

Packet& Packet::operator<<(short shortvalue)
{
	PutData(reinterpret_cast<char*>(&shortvalue), sizeof(short));
	return *this;
}

Packet& Packet::operator<<(WORD wordvalue)
{
	PutData(reinterpret_cast<char*>(&wordvalue), sizeof(WORD));
	return *this;
}

Packet& Packet::operator<<(int intvalue)
{
	PutData(reinterpret_cast<char*>(&intvalue), sizeof(int));
	return *this;
}

Packet& Packet::operator<<(float floatvalue)
{
	PutData(reinterpret_cast<char*>(&floatvalue), sizeof(float));
	return *this;
}

Packet& Packet::operator<<(DWORD wordvalue)
{
	PutData(reinterpret_cast<char*>(&wordvalue), sizeof(DWORD));
	return *this;
}

Packet& Packet::operator<<(__int64 wordvalue)
{
	PutData(reinterpret_cast<char*>(&wordvalue), sizeof(__int64));
	return *this;
}

Packet& Packet::operator<<(double wordvalue)
{
	PutData(reinterpret_cast<char*>(&wordvalue), sizeof(double));
	return *this;
}

Packet& Packet::operator>>(BYTE& bytevalue)
{
	GetData(reinterpret_cast<char*>(&bytevalue), sizeof(BYTE));
	return *this;
}

Packet& Packet::operator>>(char& charvalue)
{
	GetData(&charvalue, sizeof(char));
	return *this;
}

Packet& Packet::operator>>(short& shortvalue)
{
	GetData(reinterpret_cast<char*>(&shortvalue), sizeof(short));
	return *this;
}

Packet& Packet::operator>>(WORD& wordvalue)
{
	GetData(reinterpret_cast<char*>(&wordvalue), sizeof(WORD));
	return *this;
}

Packet& Packet::operator>>(int& intvalue)
{
	GetData(reinterpret_cast<char*>(&intvalue), sizeof(int));
	return *this;
}

Packet& Packet::operator>>(float& floatvalue)
{
	GetData(reinterpret_cast<char*>(&floatvalue), sizeof(float));
	return *this;
}

Packet& Packet::operator>>(DWORD& wordvalue)
{
	GetData(reinterpret_cast<char*>(&wordvalue), sizeof(DWORD));
	return *this;
}

Packet& Packet::operator>>(__int64& wordvalue)
{
	GetData(reinterpret_cast<char*>(&wordvalue), sizeof(__int64));
	return *this;
}

Packet& Packet::operator>>(double& wordvalue)
{
	GetData(reinterpret_cast<char*>(&wordvalue), sizeof(double));
	return *this;
}

int Packet::PutData(char* pSrc, int iSrcSize)
{
	//버퍼에 공간이 없다면
	if (m_pWritePos + iSrcSize > m_pEndPos)
		return 0;
	
	memcpy(m_pWritePos, pSrc, iSrcSize);
	m_pWritePos += iSrcSize;

	m_iDataSize += iSrcSize;

	return iSrcSize;
}

int Packet::GetData(char* pScr, int iSrcSize)
{
	if (iSrcSize > m_iDataSize)
		return 0;
	
	memcpy(pScr, m_pReadPos, iSrcSize);
	m_pReadPos += iSrcSize;
	m_iDataSize -= iSrcSize;

	return iSrcSize;
}

void Packet::Init(int iBuffSize)
{
	m_iBufferSize = iBuffSize;

	if (NULL == m_pBuffer)
	{
		if (eBUFFER_DEFAULT < m_iBufferSize)
		{
			m_pBufferExpansion = new char[m_iBufferSize];
			m_pBuffer = m_pBufferExpansion;
		}
		else
		{
			m_pBufferExpansion = NULL;
			m_pBuffer = m_BufferDafault;
		}
	}
	assert(m_pBuffer);

	m_pStartPos = m_pBuffer;
	m_pEndPos = m_pBuffer + m_iBufferSize;
	m_pReadPos = m_pWritePos = m_pStartPos;
	m_iDataSize = 0;
}

int Packet::MoveWritePos(int iSize)
{
	if (0 > iSize) return 0;

	if (m_pWritePos + iSize > m_pEndPos)
		return 0;

	m_pWritePos += iSize;
	m_iDataSize += iSize;

	return iSize;
}

Packet::Packet()
	: m_pBuffer(NULL)
	,m_pBufferExpansion(NULL)
	, m_pStartPos(NULL)
	, m_pEndPos(NULL)
	, m_pReadPos(NULL)
	, m_pWritePos(NULL)
{
	Init();
}

Packet::Packet(int iBuffSize)
	: m_pBuffer(NULL)
	, m_pBufferExpansion(NULL)
	, m_pStartPos(NULL)
	, m_pEndPos(NULL)
	, m_pReadPos(NULL)
	, m_pWritePos(NULL)
{
	Init(iBuffSize);
}

Packet::Packet(const Packet& Pack)
	: m_pBuffer(NULL)
	, m_pBufferExpansion(NULL)
	, m_iBufferSize(0)
	, m_pStartPos(NULL)
	, m_pEndPos(NULL)
	, m_pReadPos(NULL)
	, m_pWritePos(NULL)
{
	Init(Pack.m_iBufferSize);
	PutData(Pack.m_pReadPos, Pack.m_iDataSize);
}


Packet::~Packet()
{
	if (NULL != m_pBufferExpansion)
		delete[] m_pBufferExpansion;
}
