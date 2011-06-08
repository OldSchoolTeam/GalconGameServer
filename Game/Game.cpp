#include <QDebug>
#include <algorithm>
#include "QtCore/qmath.h"
#include "Game.h"


CGame::CGame(QList<int> i_id)
{
    m_playerList = i_id;

    m_fleetSpeed = 25;
    int mapWidth = 300, mapHeight = 200;
    int startFleetSize = 50;
    int startRadius = 1;
    //int maxDeviationX = 3* mapWidth / 100, maxDeviationY = 3 * mapHeight / 100;

    static int lastPlanetId = 1;
    // map generation
    int cPlayers = i_id.size();

    int rectWidth = sqrt(1/(float)cPlayers) * mapWidth;
    int rectHeight = sqrt(1/(float)cPlayers) * mapHeight;

    int xStart = rectWidth / 2;
    int yStart = rectHeight / 2;

    QTime currentTime = QTime::currentTime();
    // here we need some changes
    for (int x = xStart; x < mapWidth; x+=rectWidth)
    {
        for (int y = yStart; y<mapHeight; y+=rectHeight)
        {
            if (!i_id.isEmpty())
            {
                CPlanet mainPlanet(lastPlanetId++, i_id.takeFirst(),
                                   startFleetSize,
                                   startRadius,
                                   x, y);
                mainPlanet.SetStartTime(currentTime);
                m_planetList[mainPlanet.GetPlanetId()] = mainPlanet;
            }
            // generate other planets for this part
            // ...
        }
    }

    m_runFlag = true;
}

CStateMsg CGame::AddStep(CStepMsg *i_step)
{    

    qDebug() << "New step";
    static int lastFleetId = 0;

    m_dataLock.lock();
    QTime currentTime = QTime::currentTime();
    QList<int> m_srcPlanets = i_step->GetSrcPlanetsList();
    int attackerId = m_planetList[m_srcPlanets.first()].GetPlayerId();
    int dstPlanetId = i_step->GetDstPlanetId();

    foreach (int srcPlanetId, m_srcPlanets)
    {
        // add new fleet
        CPlanet srcPlanet = m_planetList[srcPlanetId];
        int percent = i_step->GetPercent();
        int fleetSize = percent * srcPlanet.GetFleetSize() / 100;
        int srcPlanetId = srcPlanet.GetPlanetId();

        // ban on sending fleet
        // from planet to itself
        if (srcPlanetId != dstPlanetId)
        {
            float routeLength = GetRouteLength(srcPlanetId, dstPlanetId);
            CFleet fleet(lastFleetId++, attackerId, fleetSize,
                         srcPlanetId,
                         dstPlanetId,
                         routeLength,
                         currentTime);
            m_fleetList.insert(fleet.GetFleetId(), fleet);



            // remove from garrison appropriate ships count
            int srcPlanetFleetSize = m_planetList[srcPlanetId].GetFleetSize();
            m_planetList[srcPlanetId].SetFleetSize(srcPlanetFleetSize-fleetSize);
            m_planetList[srcPlanetId].SetStartFleetSize(srcPlanetFleetSize-fleetSize);
        }

    }

    CStateMsg result(m_fleetList.values(), m_planetList.values());
    m_dataLock.unlock();

    return result;

}

CStateMsg CGame::GetState()
{
    m_dataLock.lock();
    CStateMsg result(m_fleetList.values(), m_planetList.values());
    m_dataLock.unlock();
    return result;
}

float CGame::GetRouteLength(int firstPlanetId, int secondPlanetId)
{
    CPlanet firstPlanet = m_planetList[firstPlanetId];
    CPlanet secondPlanet = m_planetList[secondPlanetId];
    float distance = qSqrt(qPow(firstPlanet.GetX()-secondPlanet.GetX(), 2) +
                           qPow(firstPlanet.GetY()-secondPlanet.GetY(), 2));
    return distance;
}

void CGame::run()
{
    while (m_runFlag)
    {
        qDebug() << "main loop";
        CStateMsg state = GetState();
        qDebug() << "----------------------------";
        qDebug() << "Fleets";
        for (int i = 0; i<state.GetFleets().size(); ++i)
        {
            CFleet fleet = state.GetFleets().at(i);
            qDebug() << fleet.GetFleetId() << ": " << fleet.GetFleetSize() << ", " << fleet.GetPercent();
        }
        qDebug() << "Planets";
        for (int i = 0; i<state.GetPlanets().size(); ++i)
        {
            CPlanet planet = state.GetPlanets().at(i);
            qDebug() << planet.GetPlanetId() << ": " << planet.GetPlayerId() << ", " << planet.GetFleetSize();
        }


        m_dataLock.lock();       
        recalculation();
        m_dataLock.unlock();

        sleep(1);
    }
}

