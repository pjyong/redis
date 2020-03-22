#ifndef REDIS_H
#define REDIS_H

#include <QVariant>
#include <qredisclient/redisclient.h>
#include "setting.h"
#include "logger.h"
#include <QException>

class Redis
{
public:
    Redis();
    static QVariant runSyncCommand(RedisClient::Command cmd);
    static QVariant runSingleSyncCommand(RedisClient::Command cmd);
    // somecommand
    static bool addString(QString key, QString value);
    static bool addHash(QString key, QString field, QString value);
    static bool addList(QString key, QString value);
    static bool addSet(QString key, QString value);
    static bool addZset(QString key, QString score, QString value);
};

#endif // REDIS_H
