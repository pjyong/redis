#ifndef KEYEDITSTRINGDIALOG_H
#define KEYEDITSTRINGDIALOG_H

#include <QDialog>

#include "keyeditdialog.h"


namespace Ui {
class KeyEditStringDialog;
}

class KeyEditStringDialog : public KeyEditDialog
{
    Q_OBJECT

public:
    explicit KeyEditStringDialog(QWidget *parent = nullptr);
    ~KeyEditStringDialog();
    void init();
private:
    Ui::KeyEditStringDialog *ui;
public slots:
    virtual void done(int);
    void on_saveButton_clicked();
    void on_closeButton_clicked();
};

#endif // KEYEDITSTRINGDIALOG_H
