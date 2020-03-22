#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
private:
    static Logger *mLogger;
    explicit Logger(QObject *parent = nullptr);
    void logToFile(QString msg, QString type);

public:
    static Logger* getInstance();
    void log(QString msg);
    void log(QString msg, QString type);

signals:
    void msgSignal(QString msg, QString type);

public slots:
};

#endif // LOGGER_H
