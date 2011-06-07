#include <QtCore/QCoreApplication>
#include <QList>
#include <QDebug>
#include "Game.h"

class Sleeper : public QThread
{
public:
    static void sleep(int sec)
    {
        QThread::sleep(sec);
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<int> playerIdList;
    playerIdList += 1;
    playerIdList += 2;
    playerIdList += 3;
    CGame game(playerIdList);
    game.start();
/*
    QList<int> srcPlanetList1;
    srcPlanetList1 += 1;
    CStepMsg step1(srcPlanetList1, 50, 2);

    QList<int> srcPlanetList2;
    srcPlanetList2 += 3;
    CStepMsg step2(srcPlanetList2, 100, 1);

    game.AddStep(&step1);
    game.AddStep(&step2);
*/
    qDebug() << "SLEEP";
    Sleeper::sleep(6);
    qDebug() << "END SLEEP";

    QList<int> srcPlanetList3;
    srcPlanetList3 += 1;
    srcPlanetList3 += 3;
    CStepMsg step3(srcPlanetList3, 100, 2);
    qDebug() << "-----------------";
    qDebug() << "Send new fleet";
    game.AddStep(&step3);

    qDebug() << "SLEEP";
    Sleeper::sleep(1);
    qDebug() << "END SLEEP";

    QList<int> srcPlanetList1;
    srcPlanetList1 += 1;
    CStepMsg step1(srcPlanetList1, 50, 2);

    game.AddStep(&step1);
    /*
    QMap<int, int> map;
    map.insert(1, 1);
    map.insert(2, 2);
    map.insert(3, 3);
    for (QMap<int, int>::iterator iter = map.begin(); iter != map.end(); ++iter)
    {
        if (iter.value()<=2)
            map.erase(iter);
    }

    for (QMap<int, int>::iterator iter = map.begin(); iter != map.end(); ++iter)
    {
        qDebug() << iter.key() << " " << iter.value();
    }*/
    return a.exec();
}
