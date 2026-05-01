#ifndef HVACCONTROLLER_H
#define HVACCONTROLLER_H
#include <QObject>

class HvacController : public QObject{
    Q_OBJECT

    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(int fanSpeed READ fanSpeed WRITE setFanSpeed NOTIFY fanSpeedChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(bool power READ power WRITE setPower NOTIFY powerChanged)

public:
    explicit HvacController(QObject *parent = nullptr);

    int     temperature() const { return m_temp; }
    int     fanSpeed()    const { return m_fanSpeed; }
    QString mode()        const { return m_mode; }
    bool    power()       const { return m_power; }

public slots:
    void setTemperature(int temp);
    void setFanSpeed(int speed);
    void setMode(const QString &mode);
    void setPower(bool on);

signals:
    void temperatureChanged(int temp);
    void fanSpeedChanged(int speed);
    void modeChanged(const QString &mode);
    void powerChanged(bool on);

private:
    int m_temp = 22;
    int m_fanSpeed = 2;
    QString m_mode = "cool";
    bool m_power = true;
};

#endif // HVACCONTROLLER_H
