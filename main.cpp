#include <QtCore/QCoreApplication>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QDebug>

#include "Options.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // add server_port to options
    COptions options = COptions::Parse(QCoreApplication::arguments());
    qDebug() << "Server options:\n"
             << "\tMaxNumPlayers = " << options.GetMaxNumPlayer() << "\n"
             << "\tTimeOut = " << options.GetTimeOut() << "\n"
             << "\tTimeToStart = " << options.GetTimeToStart() << "\n";


    // add catch exceptions
    CServer server(options);
    server.Start();


    return a.exec();
}
