#ifndef SETTING_H
#define SETTING_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QSettings>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QByteArray>
#include <QJsonArray>

class Setting
{
public:
    Setting();
    static QJsonObject getAllServers();
    static QJsonArray getAllSearchHistory();

    static QString toStr(QJsonObject);
    static QString toStr(QJsonArray);

    static void setCurrentServer(QString);
    static QString getCurrentServer();
    static QJsonObject getServerInfo(QString);
};

#endif // SETTING_H
