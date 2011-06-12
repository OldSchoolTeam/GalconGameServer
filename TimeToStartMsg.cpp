#include "TimeToStartMsg.h"

CTimeToStart::CTimeToStart(int i_timeToStart)
{
    m_timeToStart = i_timeToStart;
}


QString CTimeToStart::ToString()
{
    return QString("SC_TIMETOSTART#"+QString::number(m_timeToStart)+"##");
}
