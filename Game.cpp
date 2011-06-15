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


void CGame::GenerateMap(QList<CPlayer *> i_players, int i_width, int i_height,
                         int i_startFleetSize, int i_startRadius)
{
    int lastPlanetId = 1;
    QTime currentTime = QTime::currentTime();
    srand((currentTime.msec() / 1000.0) * INT_MAX);
    int playersCount = i_players.size();

    QMap<QPair<int,int>, bool> totalPlanets;
    int density = 5;
    float neutralRectWidth = i_width / density;
    float neutralRectHeight = i_height / density;

    for (int i = 0; i<10; ++i)
    {
        for (int j = 0; j<10; ++j)
        {
            totalPlanets[QPair<int,int>(i, j)] = false;
        }
    }

    // players planet generation
    int playersLeft = playersCount;

    QMap<int, bool> players;
    for (int i = 0; i<=8; ++i)
        players[i] = false;

    while (playersLeft != 0)
    {
        // generate number from 0 to 8
        int r = rand();
        int cellNum = 8.0 * (((float)r) / RAND_MAX);
        if (cellNum == 4)
        {
            continue;
        }
        if (!players[cellNum])
        {
            players[cellNum] = true;
            //qDebug() << "Game: add planet " << cellNum;
            if (playersLeft != 1)
            {
                players[8-cellNum] = true;
                //qDebug() << "Game: add planet " << 8-cellNum;
                playersLeft -= 2;
            }
            else
            {
                playersLeft -= 1;
            }
        }
    }

    float playersRectWidth = i_width / 3.0;
    float playersRectHeight = i_height / 3.0;
    float startPlayerX = playersRectWidth / 2;
    float startPlayerY = playersRectHeight / 2;
    for (int i = 0; i<=8; ++i)
    {
        if (players[i] == true)
        {
            int x = i % 3 ; // from 0 to 2
            int y = i / 3 ; // from 0 to 2

            float baseXPos = startPlayerX + x*playersRectWidth;
            float baseYPos = startPlayerY + y*playersRectHeight;

            int deviationXPos = ((playersRectWidth/2) * ((float)rand() / RAND_MAX)) - playersRectWidth/4;
            int deviationYPos = ((playersRectHeight/2) * ((float)rand() / RAND_MAX)) - playersRectHeight/4;
            int xPos = baseXPos + deviationXPos;
            int yPos = baseYPos + deviationYPos;
            qDebug() << "Game: new planet " << xPos << " " << yPos;
            CPlanet mainPlanet(lastPlanetId++, i_players.takeFirst()->GetId(),
                               i_startFleetSize,
                               i_startRadius,
                               xPos, yPos);
            mainPlanet.SetStartTime(currentTime);
            m_planetList[mainPlanet.GetPlanetId()] = mainPlanet;

            int totalX = xPos / neutralRectWidth;
            int totalY = yPos / neutralRectHeight;
            //qDebug() << "total: " << totalX << " " << totalY;
            totalPlanets.insert(QPair<int,int>(totalX, totalY), true);
        }
    }
    // -------------------------

    // neutral planets generation
    float startNeutralX = neutralRectWidth / 2;
    float startNeutralY = neutralRectHeight / 2;

    int totalPlanetCount = 20;
    int neutralPlanetCount = totalPlanetCount - playersCount;
    for (int i = 0; i<neutralPlanetCount;)
    {
        int cellX = (density-1) * ((double)rand() / RAND_MAX);
        int cellY = (density-1) * ((double)rand() / RAND_MAX);
        if (totalPlanets[QPair<int,int>(cellX, cellY)])
        {
            continue;
        }
        int baseXPos = cellX * neutralRectWidth + startNeutralX;
        int baseYPos = cellY * neutralRectHeight + startNeutralY;
        int deviationXPos = ((neutralRectWidth/2) * ((float)rand() / RAND_MAX)) - neutralRectWidth/4;
        int deviationYPos = ((neutralRectHeight/2) * ((float)rand() / RAND_MAX)) - neutralRectHeight/4;
        int xPos = baseXPos + deviationXPos;
        int yPos = baseYPos + deviationYPos;
        int neutralPlanetRadius = 5.0 * ((float)rand() / RAND_MAX) + 1;
        int startNeutralFleetSize = 49.0 * ((float)rand() / RAND_MAX) + 1.0;
        qDebug() << xPos << " " << yPos;
        totalPlanets[QPair<int,int>(cellX, cellY)] = true;
        CPlanet mainPlanet(lastPlanetId++, 0,
                           startNeutralFleetSize,
                           neutralPlanetRadius,
                           xPos, yPos);
        qDebug() << "Game: neutral planet " << cellX << " " << cellY;
        mainPlanet.SetStartTime(currentTime);
        m_planetList[mainPlanet.GetPlanetId()] = mainPlanet;
        i++;
    }
    // --------------------------
}

CStateMsg CGame::AddStep(CStepMsg *i_step)
{

    //qDebug() << "CGame::AddStep(CStepMsg *i_step) start \n\t sent " << i_step->GetPercent() << "percent of fleet";
    static int lastFleetId = 1;

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
        int fleetSize = percent * srcPlanet.GetFleetSize() / 100 ;
        int srcPlanetId = srcPlanet.GetPlanetId();
/*
        qDebug() << "\n\t\tpercent=" << percent << " * ";
        qDebug() << "\t\tsrcPlanet.GetFleetSize()=" << srcPlanet.GetFleetSize() << " ==== ";
        qDebug() << "\t\tfleetSize=" << fleetSize << "\n";
*/
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
    qDebug() << "StateMsg CGame: GetState";
    recalculation();
    CStateMsg result(m_fleetList.values(), m_planetList.values());
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

    exec();
}

void CGame::recalculation()
{
    qDebug() << "Game: recalculation";
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
        qDebug() << "Game: GAME OVER !";
        qDebug() << "Game: winner is " << firstPlayerId;
        CFinishMsg *finishMsg = new CFinishMsg(firstPlayerId);
        emit SignalFinish(finishMsg);
        quit();
    }

    // remove players, who lose
    QMap<int, bool> players;

    foreach (CPlayer* player, m_playerList)
    {
        players.insert(player->GetId(), false);
    }

    for (PlanetIterator iter = m_planetList.begin(); iter != m_planetList.end(); ++iter)
    {
        players[iter->GetPlayerId()] = true;
    }

    for (PlayerIterator iter = m_playerList.begin(); iter != m_playerList.end();/* ++iter*/)
    {
        if (!players[(*iter)->GetId()])
        {
            qDebug() << "REMOVE PLAYER !" << (*iter)->GetId();
            iter = m_playerList.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    // remove all fleets belonged players-losers

    qDebug() << "Game: remove fleets";
    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end(); /*++iter*/)
    {
        if (!m_planetList.contains(iter.value().GetPlayerId()))
        {
            iter = m_fleetList.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    QTime currentTime = QTime::currentTime();

    // update all planets

    qDebug() << "Game: update planets";
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

    qDebug() << "update fleets";
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

    // resolve attacks
    for (FleetIterator iter = m_fleetList.begin(); iter != m_fleetList.end(); /*++iter*/)
    {
        if (iter.value().GetPercent() >= 100)
        {
            iter = m_fleetList.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}
