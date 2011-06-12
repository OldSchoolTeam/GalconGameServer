#ifndef TIMETOSTARTMSG_H
#define TIMETOSTARTMSG_H

#include "Message.h"

class CTimeToStart : public CMessage
{
public:
    CTimeToStart(int i_timeToStart);
    QString ToString();

private:
    int m_timeToStart;
};

#endif // TIMETOSTARTMSG_H
