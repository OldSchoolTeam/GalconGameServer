#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include "Message.h"
#include "ConnMsg.h"
#include "StepMsg.h"
#include "ParsingException.h"

// class for parsing string messages
// in format specified in protocol
// it returns appropriate message object
class CParser : public QObject
{
    Q_OBJECT

public:
    void ParseMessage(int i_senderId, QString i_str) throw(CParsingException*);

signals:
    void signalMessageConn(CConnMsg* i_msg);
    void signalStep(CStepMsg* i_msg);
};

#endif // PARSER_H
