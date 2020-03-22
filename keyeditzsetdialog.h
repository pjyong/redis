#ifndef KEYEDITZSETDIALOG_H
#define KEYEDITZSETDIALOG_H

#include <QDialog>
#include "keyeditdialog.h"
#include "keyactionwidget.h"
namespace Ui {
class KeyEditZsetDialog;
}

class KeyEditZsetDialog : public KeyEditDialog
{
    Q_OBJECT

public:
    explicit KeyEditZsetDialog(QWidget *parent = nullptr);
    ~KeyEditZsetDialog();
    void init();

private:
    Ui::KeyEditZsetDialog *ui;
public slots:
    void keyAction(QString action, QString value, int row);
    void on_addHashBtn_clicked();
    void on_closeButton_clicked();

};

#endif // KEYEDITZSETDIALOG_H
