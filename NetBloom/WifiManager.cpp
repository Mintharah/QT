#include "WifiManager.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QTimer>
#include <QDBusMetaType>

WifiManager::WifiManager(QObject *parent) : QObject(parent){
    m_wifiEnabled = false;
    m_status = "Ready";

    //connect to network via d-bus

    m_interface = new QDBusInterface(
        "org.freedesktop.NetworkManager",   // service name
        "/org/freedesktop/NetworkManager",  // object path
        "org.freedesktop.NetworkManager",   // interface name
        QDBusConnection::systemBus(),       // system bus (not session bus)
        this
        );

    if(!m_interface->isValid()){
        setStatus("Could not connect to NetworkManager");
        qDebug() << "D-Bus error:" << m_interface->lastError().message();
    }
    else{
        setStatus("Connected to NetworkManager");

        //read curreny wifi state on startup
        QVariant state = m_interface->property("WirelessEnabled");
        if(state.isValid()){
            m_wifiEnabled = state.toBool();
            emit wifiEnabledChanged();
        }
    }
}

void WifiManager::setStatus(const QString &msg){
    m_status = msg;
    emit statusChanged();
}

bool WifiManager::wifiEnabled() const { return m_wifiEnabled; }
QStringList WifiManager::networks() const { return m_networks; }
QString WifiManager::status() const { return m_status; }

void WifiManager::turnOn(){
    if(!m_interface->isValid()){
        setStatus("NetworkManager not available");
        return;
    }

    //set wirelessEnabled property to true via d-bus
    m_interface->setProperty("WirelessEnabled", true);
    m_wifiEnabled = true;
    emit wifiEnabledChanged();
    setStatus("Wifi turned on");
}

void WifiManager::turnOff(){
    if(!m_interface->isValid()){
        setStatus("NetworkManager not available");
        return;
    }

    m_interface->setProperty("WirelessEnabled", false);
    m_wifiEnabled = false;
    emit wifiEnabledChanged();
    setStatus("Wifi turned off");
}


void WifiManager::scan()
{
    if (!m_wifiEnabled) {
        setStatus("Turn on WiFi first!");
        return;
    }

    setStatus("Scanning...");
    m_networks.clear();
    emit networksChanged();

    QDBusReply<QList<QDBusObjectPath>> devicesReply =
        m_interface->call("GetAllDevices");

    if (!devicesReply.isValid()) {
        setStatus("Scan failed");
        return;
    }

    for (const QDBusObjectPath &devicePath : devicesReply.value()) {
        QDBusInterface deviceIface(
            "org.freedesktop.NetworkManager",
            devicePath.path(),
            "org.freedesktop.NetworkManager.Device",
            QDBusConnection::systemBus()
            );

        QVariant deviceType = deviceIface.property("DeviceType");
        if (!deviceType.isValid() || deviceType.toUInt() != 2)
            continue;

        QDBusInterface wifiIface(
            "org.freedesktop.NetworkManager",
            devicePath.path(),
            "org.freedesktop.NetworkManager.Device.Wireless",
            QDBusConnection::systemBus()
            );

        // Request scan then wait 3 seconds before reading results
        wifiIface.call("RequestScan", QVariantMap());

        QTimer::singleShot(3000, this, [this, devicePath]() {
            QDBusInterface wifiIface2(
                "org.freedesktop.NetworkManager",
                devicePath.path(),
                "org.freedesktop.NetworkManager.Device.Wireless",
                QDBusConnection::systemBus()
                );

            QDBusReply<QList<QDBusObjectPath>> apReply =
                wifiIface2.call("GetAllAccessPoints");

            if (!apReply.isValid()) {
                setStatus("Could not read access points");
                return;
            }

            for (const QDBusObjectPath &apPath : apReply.value()) {
                QDBusInterface apIface(
                    "org.freedesktop.NetworkManager",
                    apPath.path(),
                    "org.freedesktop.NetworkManager.AccessPoint",
                    QDBusConnection::systemBus()
                    );

                QVariant ssidVariant = apIface.property("Ssid");
                if (ssidVariant.isValid()) {
                    QByteArray ssidBytes = ssidVariant.toByteArray();
                    QString ssid = QString::fromUtf8(ssidBytes);
                    if (!ssid.isEmpty() && !m_networks.contains(ssid))
                        m_networks.append(ssid);
                }
            }

            emit networksChanged();
            setStatus("Found " + QString::number(m_networks.size()) + " networks");
        });
    }
}

void WifiManager::connectToNetwork(const QString &ssid, const QString &password)
{
    if (!m_wifiEnabled) {
        setStatus("Turn on WiFi first!");
        return;
    }

    setStatus("Connecting to " + ssid + "...");

    //find WiFi device path
    QDBusReply<QList<QDBusObjectPath>> devicesReply =
        m_interface->call("GetAllDevices");

    if (!devicesReply.isValid()) {
        setStatus("Could not find WiFi device");
        return;
    }

    QString wifiDevicePath;
    for (const QDBusObjectPath &devicePath : devicesReply.value()) {
        QDBusInterface deviceIface(
            "org.freedesktop.NetworkManager",
            devicePath.path(),
            "org.freedesktop.NetworkManager.Device",
            QDBusConnection::systemBus()
            );
        QVariant deviceType = deviceIface.property("DeviceType");
        if (deviceType.isValid() && deviceType.toUInt() == 2) {
            wifiDevicePath = devicePath.path();
            break;
        }
    }

    if (wifiDevicePath.isEmpty()) {
        setStatus("No WiFi device found");
        return;
    }

    QVariantMap wireless;
    wireless["ssid"]  = ssid.toUtf8();
    wireless["mode"]  = QString("infrastructure");

    QVariantMap security;
    security["key-mgmt"] = QString("wpa-psk");
    security["psk"]      = password;

    QVariantMap connection;
    connection["type"] = QString("802-11-wireless");
    connection["id"]   = ssid;

    QMap<QString, QVariantMap> settings;
    settings["connection"]               = connection;
    settings["802-11-wireless"]          = wireless;
    settings["802-11-wireless-security"] = security;

    //register the type so D-Bus knows how to serialize it
    qDBusRegisterMetaType<QMap<QString, QVariantMap>>();

    QDBusReply<QDBusObjectPath> reply =
        m_interface->call(
            "AddAndActivateConnection",
            QVariant::fromValue(settings),
            QVariant::fromValue(QDBusObjectPath(wifiDevicePath)),
            QVariant::fromValue(QDBusObjectPath("/"))
            );

    if (reply.isValid()) {
        setStatus("Connected to " + ssid + "!");
    } else {
        setStatus("Failed: " + reply.error().message());
        qDebug() << "Connect error:" << reply.error().message();
    }
}
