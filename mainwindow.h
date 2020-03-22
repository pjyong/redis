#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serverconfigdialog.h"
#include "keyrenamedialog.h"
#include <QDebug>
#include "setting.h"
#include <QJsonObject>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QSignalMapper>

#include <QList>
#include <qredisclient/redisclient.h>
#include "redis.h"
#include "keyactionwidget.h"

#include "keyeditdialog.h"
#include "keyeditstringdialog.h"
#include "keyedithashdialog.h"
#include "keyeditlistdialog.h"
#include "keyeditsetdialog.h"
#include "keyeditzsetdialog.h"
#include <QFile>
#include <QMessageBox>
#include <QMenu>
#include <QStandardItemModel>

#include "logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Logger *loggerManager = nullptr;
    QStandardItemModel *serverModelWidget = nullptr;
private:
    Ui::MainWindow *ui;
    void loadAllServersList();
    void loadAllSearchHistory(QString key = "");
    void chooseServer(QString);
    // run redis command
    void search(QString input);

public slots:
    void on_addNewServerBtn_clicked();
    void on_editServerBtn_clicked();
    void on_deleteServerBtn_clicked();
    void on_connectServerBtn_clicked();
    void on_clearHistoryBtn_clicked();
    void on_searchCommandBtn_clicked();
    void onListMailItemClicked(QListWidgetItem*);
    void onHistoryItemClicked(QListWidgetItem*);
    void on_showAllKeysBtn_clicked();
    void on_searchTipBtn_clicked();
    void keyAction(QString, QString, QString, int);
    // accept rename key sigal
    void renameKey(QString, QString);
    void logTextBrowser(QString, QString);
    void clearLog();
    void showLogContextMenu(const QPoint &);

    // add btns
    void on_addStringBtn_clicked();
    void on_addEditStringBtn_clicked();

    void on_addHashBtn_clicked();
    void on_addEditHashBtn_clicked();

    void on_addListBtn_clicked();
    void on_addEditListBtn_clicked();

    void on_addSetBtn_clicked();
    void on_addEditSetBtn_clicked();

    void on_addZsetBtn_clicked();
    void on_addEditZsetBtn_clicked();

    void openServerManager(bool);
};

#endif // MAINWINDOW_H
