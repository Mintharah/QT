#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QMovie>

class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);

signals:
    void splashFinished(); //fired when timer ends

private slots:
    void onTimerDone(); //called when display time is up

private:
    QLabel *gifLabel;
    QMovie *movie;
    QTimer *timer;
};

#endif // SPLASHSCREEN_H
