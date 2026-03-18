#include "WeatherManager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>

WeatherManager::WeatherManager(QObject *parent) : QObject(parent){
    m_loading = false;
    m_statusMsg = "Search for a city to get started!";

    m_manager = new QNetworkAccessManager(this);
    m_forecastManager = new QNetworkAccessManager(this);

    connect(m_manager, &QNetworkAccessManager::finished,
            this, &WeatherManager::onCurrentWeatherReply);

    connect(m_forecastManager, &QNetworkAccessManager::finished,
            this, &WeatherManager::onForecastReply);

}
    QString WeatherManager::cityName()    const { return m_cityName; }
    QString WeatherManager::temperature() const { return m_temperature; }
    QString WeatherManager::condition()   const { return m_condition; }
    QString WeatherManager::humidity()    const { return m_humidity; }
    QString WeatherManager::windSpeed()   const { return m_windSpeed; }
    QString WeatherManager::weatherIcon() const { return m_weatherIcon; }
    QString WeatherManager::statusMsg()   const { return m_statusMsg; }
    QVariantList WeatherManager::forecast() const { return m_forecast; }
    bool WeatherManager::loading()        const { return m_loading; }

    void WeatherManager::searchCity(const QString &city){
        if(city.trimmed().isEmpty()){
            m_statusMsg = "Please enter a city name";
            emit statusChanged();
            return;
        }
        m_loading = true;
        emit loadingChanged();

        //current weather request
        QString currentUrl = BASE_URL + "weather?q=" + city.trimmed() +
                             "&appid=" + API_KEY + "&units=metric";

        m_manager->get(QNetworkRequest(QUrl(currentUrl)));

        //forecast request
        QString forecastUrl = BASE_URL + "forecast?q=" + city.trimmed() +
                              "&appid=" + API_KEY + "&units=metric";

        m_forecastManager->get(QNetworkRequest(QUrl(forecastUrl)));
    }


    QString WeatherManager::iconToEmoji(const QString &iconCode)
    {
        if (iconCode.startsWith("01")) return "☀️";
        if (iconCode.startsWith("02")) return "🌤️";
        if (iconCode.startsWith("03")) return "☁️";
        if (iconCode.startsWith("04")) return "☁️";
        if (iconCode.startsWith("09")) return "🌧️";
        if (iconCode.startsWith("10")) return "🌦️";
        if (iconCode.startsWith("11")) return "⛈️";
        if (iconCode.startsWith("13")) return "❄️";
        if (iconCode.startsWith("50")) return "🌫️";
        return "🌡️";
    }

    void WeatherManager::onCurrentWeatherReply(QNetworkReply *reply)
    {
        m_loading = false;
        emit loadingChanged();

        QByteArray responseData = reply->readAll();
        //qDebug() << "RAW RESPONSE:" << responseData;

        if (reply->error() != QNetworkReply::NoError) {
            m_statusMsg = "City not found. Please try again!";
            emit statusChanged();
            reply->deleteLater();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject root  = doc.object();

        m_cityName = root["name"].toString();

        QJsonObject main = root["main"].toObject();
        m_temperature = QString::number(qRound(main["temp"].toDouble())) + "°C";
        m_humidity = QString::number(main["humidity"].toInt()) + "%";

        QJsonObject wind = root["wind"].toObject();
        m_windSpeed = QString::number(qRound(wind["speed"].toDouble() * 3.6)) + " km/h";

        QJsonArray weatherArray = root["weather"].toArray();
        if (!weatherArray.isEmpty()) {
            QJsonObject weatherObj = weatherArray[0].toObject();
            m_condition = weatherObj["description"].toString();
            m_condition[0] = m_condition[0].toUpper();
            m_weatherIcon = iconToEmoji(weatherObj["icon"].toString());
        }

        m_statusMsg = "";
        emit weatherUpdated();
        emit statusChanged();

        reply->deleteLater();
    }


    void WeatherManager::onForecastReply(QNetworkReply *reply)
    {
        QByteArray responseData = reply->readAll();
        //qDebug() << "FORECAST RESPONSE:" << responseData;

        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        QJsonObject root  = doc.object();
        QJsonArray list   = root["list"].toArray();

        m_forecast.clear();
        int count = 0;

        for (int i = 0; i < list.size() && count < 5; i += 8) {
            QJsonObject entry = list[i].toObject();
            QJsonObject main  = entry["main"].toObject();

            QString icon;
            QJsonArray weatherArr = entry["weather"].toArray();
            if (!weatherArr.isEmpty()){
                icon = iconToEmoji(weatherArr[0].toObject()["icon"].toString());
            }

            qint64 timestamp = entry["dt"].toInt();
            QDateTime dt = QDateTime::fromSecsSinceEpoch(timestamp);
            QString dayName = dt.toString("ddd");

            QVariantMap day;
            day["day"]  = dayName;
            day["temp"] = QString::number(qRound(main["temp"].toDouble())) + "°";
            day["icon"] = icon;

            m_forecast.append(day);
            count++;
        }

        //qDebug() << "Forecast items parsed:" << m_forecast.size();

        emit weatherUpdated();
        reply->deleteLater();
    }
