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
    CConnMsg(int i_senderId, QString i_msg);
    QString GetName();
    int GetSenderId();
    QString ToConnIdString();

private:
    int m_senderId;     // id гравця, від якого прийшло повідомлення
    QString m_name;

};

#endif // CONNMSG_H
