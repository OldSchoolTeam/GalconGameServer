#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>

class StepMsg;


class CGame : QObject
{
    Q_OBJECT

public:
    void CGame(QList<int>);
    void AddStep(StepMsg i_msg);
    StepMsg GetState();
    void Start();
    void Run();

    QMutex* m_dataMutex;
    CFleet m_fleetList;
    CPlanet m_planetList;
    QTime m_startTime;

signals:
    void signalFinish();

private:
    void quit();
    void recalculation();
};

#endif // GAME_H
