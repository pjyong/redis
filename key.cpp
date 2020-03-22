#include "key.h"

Key::Key(QObject *parent) : QObject(parent)
{

}


Key::Key(QString key, QString type)
{
    _key = key;
    _type = type;
}
