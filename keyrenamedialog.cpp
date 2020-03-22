#include "keyrenamedialog.h"
#include "ui_keyrenamedialog.h"

KeyRenameDialog::KeyRenameDialog(QWidget *parent) :
    KeyEditDialog(parent),
    ui(new Ui::KeyRenameDialog)
{
    ui->setupUi(this);


}

KeyRenameDialog::~KeyRenameDialog()
{
    delete ui;
}

void KeyRenameDialog::init()
{

}

void KeyRenameDialog::setKey(QString key)
{
    ui->keyNameLineEdit->setText(key);
    originalKey = key;
    ui->keyField->setText(key);
}

void KeyRenameDialog::done(int r)
{
    if(KeyRenameDialog::Accepted == r)
    {
        // check new key exists first
        QString newKey = ui->keyNameLineEdit->text();
        if(newKey.compare("") == 0){
            showMessage("New key can not be empty", "error", ui->messageWidget);
            return;
        }
        RedisClient::Command cmd;
        cmd.addToPipeline({"EXISTS", QByteArray::fromStdString(newKey.toStdString())});
        QVariant v =  Redis::runSingleSyncCommand(cmd);
        if(v.toInt() == 1){
            showMessage("New key exist! please change another key.", "error", ui->messageWidget);

            return;
        }
        RedisClient::Command cmd2;
        cmd2.addToPipeline({"RENAME",QByteArray::fromStdString(originalKey.toStdString()), QByteArray::fromStdString(newKey.toStdString())});
        QVariant v2 =  Redis::runSingleSyncCommand(cmd2);
        if(v2.toString().compare("OK") != 0){
            showMessage("Rename failed! please try again.", "error", ui->messageWidget);
            return;
        }
        // rename success
        // notify main window
        emit renameKeySignal(originalKey, newKey);
    }
    QDialog::done(r);
    return;
}
