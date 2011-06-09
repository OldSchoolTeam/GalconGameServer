#include "FinishMsg.h"

CFinishMsg::CFinishMsg(int i_winnerId)
{
    m_winnerId = i_winnerId;
}

QString CFinishMsg::ToString()
{
    QString msg = QString("SC_FINISH#%1##").arg(m_winnerId);
    return msg;
}
