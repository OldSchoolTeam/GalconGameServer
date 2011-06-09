#ifndef PLANET_H
#define PLANET_H

#include <QTime>

class CPlanet
{
public:
    CPlanet();
    CPlanet(int i_planetId,
            int i_playerId,
            int i_fleetSize,
            int i_planetRadius,                       
            int i_x,
            int i_y);

    int GetPlanetId();
    int GetPlayerId();
    int GetFleetSize();
    int GetPlanetRadius();
    QTime GetStartTime();
    int GetStartFleetSize();
    int GetX();
    int GetY();


    void SetPlayerId(int i_playerId);
    void SetStartTime(QTime i_time);
    void SetStartFleetSize(int i_startSize);
    void SetFleetSize(int i_fleetSize);

private:
    int m_planetId;
    int m_playerId;
    int m_fleetSize;
    int m_startFleetSize;
    int m_planetRadius;        
    int m_x;
    int m_y;
    QTime m_startTime;
};

#endif // PLANET_H
