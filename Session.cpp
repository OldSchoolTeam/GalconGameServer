#include <QDebug>

#include "Session.h"

CSession::CSession(int i_timeToStart,
                   int i_maxNumPlayer,
                   int i_timeOut,
                   QObject *parent) : QThread(parent)
{
    m_controller = new CNetworkController(i_timeToStart,
                                          i_maxNumPlayer,
                                          i_timeOut);

    connect(m_controller, SIGNAL(sentQuit()),
            this, SLOT(slotFinish()));

    qDebug() << "Session: is creating";
}

CSession::~CSession()
{
    delete m_controller;
    qDebug() << "Session: is deleted";
}

bool CSession::AddClient(int i_socketDescriptor)
{
    return (m_controller->AddConnection(i_socketDescriptor));
}

void CSession::run()
{
    qDebug() << "CSession: run";
    exec();
}

void CSession::slotFinish()
{
    qDebug() << "Session: quit";
    quit();
}
