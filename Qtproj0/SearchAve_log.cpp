#include "SearchAve_log.h"
#include "ui_SearchAve_log.h"
#include <QMap>

SearchAve_Log::SearchAve_Log(DataManage* dataManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchAve_Log)
    , m_dataManager(dataManager)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle(tr("查询平均数据排名"));
}

SearchAve_Log::~SearchAve_Log()
{
    delete ui;
}

void SearchAve_Log::setupUI()
{
    // 添加统计项目选项
    ui->categoryComboBox->addItem(tr("得分"), "points");
    ui->categoryComboBox->addItem(tr("三分球"), "threePoints");
    ui->categoryComboBox->addItem(tr("篮板"), "rebounds");
    ui->categoryComboBox->addItem(tr("扣篮"), "dunks");
    ui->categoryComboBox->addItem(tr("抢断"), "steals");
    
    // 设置表格列标题
    ui->resultTableWidget->setColumnCount(4);
    ui->resultTableWidget->setHorizontalHeaderLabels(
        QStringList() << tr("排名") << tr("姓名") << tr("队伍") << tr("场均数据"));
    
    // 设置表格样式
    ui->resultTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->resultTableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->resultTableWidget->setAlternatingRowColors(true);
    
    // 更新初始结果
    updateResults();
}

QString SearchAve_Log::getCategoryCode(const QString& displayName) const
{
    return ui->categoryComboBox->currentData().toString();
}

void SearchAve_Log::on_categoryComboBox_currentIndexChanged(const QString&)
{
    updateResults();
}

void SearchAve_Log::updateResults()
{
    QString category = getCategoryCode(ui->categoryComboBox->currentText());
    QVector<PlayerStatsSummary> topThree = m_dataManager->getTopThreeByAverage(category);
    
    ui->resultTableWidget->setRowCount(topThree.size());
    
    for (int i = 0; i < topThree.size(); ++i) {
        const PlayerStatsSummary& summary = topThree[i];
        
        // 排名
        ui->resultTableWidget->setItem(i, 0, 
            new QTableWidgetItem(QString::number(i + 1)));
            
        // 姓名
        ui->resultTableWidget->setItem(i, 1, 
            new QTableWidgetItem(summary.name));
            
        // 队伍
        ui->resultTableWidget->setItem(i, 2, 
            new QTableWidgetItem(summary.team));
            
        // 场均数据
        double average;
        if (category == "points")
            average = summary.getAveragePoints();
        else if (category == "threePoints")
            average = summary.getAverageThreePoints();
        else if (category == "rebounds")
            average = summary.getAverageRebounds();
        else if (category == "dunks")
            average = summary.getAverageDunks();
        else if (category == "steals")
            average = summary.getAverageSteals();
        else
            average = 0;
            
        ui->resultTableWidget->setItem(i, 3, 
            new QTableWidgetItem(QString::number(average, 'f', 2)));
    }
    
    ui->resultTableWidget->resizeColumnsToContents();
}
