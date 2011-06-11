#include <QtCore/QCoreApplication>

#include "Options.h"
#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    COptions options = COptions::Parse(QCoreApplication::arguments());
    options.PrintOptions();

    CServer server(&options);
    server.Start();

    return a.exec();
}
