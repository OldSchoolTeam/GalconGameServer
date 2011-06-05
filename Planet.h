#ifndef PLANET_H
#define PLANET_H

class CPlanet
{
public:
    CPlanet(int i_planetId,
            int i_playerId,
            int i_fleetSize,
            int i_planetRadius,
            int i_startingFleetSize,
            int i_startingTime,
            int i_x,
            int i_y);

    int GetPlanetId();
    int GetPlayerId();
    int GetStartingNumFleet();
    int GetStartingTime();
    int GetPlanetRadius();
    int GetX();
    int GetY();


    void SetPlayerId(int i_playerId);
    void SetStartingFleetSize(int i_fleetSize);
    void SetStartingTime(int i_startingTime);

private:
    int m_planetId;
    int m_playerId;
    int m_fleetSize;
    int m_planetRadius;
    int m_startingFleetSize;
    int m_startingTime;
    int m_x;
    int m_y;
};

#endif // PLANET_H
