#include "setting.h"

Setting::Setting()
{
}

QJsonObject Setting::getAllServers()
{
    QSettings settings;
    // 真尼玛奇怪fromVariant居然不工作
    QByteArray ba = QByteArray::fromStdString(settings.value("ServerList").toString().toStdString());
    return QJsonDocument::fromJson(ba).object();
}

QJsonArray Setting::getAllSearchHistory()
{
    QSettings settings;
    QByteArray ba = QByteArray::fromStdString(settings.value("SearchHistoryList").toString().toStdString());
    return QJsonDocument::fromJson(ba).array();
}

QString Setting::toStr(QJsonObject obj)
{
    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}

QString Setting::toStr(QJsonArray arr)
{
    QJsonDocument doc(arr);
    return doc.toJson(QJsonDocument::Compact);
}

void Setting::setCurrentServer(QString host)
{
    QSettings settings;
    settings.setValue("CurrentServer", host);
}

QString Setting::getCurrentServer()
{
    QSettings settings;
    return settings.value("CurrentServer").toString();
}

QJsonObject Setting::getServerInfo(QString host)
{
    QJsonObject allServers = Setting::getAllServers();
    return allServers.value(host).toObject();
}
