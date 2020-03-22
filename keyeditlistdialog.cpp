#include "keyeditlistdialog.h"
#include "ui_keyeditlistdialog.h"

KeyEditListDialog::KeyEditListDialog(QWidget *parent) :
    KeyEditDialog(parent),
    ui(new Ui::KeyEditListDialog)
{
    ui->setupUi(this);

    connect(ui->actionTypeFiled, SIGNAL(currentIndexChanged(int)), this, SLOT(actionTypeChanged(int)));
    connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(occurChanged(int)));

}

KeyEditListDialog::~KeyEditListDialog()
{
    delete ui;
}

void KeyEditListDialog::init()
{
    //
    ui->keyField->setText(this->key);
    ui->keyField_2->setText(this->key);
    ui->occurrenceLineEdit->hide();

    ui->listIndexField->hide();


    RedisClient::Command cmd;
    cmd.addToPipeline({"LRANGE", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString("0"), QByteArray::fromStdString("10000")});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    QList<QVariant> l = res.toList();
    ui->allHashTableWidget->setRowCount(l.length());

    ui->allHashTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    for(int i=0;i<l.length();i=i+1){
        int tableRow = i;

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

        pWidget->value = l.value(i).toString();
        pWidget->row = tableRow;

        connect(btn_edit, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("edit", pWidget->value, pWidget->row);
        });
        connect(btn_delete, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("delete", pWidget->value, pWidget->row);
        });

        ui->allHashTableWidget->setCellWidget(tableRow, 0, pWidget);
        ui->allHashTableWidget->setItem(tableRow, 1, new QTableWidgetItem(l.value(i).toString()));
    }
    qDebug() << res;

}

void KeyEditListDialog::on_addHashBtn_clicked()
{
    int actionType = ui->actionTypeFiled->currentIndex();
    QString value = ui->hashValueField->toPlainText();
    QString index = ui->listIndexField->text();
    RedisClient::Command cmd;
    if(actionType == 0){
        // prepend
        cmd.addToPipeline({"LPUSH", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() >= 1){
            showMessage("prepend success!", "success", ui->messageWidget);
            init();
        }else{
            showMessage("prepend failed!", "error", ui->messageWidget);
        }
        return;
    }else if(actionType == 1){
        // append
        cmd.addToPipeline({"RPUSH", QByteArray::fromStdString(this->key.toStdString()), QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() >= 1){
            showMessage("append success!", "success", ui->messageWidget);

            init();
        }else{
            showMessage("append failed!", "error", ui->messageWidget);
        }
        return;

    }else if(actionType == 2){
        // after
        cmd.addToPipeline({"LINSERT", QByteArray::fromStdString(this->key.toStdString()),
            QByteArray::fromStdString("AFTER"),
            QByteArray::fromStdString(index.toStdString()),
            QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() >= 1){            
            showMessage("insert after success!", "success", ui->messageWidget);
            init();

        }else{
            showMessage("insert after failed!", "error", ui->messageWidget);

        }
        return;

    }else if(actionType == 3){
        // before
        cmd.addToPipeline({"LINSERT", QByteArray::fromStdString(this->key.toStdString()),
            QByteArray::fromStdString("BEFORE"),
            QByteArray::fromStdString(index.toStdString()),
            QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toInt() >= 1){
            showMessage("insert before success!", "success", ui->messageWidget);
            init();

        }else{
            showMessage("insert before failed!", "error", ui->messageWidget);

        }
        return;
    }else{
        // edit
        cmd.addToPipeline({"LSET", QByteArray::fromStdString(this->key.toStdString()),
            QByteArray::fromStdString(index.toStdString()),
            QByteArray::fromStdString(value.toStdString())});
        QVariant res =  Redis::runSingleSyncCommand(cmd);
        if(res.toString().compare("OK") == 0){
            showMessage("edit success!", "success", ui->messageWidget);

            init();
        }else{
            showMessage("edit failed!", "error", ui->messageWidget);
        }
        return;
    }
}

void KeyEditListDialog::on_closeButton_clicked()
{
    QDialog::done(QDialog::Accepted);
}

void KeyEditListDialog::on_removeBtn_clicked()
{
    QString value = ui->hashValueField_2->toPlainText();
    QString count = ui->occurrenceLineEdit->text();
    RedisClient::Command cmd;
    cmd.addToPipeline({"LREM", QByteArray::fromStdString(this->key.toStdString()),
        QByteArray::fromStdString(count.toStdString()),
        QByteArray::fromStdString(value.toStdString())});
    QVariant res =  Redis::runSingleSyncCommand(cmd);
    if(res.toInt() >= 1){
        showMessage("remove success!", "success", ui->messageWidget);

        init();
    }else{
        showMessage("remove failed!", "error", ui->messageWidget);
    }
}

void KeyEditListDialog::keyAction(QString action, QString value, int row)
{
    clearMessage(ui->messageWidget);

    if(action.compare("delete") == 0){
        ui->hashValueField_2->setPlainText(value);
        ui->occurrenceLineEdit->setText("0");
        return;
    }

    if(action.compare("edit") == 0){
        ui->hashValueField->setPlainText(value);
        ui->actionTypeFiled->setCurrentIndex(4);
        ui->listIndexField->setText(QString::number(row));

        return;
    }
}

void KeyEditListDialog::actionTypeChanged(int index)
{
    if(index == 0 || index == 1){
        ui->listIndexField->hide();
    }else{
        ui->listIndexField->show();
    }
}

void KeyEditListDialog::occurChanged(int index)
{
    if(index == 0){
        ui->occurrenceLineEdit->hide();
    }else{
        ui->occurrenceLineEdit->show();
    }
}
