#ifndef KEYRENAMEDIALOG_H
#define KEYRENAMEDIALOG_H

#include <QDialog>
#include "redis.h"
#include <qredisclient/redisclient.h>
#include "keyeditdialog.h"

namespace Ui {
class KeyRenameDialog;
}

class KeyRenameDialog : public KeyEditDialog
{
    Q_OBJECT

public:
    explicit KeyRenameDialog(QWidget *parent = nullptr);
    ~KeyRenameDialog();
    void init();
    void setKey(QString);
signals:
    void renameKeySignal(QString, QString);
private:
    Ui::KeyRenameDialog *ui;
    QString originalKey;
public slots:
    virtual void done(int);
};

#endif // KEYRENAMEDIALOG_H
