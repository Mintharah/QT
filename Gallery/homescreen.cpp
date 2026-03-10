#include "homescreen.h"
#include "productdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QScreen>

static const QString PRODUCTS[5][5] = {
    { "Strawberry Smoothie",  "Blush Pink",    "Cold Beverage", "Available",     "🍓" },
    { "Vanilla Cupcake",      "Cream & Gold",  "Pastry",        "Available",     "🧁" },
    { "Rose Latte",           "Dusty Rose",    "Hot Beverage",  "Available",     "☕" },
    { "Raspberry Cheesecake", "Berry & White", "Dessert",       "Limited Stock", "🍰" },
    { "Peach Iced Tea",       "Soft Peach",    "Iced Beverage", "Out of Stock",  "🍹" }
};

HomeScreen::HomeScreen(QWidget *parent) : QWidget(parent) {


    setFixedSize(860, 620);
    setWindowTitle("Bloom Bites");

    //centre on screen
    QRect screen = QApplication::primaryScreen()->geometry();
    move((screen.width() - width())/2,
         (screen.height() - height())/2);

    setStyleSheet("background-color: pink;");

    //top bar
    QWidget *topBar = new QWidget(this);
    topBar->setFixedHeight(70);
    topBar->setStyleSheet("background-color: pink;" "border-bottom: 2px solid #F48FB1;");

    QHBoxLayout *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(20, 8, 20, 8);

    //app name on top left
    QLabel *appNameLabel = new QLabel("🌸 Bloom Bites", topBar);
    appNameLabel->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: #8B3A62;"
        );

    //spacer pushes everything else to the right
    QWidget *spacer = new QWidget(topBar);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    //date, time, temp labels
    dateLabel = new QLabel(topBar);
    dateLabel->setStyleSheet("font-size: 13px; color: #8B3A62;");

    timeLabel = new QLabel(topBar);
    timeLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #8B3A62;");

    tempLabel = new QLabel(topBar);
    tempLabel->setStyleSheet("font-size: 13px; color: #8B3A62;");

    //add everything to the top bar left to right
    topLayout->addWidget(appNameLabel);
    topLayout->addWidget(spacer);
    topLayout->addWidget(dateLabel);
    topLayout->addSpacing(16);
    topLayout->addWidget(timeLabel);
    topLayout->addSpacing(16);
    topLayout->addWidget(tempLabel);

    //welcome row
    QWidget* welcomeRow = new QWidget(this);
    welcomeRow->setFixedHeight(55);
    welcomeRow->setStyleSheet("background-color: #FFE4EE;");

    QHBoxLayout *welcomeLayout = new QHBoxLayout(welcomeRow);
    welcomeLayout->setContentsMargins(24, 6, 24, 6);

    QLabel *welcomeLabel = new QLabel("Welcome! What would you like today? 🌸", welcomeRow);
    welcomeLabel->setStyleSheet("font-size: 15px; color: #C06080;");

    QPushButton *infoBtn = new QPushButton("ℹ️  About This App", welcomeRow);
    infoBtn->setFixedSize(160, 36);
    infoBtn->setCursor(Qt::PointingHandCursor);
    infoBtn->setStyleSheet(
        "QPushButton { background-color: #E91E8C; color: white;"
        "              border-radius: 18px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #C2185B; }"
        );
    connect(infoBtn, SIGNAL(clicked()), this, SLOT(showAppInfo()));

    welcomeLayout->addWidget(welcomeLabel);
    welcomeLayout->addStretch();
    welcomeLayout->addWidget(infoBtn);

    //gallery
    QLabel *galleryTitle = new QLabel("🍽️  Our Menu — click a card to see details", this);
    galleryTitle->setContentsMargins(24, 14, 0, 8);
    galleryTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #8B3A62;");

    QWidget *galleryWidget = new QWidget(this);
    QGridLayout *grid = new QGridLayout(galleryWidget);
    grid->setSpacing(18);
    grid->setContentsMargins(24, 0, 24, 24);

    QPushButton *card0 = makeCard(PRODUCTS[0][4], PRODUCTS[0][0]);
    QPushButton *card1 = makeCard(PRODUCTS[1][4], PRODUCTS[1][0]);
    QPushButton *card2 = makeCard(PRODUCTS[2][4], PRODUCTS[2][0]);
    QPushButton *card3 = makeCard(PRODUCTS[3][4], PRODUCTS[3][0]);
    QPushButton *card4 = makeCard(PRODUCTS[4][4], PRODUCTS[4][0]);

    //connect them to their slots
    connect(card0, SIGNAL(clicked()), this, SLOT(showProduct0()));
    connect(card1, SIGNAL(clicked()), this, SLOT(showProduct1()));
    connect(card2, SIGNAL(clicked()), this, SLOT(showProduct2()));
    connect(card3, SIGNAL(clicked()), this, SLOT(showProduct3()));
    connect(card4, SIGNAL(clicked()), this, SLOT(showProduct4()));

    grid->addWidget(card0, 0, 0);
    grid->addWidget(card1, 0, 1);
    grid->addWidget(card2, 0, 2);
    grid->addWidget(card3, 1, 0);
    grid->addWidget(card4, 1, 1);

    //main layout, stack everything from top to bottom
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(welcomeRow);
    mainLayout->addWidget(galleryTitle);
    mainLayout->addWidget(galleryWidget);

    //timers
    clockTimer = new QTimer(this);
    connect(clockTimer, SIGNAL(timeout()), this, SLOT(updateDateTime()));
    clockTimer->start(1000);
    updateDateTime(); //call once immediately so labels arent empty at start

    tempTimer = new QTimer(this);
    connect(tempTimer, SIGNAL(timeout()), this, SLOT(updateTemperature()));
    tempTimer->start(5000);
    updateTemperature();

}

