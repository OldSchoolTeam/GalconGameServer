#include "ConnIdMsg.h"

CConnIdMsg::CConnIdMsg(int i_id)
{
    m_id = i_id;
}

QString CConnIdMsg::ToString()
{
    return QString("CS_CONNID#"+QString::number(m_id)+"##");
}
