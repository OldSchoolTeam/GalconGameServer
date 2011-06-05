#ifndef ERRMSG_H
#define ERRMSG_H

#include "Message.h"

class CErrMsg : public CMessage
{
public:
    CErrMsg(QString i_msg);
    QString ToString();
private:
    QString m_msgString;
};

#endif // ERRMSG_H
