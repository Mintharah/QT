#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class HomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = nullptr);

private slots:
    void updateDateTime();
    void updateTemperature();
    void showAppInfo();
    void showProduct0();
    void showProduct1();
    void showProduct2();
    void showProduct3();
    void showProduct4();

private:
    //top bar widgets
    QLabel *dateLabel;
    QLabel *timeLabel;
    QLabel *tempLabel;

    //timers
    QTimer *clockTimer;
    QTimer *tempTimer;

    //helper functions
    QString getLaptopTemperature();
    void openProduct(int index);
    QPushButton* makeCard(const QString &emoji, const QString &name);
};

#endif // HOMESCREEN_H
