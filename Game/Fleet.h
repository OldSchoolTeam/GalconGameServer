#ifndef FLEET_H
#define FLEET_H

#include <QTime>

class CFleet
{
public:
    CFleet(int i_fleetId,
           int i_playerId,
           int i_fleetSize,
           int i_srcPlanetId,
           int i_dstPlanetId,
           int i_routeLength,
           QTime i_startTime);

    int GetFleetId();
    int GetPlayerId();
    int GetFleetSize();
    int GetSrcPlanetId();
    int GetDstPlanetId();
    int GetRouteLength();    
    int GetPercent();    
    QTime GetStartTime();
    void SetFleetSize(int i_size);
    void SetPercent(int i_percent);

private:
    int m_fleetId;
    int m_playerId;
    int m_fleetSize;
    int m_percentOfLength;
    int m_routeLength;
    int m_startPlanetId;
    int m_finishPlanetId;    
    QTime m_startTime;
};

#endif // FLEET_H
