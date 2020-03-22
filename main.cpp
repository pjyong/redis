#include "mainwindow.h"
#include <QApplication>
#include <QCoreApplication>

void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    abort();
    qDebug() << "######";
//    QByteArray localMsg = msg.toLocal8Bit();
//    const char *file = context.file ? context.file : "";
//    const char *function = context.function ? context.function : "";
//    switch (type) {
//    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtInfoMsg:
//        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
//        break;
//    }
}

int main(int argc, char *argv[])
{
//    qInstallMessageHandler(logMessageOutput);
    // 配置
    QCoreApplication::setOrganizationName("RedisClient");
    QCoreApplication::setApplicationName("Suyou");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QFile file(":/static/stylesheet/ui.qss");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(file.readAll());
    file.close();
    return a.exec();
}
