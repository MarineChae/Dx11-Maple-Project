#pragma once


class Packet
{
	enum ePacket
	{
		eBUFFER_DEFAULT = 512		// 패킷의 기본 버퍼 사이즈.
	};
private:

	char m_BufferDafault[512];
	char* m_pBufferExpansion;

	char* m_pBuffer;
	int m_iBufferSize;

	char* m_pStartPos;
	char* m_pEndPos;

	char* m_pReadPos;
	char* m_pWritePos;

	int m_iDataSize;

	BYTE m_PacketType;
public:

	Packet operator = (Packet& Packet);


	Packet& operator << (BYTE bytevalue);
	Packet& operator << (char charvalue);

	Packet& operator << (short shortvalue);
	Packet& operator << (WORD wordvalue);

	Packet& operator << (int intvalue);
	Packet& operator << (float floatvalue);
	Packet& operator << (DWORD wordvalue);

	Packet& operator << (__int64 wordvalue);
	Packet& operator << (double wordvalue);


	Packet& operator >> (BYTE& bytevalue);
	Packet& operator >> (char& charvalue);

	Packet& operator >> (short& shortvalue);
	Packet& operator >> (WORD& wordvalue);

	Packet& operator >> (int& intvalue);
	Packet& operator >> (float& floatvalue);
	Packet& operator >> (DWORD& wordvalue);

	Packet& operator >> (__int64& wordvalue);
	Packet& operator >> (double& wordvalue);

public:

	char* GetBufferPointer() { return m_pBuffer; };
	int GetBufferSize() const { return m_iBufferSize; };
	int GetDataSize() const { return m_iDataSize; };
	int PutData(char* pSrc, int iSrcSize);
	int GetData(char* pScr, int iSrcSize);
	void Init(int iBuffSize = 512);
	int MoveWritePos(int iSize);
	void SetPacketType(BYTE type) { m_PacketType = type; }
public:

	Packet();
	Packet(int iBuffSize);
	Packet(const Packet& Pack);
	~Packet();
};

