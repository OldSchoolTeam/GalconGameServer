#ifndef CONNMSG_H
#define CONNMSG_H

#include <QString>
#include "Message.h"

// Message class
// contains information
// about username
// CS_CONN in protocol
class CConnMsg : public CMessage
{
public:
    CConnMsg(QString i_msg);
    QString GetName();

private:
    QString m_name;

};

#endif // CONNMSG_H
