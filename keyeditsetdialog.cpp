#include "keyeditsetdialog.h"
#include "ui_keyeditsetdialog.h"

KeyEditSetDialog::KeyEditSetDialog(QWidget *parent) :
    KeyEditDialog(parent),
    ui(new Ui::KeyEditSetDialog)
{
    ui->setupUi(this);
}

KeyEditSetDialog::~KeyEditSetDialog()
{
    delete ui;
}

void KeyEditSetDialog::init()
{
    ui->keyField->setText(this->key);

    RedisClient::Command cmd;
    cmd.addToPipeline({"SMEMBERS", QByteArray::fromStdString(this->key.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    QList<QVariant> l = res.toList();
    ui->allHashTableWidget->setRowCount(l.length());

    ui->allHashTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    for(int i=0;i<l.length();i=i+1){
        int tableRow = i;

        KeyActionWidget* pWidget = new KeyActionWidget();
//        QPushButton* btn_edit = new QPushButton();
//        btn_edit->setText("Edit");
        QPushButton* btn_delete = new QPushButton();
//        btn_delete->setText("Delete");
        btn_delete->setIcon(QIcon(":/static/images/delete.png"));
        btn_delete->setCursor(Qt::PointingHandCursor);
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);

//        pLayout->addWidget(btn_edit);
        pLayout->addWidget(btn_delete);

        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);

        pWidget->value = l.value(i).toString();
        pWidget->row = tableRow;

//        connect(btn_edit, &QPushButton::clicked, [this, pWidget](){
//            this->keyAction("edit", pWidget->value, pWidget->row);
//        });
        connect(btn_delete, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("delete", pWidget->value, pWidget->row);
        });

        ui->allHashTableWidget->setCellWidget(tableRow, 0, pWidget);
        ui->allHashTableWidget->setItem(tableRow, 1, new QTableWidgetItem(l.value(i).toString()));
    }
    qDebug() << res;
}

void KeyEditSetDialog::on_addHashBtn_clicked()
{
    QString value = ui->hashValueField->toPlainText();
    RedisClient::Command cmd;
    cmd.addToPipeline({"SADD", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() >= 1){
        showMessage("add success!", "success", ui->messageWidget);
        init();
    }else{
        showMessage("add failed!", "error", ui->messageWidget);
    }
}

void KeyEditSetDialog::on_closeButton_clicked()
{
    QDialog::done(QDialog::Accepted);
}

void KeyEditSetDialog::keyAction(QString action, QString value, int row)
{
    if(action.compare("delete") == 0){
        RedisClient::Command cmd;
        cmd.addToPipeline({"SREM", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() >= 1){
            showMessage("delete success!", "success", ui->messageWidget);
            init();
        }else{
            showMessage("remove failed!", "error", ui->messageWidget);
        }
    }
}
