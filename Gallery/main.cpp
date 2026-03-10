#include "mainwindow.h"

#include <QApplication>
#include "splashscreen.h"
#include "homescreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SplashScreen *splash = new SplashScreen();
    HomeScreen *home = new HomeScreen();

    //when splash signals "done!" close splash and show home
    QObject::connect(splash, SIGNAL(splashFinished()), splash, SLOT(close()));
    QObject::connect(splash, SIGNAL(splashFinished()), home,   SLOT(show()));

    splash->show();

    return a.exec();
}
