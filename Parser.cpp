#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include "Message.h"
#include "ConnMsg.h"
#include "StepMsg.h"
#include "Parser.h"
#include "ParsingException.h"

// high nesting level,
// because we want to know
// in what place checking for
// pattern matching fails
void CParser::ParseMessage(QString i_str) throw(CParsingException*)
{        
    QRegExp messagePattern("(CS_STEP|CS_CONN)#(.*)##");
    if (messagePattern.exactMatch(i_str))
    {
        QString msgType = messagePattern.cap(1);
        QString msgParams = messagePattern.cap(2);        
        QRegExp msgParamsPattern;
        if (msgType == "CS_STEP")
        {
            QString numberPatternStr("[1-9][0-9]?");
            QString firstArgPatternStr(".+");

            // CS_STEP has parameters: list of planets(not checked yet),
            // number(from 1 to 99) ш number(from 1 to 80)
            msgParamsPattern.setPattern("("+firstArgPatternStr+")"+"#"+
                                        "("+numberPatternStr+")"+"#"+
                                        "("+numberPatternStr+")");
            if (msgParamsPattern.exactMatch(msgParams))
            {                
                QString srcPlanetListStr = msgParamsPattern.cap(1);

                // list of planets - sequence of numbers,
                // every number enclosed in brackets
                QRegExp planetListPattern("([\(]"+numberPatternStr+"[\)])+");
                QList<int> srcPlanetsId;
                if (planetListPattern.exactMatch(srcPlanetListStr))
                {                                        
                    srcPlanetListStr.remove(0, 1);
                    srcPlanetListStr.remove(srcPlanetListStr.length()-1, 1);

                    QStringList planets = srcPlanetListStr.split(")(");
                    foreach (QString srcPlanet, planets)
                    {
                        int srcPlanetId = srcPlanet.toInt();
                        if (srcPlanetId > 80)
                        {
                            throw new CParsingException("CS_STEP: Source planet id greater than 80");
                        }
                        srcPlanetsId.append(srcPlanetId);
                    }
                }
                else
                {
                    throw new CParsingException("CS_STEP: incorrect source planet list");
                }                
                QString percentStr = msgParamsPattern.cap(2);
                int percent = percentStr.toInt();
                if (percent > 100)
                {                   
                    throw new CParsingException("CS_STEP: Fleet percent greater than 100");
                }

                QString dstPlanetStr = msgParamsPattern.cap(3);
                int dstPlanet = dstPlanetStr.toInt();
                if (dstPlanet > 80)
                {                    
                    throw new CParsingException("CS_STEP: Destination planet id greater than 80");
                }

                emit signalStep(new CStepMsg(srcPlanetsId, percent, dstPlanet));
            }
            else
            {
                // maybe too many arguments
                throw new CParsingException("CS_STEP: incorrect format");
            }
        }
        else
        if (msgType == "CS_CONN")
        {
            QString stringPatternStr("[A-Za-zР-пр-џ0-9 +-*<>=!;%?.,()/\\x005b\\x005d]{1,256}");
            msgParamsPattern.setPattern(stringPatternStr);
            if (msgParamsPattern.exactMatch(msgParams))
            {                               
                emit signalMessageConn(new CConnMsg(msgParams));
            }
            else
            {                
                throw new CParsingException("CS_CONN: invalid nickname");
            }
        }
        else
        {            
            // if we are here, then something wrong
            throw new CParsingException("No such message");
        }

    }
    else
    {
        throw new CParsingException("Incorrect message format");
    }
}

