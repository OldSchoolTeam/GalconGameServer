#include "Options.h"

COptions::COptions()
{
    m_timeToStart = 10;
    m_maxNumPlayer = 8;
    m_timeOut = 1;
}

COptions COptions::Parse(QStringList i_args)
{
    COptions result;

    // first item - application name,
    i_args.pop_front();

    // sum of the lengths of substrings, matches some pattern
    // if total length greater than expected, then something wrong
    int expectedLength = 0;

    QString argsStr = i_args.join(" ");

    // one pattern - one cmd parameter

    QRegExp timeToStartPattern("(TimeToStart[ ]*=[ ]*([1-9][0-9]*)[ ]*)");
    if (timeToStartPattern.indexIn(argsStr) != -1)
    {
        QString total = timeToStartPattern.cap(1);
        expectedLength += total.length();

        qDebug() << "total: " << total;
        QString timeToStart = timeToStartPattern.cap(2);
        qDebug() << "timeToStart: " << timeToStart;
        result.SetTimeToStart(timeToStart.toInt());
    }

    QRegExp maxPlayerPattern("(MaxNumPlayer[ ]*=[ ]*([1-9][0-9]*)[ ]*)");
    if (maxPlayerPattern.indexIn(argsStr) != -1)
    {
        QString total = maxPlayerPattern.cap(1);
        expectedLength += total.length();

        qDebug() << "total: " << total;
        QString maxPlayer = maxPlayerPattern.cap(2);
        qDebug() << "maxNumPlayer: " << maxPlayer;
        result.SetMaxNumPlayer(maxPlayer.toInt());
    }

    QRegExp timeOutPattern("(TimeOut[ ]*=[ ]*([1-9][0-9]*)[ ]*)");
    if (timeOutPattern.indexIn(argsStr) != -1)
    {
        QString total = timeOutPattern.cap(1);
        expectedLength += total.length();

        qDebug() << "total: " << total;
        QString timeOut = timeOutPattern.cap(2);
        qDebug() << "timeOut: " << timeOut;
        result.SetTimeOut(timeOut.toInt());
    }

    QRegExp portPattern("(Port[]*=[]*([1-9][0-9]*)[]*)");
    if (portPattern.indexIn(argsStr))
    {
        QString total = portPattern.cap(1);
        expectedLength += total.length();

        qDebug() << "total: " << total;
        int port = portPattern.cap(2).toInt();

        // check port number < 65...
        // ...

        qDebug() << port;
        result.SetPort(port);
    }

    // if total length of args not equal to expected => error
    if (expectedLength != argsStr.length())
    {
        qDebug() << "Printing help";
        PrintHelp();
    }

    return result;
}

void COptions::PrintHelp()
{
    qDebug() << "Problem with cmd arguments";
    qDebug() << "TimeOut=<value>";
    qDebug() << "MaxNumPlayer=<value>";
    qDebug() << "TimeToStart=<value>";
}

int COptions::GetMaxNumPlayer()
{
    return m_maxNumPlayer;
}

int COptions::GetTimeOut()
{
    return m_timeOut;
}

int COptions::GetTimeToStart()
{
    return m_timeToStart;
}

int COptions::GetPort()
{
    m_port;
}

void COptions::SetMaxNumPlayer(int i_maxNumPlayer)
{
    m_maxNumPlayer = i_maxNumPlayer;
}

void COptions::SetTimeOut(int i_timeOut)
{
    m_timeOut = i_timeOut;
}

void COptions::SetTimeToStart(int i_timeToStart)
{
    m_timeToStart = i_timeToStart;
}

void COptions::SetPort(int i_port)
{
    m_port = i_port;
}


