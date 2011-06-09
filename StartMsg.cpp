#include "StartMsg.h"

CStartMsg::CStartMsg(int i_flySpeed,
                     int i_growFactor,
                     QList<CPlanet> i_planets,
                     QList<CPlayer*> i_players,
                     int i_sizeX,
                     int i_sizeY)
{
    m_flySpeed = i_flySpeed;
    m_growFactor = i_growFactor;
    m_planets = i_planets;
    m_players = i_players;
    m_sizeX = i_sizeX;
    m_sizeY = i_sizeY;
}


QString CStartMsg::ToString()
{
    QString result = QString("SC_START#%1#%2#%3#%4#")\
                     .arg(m_sizeX)\
                     .arg(m_sizeY)\
                     .arg(m_growFactor)\
                     .arg(m_flySpeed);
    foreach (CPlanet planet, m_planets)
    {
        QString planetDesc = QString("(%1,%2,%3,%4,%5,%6)")\
                             .arg(planet.GetPlanetId())\
                             .arg(planet.GetPlayerId())\
                             .arg(planet.GetX())\
                             .arg(planet.GetY())\
                             .arg(planet.GetPlanetRadius())\
                             .arg(planet.GetStartFleetSize());  //planet.GetStartingNumFleet()
        result += planetDesc;
    }
    result += "#";

    foreach (CPlayer* player, m_players)
    {
        QString playerDesc = QString("(%1,%2)")\
                             .arg(player->GetId())\
                             .arg(player->GetName());
        result += playerDesc;
    }
    result += "##";

    return result;
}
