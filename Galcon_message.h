// Файл Galcon_message.h

#ifndef GALCON_MESSAGE_H
#define GALCON_MESSAGE_H

#include <string>

// Возможные типы сообщений
namespace Galcon_message
{
   static std::string connid = "SC_CONNID";            // Выдача ID клиенту
   static std::string timetostart = "SC_TIMETOSTART";  // Время до старта игры
   static std::string start = "SC_START";              // Начало игры
   static std::string state = "SC_STATE";              // Состояние игры
   static std::string finish = "SC_FINISH";            // Конец игры
   static std::string err = "SC_ERR";                  // Информация об ошибке
   static std::string conn = "CS_CONN";                // Подтверждение подключения
   static std::string step = "CS_STEP";                // Ход игрока
}

#endif // GALCON_MESSAGE_H
