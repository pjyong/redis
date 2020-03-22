#include "keyedithashdialog.h"
#include "ui_keyedithashdialog.h"

KeyEditHashDialog::KeyEditHashDialog(QWidget *parent) :
    KeyEditDialog(parent),
    ui(new Ui::KeyEditHashDialog)
{
    ui->setupUi(this);
}

KeyEditHashDialog::~KeyEditHashDialog()
{
    delete ui;
}

void KeyEditHashDialog::init()
{
    ui->keyField->setText(this->key);
    RedisClient::Command cmd;
    cmd.addToPipeline({"HGETALL", QByteArray::fromStdString(this->key.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    QList<QVariant> l = res.toList();
    ui->allHashTableWidget->setRowCount(l.length()/2);

    ui->allHashTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    for(int i=0;i<l.length();i=i+2){
        int tableRow = i/2;
        KeyActionWidget* pWidget = new KeyActionWidget();
        QPushButton* btn_edit = new QPushButton();
//        btn_edit->setText("Edit");
        btn_edit->setIcon(QIcon(":/static/images/rename.png"));
        btn_edit->setCursor(Qt::PointingHandCursor);

        QPushButton* btn_delete = new QPushButton();
//        btn_delete->setText("Delete");
        btn_delete->setIcon(QIcon(":/static/images/delete.png"));
        btn_delete->setCursor(Qt::PointingHandCursor);
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);

        pLayout->addWidget(btn_edit);
        pLayout->addWidget(btn_delete);

        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);

        pWidget->key = l.value(i).toString();
        pWidget->row = tableRow;

        connect(btn_edit, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("edit", pWidget->key, pWidget->row);
        });
        connect(btn_delete, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("delete", pWidget->key, pWidget->row);
        });

        ui->allHashTableWidget->setCellWidget(tableRow, 0, pWidget);
        ui->allHashTableWidget->setItem(tableRow, 1, new QTableWidgetItem(l.value(i).toString()));
        ui->allHashTableWidget->setItem(tableRow, 2, new QTableWidgetItem(l.value(i+1).toString()));
    }
    qDebug() << res;

}

void KeyEditHashDialog::done(int r)
{

    QDialog::done(r);
    return;
}

void KeyEditHashDialog::on_addHashBtn_clicked()
{
    QString hashKey = ui->hashKeyField->text();
    QString hashValue = ui->hashValueField->toPlainText();
    RedisClient::Command cmd;
    cmd.addToPipeline({"HEXISTS", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(hashKey.toStdString())});
    cmd.addToPipeline({"HSET", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(hashKey.toStdString()), QByteArray::fromStdString(hashValue.toStdString())});
    QString action = "add";
    QVariant res =  Redis::runSyncCommand(cmd);
    QList<QVariant> l = res.toList();
    if(l.value(0).toInt() == 1){
        action = "edit";
    }
    if(action.compare("add") == 0){
        if(l.value(1).toInt() != 1){
            showMessage("add field failed! please try again.", "error", ui->messageWidget);
            return;
        }else{
            showMessage("add field successed!", "success", ui->messageWidget);
            // reset again
            init();
            return;
        }
    }else{
        // succcess: return 0  what happend?
        showMessage("edit field successed!", "success", ui->messageWidget);
        // reset again
        init();
    }
}

void KeyEditHashDialog::on_closeButton_clicked()
{
    QDialog::done(QDialog::Accepted);
}

void KeyEditHashDialog::keyAction(QString action, QString key, int row)
{
    clearMessage(ui->messageWidget);

    if(action.compare("delete") == 0){
        // delete key
        RedisClient::Command cmd;
        cmd.addToPipeline({"HDEL", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(key.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() == 1){
            showMessage("delete field successed!", "success", ui->messageWidget);
            // find
            ui->allHashTableWidget->removeRow(row);
        }else{
            showMessage("delete field failed! please try again.", "error", ui->messageWidget);
        }
        return;
    }

    if(action.compare("edit") == 0){
        RedisClient::Command cmd;
        cmd.addToPipeline({"HGET", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(key.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        ui->hashKeyField->setText(key);
        ui->hashValueField->setPlainText(res.toString());

        return;
    }
}
