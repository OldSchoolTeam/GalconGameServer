#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

class CException
{
public:
    virtual QString GetDescription();

protected:
    QString m_errorDescription;
};

#endif // EXCEPTION_H
