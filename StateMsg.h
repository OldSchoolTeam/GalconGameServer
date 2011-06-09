#ifndef STATEMSG_H
#define STATEMSG_H

#include <QList>
#include <Fleet.h>
#include <Planet.h>
#include "Message.h"

class CStateMsg : public CMessage
{
public:
    CStateMsg(QList<CFleet> i_fleets, QList<CPlanet> i_planets);
    QString ToString();
    QList<CFleet> GetFleets();
    QList<CPlanet> GetPlanets();
private:
    QList<CFleet> m_fleetList;
    QList<CPlanet> m_planetList;
};

#endif // STATEMSG_H
