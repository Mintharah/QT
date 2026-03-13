#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "WifiManager.h"
#include "BluetoothManager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //create backend objects
    WifiManager wifiManager;
    BluetoothManager bluetoothManager;

    QQmlApplicationEngine engine;

    //expose C++ objects to QML
    //now QML can access them by these exact names
    engine.rootContext()->setContextProperty("wifiManager", &wifiManager);
    engine.rootContext()->setContextProperty("bluetoothManager", &bluetoothManager);

    //load the main QML file
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection
        );

    engine.loadFromModule("NetBloom", "Main");

    return app.exec();
}
