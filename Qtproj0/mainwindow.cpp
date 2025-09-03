#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datamanage.h"
#include "addgamestat_log.h"
#include "searchtopteam_log.h"
#include "searchave_log.h"
#include "playerdatatable.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_dataManager(new DataManage(this))  // 数据管理器应该在 UI 之前初始化
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializePresetData();  // 初始化预设数据
    setupConnections();
    initializeTables();

    // 创建数据目录
    QDir dataDir = QDir::current();
    if (!dataDir.exists("data")) {
        if (!dataDir.mkdir("data")) {
            QMessageBox::warning(this, tr("错误"),
                tr("无法创建数据目录，请检查权限。"));
            return;
        }
    }
    dataDir.cd("data");  // 进入数据目录
    
    // 构建文件路径
    QString gamePath = dataDir.filePath(GAME_STATS_FILE);
    QString summaryPath = dataDir.filePath(SUMMARY_STATS_FILE);

    // 尝试加载已有数据
    bool isFirstRun = !QFile::exists(gamePath);
    if (isFirstRun) {
        // 第一次运行，创建存储系统
        QMessageBox::information(this, tr("欢迎"),
            tr("欢迎使用篮球联赛个人技术数据处理系统！\n"
               "这是系统第一次运行，将为您创建数据存储系统。"));
        // 创建并保存空的数据文件
        if (!m_dataManager->saveGameStats(gamePath) ||
            !m_dataManager->saveSummaryStats(summaryPath)) {
            QMessageBox::warning(this, tr("错误"),
                tr("无法创建数据文件，请检查磁盘空间和权限。"));
        }
    } else {
        // 加载已有数据
        if (!m_dataManager->loadGameStats(gamePath) ||
            !m_dataManager->loadSummaryStats(summaryPath)) {
            QMessageBox::warning(this, tr("错误"),
                tr("无法加载数据文件，文件可能已损坏。"));
        } else {
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
    QString gamePath = QDir(QDir::current()).filePath("data/" + GAME_STATS_FILE);
    QString summaryPath = QDir(QDir::current()).filePath("data/" + SUMMARY_STATS_FILE);
    
    m_dataManager->saveGameStats(gamePath);
    m_dataManager->saveSummaryStats(summaryPath);
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
            
    // 设置汇总表格双击事件 - 显示球员详细统计
    connect(ui->summaryTableWidget, &QTableWidget::itemDoubleClicked,
            [this](QTableWidgetItem* item) {
                int row = item->row();
                QString playerName = ui->summaryTableWidget->item(row, 0)->text();
                showPlayerDetailedStats(playerName);
            });
            
    // 设置比赛记录表格双击事件
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
    ui->summaryTableWidget->setColumnCount(10);  // 增加一列用于查看按钮
    ui->summaryTableWidget->setHorizontalHeaderLabels(QStringList()
        << tr("姓名") << tr("队伍") << tr("场次")
        << tr("总得分") << tr("总三分") << tr("总篮板")
        << tr("总扣篮") << tr("总抢断") << tr("场均得分") << tr("操作"));
        
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
        // 先设置所有列为Fixed，防止自动调整
        for (int i = 0; i < table->columnCount(); ++i) {
            table->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        }
        
        // 姓名/日期列和队伍/姓名列自适应内容
        table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
        
        // 数据列固定宽度
        for (int i = 2; i < table->columnCount() - 1; ++i) {
            table->setColumnWidth(i, 70);  // 稍微加宽，确保数字显示完整
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

        // 添加查看详情按钮
        QPushButton* detailsButton = new QPushButton(tr("查看详情"));
        detailsButton->setProperty("playerName", summary.name);
        connect(detailsButton, &QPushButton::clicked, this, [this, name = summary.name]() {
            showPlayerDetailedStats(name);
        });
        ui->summaryTableWidget->setCellWidget(i, col++, detailsButton);
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
    QString gamePath = QDir(QDir::current()).filePath("data/" + GAME_STATS_FILE);
    QString summaryPath = QDir(QDir::current()).filePath("data/" + SUMMARY_STATS_FILE);
    
    if (m_dataManager->saveGameStats(gamePath) &&
        m_dataManager->saveSummaryStats(summaryPath)) {
        QMessageBox::information(this, tr("保存成功"),
                               tr("数据已成功保存到文件。"));
    } else {
        QMessageBox::warning(this, tr("保存失败"),
                           tr("保存数据时发生错误。"));
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString gamePath = QDir(QDir::current()).filePath("data/" + GAME_STATS_FILE);
    QString summaryPath = QDir(QDir::current()).filePath("data/" + SUMMARY_STATS_FILE);
    
    if (m_dataManager->loadGameStats(gamePath) &&
        m_dataManager->loadSummaryStats(summaryPath)) {
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

void MainWindow::initializePresetData()
{
    // 预设的球队
    QStringList teams = {
        "勇士队", "湖人队", "凯尔特人", "独行侠", "火箭队"
    };
    
    // 预设的球员及其所属球队
    m_presetPlayers = {
        {"斯蒂芬·库里", "勇士队"},
        {"克莱·汤普森", "勇士队"},
        {"德雷蒙德·格林", "勇士队"},
        {"安德鲁·威金斯", "勇士队"},
        
        {"勒布朗·詹姆斯", "湖人队"},
        {"安东尼·戴维斯", "湖人队"},
        {"拉塞尔·威斯布鲁克", "湖人队"},
        {"奥斯汀·里夫斯", "湖人队"},
        
        {"杰森·塔图姆", "凯尔特人"},
        {"杰伦·布朗", "凯尔特人"},
        {"克里斯托弗·波尔津吉斯", "凯尔特人"},
        {"德里克·怀特", "凯尔特人"},
        
        {"卢卡·东契奇", "独行侠"},
        {"凯里·欧文", "独行侠"},
        {"格兰特·威廉姆斯", "独行侠"},
        {"蒂姆·哈达威Jr", "独行侠"},
        
        {"阿尔佩伦·申京", "火箭队"},
        {"贾巴里·史密斯", "火箭队"},
        {"弗雷德·范弗利特", "火箭队"},
        {"杰伦·格林", "火箭队"}
    };
}

void MainWindow::showPlayerDetailedStats(const QString& playerName)
{
    // 获取该球员的所有比赛记录
    QVector<PlayerStats> allGames = m_dataManager->getAllGames();
    QVector<PlayerStats> playerGames;
    
    // 过滤出该球员的比赛记录
    for (const auto& game : allGames) {
        if (game.getName() == playerName) {
            playerGames.append(game);
        }
    }
    
    if (playerGames.isEmpty()) {
        QMessageBox::information(this, tr("提示"),
            tr("该球员暂无比赛记录。"));
        return;
    }
    
    // 创建详细统计表格窗口
    PlayerDataTable* detailWindow = new PlayerDataTable(playerName, this);
    detailWindow->setAttribute(Qt::WA_DeleteOnClose);
    
    // 设置表格数据
    for (const auto& game : playerGames) {
        detailWindow->addGameRecord(
            game.getDate(),
            game.getTeam(),
            game.getPoints(),
            game.getThreePoints(),
            game.getRebounds(),
            game.getDunks(),
            game.getSteals()
        );
    }
    
    // 计算并显示平均数据
    detailWindow->calculateAndShowAverages();
    
    // 显示窗口
    detailWindow->resize(800, 500);
    detailWindow->show();
}
