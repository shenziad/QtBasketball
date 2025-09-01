#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextEdit>
#include "addgamestat_log.h"
#include "searchtopteam_log.h"
#include "searchave_log.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dataManager(new DataManage(this))
{
    ui->setupUi(this);
    createMenus();
    setupConnections();

    // 尝试加载已有数据
    if (m_dataManager->loadGameStats(GAME_STATS_FILE)) {
        m_dataManager->loadSummaryStats(SUMMARY_STATS_FILE);
        updateDisplay();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    // 文件菜单
    QMenu* fileMenu = menuBar()->addMenu(tr("文件"));
    
    QAction* newGameAction = new QAction(tr("添加比赛数据"), this);
    QAction* saveAction = new QAction(tr("保存数据"), this);
    QAction* loadAction = new QAction(tr("加载数据"), this);
    
    fileMenu->addAction(newGameAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    
    // 查询菜单
    QMenu* queryMenu = menuBar()->addMenu(tr("查询"));
    
    QAction* summaryAction = new QAction(tr("查看汇总数据"), this);
    QAction* topThreeAction = new QAction(tr("查看平均数据排名"), this);
    QAction* teamTopThreeAction = new QAction(tr("查看队内排名"), this);
    
    queryMenu->addAction(summaryAction);
    queryMenu->addAction(topThreeAction);
    queryMenu->addAction(teamTopThreeAction);

    // 连接信号和槽
    connect(newGameAction, &QAction::triggered, this, &MainWindow::on_actionNewGame_triggered);
    connect(saveAction, &QAction::triggered, this, &MainWindow::on_actionSave_triggered);
    connect(loadAction, &QAction::triggered, this, &MainWindow::on_actionLoad_triggered);
    connect(summaryAction, &QAction::triggered, this, &MainWindow::on_actionShowSummary_triggered);
    connect(topThreeAction, &QAction::triggered, this, &MainWindow::on_actionTopThree_triggered);
    connect(teamTopThreeAction, &QAction::triggered, this, &MainWindow::on_actionTeamTopThree_triggered);
}

void MainWindow::setupConnections()
{
    // 在这里添加其他的信号槽连接
}

void MainWindow::updateDisplay()
{
    // 更新主窗口显示的数据
    QVector<PlayerStatsSummary> summaries = m_dataManager->getAllPlayersSummary();
    QString displayText;
    
    for (const PlayerStatsSummary& summary : summaries) {
        displayText += QString("球员: %1\n队伍: %2\n场次: %3\n"
                             "场均得分: %.2f\n场均三分: %.2f\n"
                             "场均篮板: %.2f\n场均扣篮: %.2f\n"
                             "场均抢断: %.2f\n\n")
                          .arg(summary.name)
                          .arg(summary.team)
                          .arg(summary.gamesPlayed)
                          .arg(summary.getAveragePoints())
                          .arg(summary.getAverageThreePoints())
                          .arg(summary.getAverageRebounds())
                          .arg(summary.getAverageDunks())
                          .arg(summary.getAverageSteals());
    }
    
    ui->textBrowser->setText(displayText);
}

void MainWindow::on_actionNewGame_triggered()
{
    AddGameStat_Log dialog(m_dataManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        updateDisplay();
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_dataManager->saveGameStats(GAME_STATS_FILE) &&
        m_dataManager->saveSummaryStats(SUMMARY_STATS_FILE)) {
        QMessageBox::information(this, tr("保存成功"),
                               tr("数据已成功保存到文件。"));
    } else {
        QMessageBox::warning(this, tr("保存失败"),
                           tr("保存数据时发生错误。"));
    }
}

void MainWindow::on_actionLoad_triggered()
{
    if (m_dataManager->loadGameStats(GAME_STATS_FILE) &&
        m_dataManager->loadSummaryStats(SUMMARY_STATS_FILE)) {
        updateDisplay();
        QMessageBox::information(this, tr("加载成功"),
                               tr("数据已成功从文件加载。"));
    } else {
        QMessageBox::warning(this, tr("加载失败"),
                           tr("加载数据时发生错误。"));
    }
}

void MainWindow::on_actionShowSummary_triggered()
{
    updateDisplay();
}

void MainWindow::on_actionTopThree_triggered()
{
    SearchAve_Log dialog(m_dataManager, this);
    dialog.exec();
}

void MainWindow::on_actionTeamTopThree_triggered()
{
    SearchTopteam_Log dialog(m_dataManager, this);
    dialog.exec();
}
