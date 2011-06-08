#ifndef TESTCPARSER_H
#define TESTCPARSER_H

#include <QtTest/QtTest>
#include <QObject>
#include "Parser.h"

class CTestCParser : public QObject
{
    Q_OBJECT

public:
    CTestCParser(QString i_connMsg,
                 QString i_name,
                 int i_senderId,
                 QString i_stepMsg,
                 QList<int> i_srcPlanetId,
                 int i_percent,
                 int i_dstPlanetId);
private:
    // правильные сообщения
    QString m_connMsg;
    QString m_stepMsg;

    // правильные параметры
    int m_senderId;
    QString m_name;
    QList<int> m_srcPlanetId;
    int m_percent;
    int m_dstPlanetId;

    CParser* m_parser;


public slots:
    void slotConn(CConnMsg* i_msg);
    void slotStep(CStepMsg* i_msg);


private slots:
    void initTestCase();
    void ConnTest();
    void StepTest();
    void cleanupTestCase();
};

#endif // TESTCPARSER_H
