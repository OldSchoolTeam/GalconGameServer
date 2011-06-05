#ifndef FINISHMSG_H
#define FINISHMSG_H

#include "Message.h"

class CFinishMsg : public CMessage
{
public:
    CFinishMsg(int i_winnerId);
    QString ToString();
private:
    int m_winnerId;

};

#endif // FINISHMSG_H
