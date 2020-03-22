#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QClipboard>
#include <QFileSystemModel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // init server list
    loadAllServersList();
    loadAllSearchHistory();
    //
    connect(ui->serverListWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(onListMailItemClicked(QListWidgetItem*)));
    // search keys input
    connect(ui->searchCommandEdit, SIGNAL(returnPressed()), this, SLOT(on_searchCommandBtn_clicked()));

    connect(ui->searchHistoryWidget, SIGNAL(itemClicked(QListWidgetItem*)),
                this, SLOT(onHistoryItemClicked(QListWidgetItem*)));

    loggerManager = Logger::getInstance();
    connect(loggerManager, SIGNAL(msgSignal(QString, QString)), this, SLOT(logTextBrowser(QString, QString)));

    // add clear log menu
    ui->logBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->logBrowser, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showLogContextMenu(const QPoint &)));

    connect(ui->actionServerManager, SIGNAL(triggered(bool)), this, SLOT(openServerManager(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete loggerManager;
    delete serverModelWidget;
}

void MainWindow::loadAllServersList()
{
    ui->serverListWidget->clear();
    QJsonObject allServers = Setting::getAllServers();
    if(allServers.isEmpty()){
        return;
    }
    QJsonObject::iterator it;
    QString firstHost;
    for(it = allServers.begin();it != allServers.end();++it){
        if(firstHost.isEmpty()){
            firstHost = it.value().toObject().value("host").toString();
        }
        ui->serverListWidget->addItem(it.value().toObject().value("host").toString());
    }
    chooseServer(firstHost);
    serverModelWidget = new QStandardItemModel();
    serverModelWidget->clear();
    for(it = allServers.begin();it != allServers.end();++it){
        QStandardItem* item0 = new QStandardItem(it.value().toObject().value("host").toString());
        item0->setEditable(false);
        // get all dbs
        RedisClient::Command cmd;
        cmd.addToPipeline({"INFO", "keyspace"});
        QVariant res = Redis::runSingleSyncCommand(cmd);
        qDebug() << res << "haha";
//        QStandardItem* item3 = new QStandardItem(QIcon("test.png"), "3 third item");
//        item0->appendRow(item3);
        serverModelWidget->appendRow(item0);
    }
    ui->serListTreeWidget->setHeaderHidden(true);
    ui->serListTreeWidget->setModel(serverModelWidget);
}

void MainWindow::loadAllSearchHistory(QString key)
{
    ui->searchHistoryWidget->clear();
    QJsonArray allSearchHistory = Setting::getAllSearchHistory();
    if(key.compare("") != 0){
        if(allSearchHistory.first().toString().compare(key) != 0){
            allSearchHistory.prepend(QJsonValue(key));
            // only get the first 30 commmands
            // save again
            QSettings settings;
            settings.setValue("SearchHistoryList", Setting::toStr(allSearchHistory));
        }
    }
    if(allSearchHistory.isEmpty()){
        ui->searchHistoryWidget->clear();
        return;
    }
    QJsonArray::iterator it;
    for(it = allSearchHistory.begin();it != allSearchHistory.end();++it){
        ui->searchHistoryWidget->addItem(it->toString());
    }
}


void MainWindow::chooseServer(QString host)
{
    QList<QListWidgetItem*> searched = ui->serverListWidget->findItems(host, Qt::MatchExactly);
    if(searched.isEmpty()){return;}
    ui->serverListWidget->setCurrentItem(searched.first());
    ui->editServerBtn->setEnabled(true);
    // save into local db
    Setting::setCurrentServer(host);
    // load right window
    ui->mainWindowLabel->setText("Current server:" + host);
}

void MainWindow::logTextBrowser(QString str, QString type)
{
    if(type == "error"){
        str = "<font style='color:red;'>" + str + "</font>";
    }else if(type == "success"){
        str = "<font style='color:green;'>" + str + "</font>";
    }
    ui->logBrowser->append(str);
}

void MainWindow::clearLog()
{

}

void MainWindow::showLogContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->logBrowser->mapToGlobal(pos);
    QMenu myMenu;
    myMenu.addAction("Copy");
    myMenu.addAction("Clear Log");
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if(selectedItem->text().compare("Copy") == 0){
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(ui->logBrowser->toPlainText());
        }else if(selectedItem->text().compare("Clear Log") == 0){
            ui->logBrowser->setText("");
        }
    }
    else
    {
        // nothing was chosen
    }
}

