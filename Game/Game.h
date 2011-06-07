#ifndef GAME_H
#define GAME_H

#include <QMutex>
#include <QTime>
#include <QMap>
#include <QThread>
#include "Fleet.h"
#include "Planet.h"
#include "StepMsg.h"
#include "StateMsg.h"

class CGame : public QThread
{
    Q_OBJECT
public:
    CGame(QList<int> i_id);
    CStateMsg AddStep(CStepMsg* i_step);
    CStateMsg GetState();    
    void run();

signals:
    void SignalFinish();

private:
    float GetRouteLength(int firstPlanetId, int secondPlanetId);
    void recalculation();    

    QMutex m_dataLock;
    bool m_runFlag;

    QList<int> m_playerList;
    typedef QList<int>::iterator PlayerIterator;

    QMap<int, CFleet> m_fleetList;
    typedef QMap<int, CFleet>::iterator FleetIterator;

    QMap<int, CPlanet> m_planetList;
    typedef QMap<int, CPlanet>::iterator PlanetIterator;

    float m_fleetSpeed;
    QTime m_time;
};

#endif // GAME_H
