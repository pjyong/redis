#include "serverconfigdialog.h"
#include "ui_serverconfigdialog.h"

ServerConfigDialog::ServerConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerConfigDialog)
{
    ui->setupUi(this);
}

ServerConfigDialog::~ServerConfigDialog()
{
    delete ui;
}

void ServerConfigDialog::markDialogEdit(QString host)
{
    QJsonObject allServers = Setting::getAllServers();
    QJsonObject someServer = allServers.value(host).toObject();
    setWindowTitle("Edit Server");
    ui->hostLineEdit->setText(someServer.value("host").toString());
    ui->portLineEdit->setText(someServer.value("port").toString());
    ui->passwordLineEdit->setText(someServer.value("password").toString());
}

void ServerConfigDialog::done(int r)
{
    if(ServerConfigDialog::Accepted == r)
    {
        // check ip format
        QString host = ui->hostLineEdit->text().isEmpty() ? "127.0.0.1" : ui->hostLineEdit->text();
        int port = ui->portLineEdit->text().isEmpty() ? 6379 : ui->portLineEdit->text().toInt();
        QString password = ui->passwordLineEdit->text().isEmpty() ? "" : ui->passwordLineEdit->text();
        QRegularExpression hostRe("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}");
        QRegularExpressionMatch hostMatch = hostRe.match(host);
        if(!hostMatch.hasMatch()){
            ui->hostErrorMsg->setText("*ip is not valid");
            return;
        }
        if(port <= 0){
            ui->portErrorMsg->setText("*port is not valid");
            return;
        }
        QSettings settings;
        // make json to save local setting
        QMap<QString, QVariant> map;
        map["host"] = host;
        map["port"] = QString::number(port);
        map["password"] = password;
        QJsonObject server = QJsonObject::fromVariantMap(map);
        QJsonObject allServers = Setting::getAllServers();
        allServers.insert(host, server);
//        qDebug() << Setting::toStr(allServers);
        settings.setValue("ServerList", Setting::toStr(allServers));
    }
    QDialog::done(r);
    return;
}
