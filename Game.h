#ifndef GAME_H
#define GAME_H

#include <QMutex>
#include <QTime>
#include <QMap>
#include <QThread>
#include "Fleet.h"
#include "Planet.h"
#include "Player.h"
#include "StepMsg.h"
#include "StateMsg.h"
#include "StartMsg.h"
#include "FinishMsg.h"

class CGame : public QThread
{
    Q_OBJECT
public:
    CGame(QList<CPlayer*> i_id);
    CStateMsg AddStep(CStepMsg *);
    CStateMsg GetState();
    void run();

signals:
    void SignalFinish(CFinishMsg*);
    void SignalStart(CStartMsg*);

private:
    float GetRouteLength(int firstPlanetId, int secondPlanetId);
    void GenerateMap(QList<CPlayer*> i_players, int i_width, int i_height,
                     int i_startFleetSize, int i_startRadius);
    void recalculation();    

    QMutex m_dataLock;
    bool m_runFlag;

    QList<CPlayer*> m_playerList;
    typedef QList<CPlayer*>::iterator PlayerIterator;

    QMap<int, CFleet> m_fleetList;
    typedef QMap<int, CFleet>::iterator FleetIterator;

    QMap<int, CPlanet> m_planetList;
    typedef QMap<int, CPlanet>::iterator PlanetIterator;

    int m_fleetSpeed;
    int m_growFactor;
    int m_mapWidth;
    int m_mapHeight;
    QTime m_time;
};

#endif // GAME_H
