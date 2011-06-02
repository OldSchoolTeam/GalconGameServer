#ifndef COPTION_H
#define COPTION_H

#include <QMap>
#include <QRegExp>
#include <QStringList>
#include <QString>
#include <QDebug>

class COptions
{
public:
    COptions()
    {
        m_timeToStart = 10;
        m_maxNumPlayer = 8;
        m_timeOut = 1;
    }

    int GetTimeToStart()
    {
        return m_timeToStart;
    }

    int GetMaxNumPlayer()
    {
        return m_maxNumPlayer;
    }

    int GetTimeOut()
    {
        return m_timeOut;
    }

    void SetTimeToStart(int i_timeToStart)
    {
        m_timeToStart = i_timeToStart;
    }

    void SetMaxNumPlayer(int i_maxNumPlayer)
    {
        m_maxNumPlayer = i_maxNumPlayer;
    }

    void SetTimeOut(int i_timeOut)
    {
        m_timeOut = i_timeOut;
    }

    static COptions Parse(QStringList i_args)
    {
        COptions result;

        // remove .exe name
        i_args.pop_front();

        int expectedLength = 0; // length of string matching some pattern

        QString argsStr = i_args.join(" ");

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

        // if total length of args not equal to expected => error
        if (expectedLength != argsStr.length())
        {
            qDebug() << "Printing help";
            PrintHelp();
        }

        return result;
    }

private:
    static void PrintHelp()
    {
        qDebug() << "Problem with cmd arguments";
        qDebug() << "TimeOut=<value>";
        qDebug() << "MaxNumPlayer=<value>";
        qDebug() << "TimeToStart=<value>";
    }

    int m_timeToStart;
    int m_maxNumPlayer;
    int m_timeOut;
};

#endif // COPTION_H
