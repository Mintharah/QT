#include "BluetoothManager.h"
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDBusArgument>
#include <QDBusMetaType>
#include <QTimer>
#include <QDebug>

using InterfaceMap = QMap<QString, QVariantMap>;
using ObjectMap    = QMap<QDBusObjectPath, InterfaceMap>;

Q_DECLARE_METATYPE(InterfaceMap)
Q_DECLARE_METATYPE(ObjectMap)

BluetoothManager::BluetoothManager(QObject *parent) : QObject(parent){
    qDBusRegisterMetaType<InterfaceMap>();
    qDBusRegisterMetaType<ObjectMap>();

    m_bluetoothEnabled = false;
    m_status = "Ready";

    //objectManager lets us find all bluetooth objects
    m_objectManager = new QDBusInterface(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        QDBusConnection::systemBus(),
        this
    );

    if(!m_objectManager->isValid()){
        updateStatus("Could not connect to BlueZ");
        qDebug() << "BlueZ error:" << m_objectManager->lastError().message();
        return;
    }

    //find the bluletooth adapter path
    QString adapterPath = findAdapterPath();

    if(adapterPath.isEmpty()){
        updateStatus("No Bluetooth adapter found");
        return;
    }

    //connect to the actual adapter
    m_adapterInterface = new QDBusInterface(
        "org.bluez",
        adapterPath,
        "org.bluez.Adapter1",
        QDBusConnection::systemBus(),
        this
    );

    if(!m_adapterInterface->isValid()){
        updateStatus("Could not connect to Bluetooth adapter");
        return;
    }

    //read current powered state
    QVariant powered = m_adapterInterface->property("Powered");
    if(powered.isValid()){
        m_bluetoothEnabled = powered.toBool();
        emit bluetoothEnabledChanged();
    }

    updateStatus("Bluetooth ready!");
}

//getters
bool BluetoothManager::bluetoothEnabled() const {
    return m_bluetoothEnabled;
}

QStringList BluetoothManager::devices() const {
    return m_devices;
}

QString BluetoothManager::status() const {
    return m_status;
}

void BluetoothManager::updateStatus(const QString &msg){
    m_status = msg;
    emit statusChanged();
}

//find adapter + turn on/off
QString BluetoothManager::findAdapterPath(){
//getmanagedobjects returns a map of all bluez objects
//we loop through them to find Adapter1 interface
QDBusReply<ObjectMap> reply =
    m_objectManager->call("GetManagedObjects");

if(!reply.isValid()){
    qDebug() << "GetManagedObjects failed:" << reply.error().message();
    return QString();
}

//loop through every object bluez knows
const auto objects = reply.value();
for(auto it = objects.begin(); it != objects.end(); ++it){
    const QString path = it.key().path();
    const InterfaceMap &interfaces = it.value();

    //if this object has Adapter1 interface -> its our BT hardware
    if(interfaces.contains("org.bluez.Adapter1")){
        qDebug() << "Found adapter at:" << path;
        return path;
    }
}
return QString(); //nothing found
}

void BluetoothManager::turnOn(){
    if(!m_adapterInterface || !m_adapterInterface->isValid()){
        updateStatus("Bluetooth adapter not available");
        return;
    }

    //setting powered to true turns on the BT adapter
    m_adapterInterface->setProperty("Powered", true);
    m_bluetoothEnabled = true;
    emit bluetoothEnabledChanged();
    updateStatus("Bluetooth turned on");
}

void BluetoothManager::turnOff(){
    if(!m_adapterInterface || !m_adapterInterface->isValid()){
        updateStatus("Buetooth adapter not available");
        return;
    }

    m_adapterInterface->setProperty("Powered", false);
    m_bluetoothEnabled = false;
    emit bluetoothEnabledChanged();
    updateStatus("Bluetooth turned off");
}

//scan + connect
void BluetoothManager::scan()
{
    if (!m_bluetoothEnabled) {
        updateStatus("Turn on Bluetooth first!");
        return;
    }

    updateStatus("Scanning for devices...");
    m_devices.clear();
    emit devicesChanged();

    QDBusReply<void> discReply = m_adapterInterface->call("StartDiscovery");

    if (!discReply.isValid()) {
        QString error = discReply.error().message();
        if (!error.contains("already in progress")) {
            updateStatus("Scan failed: " + error);
            return;
        }
        qDebug() << "Scan already running, continuing anyway...";
    }

    //wait 5 seconds then collect results
    QTimer::singleShot(5000, this, [this]() {
        m_adapterInterface->call("StopDiscovery");

        QDBusReply<ObjectMap> reply =
            m_objectManager->call("GetManagedObjects");

        qDebug() << "GetManagedObjects valid:" << reply.isValid();
        qDebug() << "Total objects found:" << reply.value().size();

        if (!reply.isValid()) {
            updateStatus("Scan failed");
            return;
        }

        const auto objects = reply.value();
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            const InterfaceMap &interfaces = it.value();

            if (interfaces.contains("org.bluez.Device1")) {
                const QVariantMap &props = interfaces["org.bluez.Device1"];

                QString name = props.value("Name").toString();
                QString address = props.value("Address").toString();

                qDebug() << "Device found:" << name << address;

                if (!address.isEmpty()) {
                    QString display = name.isEmpty()
                    ? address
                    : name + "  (" + address + ")";
                    if (!m_devices.contains(display))
                        m_devices.append(display);
                }
            }
        }

        emit devicesChanged();
        updateStatus("Found " + QString::number(m_devices.size()) + " devices");
    });
}

void BluetoothManager::connectToDevice(const QString &address){
    if(!m_bluetoothEnabled){
        updateStatus("Turn on Bluetooth first!");
        return;
    }

    updateStatus("Connecting to " + address + "...");

    //find the device path from its address
    QDBusReply<ObjectMap> reply =
        m_objectManager->call("GetManagedObjects");

    if(!reply.isValid()){
        updateStatus("Could not find devices");
        return;
    }

    QString devicePath;
    const auto objects = reply.value();
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        const InterfaceMap &interfaces = it.value();

        if (interfaces.contains("org.bluez.Device1")) {
            const QVariantMap &props = interfaces["org.bluez.Device1"];

            //match by address
            if (props.value("Address").toString() == address) {
                devicePath = it.key().path();
                break;
            }
        }
    }

    if (devicePath.isEmpty()) {
        updateStatus("Device not found: " + address);
        return;
    }

    //connect to the device using its path
    QDBusInterface deviceIface(
        "org.bluez",
        devicePath,
        "org.bluez.Device1",
        QDBusConnection::systemBus()
        );

    QDBusReply<void> connectReply = deviceIface.call("Connect");

    if (connectReply.isValid()) {
        updateStatus("Connected to " + address + "!");
    } else {
        updateStatus("Connection failed: " + connectReply.error().message());
        qDebug() << "BT connect error:" << connectReply.error().message();
    }
}
