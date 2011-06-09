#ifndef STARTMSG_H
#define STARTMSG_H

#include <QList>
#include "Player.h"
#include "Planet.h"
#include "Message.h"

class CStartMsg : public CMessage
{
public:
    CStartMsg(int i_flySpeed,
              int i_growFactor,
              QList<CPlanet> i_planets,
              QList<CPlayer*> i_players,
              int i_sizeX,
              int i_sizeY);
    QString ToString();
private:
    int m_flySpeed;
    int m_growFactor;
    QList<CPlanet> m_planets;
    QList<CPlayer*> m_players;
    int m_sizeX;
    int m_sizeY;
};

#endif // STARTMSG_H
