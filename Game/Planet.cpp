#include "Planet.h"

CPlanet::CPlanet()
{

}

CPlanet::CPlanet(int i_planetId,
                 int i_playerId,
                 int i_fleetSize,
                 int i_planetRadius,                                
                 int i_x,
                 int i_y)
{
    m_planetId = i_planetId;
    m_playerId = i_playerId;
    m_fleetSize = i_fleetSize;
    m_startFleetSize = i_fleetSize;
    m_planetRadius = i_planetRadius;    
    m_x = i_x;
    m_y = i_y;
}

int CPlanet::GetPlanetId()
{
    return m_planetId;
}

int CPlanet::GetPlanetRadius()
{
    return m_planetRadius;
}

int CPlanet::GetPlayerId()
{
    return m_playerId;
}

int CPlanet::GetFleetSize()
{
    return m_fleetSize;
}

int CPlanet::GetX()
{
    return m_x;
}

int CPlanet::GetY()
{
    return m_y;
}

QTime CPlanet::GetStartTime()
{
    return m_startTime;
}

int CPlanet::GetStartFleetSize()
{
    return m_startFleetSize;
}

void CPlanet::SetStartFleetSize(int i_startFleetSize)
{
    m_startFleetSize = i_startFleetSize;
}

void CPlanet::SetStartTime(QTime i_time)
{
    m_startTime = i_time;
}

void CPlanet::SetPlayerId(int i_playerId)
{
    m_playerId = i_playerId;
}

void CPlanet::SetFleetSize(int i_fleetSize)
{
    m_fleetSize = i_fleetSize;
}
