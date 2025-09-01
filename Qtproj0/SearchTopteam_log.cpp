#include "SearchTopteam_log.h"
#include "ui_SearchTopteam_log.h"

SearchTopteam_Log::SearchTopteam_Log(DataManage* dataManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchTopteam_Log)
    , m_dataManager(dataManager)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle(tr("查询队内排名"));
}

SearchTopteam_Log::~SearchTopteam_Log()
{
    delete ui;
}

void SearchTopteam_Log::setupUI()
{
    // 添加球队选项
    QStringList teams = m_dataManager->getAllTeams();
    ui->teamComboBox->addItems(teams);
    
    // 设置表格列标题
    ui->resultTableWidget->setColumnCount(5);
    ui->resultTableWidget->setHorizontalHeaderLabels(
        QStringList() << tr("排名") << tr("姓名") 
                     << tr("总得分") << tr("比赛场次")
                     << tr("场均得分"));
    
    // 设置表格样式
    ui->resultTableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->resultTableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->resultTableWidget->setAlternatingRowColors(true);
    
    if (!teams.isEmpty()) {
        updateResults();
    }
}

void SearchTopteam_Log::on_teamComboBox_currentIndexChanged(const QString&)
{
    updateResults();
}

void SearchTopteam_Log::updateResults()
{
    QString team = ui->teamComboBox->currentText();
    QVector<PlayerStatsSummary> topThree = m_dataManager->getTopThreeInTeam(team);
    
    ui->resultTableWidget->setRowCount(topThree.size());
    
    for (int i = 0; i < topThree.size(); ++i) {
        const PlayerStatsSummary& summary = topThree[i];
        
        // 排名
        ui->resultTableWidget->setItem(i, 0, 
            new QTableWidgetItem(QString::number(i + 1)));
            
        // 姓名
        ui->resultTableWidget->setItem(i, 1, 
            new QTableWidgetItem(summary.name));
            
        // 总得分
        ui->resultTableWidget->setItem(i, 2, 
            new QTableWidgetItem(QString::number(summary.totalPoints)));
            
        // 比赛场次
        ui->resultTableWidget->setItem(i, 3, 
            new QTableWidgetItem(QString::number(summary.gamesPlayed)));
            
        // 场均得分
        ui->resultTableWidget->setItem(i, 4, 
            new QTableWidgetItem(QString::number(summary.getAveragePoints(), 'f', 2)));
    }
    
    ui->resultTableWidget->resizeColumnsToContents();
}
