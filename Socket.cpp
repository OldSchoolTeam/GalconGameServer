#include "Socket.h"

CSocket::CSocket(int i_id, QObject *parent) : QTcpSocket(parent)
{
    m_id = i_id;
}

int CSocket::GetId()
{
    return m_id;
}

void CSocket::SetId(int i_id)
{
    m_id = i_id;
}

void CSocket::SendMsg(QString i_msg)
{
    QByteArray msg;
    QDataStream out(&msg, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << i_msg.toUtf8();
    out.device()->seek(0);
    out << quint16(msg.size()-sizeof(quint16));
    write(msg);
}
