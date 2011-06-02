#ifndef CSOCKET_H
#define CSOCKET_H

#include <QTcpSocket>

class CSocket
{
public:
    CSocket(int i_socketDecsriptor, int i_id);
    void SendMsg(QString);
    int GetId();
    QTcpSocket *GetSocket();

private:
    int m_id;
    QTcpSocket *m_socket;
};

#endif // CSOCKET_H
