#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamanage.h"
#include "addgamestat_log.h"
#include "searchtopteam_log.h"
#include "searchave_log.h"

// 静态成员变量定义
const QString MainWindow::GAME_STATS_FILE = QStringLiteral("game_stats.dat");
const QString MainWindow::SUMMARY_STATS_FILE = QStringLiteral("summary_stats.dat");

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextEdit>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QStatusBar>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_dataManager(new DataManage(this))
{
    ui->setupUi(this);
    setupConnections();
    initializeTables();

    // 创建数据目录
    QDir dataDir(QDir::current());
    if (!dataDir.exists("data")) {
        dataDir.mkdir("data");
    }
    
    // 构建文件路径
    QString gamePath = dataDir.filePath("data/" + GAME_STATS_FILE);
    QString summaryPath = dataDir.filePath("data/" + SUMMARY_STATS_FILE);

    // 尝试加载已有数据
    bool isFirstRun = !QFile::exists(gamePath);
    if (isFirstRun) {
        // 第一次运行，创建存储系统
        QMessageBox::information(this, tr("欢迎"),
            tr("欢迎使用篮球联赛个人技术数据处理系统！\n"
               "这是系统第一次运行，将为您创建数据存储系统。"));
        // 创建并保存空的数据文件
        m_dataManager->saveGameStats(gamePath);
        m_dataManager->saveSummaryStats(summaryPath);
    } else {
        // 加载已有数据
        if (m_dataManager->loadGameStats(gamePath)) {
            m_dataManager->loadSummaryStats(summaryPath);
            updateDisplay();
        }
    }
    
    // 设置窗口标题和大小
    setWindowTitle(tr("篮球联赛个人技术数据处理系统"));
    resize(1000, 700);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTableWidgetItem* MainWindow::createReadOnlyItem(const QString& text)
{
    QTableWidgetItem* item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);  // 设置为只读
    return item;
}

void MainWindow::setupTableWidget(QTableWidget* table)
{
    table->setEditTriggers(QTableWidget::NoEditTriggers);
    table->setSelectionBehavior(QTableWidget::SelectRows);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
    table->setSortingEnabled(true);  // 允许点击表头排序
}

void MainWindow::onDataChanged()
{
    updateDisplay();
    // 自动保存数据
    m_dataManager->saveGameStats(GAME_STATS_FILE);
    m_dataManager->saveSummaryStats(SUMMARY_STATS_FILE);
}

void MainWindow::setupConnections()
{
    // 连接新比赛按钮
    connect(ui->newGameButton, &QPushButton::clicked,
            this, &MainWindow::on_actionNewGame_triggered);
            
    // 连接菜单动作
    connect(ui->actionSave, &QAction::triggered,
            this, &MainWindow::on_actionSave_triggered);
    connect(ui->actionLoad, &QAction::triggered,
            this, &MainWindow::on_actionLoad_triggered);
    connect(ui->actionShowSummary, &QAction::triggered,
            this, &MainWindow::on_actionShowSummary_triggered);
    connect(ui->actionTopThree, &QAction::triggered,
            this, &MainWindow::on_actionTopThree_triggered);
    connect(ui->actionTeamTopThree, &QAction::triggered,
            this, &MainWindow::on_actionTeamTopThree_triggered);
            
    // 连接数据管理器的信号
    connect(m_dataManager, &DataManage::dataChanged,
            this, &MainWindow::onDataChanged);
            
    // 设置表格双击事件
    connect(ui->gamesTableWidget, &QTableWidget::itemDoubleClicked,
            [this](QTableWidgetItem* item) {
                // 显示详细信息
                int row = item->row();
                QString info = QString("比赛详细信息：\n\n"
                                    "日期：%1\n"
                                    "球员：%2\n"
                                    "队伍：%3\n"
                                    "得分：%4\n"
                                    "三分：%5\n"
                                    "篮板：%6\n"
                                    "扣篮：%7\n"
                                    "抢断：%8")
                    .arg(ui->gamesTableWidget->item(row, 0)->text())
                    .arg(ui->gamesTableWidget->item(row, 1)->text())
                    .arg(ui->gamesTableWidget->item(row, 2)->text())
                    .arg(ui->gamesTableWidget->item(row, 3)->text())
                    .arg(ui->gamesTableWidget->item(row, 4)->text())
                    .arg(ui->gamesTableWidget->item(row, 5)->text())
                    .arg(ui->gamesTableWidget->item(row, 6)->text())
                    .arg(ui->gamesTableWidget->item(row, 7)->text());
                    
                QMessageBox::information(this, tr("比赛信息"), info);
            });
}

