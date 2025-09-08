#include <QApplication>
#include "playerchartwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 创建图表窗口
    PlayerChartWidget chartWidget("测试球员");
    
    // 设置测试数据
    std::vector<ChartData> testData = {
        {"得分", 25.8, QColor(255, 99, 132)},
        {"篮板", 8.2, QColor(54, 162, 235)},
        {"助攻", 6.5, QColor(255, 205, 86)},
        {"抢断", 2.1, QColor(75, 192, 192)},
        {"扣篮", 1.8, QColor(153, 102, 255)}
    };
    
    chartWidget.setChartData(testData);
    chartWidget.show();
    
    return app.exec();
}
