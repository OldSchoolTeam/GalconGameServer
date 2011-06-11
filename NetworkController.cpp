#include <QDebug>
#include <QDataStream>

#include "NetworkController.h"
#include "ParsingException.h"
//#include "Game.h"
//#include "StartMsg.h"
//#include "StateMsg.h"

CNetworkController::CNetworkController(int i_timeToStart, int i_maxNumPlayer, int i_timeOut, QObject *parent ) : QObject(parent)
{
    m_timeToStart = i_timeToStart;
    m_timeOut = i_timeOut;
    gameIsStarted = false;
    for (int i=1; i<=i_maxNumPlayer; ++i)
    {
        m_freeIdList.append(i);
    }

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this , SLOT(SlotSendTimeToStart()));
    m_timer->start(1000);

    m_parser = new CParser();
    connect(m_parser, SIGNAL(signalMessageConn(CConnMsg* )), this, SLOT(SlotSendConnId(CConnMsg* )));
    connect(m_parser, SIGNAL(signalStep(CStepMsg*)), this, SLOT(SlotStep(CStepMsg*)));
}

//!
void CNetworkController::SlotReadMsg()
{
    // data sender
    CSocket *socket = (CSocket*)sender();

    quint16 m_nNextBlockSize = 0;
    QString strMessage;
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_7);
    for (;;) {
       if (!m_nNextBlockSize) {
          if (socket->bytesAvailable() < sizeof(quint16)) {
             break;
          }
          in >> m_nNextBlockSize;
       }

       if (socket->bytesAvailable() < m_nNextBlockSize) {
          break;
       }

       QByteArray str;
       in >> str;

       strMessage = str;
       qDebug() << "NetworkController::read data from the socket: " << strMessage.toUtf8();

       m_nNextBlockSize = 0;
    }
    try
    {
        m_parser->ParseMessage(socket->GetId(), strMessage);
    }
    catch(CParsingException *exc)
    {
        qDebug() << "CParsingException::" << exc->GetDescription();
    }
}

bool CNetworkController::AddConnection(int i_socketDescriptor)
{
    if ( !gameIsStarted && !m_freeIdList.isEmpty() )
    {
        int id = m_freeIdList.first();
        m_freeIdList.pop_front();
        CSocket *tmp_socket = new CSocket(/*i_socketDescriptor,*/id);
        tmp_socket->setSocketDescriptor(i_socketDescriptor);
        m_socketList.append(tmp_socket);

        connect(tmp_socket/*->GetSocket()*/, SIGNAL(readyRead()), this, SLOT(SlotReadMsg()));//, Qt::DirectConnection);
        connect(tmp_socket/*->GetSocket()*/, SIGNAL(disconnected()), this, SLOT(SlotDeleteConnection())) ;//, Qt::DirectConnection);
        //connect(tmp_socket/*->GetSocket()*/, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SlotDeleteConnection()), Qt::DirectConnection);
        qDebug() << "NetworkController: added new socket";
        timeToStart = m_timeToStart;
        return true;
    }
    else
    {
        return false;
    }
}

void CNetworkController::SlotDeleteConnection()
{
    qDebug() << "CNetworkController::SlotDeleteConnection";
    QAbstractSocket *sckt = (QAbstractSocket*)sender();
    for ( QList<CSocket*>::iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        if ( (*i)/*->GetSocket()*/ == sckt )
        {
            qDebug() << "( (*i)->GetSocket() == sckt )";
            m_freeIdList.append( (*i)->GetId() );
            for (QList<CPlayer*>::iterator ipl = m_playerList.begin(); ipl != m_playerList.end(); ++ipl)
            {
                if ( (*ipl)->GetId() == (*i)->GetId() )
                {
                    qDebug() << "start/  m_playerList.erase(ipl);";
                    //m_playerList.erase(ipl);
                    //qDebug() << "end/    m_playerList.erase(ipl);";
                }
            }
            qDebug() << "m_socketList.erase(i)";
            //m_socketList.erase(i);
            //qDebug() << "NetworkController: removed the socket";
        }
    }
    if ( m_socketList.isEmpty() )
    {
        qDebug() << "CNetworkController::size of m_socketList = " <<  m_socketList.size() << "\n\temit sentQuit()";
        emit sentQuit();
    }
}

void CNetworkController::SlotSendConnId(CConnMsg* i_connMsg)
{
    qDebug() << "CNetworkController::SlotConnIdSend \n\t\t user " << i_connMsg->GetName().toUtf8() << " go to start game";
    for ( QList<CSocket*>::iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        if ( (*i)->GetId() == i_connMsg->GetSenderId() )
        {
            (*i)->SendMsg(i_connMsg->ToConnIdString());

            CPlayer *pl = new CPlayer(i_connMsg->GetSenderId(), i_connMsg->GetName() );
            m_playerList.append(pl);

            timeToStart = m_timeToStart;
        }
    }
}

void CNetworkController::sentToAll(QString i_str)
{
    for ( QList<CSocket *>::Iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        (*i)->SendMsg(i_str);
    }
}

void CNetworkController::SlotSendTimeToStart()
{
    if ( m_playerList.size() != 0 )
    {
        if ( m_playerList.size() == 1 )
        {
            sentToAll("SC_TIMETOSTART#0##");
        }
        else
        {
            if ( timeToStart != 0 )
            {
                QString timeToStartStr = QString("SC_TIMETOSTART#%1##").arg(timeToStart);
                --timeToStart;
                sentToAll(timeToStartStr);
                if ( timeToStart == 0 )
                {
                    gameIsStarted = true;
                    m_gameGalcon = new CGame(m_playerList);
                    connect(m_gameGalcon, SIGNAL(SignalStart(CStartMsg*)), this, SLOT(SlotSendStart(CStartMsg*)),Qt::DirectConnection);
                    connect(m_gameGalcon, SIGNAL(SignalFinish(CFinishMsg*)), this, SLOT(SlotSendFinish(CFinishMsg*)), Qt::DirectConnection);
                    m_gameGalcon->start();
                }
            }
            else
            {
                m_timer->stop();
                disconnect(m_timer, SIGNAL(timeout()), this , SLOT(SlotSendTimeToStart()));
                m_timer->start(m_timeOut);
                connect(m_timer, SIGNAL(timeout()), this , SLOT(SlotSendState()));
            }
        }
    }
}

void CNetworkController::SlotSendErr()
{
    //sentToAll(stateStr.ToString());
}

void CNetworkController::SlotSendFinish(CFinishMsg *i_msg)
{
    sentToAll(i_msg->ToString());
    disconnect(m_timer, SIGNAL(timeout()), this , SLOT(SlotSendState()));
    sleep(2);
    emit sentQuit();
}

void CNetworkController::SlotSendStart(CStartMsg *i_msg)
{
    qDebug() << "CNetworkController::SlotSendStart(CStartMsg *i_msg)";
    sentToAll(i_msg->ToString());
}

void CNetworkController::SlotSendState()
{
    CStateMsg stateStr = m_gameGalcon->GetState();
    qDebug() << "CNetworkController::SlotSendState\n\t\t" << stateStr.ToString();
    sentToAll(stateStr.ToString());
}

void CNetworkController::SlotStep(CStepMsg *i_msg)
{
    qDebug() << "CNetworkController::SlotStep start";
    CStateMsg state = m_gameGalcon->AddStep(i_msg);
    qDebug() << "CNetworkController::SlotStep sent to all";
    sentToAll(state.ToString());
}