void HomeScreen::updateDateTime(){
    QDateTime now = QDateTime::currentDateTime();
    dateLabel->setText("📅  " + now.toString("dddd, MMM d yyyy"));
    timeLabel->setText("🕐  " + now.toString("hh:mm:ss"));
}

QString HomeScreen::getLaptopTemperature()
{
    // linux stores CPU temp in this file (in millidegrees)
    QFile file("/sys/class/thermal/thermal_zone0/temp");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString raw = in.readLine().trimmed();
        file.close();
        double temp = raw.toDouble() / 1000.0;  // convert millidegrees → degrees
        return QString::number(temp, 'f', 1) + " °C";
    }

    // if file not found (windows/mac), simulate a value
    int simulated = 42 + (rand() % 18);   // random between 42–59
    return QString::number(simulated) + " °C";
}

void HomeScreen::updateTemperature()
{
    tempLabel->setText("🌡  " + getLaptopTemperature());
}

QPushButton* HomeScreen::makeCard(const QString &emoji, const QString &name)
{
    QPushButton *card = new QPushButton(this);
    card->setFixedSize(230, 155);
    card->setCursor(Qt::PointingHandCursor);
    card->setText(emoji + "\n\n" + name);
    card->setStyleSheet(
        "QPushButton {"
        "   background-color: #FFD6E7;"
        "   border-radius: 14px;"
        "   border: 2px solid #F48FB1;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   color: #8B3A62;"
        "   padding: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #FFB6C1;"
        "   border: 2px solid #E91E8C;"
        "}"
        );
    return card;
}

void HomeScreen::showAppInfo()
{
    QMessageBox box(this);
    box.setWindowTitle("About Bloom Bites");
    box.setText(
        "<h2>🌸 Bloom Bites</h2>"
        "<p>Your cozy digital menu for discovering "
        "handcrafted beverages and sweet treats.</p>"
        "<p>Browse the gallery and click any item "
        "to see its full details.</p>"
        "<p>Version 1.0 — Made with Qt</p>"
        );
    box.setStyleSheet(
        "QMessageBox { background-color: #FFF0F5; }"
        "QPushButton { background-color: #E91E8C; color: white;"
        "              border-radius: 10px; padding: 6px 18px; }"
        );
    box.exec();   // .exec() means: show it and WAIT until user closes it
}

void HomeScreen::openProduct(int index)
{
    ProductDialog *dlg = new ProductDialog(
        PRODUCTS[index][0],   // name
        PRODUCTS[index][1],   // color
        PRODUCTS[index][2],   // type
        PRODUCTS[index][3],   // availability
        PRODUCTS[index][4],   // emoji
        this
        );
    dlg->exec();    // show as popup and wait
    delete dlg;     // clean up memory when closed
}

// Each slot just calls openProduct with the right index
void HomeScreen::showProduct0() { openProduct(0); }
void HomeScreen::showProduct1() { openProduct(1); }
void HomeScreen::showProduct2() { openProduct(2); }
void HomeScreen::showProduct3() { openProduct(3); }
void HomeScreen::showProduct4() { openProduct(4); }
