#ifndef KEYEDITDIALOG_H
#define KEYEDITDIALOG_H

#include <QDialog>
#include "redis.h"
#include <qredisclient/redisclient.h>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QSize>
#include <QLayout>

class KeyEditDialog : public QDialog
{
public:
    QString key;
    QString type;
    void setProperty(QString key = "", QString type = "");
    explicit KeyEditDialog(QWidget *parent = nullptr);
    // need child class finish
    virtual void init()=0;

    void showMessage(QString msg, QString type, QWidget *messageWidget);
    void clearMessage(QWidget *messageWidget);

};

#endif // KEYEDITDIALOG_H
