#ifndef KEYACTIONWIDGET_H
#define KEYACTIONWIDGET_H

#include <QWidget>

class KeyActionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyActionWidget(QWidget *parent = nullptr);
    QString key;
    QString type;
    QString value;
    // table row index
    int row;

signals:

public slots:
};

#endif // KEYACTIONWIDGET_H
