#ifndef CONNIDMSG_H
#define CONNIDMSG_H

#include <QString>
#include "Message.h"

class CConnIdMsg : public CMessage
{
public:
    CConnIdMsg(int i_id);
    QString ToString();
private:
    int m_id;
};

#endif // CONNIDMSG_H
