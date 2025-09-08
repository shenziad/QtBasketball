#include <QApplication>
#include <QWidget>
#include "playerdatatable.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 创建简化的球员数据表
    PlayerDataTable table("测试球员");
    
    // 添加一些测试数据
    table.addGameRecord(QDate::currentDate(), "测试队", 25, 5, 8, 2, 3, 1);
    table.addGameRecord(QDate::currentDate().addDays(-1), "客队", 30, 3, 6, 1, 4, 2);
    
    table.show();
    
    return app.exec();
}
