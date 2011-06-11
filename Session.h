#ifndef CSESSION_H
#define CSESSION_H

#include <QObject>
#include <QThread>

#include "NetworkController.h"

class CSession : public QThread
{
    Q_OBJECT

public:
    CSession(int i_timeToStart,
             int i__maxNumPlayer,
             int i_timeOut,
             QObject *parent = 0);
    ~CSession();

    bool AddClient(int i_socketDescriptor);
    void run();

signals:
    void signalAddConnection(int);
    void signalStart();
    void signalTimeToStart();

private slots:
    void slotFinish();

private:
    CNetworkController *m_controller;
};

#endif // CSESSION_H
