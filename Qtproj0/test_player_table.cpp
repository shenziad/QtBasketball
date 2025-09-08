#include "playerdatatable.h"
#include <QApplication>

int main()
{
    QApplication app(0, nullptr);
    
    try {
        PlayerDataTable table("测试球员");
        table.show();
        return 0;
    } catch (const std::exception& e) {
        return -1;
    }
}
