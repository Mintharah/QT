#include "splashscreen.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>

SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent) {
    //window setup
    setFixedSize(500,500);
    setWindowFlags(Qt::FramelessWindowHint); //no title bar

    //center the window on screen
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width())/2,
         (screen.height() - height())/2);

    setStyleSheet("background-color: pink;");

    //gif setup

    gifLabel = new QLabel(this);
    movie = new QMovie(":gif/spongebob.gif");

    gifLabel->setMovie(movie);
    gifLabel->setAlignment(Qt::AlignCenter);
    gifLabel->setScaledContents(true); //makes gif fit the label
    gifLabel->setFixedSize(400,400); //controls gif size

    movie->start(); //press play!

    //layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(gifLabel, 0, Qt::AlignCenter);

    //timer

    timer = new QTimer(this);
    timer->setSingleShot(true); //fire once
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerDone()));

    timer->start(2700);

}

void SplashScreen::onTimerDone(){
    movie->stop();
    emit splashFinished(); //tell main.cpp "im done!"
}
