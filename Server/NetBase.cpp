#include "Netstd.h"
#include "NetBase.h"

//���� �̿�� ����
int NetBase::SendPacket(User* user, UPACKET& packet)
{
    return 0;
}

int NetBase::RecvPacket(User* user, UPACKET& packet)
{

    return 0;
}

bool NetBase::Broadcasting(UPACKET packet, int index)
{
    return false;
}

NetBase::NetBase()
    : m_Cs()
{

}

NetBase::~NetBase()
{
}
