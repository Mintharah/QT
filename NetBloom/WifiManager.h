#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include <QStringList>
#include <QDBusInterface>

class WifiManager : public QObject {
    Q_OBJECT

    //these properties are readable by qml
    Q_PROPERTY(bool wifiEnabled READ wifiEnabled NOTIFY wifiEnabledChanged)
    Q_PROPERTY(QStringList networks READ networks NOTIFY networksChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit WifiManager(QObject *parent = nullptr);

    //getters, qml reads these
    bool wifiEnabled() const;
    QStringList networks() const;
    QString status() const;

public slots:
    //callable from the QML
    void turnOn();
    void turnOff();
    void scan();
    void connectToNetwork(const QString &ssid, const QString &password);

signals:
    //these notify QML when something changes
    void wifiEnabledChanged();
    void networksChanged();
    void statusChanged();

private:
    bool m_wifiEnabled;
    QStringList m_networks;
    QString m_status;

    //the d-bus interface
    QDBusInterface *m_interface;

    void setStatus(const QString &msg);
};

#endif // WIFIMANAGER_H
