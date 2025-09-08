#ifndef SMARTSEARCH_H
#define SMARTSEARCH_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSlider>
#include <QLabel>
#include <vector>
#include <QString>

// 搜索条件结构
struct SearchCriteria {
    QString playerNamePattern;     // 球员名称模式
    QString teamNamePattern;       // 球队名称模式
    QDate startDate;              // 开始日期
    QDate endDate;                // 结束日期
    
    // 数值范围条件
    int minPoints = -1;
    int maxPoints = -1;
    int minRebounds = -1;
    int maxRebounds = -1;
    int minAssists = -1;
    int maxAssists = -1;
    
    // 高级筛选
    bool onlyHighPerformance = false;  // 仅显示高表现比赛
    bool onlyRecentGames = false;      // 仅显示最近比赛
    QString sortBy = "date";           // 排序方式
    bool ascending = false;            // 排序方向
};

class SmartSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmartSearchWidget(QWidget* parent = nullptr);
    
    // 获取搜索结果
    std::vector<QString> getSearchResults() const;
    
    // 重置搜索条件
    void resetSearch();

signals:
    void searchRequested(const SearchCriteria& criteria);
    void searchResultsChanged(int resultCount);

private slots:
    void onSearchClicked();
    void onResetClicked();
    void onAdvancedToggled(bool enabled);
    void onRealTimeSearchToggled(bool enabled);
    void onCriteriaChanged();

private:
    void setupUI();
    void setupBasicSearch(QVBoxLayout* layout);
    void setupAdvancedSearch(QVBoxLayout* layout);
    void setupRangeSlider(const QString& label, QSlider*& minSlider, 
                         QSlider*& maxSlider, QLabel*& valueLabel, 
                         QFormLayout* layout, int min = 0, int max = 100);
    
    SearchCriteria getCurrentCriteria();
    void performSearch();

private:
    // 基础搜索控件
    QLineEdit* m_playerNameEdit;
    QLineEdit* m_teamNameEdit;
    QDateEdit* m_startDateEdit;
    QDateEdit* m_endDateEdit;
    
    // 高级搜索控件
    QGroupBox* m_advancedGroup;
    QCheckBox* m_advancedCheckBox;
    
    // 数值范围滑块
    QSlider* m_minPointsSlider;
    QSlider* m_maxPointsSlider;
    QLabel* m_pointsRangeLabel;
    
    QSlider* m_minReboundsSlider;
    QSlider* m_maxReboundsSlider;
    QLabel* m_reboundsRangeLabel;
    
    QSlider* m_minAssistsSlider;
    QSlider* m_maxAssistsSlider;
    QLabel* m_assistsRangeLabel;
    
    // 特殊筛选
    QCheckBox* m_highPerformanceCheck;
    QCheckBox* m_recentGamesCheck;
    QComboBox* m_sortByCombo;
    QCheckBox* m_ascendingCheck;
    
    // 实时搜索
    QCheckBox* m_realTimeSearchCheck;
    
    // 操作按钮
    QPushButton* m_searchButton;
    QPushButton* m_resetButton;
    QPushButton* m_saveSearchButton;
    QPushButton* m_loadSearchButton;
    
    // 结果统计
    QLabel* m_resultsCountLabel;
    
    bool m_realTimeSearchEnabled = false;
};

#endif // SMARTSEARCH_H
