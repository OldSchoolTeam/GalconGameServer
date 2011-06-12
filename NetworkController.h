#ifndef CNETWORKCONTROLLER_H
#define CNETWORKCONTROLLER_H

#include <QObject>
#include <QTimer>

#include "Player.h"
#include "Socket.h"
#include "Parser.h"
#include "Game.h"
#include "FinishMsg.h"
#include "StepMsg.h"

class CNetworkController : public QObject
{
    Q_OBJECT

public:
    CNetworkController(int i_timeToStart,
                       int i_maxNumPlayer,
                       int i_timeOut,
                       QObject *parent = 0);
    ~CNetworkController();
    bool AddConnection(int);

public slots:
    void SlotReadMsg();
    void SlotDeleteConnection();
    void SlotSendConnId(CConnMsg* );
    //void SlotSendErr();
    void SlotSendFinish(CFinishMsg*);
    void SlotSendStart(CStartMsg*);
    void SlotSendState();
    void SlotSendTimeToStart();
    void SlotStep(CStepMsg *);

private:
    void sentToAll(QString );

    CGame* m_gameGalcon;
    CParser* m_parser;
    QTimer *m_timer;

    QList<CPlayer*> m_playerList;
    QList<CSocket*> m_socketList;

    QList<int> m_freeIdList;

    // true - game started
    bool gameIsStarted;

    int m_timeToStart;  // etalon time
    int timeToStart;    // var time
    int m_timeOut;

signals:
    void sentQuit();
};

#endif // CNETWORKCONTROLLER_H
