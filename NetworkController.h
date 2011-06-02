#ifndef CNETWORKCONTROLLER_H
#define CNETWORKCONTROLLER_H

#include <QObject>

#include "Player.h"
#include "Socket.h"

class CNetworkController : public QObject
{
    Q_OBJECT

public:
    CNetworkController(int i_timeToStart, int i_maxNumPlayer, int i_timeOut, QObject *parent = 0);
    bool AddConnection(int);

public slots:
    void SlotReadMsg();
    void SlotDeleteConnection();
    void SlotSendConnId(int);
    void SlotSendErr();
    void SlotSendFinish();
    void SlotSendStart();
    void SlotSendState();
    void SlotSendTimeToStart();
    void SlotStep();

private:
    //CGame* m_gameGalcon;
    //CParser* m_parser;
    QList<CPlayer*> m_playerList;
    QList<CSocket*> m_socketList;

    QList<int> m_freeIdList;

    int m_timeToStart;
    int m_timeOut;

signals:
    void sentQuit();
};

#endif // CNETWORKCONTROLLER_H
