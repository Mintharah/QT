#include "hvacController.h"

HvacController::HvacController(QObject *parent) : QObject(parent) {}

void HvacController::setTemperature(int temp) {
    temp = qBound(16, temp, 30);

    if(m_temp == temp){
        return;
    }

    m_temp = temp;
    emit temperatureChanged(temp);
}

void HvacController::setFanSpeed(int speed) {
    speed = qBound(0, speed, 5);
    if (m_fanSpeed == speed) {
        return;
    }

    m_fanSpeed = speed;
    emit fanSpeedChanged(speed);
}

void HvacController::setMode(const QString &mode) {
    if (m_mode == mode) {
        return;
    }
    m_mode = mode;
    emit modeChanged(mode);
}

void HvacController::setPower(bool on) {
    if (m_power == on) {
        return;
    }
    m_power = on;
    emit powerChanged(on);
}
