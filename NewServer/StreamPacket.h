#pragma once

class StreamPacket
{
	enum StreamPacket_SQ
	{
		eBUFFER_DEFAULT = 20960,		// 버퍼의 기본 크기.
		eBUFFER_BLANK = 8				// 확실한 구분을 위해 8Byte 의 빈공간.

	};

private:
	char* m_pBuffer;
	int	  m_iBufferSize;
	int	  m_iReadPos;
	int	  m_iWritePos;

public:
	void Init(int iBufferSize);
	int  Put(char* pData, int iSize);
	int  Get(char* pData, int iSize);
	int  Peek(char* pData, int iSize);
	void RemoveData(int iSize);

public:
	int GetUseSize();
	int GetFreeSize();

public:
	StreamPacket();
	StreamPacket(int BuffSize);
	~StreamPacket();
};
