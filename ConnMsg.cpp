#include <QDebug>
#include <QString>

#include "ConnMsg.h"

CConnMsg::CConnMsg(int i_senderId, QString i_msg)
{
    m_senderId = i_senderId;
    m_name = i_msg;
}

QString CConnMsg::GetName()
{
    return m_name;
}

int CConnMsg::GetSenderId()
{
    return m_senderId;
}

QString CConnMsg::ToConnIdString()
{
    QString msg = QString("SC_CONNID#%1##") .arg(m_senderId);
    return msg;
}
