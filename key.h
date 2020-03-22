#ifndef KEY_H
#define KEY_H

#include <QObject>

class Key : public QObject
{
    Q_OBJECT
public:
    QString _key;
    QString _type;
    explicit Key(QObject *parent = nullptr);
    explicit Key(QString key, QString type);

signals:

public slots:
};

#endif // KEY_H
