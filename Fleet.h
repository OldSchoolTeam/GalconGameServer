#ifndef FLEET_H
#define FLEET_H

class CFleet
{
public:
    CFleet(int i_fleetId,
           int i_playerId,
           int i_fleetSize,
           int i_srcPlanetId,
           int i_dstPlanetId,
           int i_routeLength,
           int i_startTime);

    int GetFleetId();
    int GetPlayerId();
    int GetFleetSize();
    int GetSrcPlanetId();
    int GetDstPlanetId();
    int GetSize();
    int GetPercent();
    void Refresh(int i_time);

private:
    int m_fleetId;
    int m_playerId;
    int m_fleetSize;
    int m_percentOfLength;    
    int m_startPlanetId;
    int m_finishPlanetId;    
    int m_startTime;

};

#endif // FLEET_H
