#ifndef STEPMSG_H
#define STEPMSG_H

#include <QList>
#include "Message.h"

// Message class
// contains information
// about user step
// CS_STEP in protocol
class CStepMsg : public CMessage
{
public:
    CStepMsg(QList<int> i_srcPlanetList, int i_percent, int i_dstPlanet, int i_sender);
    int GetDstPlanetId();
    int GetPercent();
    int GetSenderId();
    QList<int> GetSrcPlanetsList();

private:
    int m_sender;
    int m_dstPlanetId;
    int m_percent;
    QList<int> m_srcPlanetList;
};

#endif // STEPMSG_H
