#include <QDebug>
#include <algorithm>

#include "QtCore/qmath.h"
#include "Game.h"

//
CGame::CGame(QList<CPlayer*> i_id)
{
    m_playerList = i_id;

    m_growFactor = 1;
    m_fleetSpeed = 25;
    m_mapWidth = 400, m_mapHeight = 320;
    int startFleetSize = 50;
    int startRadius = 1;

    GenerateMap(i_id, m_mapWidth, m_mapHeight, startFleetSize, startRadius);
    m_runFlag = true;
}

void CGame::GenerateMap(QList<CPlayer*> i_players, int i_width, int i_height,
                        int i_startFleetSize, int i_startRadius)
{
    int centerX = i_width / 2;
    int centerY = i_height / 2;
    int playersCount = i_players.size();

    int startPositionX;
    int startPositionY;
    if (i_width > i_height)
    {
        startPositionX = centerX;
        startPositionY = centerY / 4;
    }
    else
    {
        startPositionX = centerX / 4;
        startPositionY = centerY;
    }

    const float PI = 3.14;
    static int lastPlanetId = 1;
    QTime currentTime = QTime::currentTime();
    float da = 2 * PI / playersCount;
    float vecX = startPositionX - centerX;
    float vecY = startPositionY - centerY;            
    qDebug() << vecX << ", " << vecY;
    for (float angle = 0; angle < 2*PI; angle += da)
    {
        // rotate vector here
        float curVx = vecX * cos(angle) - vecY * sin(angle);
        float curVy = vecX * sin(angle) + vecY * cos(angle);
        qDebug() << angle << ": " << curVx << ", " << curVy;

        // new planet coords
        int x = curVx + centerX;
        int y = curVy + centerY;                
        if (!i_players.empty())
        {
            qDebug() << "New planet: " << x << " " << y;
            CPlanet mainPlanet(lastPlanetId++, i_players.takeFirst()->GetId(),
                               i_startFleetSize,
                               i_startRadius,
                               x, y);
            mainPlanet.SetStartTime(currentTime);
            m_planetList[mainPlanet.GetPlanetId()] = mainPlanet;
        }


        /////////////////////////////////
        // adding neutral planets near the player
        srand((QTime::currentTime().msec() / 1000.0) * INT_MAX);
        //int r = rand();
        int neutralPlanetsCount = 2.0 * rand() / RAND_MAX + 2;
        srand(rand());
        for (int i = 0; i<neutralPlanetsCount; ++i)
        {
            float t = (float)rand() / RAND_MAX;
            qDebug() << "t: " << t;
            t = t / neutralPlanetsCount + (float)i / neutralPlanetsCount;
            qDebug() << "t: " << t << " i: " << i;
            float newAngle = t * (angle - da/2) + (1.0-t) * (angle + da/2);
            qDebug() << "newAngle: " << newAngle;
            srand(rand());
            float s = (float)rand() / RAND_MAX;
            qDebug() << "s: " << s;
            float neutralVx = s * (vecX * cos(newAngle) - vecY * sin(newAngle));
            float neutralVy = s * (vecX * sin(newAngle) + vecY * cos(newAngle));

            qDebug() << "vector: " << neutralVx << ", " << neutralVy;
            int neutralX = (neutralVx + centerX);
            int neutralY = (neutralVy + centerY);

            srand(rand());
            int neutralR = 10.0 * rand() / RAND_MAX + 1;
            int neutralStartFleetSize = 20 * ((float)rand() / RAND_MAX)+15;
            qDebug() << "fleet size: " << neutralStartFleetSize;
            qDebug() << "New neutral planet: " << neutralX << " " << neutralY;
            CPlanet mainPlanet(lastPlanetId++, 0,
                               neutralStartFleetSize,
                               neutralR,
                               neutralX, neutralY);
            mainPlanet.SetStartTime(currentTime);
            m_planetList[mainPlanet.GetPlanetId()] = mainPlanet;
        }
        ////////////////////////////////
    }
}

CStateMsg CGame::AddStep(CStepMsg *i_step)
{    

    qDebug() << "CGame::AddStep(CStepMsg *i_step) start \n\t sent " << i_step->GetPercent() << "percent of fleet";
    static int lastFleetId = 0;

    //m_dataLock.lock();
    QTime currentTime = QTime::currentTime();
    QList<int> m_srcPlanets = i_step->GetSrcPlanetsList();

    // check if attacker lose
    bool acceptFlag = false;
    int attackerId = i_step->GetSenderId();//m_planetList[m_srcPlanets.first()].GetPlayerId();
    foreach (CPlayer* player, m_playerList)
    {
        if (player->GetId() == attackerId)
        {
            acceptFlag = true;
        }
    }

    if (!acceptFlag)
    {
        return GetState();
    }

    int dstPlanetId = i_step->GetDstPlanetId();

    foreach (int srcPlanetId, m_srcPlanets)
    {
        // add new fleet
        CPlanet srcPlanet = m_planetList[srcPlanetId];
        int percent = i_step->GetPercent();
        int fleetSize = percent * srcPlanet.GetFleetSize() / 100;
        int srcPlanetId = srcPlanet.GetPlanetId();

        qDebug() << "\n\t\tpercent=" << percent << " * ";
        qDebug() << "\t\tsrcPlanet.GetFleetSize()=" << srcPlanet.GetFleetSize() << " ==== ";
        qDebug() << "\t\tfleetSize=" << fleetSize << "\n";

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

            qDebug() << "CGame::AddStep(CStepMsg *i_step) start \n\t\t fleet size = " << fleet.GetFleetSize() << " \n\t\tfleetSize = " << fleetSize;



            // remove from garrison appropriate ships count
            int srcPlanetFleetSize = m_planetList[srcPlanetId].GetFleetSize();
            m_planetList[srcPlanetId].SetStartTime(currentTime);
            m_planetList[srcPlanetId].SetFleetSize(srcPlanetFleetSize-fleetSize);
            m_planetList[srcPlanetId].SetStartFleetSize(srcPlanetFleetSize-fleetSize);
        }

    }

    //CStateMsg result(m_fleetList.values(), m_planetList.values());
    //recalculation();
    CStateMsg result = GetState();

   //m_dataLock.unlock();
    qDebug() << "CGame::AddStep(CStepMsg *i_step) exit";
    return result;

}

