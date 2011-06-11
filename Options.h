#ifndef COPTION_H
#define COPTION_H

#include <QStringList>

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

    void PrintOptions();

    static COptions Parse(QStringList i_args);

private:
    static void PrintHelp();

    int m_timeToStart;
    int m_maxNumPlayer;
    int m_timeOut;      // time in milliseconds
    int m_port;
};

#endif // COPTION_H
