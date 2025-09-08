#include "playerchartwidget.h"
#include "datamanage.h"
#include <QPainter>
#include <QPaintEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QtMath>
#include <QDebug>

PlayerChartWidget::PlayerChartWidget(const QString& playerName, QWidget* parent)
    : QWidget(parent)
    , m_playerName(playerName)
    , m_chartType(ChartType::Pie)
{
    setWindowTitle(tr("%1 - 数据图表分析").arg(playerName));
    resize(800, 600);
    setupUI();
    updatePlayerStats(playerName);
}

void PlayerChartWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 控制面板
    m_controlsLayout = new QHBoxLayout;
    
    // 图表类型选择
    QLabel* typeLabel = new QLabel(tr("图表类型:"));
    m_chartTypeCombo = new QComboBox;
    m_chartTypeCombo->addItem("饼图", static_cast<int>(ChartType::Pie));
    m_chartTypeCombo->addItem("雷达图", static_cast<int>(ChartType::Radar));
    
    connect(m_chartTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PlayerChartWidget::onChartTypeChanged);
    
    // 导出按钮
    m_exportButton = new QPushButton(tr("导出图表"));
    m_exportButton->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; border: none; "
        "border-radius: 6px; padding: 8px 16px; font-weight: bold; } "
        "QPushButton:hover { background-color: #2980b9; }"
    );
    connect(m_exportButton, &QPushButton::clicked, this, &PlayerChartWidget::exportChart);
    
    m_controlsLayout->addWidget(typeLabel);
    m_controlsLayout->addWidget(m_chartTypeCombo);
    m_controlsLayout->addStretch();
    m_controlsLayout->addWidget(m_exportButton);
    
    m_mainLayout->addLayout(m_controlsLayout);
    
    // 图表绘制区域（留空，在paintEvent中绘制）
    setMinimumHeight(500);
}

void PlayerChartWidget::updatePlayerStats(const QString& playerName)
{
    // 这里需要从DataManage获取球员数据
    // 为了演示，我使用模拟数据
    m_data.clear();
    
    // 模拟球员各项数据（实际应该从数据管理器获取）
    m_data.push_back({"得分", 25.5, QColor(255, 99, 132)});      // 红色
    m_data.push_back({"篮板", 8.2, QColor(54, 162, 235)});       // 蓝色
    m_data.push_back({"助攻", 6.8, QColor(255, 205, 86)});       // 黄色
    m_data.push_back({"抢断", 2.1, QColor(75, 192, 192)});       // 青色
    m_data.push_back({"扣篮", 1.5, QColor(153, 102, 255)});      // 紫色
    
    update(); // 触发重绘
}

void PlayerChartWidget::setChartData(const std::vector<ChartData>& data)
{
    m_data = data;
    update();
}

void PlayerChartWidget::setChartType(ChartType type)
{
    m_chartType = type;
    update();
}

void PlayerChartWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 计算绘制区域（排除控件区域）
    QRect drawRect = rect();
    drawRect.setTop(60); // 为控件留出空间，移除了标题空间
    drawRect.setBottom(drawRect.bottom() - 20); // 底部边距
    
    if (m_data.empty()) {
        painter.setPen(QPen(Qt::gray, 2));
        painter.drawText(drawRect, Qt::AlignCenter, tr("暂无数据"));
        return;
    }
    
    switch (m_chartType) {
        case ChartType::Pie:
            drawPieChart(painter);
            break;
        case ChartType::Radar:
            drawRadarChart(painter);
            break;
        default:
            break;
    }
}

void PlayerChartWidget::drawPieChart(QPainter& painter)
{
    // 计算总值
    double total = 0;
    for (const auto& data : m_data) {
        total += data.value;
    }
    
    if (total <= 0) return;
    
    // 饼图绘制区域
    int centerX = width() / 2;
    int centerY = (height() + 60) / 2; // 调整中心位置，移除了标题空间
    int radius = qMin(width(), height() - 80) / 3; // 饼图半径
    
    QRect pieRect(centerX - radius, centerY - radius, radius * 2, radius * 2);
    
    // 绘制饼图扇形
    double startAngle = 0;
    for (size_t i = 0; i < m_data.size(); ++i) {
        const auto& data = m_data[i];
        double percentage = data.value / total;
        double sweepAngle = percentage * 360.0;
        
        // 绘制扇形
        painter.setBrush(QBrush(data.color));
        painter.setPen(QPen(Qt::white, 2));
        painter.drawPie(pieRect, static_cast<int>(startAngle * 16), 
                       static_cast<int>(sweepAngle * 16));
        
        startAngle += sweepAngle;
    }
    
    // 绘制图例
    drawLegend(painter, width() - 200, 80);
}