void MainWindow::search(QString input)
{
    ui->searchResultWidget->clearContents();

    RedisClient::Command cmd;
    cmd.addToPipeline({"KEYS", QByteArray::fromStdString(input.toStdString())});
    QVariant v = Redis::runSingleSyncCommand(cmd);
    if(!v.toBool()){
        logTextBrowser("Redis server down", "error");
        return;
    }
    QList<QVariant> list = v.toList();
    if(list.isEmpty()){
        logTextBrowser("Did not find any keys", "error");
        return;
    }
    QList<QVariant>::iterator i;
    RedisClient::Command allKeysCommand;
    QByteArray tmp;
    for(i=list.begin();i!=list.end();++i){
        tmp = i->toByteArray();
        allKeysCommand.addToPipeline({"TYPE", tmp});
        allKeysCommand.addToPipeline({"TTL", tmp});
        allKeysCommand.addToPipeline({"OBJECT", "encoding", tmp});
        allKeysCommand.addToPipeline({"OBJECT", "refcount", tmp});
    }
    QVariant v2 = Redis::runSyncCommand(allKeysCommand);
    if(!v2.toBool()){
        logTextBrowser("Redis server down", "error");
        return;
    }
    QList<QVariant> batchResList = v2.toList();

    // split 4 result
    int rows = batchResList.length()/4;
    ui->searchResultWidget->setRowCount(rows);
    ui->searchResultWidget->setColumnCount(7);

    qDebug() << rows;
    QVariant tmpType;
    QVariant tmpTtl;
    QVariant tmpEncoding;
    QVariant tmpRefcount;

    RedisClient::Command sizeAllCommands;
    // adjust column size
//    ui->searchResultWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->searchResultWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->searchResultWidget->horizontalHeader()->resizeSection(1, 150);
    ui->searchResultWidget->horizontalHeader()->resizeSection(2, 50);
    ui->searchResultWidget->horizontalHeader()->resizeSection(3, 50);
    ui->searchResultWidget->horizontalHeader()->resizeSection(4, 70);
    ui->searchResultWidget->horizontalHeader()->resizeSection(5, 70);
    ui->searchResultWidget->horizontalHeader()->resizeSection(6, 50);
//    ui->searchResultWidget-

    ui->searchResultWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
//    ui->searchResultWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
//    ui->searchResultWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
//    ui->searchResultWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
//    ui->searchResultWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    for(int j=0;j<rows;++j){

        KeyActionWidget* pWidget = new KeyActionWidget();
        QPushButton* btn_view = new QPushButton();
//        btn_view->setText("View");
        btn_view->setIcon(QIcon(":/static/images/view.png"));
        QPushButton* btn_move = new QPushButton();
        btn_view->setCursor(Qt::PointingHandCursor);

//        btn_move->setText("Move");
        btn_move->setIcon(QIcon(":/static/images/move.png"));
        btn_move->setCursor(Qt::PointingHandCursor);

        QPushButton* btn_rename = new QPushButton();
//        btn_rename->setText("Rename");
        btn_rename->setIcon(QIcon(":/static/images/rename.png"));
        btn_rename->setCursor(Qt::PointingHandCursor);


        QPushButton* btn_delete = new QPushButton();
//        btn_delete->setText("Delete");
        btn_delete->setIcon(QIcon(":/static/images/delete.png"));
        btn_delete->setCursor(Qt::PointingHandCursor);


        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);

        pLayout->addWidget(btn_view);
        pLayout->addWidget(btn_move);
        pLayout->addWidget(btn_rename);
        pLayout->addWidget(btn_delete);

        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);

        tmpType = batchResList.takeFirst();
        tmpTtl = batchResList.takeFirst();
        tmpEncoding = batchResList.takeFirst();
        tmpRefcount = batchResList.takeFirst();
        QString tmpKey = list.value(j).toString();

        pWidget->key = tmpKey;
        pWidget->type = tmpType.toString();
        pWidget->row = j;
        connect(btn_view, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("view", pWidget->key, pWidget->type, pWidget->row);
        });
        connect(btn_move, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("move", pWidget->key, pWidget->type, pWidget->row);
        });
        connect(btn_rename, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("rename", pWidget->key, pWidget->type, pWidget->row);
        });
        connect(btn_delete, &QPushButton::clicked, [this, pWidget](){
            this->keyAction("delete", pWidget->key, pWidget->type, pWidget->row);
        });

        QTableWidgetItem *c1 = new QTableWidgetItem(list.value(j).toString());
        c1->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *c2 = new QTableWidgetItem(tmpType.toString());
        c2->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *c3 = new QTableWidgetItem(tmpTtl.toString());
        c3->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *c4 = new QTableWidgetItem(tmpEncoding.toString());
        c4->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *c5 = new QTableWidgetItem(tmpRefcount.toString());
        c5->setTextAlignment(Qt::AlignCenter);

        ui->searchResultWidget->setCellWidget(j, 0, pWidget);
        ui->searchResultWidget->setItem(j, 1, c1);
        ui->searchResultWidget->setItem(j, 2, c2);
        ui->searchResultWidget->setItem(j, 3, c3);
        ui->searchResultWidget->setItem(j, 4, c4);
        ui->searchResultWidget->setItem(j, 5, c5);

        if(tmpType.toString().compare("string") == 0){
            sizeAllCommands.addToPipeline({"STRLEN", list.value(j).toByteArray()});
        }
    }
    v2 = Redis::runSyncCommand(sizeAllCommands);
    batchResList = v2.toList();

    for(int k=0;k<batchResList.length();k++){
        QTableWidgetItem *c6 = new QTableWidgetItem(batchResList.value(k).toString());
        c6->setTextAlignment(Qt::AlignCenter);
        ui->searchResultWidget->setItem(k, 6, c6);
    }
}

