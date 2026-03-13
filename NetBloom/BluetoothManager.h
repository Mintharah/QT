#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QStringList>
#include <QDBusInterface>
#include <QDBusMetaType>

class BluetoothManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool bluetoothEnabled READ bluetoothEnabled NOTIFY bluetoothEnabledChanged)
    Q_PROPERTY(QStringList devices READ devices NOTIFY devicesChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit BluetoothManager(QObject *parent = nullptr);

    bool bluetoothEnabled() const;
    QStringList devices() const;
    QString status() const;

public slots:
    void turnOn();
    void turnOff();
    void scan();
    void connectToDevice(const QString &address);

signals:
    void bluetoothEnabledChanged();
    void devicesChanged();
    void statusChanged();

private:
    bool m_bluetoothEnabled;
    QStringList m_devices;
    QString m_status;

    //two interfaces needed for bluetooth
    QDBusInterface *m_adapterInterface; //controls bluetooth adapter
    QDBusInterface *m_objectManager; //finds bluetooth devices

    void updateStatus(const QString &msg);
    QString findAdapterPath(); //finds your bluetooth hardware path

};

#endif // BLUETOOTHMANAGER_H
