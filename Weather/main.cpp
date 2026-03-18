#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "WeatherManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    WeatherManager weatherManager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("weatherManager", &weatherManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Weather", "Main");

    return app.exec();
}
