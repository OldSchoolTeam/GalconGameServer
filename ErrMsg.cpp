#include "ErrMsg.h"

CErrMsg::CErrMsg(QString i_msg)
{
    m_msgString = i_msg;
}

QString CErrMsg::ToString()
{
    return QString("SC_ERR#"+m_msgString+"##");
}
