#include "keyeditzsetdialog.h"
#include "ui_keyeditzsetdialog.h"

KeyEditZsetDialog::KeyEditZsetDialog(QWidget *parent) :
    KeyEditDialog(parent),
    ui(new Ui::KeyEditZsetDialog)
{
    ui->setupUi(this);
}

KeyEditZsetDialog::~KeyEditZsetDialog()
{
    delete ui;
}

void KeyEditZsetDialog::init()
{
    ui->keyField->setText(this->key);

    RedisClient::Command cmd;
    cmd.addToPipeline({"ZRANGE", QByteArray::fromStdString(this->key.toStdString()),
                       QByteArray::fromStdString("0"),
                       QByteArray::fromStdString("-1"),
                       QByteArray::fromStdString("WITHSCORES")
    });
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    QList<QVariant> l = res.toList();
    ui->tableWidget->setRowCount(l.length()/2);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    for(int i=0;i<l.length();i=i+2){
        qDebug() << i;
        int tableRow = i/2;
        qDebug() <<tableRow << "table row";

        KeyActionWidget* pWidget = new KeyActionWidget();

        QPushButton* btn_delete = new QPushButton();
//        btn_delete->setText("Delete");
        btn_delete->setIcon(QIcon(":/static/images/delete.png"));
        btn_delete->setCursor(Qt::PointingHandCursor);
        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);

        pLayout->addWidget(btn_delete);

        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);

        pWidget->value = l.value(i).toString();
        pWidget->row = tableRow;

        connect(btn_delete, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("delete", pWidget->value, pWidget->row);
        });

        ui->tableWidget->setCellWidget(tableRow, 0, pWidget);
        ui->tableWidget->setItem(tableRow, 1, new QTableWidgetItem(l.value(i).toString()));
        ui->tableWidget->setItem(tableRow, 2, new QTableWidgetItem(l.value(i+1).toString()));
    }
    qDebug() << l;
}

void KeyEditZsetDialog::keyAction(QString action, QString value, int row)
{
    clearMessage(ui->messageWidget);

    if(action.compare("delete") == 0){
        // delete key
        RedisClient::Command cmd;
        cmd.addToPipeline({"ZREM", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() >= 1){
            showMessage("delete successed!", "success", ui->messageWidget);
            // find
            ui->tableWidget->removeRow(row);
        }else{
            showMessage("delete failed! please try again.", "error", ui->messageWidget);
        }
        return;
    }
}

void KeyEditZsetDialog::on_addHashBtn_clicked()
{
    QString score = ui->scoreField->text();
    QString value = ui->hashValueField->toPlainText();

    RedisClient::Command cmd;
    cmd.addToPipeline({"ZADD", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(score.toStdString()), QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() >= 1){
        showMessage("add successed!", "success", ui->messageWidget);
        init();
    }else{
        showMessage("add failed!", "error", ui->messageWidget);
    }


}

void KeyEditZsetDialog::on_closeButton_clicked()
{
    QDialog::done(QDialog::Accepted);

}
