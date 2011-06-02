#include "Socket.h"

CSocket::CSocket(int i_socketDecsriptor, int i_id)
{
    m_socket = new QTcpSocket();
    if ( !m_socket->setSocketDescriptor(i_socketDecsriptor) )
    {
            m_id = 0;
    }
    m_id = i_id;
}

int CSocket::GetId()
{
    return m_id;
}

QTcpSocket *CSocket::GetSocket()
{
    return m_socket;
}

void CSocket::SendMsg(QString i_msg)
{
    m_socket->write(i_msg.toUtf8());
}