void PlayerChartWidget::drawRadarChart(QPainter& painter)
{
    int centerX = width() / 2;
    int centerY = (height() + 60) / 2; // 调整中心位置
    int radius = qMin(width(), height() - 80) / 3;
    
    // 找到数据最大值用于标准化
    double maxValue = 0;
    for (const auto& data : m_data) {
        maxValue = qMax(maxValue, data.value);
    }
    
    if (maxValue <= 0) return;
    
    int sides = static_cast<int>(m_data.size());
    double angleStep = 2 * M_PI / sides;
    
    // 绘制网格线（同心圆和射线）
    painter.setPen(QPen(Qt::lightGray, 1));
    
    // 绘制同心圆
    for (int i = 1; i <= 5; ++i) {
        int gridRadius = radius * i / 5;
        painter.drawEllipse(centerX - gridRadius, centerY - gridRadius, 
                          gridRadius * 2, gridRadius * 2);
    }
    
    // 绘制射线
    for (int i = 0; i < sides; ++i) {
        double angle = i * angleStep - M_PI / 2; // 从顶部开始
        int endX = centerX + static_cast<int>(radius * cos(angle));
        int endY = centerY + static_cast<int>(radius * sin(angle));
        painter.drawLine(centerX, centerY, endX, endY);
        
        // 绘制标签
        int labelX = centerX + static_cast<int>((radius + 30) * cos(angle));
        int labelY = centerY + static_cast<int>((radius + 30) * sin(angle));
        painter.setPen(QPen(Qt::black, 1));
        painter.setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
        painter.drawText(QRect(labelX - 30, labelY - 10, 60, 20), 
                        Qt::AlignCenter, m_data[i].label);
        painter.setPen(QPen(Qt::lightGray, 1));
    }
    
    // 绘制数据多边形
    QPolygonF dataPolygon;
    for (int i = 0; i < sides; ++i) {
        double angle = i * angleStep - M_PI / 2;
        double normalizedValue = m_data[i].value / maxValue;
        int pointRadius = static_cast<int>(radius * normalizedValue);
        int pointX = centerX + static_cast<int>(pointRadius * cos(angle));
        int pointY = centerY + static_cast<int>(pointRadius * sin(angle));
        dataPolygon << QPointF(pointX, pointY);
    }
    
    // 填充多边形
    painter.setBrush(QBrush(QColor(54, 162, 235, 100))); // 半透明蓝色
    painter.setPen(QPen(QColor(54, 162, 235), 3));
    painter.drawPolygon(dataPolygon);
    
    // 绘制数据点
    painter.setBrush(QBrush(QColor(255, 99, 132))); // 红色数据点
    painter.setPen(QPen(Qt::white, 2));
    for (const QPointF& point : dataPolygon) {
        painter.drawEllipse(point, 6, 6);
    }
    
    // 绘制数值标签
    painter.setPen(QPen(Qt::black, 1));
    painter.setFont(QFont("Microsoft YaHei", 9));
    for (int i = 0; i < sides; ++i) {
        double angle = i * angleStep - M_PI / 2;
        double normalizedValue = m_data[i].value / maxValue;
        int pointRadius = static_cast<int>(radius * normalizedValue);
        int labelX = centerX + static_cast<int>((pointRadius + 15) * cos(angle));
        int labelY = centerY + static_cast<int>((pointRadius + 15) * sin(angle));
        painter.drawText(QRect(labelX - 20, labelY - 10, 40, 20), 
                        Qt::AlignCenter, QString::number(m_data[i].value, 'f', 1));
    }
}

void PlayerChartWidget::drawLegend(QPainter& painter, int x, int y)
{
    painter.setFont(QFont("Microsoft YaHei", 10));
    
    for (size_t i = 0; i < m_data.size(); ++i) {
        const auto& data = m_data[i];
        int itemY = y + static_cast<int>(i * 25);
        
        // 绘制颜色块
        painter.setBrush(QBrush(data.color));
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(x, itemY, 15, 15);
        
        // 绘制文字
        painter.setPen(QPen(Qt::black, 1));
        QString legendText = QString("%1: %2").arg(data.label).arg(data.value, 0, 'f', 1);
        painter.drawText(x + 20, itemY + 12, legendText);
    }
}

void PlayerChartWidget::mousePressEvent(QMouseEvent* event)
{
    // 可以添加交互功能，比如点击显示详细信息
    Q_UNUSED(event)
    QWidget::mousePressEvent(event);
}

void PlayerChartWidget::onChartTypeChanged(int index)
{
    m_chartType = static_cast<ChartType>(m_chartTypeCombo->itemData(index).toInt());
    update();
}

void PlayerChartWidget::exportChart()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("导出图表"),
        QString("%1_chart.png").arg(m_playerName),
        tr("PNG图片 (*.png);;JPG图片 (*.jpg);;所有文件 (*)"));
        
    if (!fileName.isEmpty()) {
        QPixmap pixmap(size());
        render(&pixmap);
        
        if (pixmap.save(fileName)) {
            QMessageBox::information(this, tr("导出成功"),
                tr("图表已成功导出到: %1").arg(fileName));
        } else {
            QMessageBox::warning(this, tr("导出失败"),
                tr("无法保存图表到指定位置"));
        }
    }
}
