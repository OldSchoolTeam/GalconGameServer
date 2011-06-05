#include "Player.h"

CPlayer::CPlayer(int i_id, QString i_name)
{
    m_id = i_id;
    i_name = m_name;
}

int CPlayer::GetId()
{
    return m_id;
}

QString CPlayer::GetName()
{
    return m_name;
}
