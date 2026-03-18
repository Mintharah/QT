#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WeatherManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString cityName      READ cityName      NOTIFY weatherUpdated)
    Q_PROPERTY(QString temperature   READ temperature   NOTIFY weatherUpdated)
    Q_PROPERTY(QString condition     READ condition     NOTIFY weatherUpdated)
    Q_PROPERTY(QString humidity      READ humidity      NOTIFY weatherUpdated)
    Q_PROPERTY(QString windSpeed     READ windSpeed     NOTIFY weatherUpdated)
    Q_PROPERTY(QString weatherIcon   READ weatherIcon   NOTIFY weatherUpdated)
    Q_PROPERTY(QString statusMsg     READ statusMsg     NOTIFY statusChanged)
    Q_PROPERTY(QVariantList forecast READ forecast      NOTIFY weatherUpdated)
    Q_PROPERTY(bool loading          READ loading       NOTIFY loadingChanged)

public:
    explicit WeatherManager(QObject *parent = nullptr);

    QString cityName()    const;
    QString temperature() const;
    QString condition()   const;
    QString humidity()    const;
    QString windSpeed()   const;
    QString weatherIcon() const;
    QString statusMsg()   const;
    QVariantList forecast() const;
    bool loading()        const;

public slots:
    void searchCity(const QString &city);

signals:
    void weatherUpdated();
    void statusChanged();
    void loadingChanged();

private slots:
    void onCurrentWeatherReply(QNetworkReply *reply);
    void onForecastReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
    QNetworkAccessManager *m_forecastManager;

    QString m_cityName;
    QString m_temperature;
    QString m_condition;
    QString m_humidity;
    QString m_windSpeed;
    QString m_weatherIcon;
    QString m_statusMsg;
    QVariantList m_forecast;
    bool m_loading;

    const QString API_KEY = "9006655663a51ab0a88c7aae26a0160b";
    const QString BASE_URL = "https://api.openweathermap.org/data/2.5/";

    QString iconToEmoji(const QString &iconCode);
};

#endif
