#include "Fleet.h"

CFleet::CFleet(int i_fleetId,
               int i_playerId,
               int i_fleetSize,
               int i_srcPlanetId,
               int i_dstPlanetId,
               int i_routeLength,
               int i_startTime)
{
    m_fleetId = i_fleetId;
    m_playerId = i_playerId;
    m_fleetSize = i_fleetSize;
    m_startPlanetId = i_srcPlanetId;
    m_finishPlanetId = i_dstPlanetId;
    m_startTime = i_startTime;
}

int CFleet::GetFleetId()
{
    return m_fleetId;
}

int CFleet::GetPlayerId()
{
    return m_playerId;
}

int CFleet::GetFleetSize()
{
    return m_fleetSize;
}

int CFleet::GetSrcPlanetId()
{
    return m_startPlanetId;
}

int CFleet::GetDstPlanetId()
{
    return m_finishPlanetId;
}

int CFleet::GetPercent()
{
    return m_percentOfLength;
}
