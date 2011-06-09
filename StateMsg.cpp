#include "StateMsg.h"

CStateMsg::CStateMsg(QList<CFleet> i_fleets, QList<CPlanet> i_planets)
{
    m_fleetList = i_fleets;
    m_planetList = i_planets;
}

QList<CFleet> CStateMsg::GetFleets()
{
    return m_fleetList;
}

QList<CPlanet> CStateMsg::GetPlanets()
{
    return m_planetList;
}

QString CStateMsg::ToString()
{
    QString result = "SC_STATE#";
    foreach (CPlanet planet, m_planetList)
    {
        QString planetDesc = QString("(%1,%2,%3)")\
                             .arg(planet.GetPlanetId())\
                             .arg(planet.GetPlayerId())\
                             .arg(planet.GetFleetSize());
        result += planetDesc;
    }
    result += "#";

    foreach (CFleet fleet, m_fleetList)
    {
        QString fleetDesc = QString("(%1,%2,%3,%4,%5,%6)")\
                            .arg(fleet.GetFleetId()) \
                            .arg(fleet.GetPlayerId()) \
                            .arg(fleet.GetSrcPlanetId()) \
                            .arg(fleet.GetDstPlanetId()) \
                            .arg(fleet.GetFleetSize()) \
                            .arg(fleet.GetPercent());
        result += fleetDesc;
    }
    result += "##";

    return result;
}
