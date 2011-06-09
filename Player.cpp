#include "Player.h"

CPlayer::CPlayer(int i_id, QString i_name)
{
    m_id = i_id;
    m_name = i_name;
}

QString CPlayer::ToString()
{
    QString str = QString("(%1,%2)").arg(m_id).arg(m_name);
    return str;
}

int CPlayer::GetId()
{
    return m_id;
}

QString CPlayer::GetName()
{
    return m_name;
}
