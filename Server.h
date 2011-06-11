#ifndef CSERVER_H
#define CSERVER_H

#include <QtNetwork>

#include "Options.h"
#include "Session.h"

class CServer : public QTcpServer
{
    Q_OBJECT

public:
    CServer(COptions *, QObject *parent=0);
    bool Start();

public slots:
    void SlotRemoveSession();

protected:
    void incomingConnection(int i_socketDescriptor);

private:
    QList<CSession*> m_sessionList;
    CSession* m_currentSession;
    COptions* m_options;
    int m_port;
};

#endif // CSERVER_H