void MainWindow::on_addNewServerBtn_clicked()
{
    ServerConfigDialog *scd = new ServerConfigDialog(this);
    scd->setWindowTitle("Add New Server");
    int rec = scd->exec();
    if(rec == ServerConfigDialog::Accepted){
        loadAllServersList();
//        qDebug() << "acceped";
//        qDebug() << Setting::getAllServers();
//        qDebug() << Setting::toStr(Setting::getAllServers());
    }
}

void MainWindow::on_editServerBtn_clicked()
{
    QString host = ui->serverListWidget->currentItem()->text();
    //
    ServerConfigDialog *scd = new ServerConfigDialog(this);
    scd->markDialogEdit(host);
    int rec = scd->exec();
    if(rec == ServerConfigDialog::Accepted){
        loadAllServersList();
    }
}

void MainWindow::on_deleteServerBtn_clicked()
{
    if(ui->serverListWidget->count() == 0){
        logTextBrowser("please choose host.", "error");
        return;
    }
    QString host = ui->serverListWidget->currentItem()->text();
    qDebug() << host << "host......";
    QJsonObject allServers = Setting::getAllServers();
    allServers.remove(host);
    QSettings settings;
    settings.setValue("ServerList", Setting::toStr(allServers));
    logTextBrowser("Delete host successfully.", "success");
    loadAllServersList();
}

void MainWindow::on_connectServerBtn_clicked()
{
    RedisClient::Command cmd;
    QVariant v = Redis::runSyncCommand(cmd);
    if(v.toBool()){
        logTextBrowser("Current Host can be connected successfully.", "success");
    }else{
        logTextBrowser("Current Host can not be connected, please switch anohter server.", "error");
    }
}

void MainWindow::on_clearHistoryBtn_clicked()
{
    QSettings settings;
    settings.remove("SearchHistoryList");
    logTextBrowser("Clear history successfully.", "success");
    loadAllSearchHistory();
}

void MainWindow::on_searchTipBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("If you search wildcards like \"foo*\" or use \"Show All Keys\", it possiblly block redis server. Be careful!");
    msgBox.exec();
}

void MainWindow::on_searchCommandBtn_clicked()
{
    QString input = ui->searchCommandEdit->text();
    if(input.compare("") == 0){
        logTextBrowser("Please type any words", "error");
        return;
    }
    // add this into history
    loadAllSearchHistory(input);

    search(input);


}

void MainWindow::onListMailItemClicked(QListWidgetItem *item)
{
    chooseServer(item->text());
}

void MainWindow::onHistoryItemClicked(QListWidgetItem *item)
{
    ui->searchCommandEdit->setText(item->text());
}

