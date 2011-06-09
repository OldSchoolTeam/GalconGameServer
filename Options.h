#ifndef COPTION_H
#define COPTION_H

#include <QMap>
#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QDebug>

// Class for parsing
// cmd parameters
class COptions
{
public:
    COptions();

    int GetTimeToStart();
    int GetMaxNumPlayer();
    int GetTimeOut();
    int GetPort();

    void SetTimeToStart(int i_timeToStart);
    void SetMaxNumPlayer(int i_maxNumPlayer);
    void SetTimeOut(int i_timeOut);
    void SetPort(int i_port);

    static COptions Parse(QStringList i_args);

private:
    static void PrintHelp();

    int m_timeToStart;
    int m_maxNumPlayer;
    int m_timeOut;
    int m_port;
};

#endif // COPTION_H