CStateMsg CGame::GetState()
{
    //m_dataLock.lock();
    qDebug() << "CGame::GetState()    m_dataLock.lock()";
    recalculation();
    qDebug() << "CGame::GetState()    exit recalculate";
    CStateMsg result(m_fleetList.values(), m_planetList.values());
    //m_dataLock.unlock();
    qDebug() << "CGame::GetState()    m_dataLock.unlock()";
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
    CStartMsg *start = new CStartMsg(m_fleetSpeed,
                                     m_growFactor,
                                     m_planetList.values(),
                                     m_playerList,
                                     m_mapWidth,
                                     m_mapHeight);
    emit SignalStart(start);



//    qDebug() << "CGame::run()";
//    while (m_runFlag)
//    {
//        qDebug() << "main loop";
//        qDebug() << "----------------------------";
//        qDebug() << "Fleets";
//        for (int i = 0; i<state.GetFleets().size(); ++i)
//        {
//            CFleet fleet = state.GetFleets().at(i);
//            qDebug() << fleet.GetFleetId() << ": " << fleet.GetFleetSize() << ", " << fleet.GetPercent();
//        }
//        qDebug() << "Planets";
//        for (int i = 0; i<state.GetPlanets().size(); ++i)
//        {
//            CPlanet planet = state.GetPlanets().at(i);
//            qDebug() << planet.GetPlanetId() << ": " << planet.GetPlayerId() << ", " << planet.GetFleetSize();
//        }


//        m_dataLock.lock();
//        recalculation();
//        m_dataLock.unlock();

////        sleep(1);
//    }
    exec();
}

void CGame::recalculation()
{
    // checking for game finish
    // ...
    bool endGameFlag = true;
    int firstPlayerId = -1;

    // looking for two active players
    qDebug() << "CGame::recalculation()   foreach (CPlanet planet, m_planetList.values())";
    foreach (CPlanet planet, m_planetList.values())
    {
        qDebug() << "CGame::recalculation()   in foreach (CPlanet planet, m_planetList.values())";
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
        qDebug() << "CGame::recalculation()   exit foreach (CPlanet planet, m_planetList.values())";
    }

    qDebug() << "if (endGameFlag)";
    if (endGameFlag)
    {
        qDebug() << "in if (endGameFlag)";
        qDebug() << "GAME OVER !";

        CFinishMsg *finishMsg = new CFinishMsg(firstPlayerId);
        emit SignalFinish(finishMsg);

    }

    // remove players, who lose
    QMap<int, bool> players;

    qDebug() << "01";
    foreach (CPlayer* player, m_playerList)
    {
        players.insert(player->GetId(), false);
    }

    qDebug() << "02";
    for (PlanetIterator iter = m_planetList.begin(); iter != m_planetList.end(); ++iter)
    {
        players[iter->GetPlayerId()] = true;
    }

    qDebug() << "03";
    for (PlayerIterator iter = m_playerList.begin(); iter != m_playerList.end(); ++iter)
    {
        if (!players[(*iter)->GetId()])
        {
            qDebug() << "REMOVE PLAYER !";
            m_playerList.erase(iter);
        }
//        else
//        {
//            ++iter;
//        }
    }

    // not tested yet!!!
    // remove all fleets belonged players-losers

    qDebug() << "04";
    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end(); ++iter)
    {
        if (!m_planetList.contains(iter.value().GetPlayerId()))
        {
            m_fleetList.erase(iter);
        }
//        else
//        {
//            ++iter;
//        }
    }

    QTime currentTime = QTime::currentTime();

    // update all planets

    qDebug() << "05";
    for (PlanetIterator iter = m_planetList.begin(); iter != m_planetList.end(); ++iter)
    {
        if ( iter.value().GetPlayerId() != 0 )
        {
            QTime planetStartTime = iter.value().GetStartTime();
            int dt = planetStartTime.secsTo(currentTime);
            int startFleetSize = iter.value().GetStartFleetSize();
            int radius = iter.value().GetPlanetRadius();
            int newFleetSize = startFleetSize + m_growFactor*radius*dt;
            iter.value().SetFleetSize(newFleetSize);
        }
    }

    qDebug() << "06";
    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end(); ++iter)
    {
        int totalDistance = iter.value().GetRouteLength();
        //int srcPlanetId = iter.value().GetSrcPlanetId();
        int dstPlanetId = iter.value().GetDstPlanetId();

        // calculate current percent for fleet
        QTime fleetStartTime = iter.value().GetStartTime();
        int dt = fleetStartTime.secsTo(currentTime);
        int currentDistance = dt * m_fleetSpeed;

        int newPercent = ((float)currentDistance / totalDistance) * 100;
        iter.value().SetPercent(newPercent);
        if (newPercent >= 100)
        {
            // planet changed suddenly, so change start time
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

    qDebug() << "07";
    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end(); ++iter)
    {
        if (iter.value().GetPercent() >= 100)
        {
            qDebug() << "(iter.value().GetPercent() >= 100) is TRUE";
            m_fleetList.erase(iter);
        }
//        else
//        {
//            qDebug() << "else ";
//            ++iter;
//        }
    }
    qDebug() << "08";
}
