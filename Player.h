#ifndef CPLAYER_H
#define CPLAYER_H

#include <QString>

class CPlayer
{
public:
    CPlayer(int, QString);
    QString ToString();

private:
    int m_id;
    QString m_name;
};

#endif // CPLAYER_H
