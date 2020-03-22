#ifndef KEYEDITSETDIALOG_H
#define KEYEDITSETDIALOG_H

#include <QDialog>
#include "keyeditdialog.h"
#include "keyactionwidget.h"

namespace Ui {
class KeyEditSetDialog;
}

class KeyEditSetDialog : public KeyEditDialog
{
    Q_OBJECT

public:
    explicit KeyEditSetDialog(QWidget *parent = nullptr);
    ~KeyEditSetDialog();
    void init();
private:
    Ui::KeyEditSetDialog *ui;
public slots:
    void on_addHashBtn_clicked();
    void on_closeButton_clicked();
    void keyAction(QString action, QString value, int row);
};

#endif // KEYEDITSETDIALOG_H
