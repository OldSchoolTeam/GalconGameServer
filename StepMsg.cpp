#include "StepMsg.h"

CStepMsg::CStepMsg(QList<int> i_srcPlanetList,
                   int i_percent,
                   int i_dstPlanet)
{
    m_srcPlanetList = i_srcPlanetList;
    m_percent = i_percent;
    m_dstPlanetId = i_dstPlanet;
}


int CStepMsg::GetDstPlanetId()
{
    return m_dstPlanetId;
}

int CStepMsg::GetPercent()
{
    return m_percent;
}

QList<int> CStepMsg::GetSrcPlanetsList()
{
    return m_srcPlanetList;
}
