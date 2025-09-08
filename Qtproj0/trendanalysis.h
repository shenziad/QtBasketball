#ifndef TRENDANALYSIS_H
#define TRENDANALYSIS_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QTableWidget>
#include <vector>
#include <QString>
#include <QDate>

// 趋势数据点
struct TrendPoint {
    QDate date;
    double value;
    QString gameInfo;
};

// 趋势类型
enum class TrendType {
    Points,         // 得分趋势
    Rebounds,       // 篮板趋势
    Assists,        // 助攻趋势
    Efficiency,     // 效率趋势
    Performance     // 综合表现趋势
};

class TrendAnalysisWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TrendAnalysisWidget(const QString& playerName, QWidget* parent = nullptr);
    
    // 设置分析时间范围
    void setDateRange(const QDate& startDate, const QDate& endDate);
    
    // 更新趋势数据
    void updateTrendData();

private slots:
    void onTrendTypeChanged(int index);
    void onDateRangeChanged();
    void exportTrendReport();

private:
    void setupUI();
    void calculateTrend();
    void displayTrendAnalysis();
    std::vector<TrendPoint> getTrendData(TrendType type);
    
    // 趋势分析算法
    double calculateSlope(const std::vector<TrendPoint>& data);
    QString getTrendDescription(double slope, TrendType type);
    
private:
    QString m_playerName;
    TrendType m_currentTrendType;
    QDate m_startDate;
    QDate m_endDate;
    
    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_controlsLayout;
    
    QComboBox* m_trendTypeCombo;
    QDateEdit* m_startDateEdit;
    QDateEdit* m_endDateEdit;
    QPushButton* m_analyzeButton;
    QPushButton* m_exportButton;
    
    QLabel* m_trendSummaryLabel;
    QTableWidget* m_trendTable;
    QLabel* m_recommendationLabel;
    
    std::vector<TrendPoint> m_currentData;
};

#endif // TRENDANALYSIS_H
