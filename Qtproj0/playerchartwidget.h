#ifndef PLAYERCHARTWIDGET_H
#define PLAYERCHARTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <vector>
#include <QString>

// 简单的图表数据结构
struct ChartData {
    QString label;
    double value;
    QColor color;
};

// 自定义图表类型枚举
enum class ChartType {
    Pie,        // 饼图
    Radar       // 雷达图
};

class PlayerChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerChartWidget(const QString& playerName, QWidget* parent = nullptr);
    
    // 设置图表数据
    void setChartData(const std::vector<ChartData>& data);
    void setChartType(ChartType type);
    
    // 更新球员统计数据
    void updatePlayerStats(const QString& playerName);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onChartTypeChanged(int index);
    void exportChart();

private:
    void setupUI();
    void drawPieChart(QPainter& painter);
    void drawRadarChart(QPainter& painter);
    void drawLegend(QPainter& painter, int x, int y);
    
    QString m_playerName;
    std::vector<ChartData> m_data;
    ChartType m_chartType;
    
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_controlsLayout;
    QComboBox* m_chartTypeCombo;
    QPushButton* m_exportButton;
};

#endif // PLAYERCHARTWIDGET_H
