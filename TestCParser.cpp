#include "TestCParser.h"
#include "Parser.h"
#include "Message.h"
#include "ConnMsg.h"
#include "StepMsg.h"
#include "ParsingException.h"

CTestCParser::CTestCParser(QString i_connMsg,
                           QString i_name,
                           int i_senderId,
                           QString i_stepMsg,
                           QList<int> i_srcPlanetId,
                           int i_percent,
                           int i_dstPlanetId)
{
    m_connMsg = i_connMsg;
    m_stepMsg = i_stepMsg;
    m_senderId = i_senderId;
    m_srcPlanetId = i_srcPlanetId;
    m_percent = i_percent;
    m_dstPlanetId = i_dstPlanetId;
    m_name = i_name;
}

void CTestCParser::initTestCase()
{
    m_parser = new CParser();

    QObject::connect(m_parser, SIGNAL(signalMessageConn(CConnMsg*)), this, SLOT(slotConn(CConnMsg*)));
    QObject::connect(m_parser, SIGNAL(signalStep(CStepMsg*)), this, SLOT(slotStep(CStepMsg*)));

}

void CTestCParser::slotConn(CConnMsg *i_msg)
{
    QVERIFY(i_msg != 0);
    QVERIFY(i_msg->GetName() == m_name);
}

void CTestCParser::slotStep(CStepMsg *i_msg)
{
    QVERIFY(i_msg != 0);
    QList<int> ids = i_msg->GetSrcPlanetsList();
    QVERIFY(ids.length() == m_srcPlanetId.length());
    foreach(int id, ids)
    {
        QVERIFY(m_srcPlanetId.contains(id));
    }
    QVERIFY(i_msg->GetPercent() == m_percent);
    QVERIFY(i_msg->GetDstPlanetId() == m_dstPlanetId);
}

void CTestCParser::ConnTest()
{        
    try {
        m_parser->ParseMessage(m_senderId, m_connMsg);
    }
    catch (CParsingException* e)
    {
        QFAIL(e->GetDescription().toAscii().data());
    }
}


void CTestCParser::StepTest()
{           
    try {
        m_parser->ParseMessage(m_senderId, m_stepMsg);
    } catch (CParsingException* e)
    {
        QFAIL(e->GetDescription().toAscii().data());
    }
}

void CTestCParser::cleanupTestCase()
{
    delete m_parser;
}
