#include "logger.h"

Logger* Logger::mLogger = nullptr;
Logger::Logger(QObject *parent) : QObject(parent)
{

}

void Logger::logToFile(QString msg, QString type)
{
    // save into logfile, only log error
    if(type == "error"){

    }
}

Logger *Logger::getInstance()
{
    if(mLogger == NULL){
        mLogger = new Logger();
    }
    return mLogger;
}

void Logger::log(QString msg)
{
    emit msgSignal(msg, "info");
}

void Logger::log(QString msg, QString type)
{
    emit msgSignal(msg, type);
    logToFile(msg, type);
}