void MainWindow::initializeTables()
{
    // 设置汇总表格的列
    ui->summaryTableWidget->setColumnCount(9);
    ui->summaryTableWidget->setHorizontalHeaderLabels(QStringList()
        << tr("姓名") << tr("队伍") << tr("场次")
        << tr("总得分") << tr("总三分") << tr("总篮板")
        << tr("总扣篮") << tr("总抢断") << tr("场均得分"));
        
    // 设置比赛记录表格的列
    ui->gamesTableWidget->setColumnCount(9);
    ui->gamesTableWidget->setHorizontalHeaderLabels(QStringList()
        << tr("日期") << tr("姓名") << tr("队伍")
        << tr("得分") << tr("三分") << tr("篮板")
        << tr("扣篮") << tr("抢断") << tr("比赛ID"));
        
    // 设置表格属性
    setupTableWidget(ui->summaryTableWidget);
    setupTableWidget(ui->gamesTableWidget);
    
    // 设置列宽
    for (QTableWidget* table : {ui->summaryTableWidget, ui->gamesTableWidget}) {
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);  // 姓名/日期列
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);  // 队伍/姓名列
        
        // 数据列固定宽度
        for (int i = 2; i < table->columnCount() - 1; ++i) {
            table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
            table->setColumnWidth(i, 60);
        }
        
        // 最后一列自适应
        table->horizontalHeader()->setSectionResizeMode(
            table->columnCount() - 1, QHeaderView::Stretch);
    }
    
    // 设置提示信息
    ui->summaryTableWidget->setToolTip(tr("双击行查看球员详细统计"));
    ui->gamesTableWidget->setToolTip(tr("双击行查看比赛详细信息"));
}

void MainWindow::updateDisplay()
{
    // 更新汇总数据表格
    QVector<PlayerStatsSummary> summaries = m_dataManager->getAllPlayersSummary();
    ui->summaryTableWidget->setRowCount(summaries.size());
    
    for (int i = 0; i < summaries.size(); ++i) {
        const PlayerStatsSummary& summary = summaries[i];
        int col = 0;
        
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(summary.name));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(summary.team));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(summary.gamesPlayed)));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(summary.totalPoints)));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(summary.totalThreePoints)));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(summary.totalRebounds)));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(summary.totalDunks)));
        ui->summaryTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(summary.totalSteals)));
        
        // 设置场均得分，右对齐显示
        QTableWidgetItem* avgItem = createReadOnlyItem(QString::number(summary.getAveragePoints(), 'f', 2));
        avgItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->summaryTableWidget->setItem(i, col++, avgItem);
    }
    ui->summaryTableWidget->resizeColumnsToContents();
    
    // 更新比赛记录表格
    QVector<PlayerStats> games = m_dataManager->getAllGames();
    ui->gamesTableWidget->setRowCount(games.size());
    
    for (int i = 0; i < games.size(); ++i) {
        const PlayerStats& game = games[i];
        int col = 0;
        
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(game.getDate().toString("yyyy-MM-dd")));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(game.getName()));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(game.getTeam()));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(game.getPoints())));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(game.getThreePoints())));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(game.getRebounds())));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(game.getDunks())));
        ui->gamesTableWidget->setItem(i, col++, createReadOnlyItem(QString::number(game.getSteals())));
        
        // 设置比赛ID，右对齐显示
        QTableWidgetItem* idItem = createReadOnlyItem(QString::number(i + 1));
        idItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->gamesTableWidget->setItem(i, col++, idItem);
    }
    ui->gamesTableWidget->resizeColumnsToContents();
    
    // 更新状态栏显示
    statusBar()->showMessage(tr("共有 %1 场比赛记录，%2 名球员")
        .arg(games.size())
        .arg(summaries.size()));
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
