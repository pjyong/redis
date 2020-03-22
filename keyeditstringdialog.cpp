#include "keyeditstringdialog.h"
#include "ui_keyeditstringdialog.h"

KeyEditStringDialog::KeyEditStringDialog(QWidget *parent) :
    KeyEditDialog(parent),
    ui(new Ui::KeyEditStringDialog)
{
    ui->setupUi(this);
}

KeyEditStringDialog::~KeyEditStringDialog()
{
    delete ui;
}

void KeyEditStringDialog::init()
{
    ui->keyField->setText(this->key);
    // get value
    RedisClient::Command cmd;
    cmd.addToPipeline({"GET", QByteArray::fromStdString(this->key.toStdString())});
    QVariant v =  Redis::runSingleSyncCommand(cmd);
    ui->valueField->setPlainText(v.toString());
}

void KeyEditStringDialog::done(int r)
{
    if(KeyEditStringDialog::Accepted == r){

    }
    QDialog::done(r);
    return;
}

void KeyEditStringDialog::on_saveButton_clicked()
{
    QString v = ui->valueField->toPlainText();
    RedisClient::Command cmd;
    cmd.addToPipeline({"SET", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(v.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toString().compare("OK") != 0){
        showMessage("edit failed! please try again.", "error", ui->messageWidget);
    }else{
        showMessage("edit success!", "success", ui->messageWidget);
    }
}

void KeyEditStringDialog::on_closeButton_clicked()
{
    QDialog::done(QDialog::Accepted);
}


