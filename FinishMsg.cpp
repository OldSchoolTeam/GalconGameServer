#include "FinishMsg.h"

CFinishMsg::CFinishMsg(int i_winnerId)
{
    m_winnerId = i_winnerId;
}

QString CFinishMsg::ToString()
{
    return QString("SC_FINISH#"+QString::number(m_winnerId)+"##");
}
