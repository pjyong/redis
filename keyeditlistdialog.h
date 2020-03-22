#ifndef KEYEDITLISTDIALOG_H
#define KEYEDITLISTDIALOG_H

#include <QDialog>
#include "keyeditdialog.h"
#include "keyactionwidget.h"

namespace Ui {
class KeyEditListDialog;
}

class KeyEditListDialog : public KeyEditDialog
{
    Q_OBJECT

public:
    explicit KeyEditListDialog(QWidget *parent = nullptr);
    ~KeyEditListDialog();

    void init();

private:
    Ui::KeyEditListDialog *ui;
public slots:
    void on_addHashBtn_clicked();
    void on_closeButton_clicked();
    void on_removeBtn_clicked();
    void keyAction(QString action, QString value, int row);
    void actionTypeChanged(int);
    void occurChanged(int);
};

#endif // KEYEDITLISTDIALOG_H