void MainWindow::keyAction(QString action, QString key, QString type, int row)
{
    qDebug() << action << key << type << row;

    if(action.compare("view") == 0){
        KeyEditDialog *editDialog = nullptr;
        if(type.compare("hash") == 0){
            editDialog = new KeyEditHashDialog(this);
        }else if(type.compare("list") == 0){
            editDialog = new KeyEditListDialog(this);
        }else if(type.compare("set") == 0){
            editDialog = new KeyEditSetDialog(this);
        }else if(type.compare("zset") == 0){
            editDialog = new KeyEditZsetDialog(this);
        }else{
            editDialog = new KeyEditStringDialog(this);
        }
        editDialog->setProperty(key, type);
        editDialog->init();
        editDialog->exec();
        return;
    }

    if(action.compare("delete") == 0){
        RedisClient::Command cmd;
        cmd.addToPipeline({"DEL", QByteArray::fromStdString(key.toStdString())});
        QVariant v = Redis::runSyncCommand(cmd);
        if(!v.toBool()){
            logTextBrowser("Redis server down", "error");
            return;
        }
        if(v.toList().first().toInt() == 1){
            logTextBrowser("Delete successed!", "success");
            ui->searchResultWidget->removeRow(row);
        }else{
            logTextBrowser("Delete failed", "error");
        }
        return;
    }

    if(action.compare("rename") == 0){
        KeyRenameDialog *renameDialog = new KeyRenameDialog(this);
        connect(renameDialog, SIGNAL(renameKeySignal(QString, QString)), this, SLOT(renameKey(QString, QString)));
        renameDialog->setKey(key);
        renameDialog->exec();
        return;
    }

    QMessageBox msgBox;
    msgBox.setText("Next version will support Move.");
    msgBox.exec();
}

void MainWindow::renameKey(QString orginalKey, QString newKey)
{
    QList<QTableWidgetItem *> l = ui->searchResultWidget->findItems(orginalKey, Qt::MatchExactly);
    if(l.isEmpty()){
        logTextBrowser("renamed key can not found", "error");
        return;
    }
    for(int i=0;i<l.size();i++){
        if(l.at(i)->column() == 1){
            // find the key item
            l.at(i)->setText(newKey);
            if(KeyActionWidget* v = dynamic_cast<KeyActionWidget*>(ui->searchResultWidget->cellWidget(l.at(i)->row(), 0))) {
                v->key = newKey;
            }
            break;
        }
    }
}

void MainWindow::on_addStringBtn_clicked()
{
    QString key = ui->keyField->text();
    QString value = ui->valueField->toPlainText();
    bool res = Redis::addString(key, value);
    if(!res){
        logTextBrowser("add string failed", "error");
    }else{
        logTextBrowser("add string success", "success");
    }
}

void MainWindow::on_addEditStringBtn_clicked()
{
    on_addStringBtn_clicked();
    QString key = ui->keyField->text();
    keyAction("view", key, "string", 0);
}

void MainWindow::on_addHashBtn_clicked()
{
    QString key = ui->hashKeyField->text();
    QString field = ui->hashFieldField->text();
    QString value = ui->hashValueField->toPlainText();
    if(!Redis::addHash(key, field, value)){
        logTextBrowser("add hash failed", "error");
    }else{
        logTextBrowser("add hash success", "success");
    }
}

void MainWindow::on_addEditHashBtn_clicked()
{
    on_addHashBtn_clicked();
    QString key = ui->hashKeyField->text();
    keyAction("view", key, "hash", 0);
}

void MainWindow::on_addListBtn_clicked()
{
    QString key = ui->listKeyField->text();
    QString value = ui->listValueField->toPlainText();
    if(Redis::addList(key, value)){
        logTextBrowser("add list success", "success");
    }else{
        logTextBrowser("add list failed", "error");
    }
}

void MainWindow::on_addEditListBtn_clicked()
{
    on_addListBtn_clicked();
    QString key = ui->listKeyField->text();
    keyAction("view", key, "list", 0);
}

void MainWindow::on_showAllKeysBtn_clicked()
{
    search("*");
}

void MainWindow::on_addSetBtn_clicked()
{
    QString key = ui->setKeyField->text();
    QString value = ui->setValueField->toPlainText();
    if(Redis::addSet(key, value)){
        logTextBrowser("add set success", "success");
    }else{
        logTextBrowser("add set failed", "error");
    }
}

void MainWindow::on_addEditSetBtn_clicked()
{
    on_addSetBtn_clicked();
    QString key = ui->setKeyField->text();
    keyAction("view", key, "set", 0);
}

void MainWindow::on_addZsetBtn_clicked()
{
    QString key = ui->zsetKeyField->text();
    QString score = ui->zsetScoreField->text();
    QString value = ui->zsetValueField->toPlainText();
    if(Redis::addZset(key, score, value)){
        logTextBrowser("add zset success", "success");
    }else{
        logTextBrowser("add zset failed", "error");
    }
}

void MainWindow::on_addEditZsetBtn_clicked()
{
    on_addZsetBtn_clicked();
    QString key = ui->zsetKeyField->text();
    keyAction("view", key, "zset", 0);
}

void MainWindow::openServerManager(bool checked)
{
    qDebug() << "open server mangger";
}
