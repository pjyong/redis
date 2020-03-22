#include "redis.h"
Redis::Redis()
{

}


QVariant Redis::runSyncCommand(RedisClient::Command cmd)
{
//    qDebug() << cmd.getRawString() << "##########";
//    Logger::getInstance()->log("running redis command", "info");
    QJsonObject info = Setting::getServerInfo(Setting::getCurrentServer());
    QString host = info.value("host").toString();
    initRedisClient();
    RedisClient::ConnectionConfig config(host);
    RedisClient::Connection connection(config);
    try {
        connection.connect(true);
    } catch (RedisClient::Connection::Exception e) {
        return QVariant(false);
    }
    if(!connection.isConnected()){
      return QVariant(false);
    }
    if(cmd.isEmpty()){
        return QVariant(true);
    }
    RedisClient::Response response = connection.commandSync(cmd);
    return response.value();
}

QVariant Redis::runSingleSyncCommand(RedisClient::Command cmd)
{
    QVariant v = runSyncCommand(cmd);
    if(!v.toBool()){
        return v;
    }
    return v.toList().first();
}

bool Redis::addString(QString key, QString value)
{
    RedisClient::Command cmd;
    cmd.addToPipeline({"SET", QByteArray::fromStdString(key.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toString().compare("OK") != 0){
        return false;
    }
    return true;
}

bool Redis::addHash(QString key, QString field, QString value)
{
    RedisClient::Command cmd;
    cmd.addToPipeline({"HSET", QByteArray::fromStdString(key.toStdString()), QByteArray::fromStdString(field.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() != 1){
        return false;
    }
    return true;
}

bool Redis::addList(QString key, QString value)
{
    RedisClient::Command cmd;
    cmd.addToPipeline({"RPUSH", QByteArray::fromStdString(key.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() >= 1){
        return true;
    }
    return false;
}

bool Redis::addSet(QString key, QString value)
{
    RedisClient::Command cmd;
    cmd.addToPipeline({"SADD", QByteArray::fromStdString(key.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() >= 1){
        return true;
    }
    return false;
}

bool Redis::addZset(QString key, QString score, QString value)
{
    RedisClient::Command cmd;
    cmd.addToPipeline({"ZADD", QByteArray::fromStdString(key.toStdString()), QByteArray::fromStdString(score.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() >= 1){
        return true;
    }
    return false;
}
