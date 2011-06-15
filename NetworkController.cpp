#include <QDebug>
#include <QDataStream>

#include "NetworkController.h"
#include "ParsingException.h"

CNetworkController::CNetworkController(int i_timeToStart,
                                       int i_maxNumPlayer,
                                       int i_timeOut,
                                       QObject *parent ) : QObject(parent)
{
    m_timeToStart = i_timeToStart;
    m_timeOut = i_timeOut;

    gameIsStarted = false;

    for (int i=1; i<=i_maxNumPlayer; ++i)
    {
        m_freeIdList << i;
    }

    m_timer = new QTimer();
    m_stepTimer = new QTimer();
    connect(m_timer, SIGNAL(timeout()),
            this , SLOT(SlotSendTimeToStart()));
    m_timer->start(1000);

    m_parser = new CParser();
    connect(m_parser, SIGNAL(signalMessageConn(CConnMsg* )),
            this, SLOT(SlotSendConnId(CConnMsg* )),
            Qt::QueuedConnection);
    connect(m_parser, SIGNAL(signalStep(CStepMsg*)),
            this, SLOT(SlotStep(CStepMsg*)),
            Qt::QueuedConnection);

    qDebug() << "NetworkController: is started";
}

CNetworkController::~CNetworkController()
{
    delete m_gameGalcon;
    delete m_parser;
    delete m_timer;
    delete m_stepTimer;
    qDebug() << "NetworkController: is deleted";
}

bool CNetworkController::AddConnection(int i_socketDescriptor)
{
    qDebug() << "NetworkController: add new connection";
    if ( !gameIsStarted && !m_freeIdList.isEmpty() )
    {
        CSocket *tmp_socket = new CSocket(m_freeIdList.takeFirst());
        tmp_socket->setSocketDescriptor(i_socketDescriptor);
        m_socketList.append(tmp_socket);

        connect(tmp_socket, SIGNAL(readyRead()),
                this, SLOT(SlotReadMsg()));
        connect(tmp_socket, SIGNAL(disconnected()),
                this, SLOT(SlotDeleteConnection()));

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
    CSocket *delSocket = (CSocket *)sender();

    m_freeIdList.append( delSocket->GetId() );

    // delete connected client from list
    for (QList<CPlayer*>::iterator ipl = m_playerList.begin(); ipl != m_playerList.end(); /*++ipl*/)
    {
        if ( (*ipl)->GetId() == delSocket->GetId() )
        {
            ipl = m_playerList.erase(ipl);
            delete (*ipl);
            // get player Id to free Id

        }
        else
        {
            ++ipl;
        }
    }

    // delete socket from list
    m_socketList.removeAt(m_socketList.indexOf(delSocket));
    delSocket->deleteLater();

    qDebug() << "NetworkController: delete connection";

    if ( m_playerList.isEmpty() )
    {
        for (QList<CSocket *>::iterator isck = m_socketList.begin(); isck != m_socketList.end(); ++isck)
        {
            (*isck)->disconnectFromHost();
            m_socketList.removeAt(m_socketList.indexOf((*isck)));
            (*isck)->deleteLater();
        }
        emit sentQuit();
    }
}

void CNetworkController::SlotReadMsg()
{
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

       m_nNextBlockSize = 0;
    }

    qDebug() << "NetworkController: read text from client " << strMessage.toUtf8();

    try
    {
        m_parser->ParseMessage(socket->GetId(), strMessage);
    }
    catch(CParsingException *exc)
    {
        qDebug() << "NetworkController::parsingException: " << exc->GetDescription();
    }
}

void CNetworkController::SlotSendConnId(CConnMsg* i_connMsg)
{
    for ( QList<CSocket*>::iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        if ( (*i)->GetId() == i_connMsg->GetSenderId() )
        {
            (*i)->SendMsg(i_connMsg->ToConnIdString());
            qDebug() << "NetworkController: send to player: " << i_connMsg->ToConnIdString();

            CPlayer *pl = new CPlayer(i_connMsg->GetSenderId(), i_connMsg->GetName() );
            m_playerList.append(pl);

            timeToStart = m_timeToStart;
        }
    }
    delete i_connMsg;
}

void CNetworkController::sentToAll(QString i_str)
{
    for ( QList<CSocket *>::Iterator i = m_socketList.begin(); i != m_socketList.end(); ++i)
    {
        qDebug() << "send" << (*i)->GetId();
        (*i)->SendMsg(i_str);
    }
    qDebug() << "NetworkController: send to all players: " << i_str;
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
                    connect(m_gameGalcon, SIGNAL(SignalStart(CStartMsg*)),
                            this, SLOT(SlotSendStart(CStartMsg*)),
                            Qt::QueuedConnection);
                    connect(m_gameGalcon, SIGNAL(SignalFinish(CFinishMsg*)),
                            this, SLOT(SlotSendFinish(CFinishMsg*)),
                            Qt::QueuedConnection);

                    m_gameGalcon->start();
                }
            }
            else
            {
                //m_timer->stop();
                disconnect(m_timer, SIGNAL(timeout()),
                           this , SLOT(SlotSendTimeToStart()));
            }
        }
    }
}

void CNetworkController::SlotSendStart(CStartMsg *i_msg)
{
    qDebug() << "CNetworkController: start game";

    //m_timer->start(m_timeOut);
    m_stepTimer->start(m_timeOut);
    connect(m_stepTimer, SIGNAL(timeout()),
            this , SLOT(SlotSendState()), Qt::QueuedConnection);

    sentToAll(i_msg->ToString());
    delete i_msg;
}

void CNetworkController::SlotSendFinish(CFinishMsg *i_msg)
{
    qDebug() << "CNetworkController: finish game";

    sentToAll(i_msg->ToString());
    delete i_msg;

    disconnect(m_stepTimer, SIGNAL(timeout()), this , SLOT(SlotSendState()));

    for (QList<CSocket *>::iterator isck = m_socketList.begin(); isck != m_socketList.end(); ++isck)
    {
        (*isck)->disconnectFromHost();
        m_socketList.removeAt(m_socketList.indexOf((*isck)));
        (*isck)->deleteLater();
    }

    m_gameGalcon->deleteLater();

    emit sentQuit();
}

void CNetworkController::SlotSendState()
{
    CStateMsg state = m_gameGalcon->GetState();
    sentToAll(state.ToString());
    qDebug() << "NetworkController: send game state to all players: " << state.ToString();
}

void CNetworkController::SlotStep(CStepMsg *i_msg)
{    
    CStateMsg state = m_gameGalcon->AddStep(i_msg);
    sentToAll(state.ToString());
    qDebug() << "NetworkController: add step to game and send state to all players: " << state.ToString();
}
