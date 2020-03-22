#include "keyeditdialog.h"


void KeyEditDialog::setProperty(QString key, QString type)
{
    this->key = key;
    this->type = type;
}

KeyEditDialog::KeyEditDialog(QWidget *parent)
{

}

void KeyEditDialog::showMessage(QString msg, QString type, QWidget *messageWidget)
{
//    QPushButton *btn = nullptr;
    QLabel *label = nullptr;
    if(messageWidget->findChild<QLabel*>("messageLabel")){
//        btn = messageWidget->findChild<QPushButton *>("iconBtn");
        label = messageWidget->findChild<QLabel *>("messageLabel");
//        btn->show();
        label->show();
    }else{
        QLayout *ql = messageWidget->layout();
        delete ql;
        QHBoxLayout* pLayout = new QHBoxLayout();
        pLayout->setMargin(0);
        pLayout->setSpacing(0);
//        btn = new QPushButton();
//        btn->setIconSize(QSize(48, 48));
//        btn->setStyleSheet("padding:0;border:none;margin:0px;padding:10px 0px 10px 10px;background:#fff;");
//        btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//        btn->setObjectName("iconBtn");
        // 暂时不使用button
//        pLayout->addWidget(btn);
        // create label
        label = new QLabel();
        label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        label->setObjectName("messageLabel");
        pLayout->addWidget(label);
        messageWidget->setLayout(pLayout);
    }
    QString labelStyle = "background:#fff;padding:10px;margin:0px;";
    if(type.compare("success") == 0){
//        btn->setIcon(QIcon(":/static/images/success.png"));
        labelStyle = labelStyle + "color:#008000;";
    }else{
//        btn->setIcon(QIcon(":/static/images/error.png"));
        labelStyle = labelStyle + "color:#FF0000;";
    }
    label->setText(msg);
    label->setStyleSheet(labelStyle);
}

void KeyEditDialog::clearMessage(QWidget *messageWidget)
{
    if(messageWidget->findChild<QLabel*>("messageLabel")){
        qDebug() << "####################run hide###########";
//        QPushButton *btn = messageWidget->findChild<QPushButton *>("iconBtn");
        QLabel *label = messageWidget->findChild<QLabel *>("messageLabel");
//        btn->hide();
        label->hide();
    }
}


