#ifndef PARSINGEXCEPTION_H
#define PARSINGEXCEPTION_H

#include <QString>
#include "Exception.h"

// exception class
// which can be thrown
// when somebody can't parse something
class CParsingException : public CException
{
public:
    CParsingException(QString i_errorDesc);
};

#endif // PARSINGEXCEPTION_H
