#ifndef CNETWORKCONTROLLER_H
#define CNETWORKCONTROLLER_H

#include <QObject>
#include <QTimer>
//#include <QString>

#include "Player.h"
#include "Socket.h"
#include "Parser.h"

class CNetworkController : public QObject
{
    Q_OBJECT

public:
    CNetworkController(int i_timeToStart, int i_maxNumPlayer, int i_timeOut, QObject *parent = 0);
    bool AddConnection(int);

public slots:
    void SlotReadMsg();
    void SlotDeleteConnection();
    void SlotSendConnId(CConnMsg* );
    void SlotSendErr();
    void SlotSendFinish();
    void SlotSendStart();
    void SlotSendState();
    void SlotSendTimeToStart();
    void SlotStep();

private:
    void sentToAll(QString );

    //CGame* m_gameGalcon;
    bool gameIsStarted;
    CParser* m_parser;
    QTimer *m_timer;
    QList<CPlayer*> m_playerList;
    QList<CSocket*> m_socketList;

    QList<int> m_freeIdList;

    int m_timeToStart;
    int timeToStart;
    int m_timeOut;

signals:
    void sentQuit();
};

#endif // CNETWORKCONTROLLER_H
