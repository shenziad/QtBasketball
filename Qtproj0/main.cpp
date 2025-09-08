#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

static void applyStyleSheet(QApplication &app, const QString &path) {
    // 尝试多个可能的路径
    QStringList possiblePaths;
    possiblePaths << path;  // 相对于当前目录
    possiblePaths << "../" + path;  // 相对于上级目录（从build目录）
    possiblePaths << "../../" + path;  // 更上级目录
    possiblePaths << QCoreApplication::applicationDirPath() + "/" + path;  // 相对于可执行文件目录
    possiblePaths << QCoreApplication::applicationDirPath() + "/../" + path;  // 可执行文件上级目录
    
    for (const QString& tryPath : possiblePaths) {
        QFile f(tryPath);
        if (f.exists()) {
            qDebug() << "找到样式表文件:" << tryPath;
            if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream ts(&f);
                QString styleSheet = ts.readAll();
                app.setStyleSheet(styleSheet);
                qDebug() << "成功加载样式表:" << tryPath;
                qDebug() << "样式表长度:" << styleSheet.length() << "字符";
                return;  // 成功加载后退出
            }
        }
    }
    
    qDebug() << "无法找到样式表文件:" << path;
    qDebug() << "当前工作目录:" << QDir::currentPath();
    qDebug() << "应用程序目录:" << QCoreApplication::applicationDirPath();
    qDebug() << "尝试的路径:";
    for (const QString& tryPath : possiblePaths) {
        qDebug() << "  -" << tryPath << "存在:" << QFile::exists(tryPath);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("Fusion");
    
    qDebug() << "应用程序启动";
    qDebug() << "当前工作目录:" << QDir::currentPath();
    qDebug() << "应用程序目录:" << QCoreApplication::applicationDirPath();
    
    // 首先尝试加载清新蓝色主题
    /*applyStyleSheet(a, "styles/assets/fresh_blue.qss");
    
    if (a.styleSheet().isEmpty()) {
        qDebug() << "蓝色主题未加载，尝试优雅版本...";
        applyStyleSheet(a, "styles/assets/basketball_elegant.qss");
    }
    
    if (a.styleSheet().isEmpty()) {
        qDebug() << "优雅版本未加载，尝试简化版本...";
        applyStyleSheet(a, "styles/assets/basketball_simple.qss");
    }
    
    if (a.styleSheet().isEmpty()) {
        qDebug() << "简化版本未加载，尝试原版本...";
        applyStyleSheet(a, "styles/assets/basketball_style.qss");
    }
    
    if (a.styleSheet().isEmpty()) {
        qDebug() << "所有样式表都未加载成功，使用默认样式";
    }*/
    
    MainWindow w;
    w.show();
    return a.exec();
}
