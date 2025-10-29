#include <catch2/catch_session.hpp>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());

    int result = Catch::Session().run(argc, argv);

    return result;
}
