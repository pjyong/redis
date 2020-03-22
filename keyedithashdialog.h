#ifndef KEYEDITHASHDIALOG_H
#define KEYEDITHASHDIALOG_H

#include <QDialog>
#include "keyeditdialog.h"
#include "keyactionwidget.h"

namespace Ui {
class KeyEditHashDialog;
}

class KeyEditHashDialog : public KeyEditDialog
{
    Q_OBJECT

public:
    explicit KeyEditHashDialog(QWidget *parent = nullptr);
    ~KeyEditHashDialog();
    void init();

private:
    Ui::KeyEditHashDialog *ui;
public slots:
    virtual void done(int);
    void on_addHashBtn_clicked();
    void on_closeButton_clicked();

    void keyAction(QString action, QString key, int row);

};

#endif // KEYEDITHASHDIALOG_H
