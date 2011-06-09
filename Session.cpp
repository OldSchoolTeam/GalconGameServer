#include <QDebug>

#include "Session.h"

CSession::CSession(int i_timeToStart, int i_maxNumPlayer, int i_timeOut, QObject *parent) : QThread(parent)
{
    qDebug() << "Session: thread start";
    controller = new CNetworkController(i_timeToStart, i_maxNumPlayer, i_timeOut);
    connect(controller, SIGNAL(sentQuit()), this, SLOT(slotFinish()));
}

bool CSession::AddClient(int i_socketDescriptor)
{
    return (controller->AddConnection(i_socketDescriptor));
}

void CSession::run()
{
    qDebug() << "CSession::run()";
    exec();
}

void CSession::slotFinish()
{
    qDebug() << "Session: stop thread";
    quit();
}
