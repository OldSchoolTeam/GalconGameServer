#include "ConnMsg.h"

CConnMsg::CConnMsg(QString i_msg)
{
    m_name = i_msg;
}

QString CConnMsg::GetName()
{
    return m_name;
}
