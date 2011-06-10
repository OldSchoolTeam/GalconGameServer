#ifndef CSOCKET_H
#define CSOCKET_H

#include <QObject>
#include <QTcpSocket>

class CSocket : public QTcpSocket
{
    Q_OBJECT

public:
    CSocket(int i_id, QObject *parent = 0);
    int GetId();
    void SetId(int i_id);

    //! send message i_msg to client in UTF8 format
    void SendMsg(QString i_msg);

private:
    //! id connected client
    /*!
        if id = 0       - client disconnected
        if id = 1...8   - client connected
    */
    int m_id;
};

#endif // CSOCKET_H
