#include "StateMsg.h"

CStateMsg::CStateMsg(QList<CFleet> i_fleets, QList<CPlanet> i_planets)
{
    m_fleetList = i_fleets;
    m_planetList = i_planets;
}

QString CStateMsg::ToString()
{
    QString result = "SC_STEP#";
    QString delim = ",";
    foreach (CPlanet planet, m_planetList)
    {
        QString planetDesc = QString("(")+planet.GetPlanetId()+","+
                             planet.GetPlayerId()+","+
                             planet.GetStartingNumFleet()+")";
        result += planetDesc;
    }
    result += "#";

    foreach (CFleet fleet, m_fleetList)
    {
        QString fleetDesc = QString("(")+fleet.GetFleetId()+","+
                            fleet.GetPlayerId()+","+
                            fleet.GetSrcPlanetId()+","+
                            fleet.GetDstPlanetId()+","+
                            fleet.GetFleetSize()+","+
                            fleet.GetPercent()+QString(")");

        result += fleetDesc;
    }
    result += "##";

    return result;
}
