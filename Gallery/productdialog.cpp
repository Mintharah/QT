#include "productdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

ProductDialog::ProductDialog(const QString &name,
                             const QString &color,
                             const QString &type,
                             const QString &availability,
                             const QString &emoji,
                             QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(name);
    setFixedSize(380, 340);
    setStyleSheet("background-color: #FFF0F5;");

    //big emoji at top
    QLabel *emojiLabel = new QLabel(emoji, this);
    emojiLabel->setAlignment(Qt::AlignCenter);
    emojiLabel->setStyleSheet("font-size: 64px;");

    //product name
    QLabel *nameLabel = new QLabel(name, this);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet(
        "font-size: 22px; font-weight: bold; color: #8B3A62;"
        );

    // divider line between name and details
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #F48FB1;");

    // color row
    QLabel *colorField = new QLabel("🎨  Color:", this);
    colorField->setStyleSheet("font-size: 14px; color: #C06080; font-weight: bold;");
    QLabel *colorValue = new QLabel(color, this);
    colorValue->setStyleSheet("font-size: 14px; color: #4A1535;");

    // type row
    QLabel *typeField = new QLabel("🍽️  Type:", this);
    typeField->setStyleSheet("font-size: 14px; color: #C06080; font-weight: bold;");
    QLabel *typeValue = new QLabel(type, this);
    typeValue->setStyleSheet("font-size: 14px; color: #4A1535;");

    // Availability row colour changes based on stock status
    QLabel *availField = new QLabel("🛒  Availability:", this);
    availField->setStyleSheet("font-size: 14px; color: #C06080; font-weight: bold;");
    QLabel *availValue = new QLabel(availability, this);

    // Green = available, Orange = limited, Red = out of stock
                                                 if (availability == "Available")
                                                 availValue->setStyleSheet("font-size: 14px; color: #2E7D32; font-weight: bold;");
    else if (availability == "Limited Stock")
        availValue->setStyleSheet("font-size: 14px; color: #E65100; font-weight: bold;");
    else
        availValue->setStyleSheet("font-size: 14px; color: #C62828; font-weight: bold;");

    // close button
    QPushButton *closeBtn = new QPushButton("Close  ✕", this);
    closeBtn->setFixedSize(110, 36);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setStyleSheet(
        "QPushButton { background-color: #E91E8C; color: white;"
        "              border-radius: 18px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #C2185B; }"
        );
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(accept()));

    // each detail row = one horizontal layout (label left, value right)
    auto makeRow = [](QLabel *field, QLabel *value) -> QWidget* {
        QWidget *row = new QWidget();
        QHBoxLayout *hl = new QHBoxLayout(row);
        hl->setContentsMargins(30, 2, 30, 2);
        hl->addWidget(field);
        hl->addStretch();   // pushes value to the right
        hl->addWidget(value);
        return row;
    };

    // stack everything top to bottom
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setSpacing(6);
    vl->setContentsMargins(10, 16, 10, 16);

    vl->addWidget(emojiLabel);
    vl->addWidget(nameLabel);
    vl->addSpacing(6);
    vl->addWidget(line);
    vl->addSpacing(4);
    vl->addWidget(makeRow(colorField, colorValue));
    vl->addWidget(makeRow(typeField,  typeValue));
    vl->addWidget(makeRow(availField, availValue));
    vl->addStretch();
    vl->addWidget(closeBtn, 0, Qt::AlignCenter);
}

