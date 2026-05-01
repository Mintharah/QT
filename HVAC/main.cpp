#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "hvacController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    HvacController hvac;
    engine.rootContext()->setContextProperty("hvac", &hvac);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("HVAC", "Main");

    return QCoreApplication::exec();
}
