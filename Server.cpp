#include <QtCore>
#include <QThread>

#include <QString>
#include <QMap>
#include <QStringList>

#include "Server.h"

CServer::CServer(COptions i_opt, QObject *parent) : QTcpServer(parent)
{
    m_options = &i_opt;
    m_port = 5678;
    m_currentSession = 0;
    //m_currentSession = new CSession(m_options->GetTimeToStart(), m_options->GetMaxNumPlayer(), m_options->GetTimeOut(), this);
    //connect(m_currentSession, SIGNAL(finished()), this, SLOT(SlotRemoveSession()), Qt::DirectConnection);
    //m_sessionList.append(m_currentSession);
    //m_currentSession->start();
}

bool CServer::Start()
{
    if(!listen(QHostAddress::Any, m_port) )
    {
        qDebug() << "Server: " << errorString() << "\n";
        close();
        // throw CServerException(errorString());
    }
    qDebug() << "Server: start\n";
    return true;
}

void CServer::SlotRemoveSession()
{
    CSession *deletingSession = qobject_cast<CSession*>(sender());
    m_sessionList.removeAt(m_sessionList.indexOf(deletingSession));
}

void CServer::incomingConnection(int i_socketDescriptor)
{
    if ( ( m_currentSession == 0 ) || ( !m_currentSession->AddClient(i_socketDescriptor) ) )
    {
        m_currentSession = new CSession(m_options->GetTimeToStart(), m_options->GetMaxNumPlayer(), m_options->GetTimeOut(), this);
        connect(m_currentSession, SIGNAL(finished()), this, SLOT(SlotRemoveSession()), Qt::DirectConnection);
        m_sessionList.append(m_currentSession);
        m_currentSession->start();
        m_currentSession->AddClient(i_socketDescriptor);
    }
}
