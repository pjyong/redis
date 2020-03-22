#ifndef SERVERCONFIGDIALOG_H
#define SERVERCONFIGDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include "setting.h"

namespace Ui {
class ServerConfigDialog;
}

class ServerConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerConfigDialog(QWidget *parent = nullptr);
    ~ServerConfigDialog();
    void markDialogEdit(QString);

private:
    Ui::ServerConfigDialog *ui;
public slots:
    virtual void done(int);
};

#endif // SERVERCONFIGDIALOG_H
