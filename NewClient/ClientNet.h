#pragma once

#include"pch.h"


class ClientNet
{
private:
	SOCKET m_SOCK;
	short m_PortNum = 12345;


public:
	int ConnentNetWork();


};

