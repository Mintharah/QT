#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>

class ProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProductDialog(const QString &name,
                           const QString &color,
                           const QString &type,
                           const QString &availability,
                           const QString &emoji,
                           QWidget *parent = nullptr);
};

#endif // PRODUCTDIALOG_H
