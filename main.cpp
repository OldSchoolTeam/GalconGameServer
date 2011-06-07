#include <QtCore/QCoreApplication>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QDebug>

#include "Options.h"
#include "Server.h"
#include "Exception.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // add server_port to options
    COptions options = COptions::Parse(QCoreApplication::arguments());
    qDebug() << "Server options:\n"
             << "\tPort = " << options.GetPort() << "\n"
             << "\tMaxNumPlayers = " << options.GetMaxNumPlayer() << "\n"
             << "\tTimeOut = " << options.GetTimeOut() << "\n"
             << "\tTimeToStart = " << options.GetTimeToStart() << "\n";

    // add catch exceptions
//    try
//    {
        CServer server(&options);
        server.Start();
//    }
//    catch(CException *exc)
//    {
//        qDebug() << "ServerException::" << exc->GetDescription();
//    }

    return a.exec();
}
