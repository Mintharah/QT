#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "RadioManager.h"
#include "AudioManager.h"
#include "VideoManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RadioManager radioManager;
    AudioManager audioManager;
    VideoManager videoManager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("radioManager", &radioManager);
    engine.rootContext()->setContextProperty("audioManager", &audioManager);
    engine.rootContext()->setContextProperty("videoManager", &videoManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );

    engine.loadFromModule("MultiMediaPlayer", "Main");

    return app.exec();
}
