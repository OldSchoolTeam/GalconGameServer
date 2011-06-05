#include "Planet.h"

CPlanet::CPlanet(int i_planetId,
                 int i_playerId,
                 int i_fleetSize,
                 int i_planetRadius,
                 int i_startingFleetSize,
                 int i_startingTime,
                 int i_x,
                 int i_y)
{
    m_planetId = i_planetId;
    m_playerId = i_playerId;
    m_fleetSize = i_fleetSize;
    m_planetRadius = i_planetRadius;
    m_startingFleetSize = i_startingFleetSize;
    m_startingTime = i_startingTime;
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

int CPlanet::GetStartingTime()
{
    return m_startingTime;
}

int CPlanet::GetStartingNumFleet()
{
    return m_startingFleetSize;
}

int CPlanet::GetX()
{
    return m_x;
}

int CPlanet::GetY()
{
    return m_y;
}

void CPlanet::SetPlayerId(int i_playerId)
{
    m_playerId = i_playerId;
}

void CPlanet::SetStartingFleetSize(int i_fleetSize)
{
    m_fleetSize = i_fleetSize;
}

void CPlanet::SetStartingTime(int i_startingTime)
{
    m_startingTime = i_startingTime;
}
