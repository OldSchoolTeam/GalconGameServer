#include "StartMsg.h"

CStartMsg::CStartMsg(int i_flySpeed,
                     int i_growFactor,
                     QList<CPlanet> i_planets,
                     QList<CPlayer> i_players,
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
    QString result = "CS_START#"+
                     QString::number(m_sizeX)+"#"+
                     QString::number(m_sizeY)+"#"+
                     QString::number(m_growFactor)+"#";
    foreach (CPlanet planet, m_planets)
    {
        QString planetDesc = QString("(")+planet.GetPlanetId()+","+
                             planet.GetPlayerId()+","+
                             planet.GetX()+","+
                             planet.GetY()+","+
                             planet.GetPlanetRadius()+","+
                             planet.GetStartingNumFleet()+QString(")");
        result += planetDesc;
    }
    result += "#";

    foreach (CPlayer player, m_players)
    {
        QString playerDesc = QString("(")+player.GetId()+","+player.GetName()+")";
        result += playerDesc;
    }
    result += "##";

    return result;
}