void CGame::recalculation()
{
    // checking for game finish
    // ...
    bool endGameFlag = true;
    int firstPlayerId = -1;

    // looking for two active players
    foreach (CPlanet planet, m_planetList.values())
    {
        int planetOwnerId = planet.GetPlayerId();
        if ((firstPlayerId == -1) && (planetOwnerId != 0))
        {
            firstPlayerId = planetOwnerId;
        }
        // looking for second player
        if ((planetOwnerId != 0) && (firstPlayerId != planetOwnerId))
        {
            endGameFlag = false;
        }
    }

    if (endGameFlag)
    {
        qDebug() << "GAME OVER !";
        emit SignalFinish();
        m_runFlag = false;
    }

    // remove players, who lose
    QMap<int, bool> players;
    foreach (int player, m_playerList)
    {
        players.insert(player, false);
    }
    for (PlanetIterator iter = m_planetList.begin(); iter != m_planetList.end(); ++iter)
    {
        players[iter->GetPlayerId()] = true;
    }
    for (PlayerIterator iter = m_playerList.begin(); iter != m_playerList.end();/* ++iter*/)
    {
        if (!players[*iter])
        {
            qDebug() << "REMOVE PLAYER !";
            m_playerList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    // not tested yet!!!
    // remove all fleets belonged to players who lose
    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end();)
    {
        if (!m_planetList.contains(iter.value().GetPlayerId()))
        {
            m_fleetList.erase(iter);
        }
        else
        {
             ++iter;
        }
    }

    QTime currentTime = QTime::currentTime();

    // update all planets
    for (PlanetIterator iter = m_planetList.begin(); iter != m_planetList.end(); ++iter)
    {
        QTime planetStartTime = iter.value().GetStartTime();
        int dt = planetStartTime.secsTo(currentTime);
        int startFleetSize = iter.value().GetStartFleetSize();
        int radius = iter.value().GetPlanetRadius();
        int newFleetSize = startFleetSize + radius*dt;
        iter.value().SetFleetSize(newFleetSize);
    }

    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end(); ++iter)
    {
        int totalDistance = iter.value().GetRouteLength();
        int srcPlanetId = iter.value().GetSrcPlanetId();
        int dstPlanetId = iter.value().GetDstPlanetId();

        // calculate current percent for fleet
        QTime fleetStartTime = iter.value().GetStartTime();
        int dt = fleetStartTime.secsTo(currentTime);
        int currentDistance = dt * m_fleetSpeed;

        int newPercent = ((float)currentDistance / totalDistance) * 100;
        iter.value().SetPercent(newPercent);
        if (newPercent >= 100)
        {
            // planet state changed suddenly, so change start time
            m_planetList[dstPlanetId].SetStartTime(currentTime);

            // change garrison size
            int planetGarnisonSize = m_planetList[dstPlanetId].GetFleetSize();
            int attackerFleetSize = iter.value().GetFleetSize();

            // rebase fleet between two
            // planets of one player
            if (iter.value().GetPlayerId() == m_planetList[dstPlanetId].GetPlayerId())
            {
                int newGarnisonSize = planetGarnisonSize + attackerFleetSize;
                m_planetList[dstPlanetId].SetFleetSize(newGarnisonSize);
                m_planetList[dstPlanetId].SetStartFleetSize(newGarnisonSize);
            }

            // attack to planet
            // planet changes its owner
            else if (planetGarnisonSize < attackerFleetSize)
            {
                int attackerId = iter.value().GetPlayerId();
                m_planetList[dstPlanetId].SetPlayerId(attackerId);
                int newGarnisonSize = attackerFleetSize - planetGarnisonSize;
                m_planetList[dstPlanetId].SetFleetSize(newGarnisonSize);
                m_planetList[dstPlanetId].SetStartFleetSize(newGarnisonSize);
            }
            // garrison size reduced
            else
            {
                int newGarnisonSize = planetGarnisonSize - attackerFleetSize;
                m_planetList[dstPlanetId].SetFleetSize(newGarnisonSize);
                m_planetList[dstPlanetId].SetStartFleetSize(newGarnisonSize);
            }
        }
    }

    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end();)
    {
        if (iter.value().GetPercent() >= 100)
        {
            m_fleetList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}
