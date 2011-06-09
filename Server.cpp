#include <QtCore>
#include <QThread>
#include <QDebug>
#include <QString>
#include <QMap>
#include <QStringList>

#include "Server.h"


CServer::CServer(COptions *i_opt, QObject *parent) : QTcpServer(parent)
{
    m_options = i_opt;
    m_port = i_opt->GetPort();
    m_currentSession = 0;
}

bool CServer::Start()
{
    if(!listen(QHostAddress::Any, m_port) )
    {
        qDebug() << "Server: " << errorString();
        close();
        // throw CServerException(errorString());
    }
    qDebug() << "Server: start";
    return true;
}

void CServer::incomingConnection(int i_socketDescriptor)
{
    qDebug() << "CServer::incomingConnection";
    if ( ( m_currentSession == 0 ) || ( !m_currentSession->AddClient(i_socketDescriptor) ) )
    {
        qDebug() << "CServer::create new CSession";
        //m_currentSession = 0;
        m_currentSession = new CSession(m_options->GetTimeToStart(), m_options->GetMaxNumPlayer(), m_options->GetTimeOut(), this);
        connect(m_currentSession, SIGNAL(finished()), this, SLOT(SlotRemoveSession()), Qt::DirectConnection);
        m_sessionList.append(m_currentSession);
        m_currentSession->start();
        m_currentSession->AddClient(i_socketDescriptor);
    }
}

void CServer::SlotRemoveSession()
{
    qDebug() << "CServer::SlotRemoveSession()";
    CSession *deletingSession = qobject_cast<CSession*>(sender());
    m_sessionList.removeAt(m_sessionList.indexOf(deletingSession));
}
