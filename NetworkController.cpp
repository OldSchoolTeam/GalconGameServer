#include <QDebug>

#include "NetworkController.h"
#include <QDebug>

CNetworkController::CNetworkController(int i_timeToStart, int i_maxNumPlayer, int i_timeOut, QObject *parent ) : QObject(parent)
{
    m_timeToStart = i_timeToStart;
    m_timeOut = i_timeOut;
    for (int i=1; i<i_maxNumPlayer; ++i)
    {
        m_freeIdList.append(i);
    }
}

void CNetworkController::SlotReadMsg()
{
    QTcpSocket *sckt = (QTcpSocket*)sender();
    for ( QList<CSocket*>::iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        if ( (*i)->GetSocket() == sckt )
        {
            //
//            m_freeIdList.append((*i)->GetId() );
//            int j = m_socketList.indexOf((*i));
//            m_socketList.removeAt(j);
//            qDebug() << "NetworkController: removed the socket\n";
        }
    }
    qDebug() << "NetworkController: read data from the socket\n";
}

bool CNetworkController::AddConnection(int i_socketDescriptor)
{
    if ( !m_freeIdList.isEmpty() )
    {
        int id = m_freeIdList.first();
        m_freeIdList.pop_front();
        CSocket *tmp_socket = new CSocket(i_socketDescriptor,id);
        m_socketList.append(tmp_socket);

        connect(tmp_socket->GetSocket(), SIGNAL(readyRead()), this, SLOT(SlotReadMsg()), Qt::DirectConnection);
        connect(tmp_socket->GetSocket(), SIGNAL(disconnected()), this, SLOT(SlotDeleteConnection()), Qt::DirectConnection);
        //connect(tmp_socket->GetSocket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotDeleteConnection()), Qt::DirectConnection);
        qDebug() << "NetworkController: added new socket\n";
        return true;
    }
    else
    {
        return false;
    }
}

void CNetworkController::SlotDeleteConnection()
{
    QTcpSocket *sckt = (QTcpSocket*)sender();
    for ( QList<CSocket*>::iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        if ( (*i)->GetSocket() == sckt )
        {
            m_freeIdList.append((*i)->GetId() );
            m_socketList.removeAt(m_socketList.indexOf((*i)));
            qDebug() << "NetworkController: removed the socket\n";
        }
    }
    if ( m_socketList.isEmpty() )
    {
        emit sentQuit();
    }
}

void CNetworkController::SlotSendConnId(int)
{
    //
}

void CNetworkController::SlotSendErr()
{
    //
}

void CNetworkController::SlotSendFinish()
{
    //
}

void CNetworkController::SlotSendStart()
{
    //
}

void CNetworkController::SlotSendState()
{
    //
}

void CNetworkController::SlotSendTimeToStart()
{
    //
}

void CNetworkController::SlotStep()
{
    //
}
