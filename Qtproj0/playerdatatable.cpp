#include "playerdatatable.h"
#include "playerchartwidget.h"
#include "datamanage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFont>
#include <QDebug>
#include <QMap>

PlayerDataTable::PlayerDataTable(const QString& playerName, QWidget* parent)
    : QWidget(parent)
    , m_playerName(playerName)
    , m_tableWidget(nullptr)
    , m_titleLabel(nullptr)
    , m_exportButton(nullptr)
    , m_deleteButton(nullptr)
    , m_chartButton(nullptr)
    , m_dataManager(nullptr)
    , m_chartWidget(nullptr)
{
    setupUI();
    initializeTable();
}

PlayerDataTable::~PlayerDataTable()
{
    // Qt的父子关系会自动删除子部件，不需要手动删除
}

void PlayerDataTable::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 20, 25, 20);
    
    // === 头部区域 - 显示头像和姓名 ===
    createHeaderSection(mainLayout);
    
    // === 比赛记录表格区域 ===
    createTableSection(mainLayout);
    
    // === 底部操作按钮区域 ===
    createActionButtons(mainLayout);
    
    setLayout(mainLayout);
    setWindowTitle(tr("%1 - 技术统计详情").arg(m_playerName));
    
    // 设置窗口图标和属性
    setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);
}

void PlayerDataTable::createHeaderSection(QVBoxLayout* mainLayout)
{
    // 简单的头部容器
    QWidget* headerWidget = new QWidget(this);
    
    QHBoxLayout* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setSpacing(15);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    
    // 球员头像
    QLabel* avatarLabel = new QLabel(this);
    QPixmap avatar = AvatarGenerator::generateAvatar(m_playerName, 60, AvatarGenerator::Initials);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setFixedSize(60, 60);
    avatarLabel->setScaledContents(true);
    
    // 球员姓名
    m_titleLabel = new QLabel(m_playerName, this);
    QFont font = m_titleLabel->font();
    font.setPointSize(16);
    font.setBold(true);
    m_titleLabel->setFont(font);
    
    headerLayout->addWidget(avatarLabel);
    headerLayout->addWidget(m_titleLabel);
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerWidget);
}

void PlayerDataTable::createTableSection(QVBoxLayout* mainLayout)
{
    // 表格容器
    QWidget* tableWidget = new QWidget(this);
    
    QVBoxLayout* tableLayout = new QVBoxLayout(tableWidget);
    
    // 表格标题
    QLabel* tableTitle = new QLabel(tr("比赛记录明细"), this);
    QFont titleFont = tableTitle->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    tableTitle->setFont(titleFont);
    
    // 表格
    m_tableWidget = new QTableWidget(this);
    
    tableLayout->addWidget(tableTitle);
    tableLayout->addWidget(m_tableWidget);
    
    mainLayout->addWidget(tableWidget);
}

void PlayerDataTable::createActionButtons(QVBoxLayout* mainLayout)
{
    // 按钮容器
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(15);
    
    // 导出按钮
    m_exportButton = new QPushButton(tr("导出数据"), this);
    connect(m_exportButton, &QPushButton::clicked, this, &PlayerDataTable::exportToFile);
    
    // 删除按钮
    m_deleteButton = new QPushButton(tr("删除记录"), this);
    connect(m_deleteButton, &QPushButton::clicked, this, &PlayerDataTable::deleteSelectedRecord);
    
    // 图表分析按钮
    m_chartButton = new QPushButton(tr("图表分析"), this);
    m_chartButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; }");
    connect(m_chartButton, &QPushButton::clicked, this, &PlayerDataTable::showChartAnalysis);
    
    // 返回按钮
    QPushButton* returnButton = new QPushButton(tr("返回主界面"), this);
    connect(returnButton, &QPushButton::clicked, this, &QWidget::close);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_exportButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_chartButton);
    buttonLayout->addWidget(returnButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
}

void PlayerDataTable::initializeTable()
{
    QStringList headers;
    headers << tr("比赛日期") << tr("所属球队") << tr("得分") << tr("三分")
            << tr("篮板") << tr("扣篮") << tr("抢断") << tr("比赛ID");
    
    m_tableWidget->setColumnCount(headers.size());
    m_tableWidget->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    m_tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    m_tableWidget->setSortingEnabled(true);
    m_tableWidget->verticalHeader()->setVisible(false);
    
    // 设置行高
    m_tableWidget->verticalHeader()->setDefaultSectionSize(40);
    
    // 设置列宽 - 更加合理的分配
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // 日期
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // 球队
    
    // 数据列设置固定宽度
    m_tableWidget->setColumnWidth(2, 80);  // 得分
    m_tableWidget->setColumnWidth(3, 80);  // 三分  
    m_tableWidget->setColumnWidth(4, 80);  // 篮板
    m_tableWidget->setColumnWidth(5, 80);  // 扣篮
    m_tableWidget->setColumnWidth(6, 80);  // 抢断
    m_tableWidget->setColumnWidth(7, 90);  // 比赛ID
    
    // 最后一列自适应
    m_tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
}

QTableWidgetItem* PlayerDataTable::createReadOnlyItem(const QString& text) const
{
    QTableWidgetItem* item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}

void PlayerDataTable::addGameRecord(const QDate& date, const QString& team,
                                  int points, int threePoints, int rebounds,
                                  int dunks, int steals, int gameId)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);
    
    int col = 0;
    m_tableWidget->setItem(row, col++, createReadOnlyItem(date.toString("yyyy-MM-dd")));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(team));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(QString::number(points)));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(QString::number(threePoints)));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(QString::number(rebounds)));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(QString::number(dunks)));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(QString::number(steals)));
    m_tableWidget->setItem(row, col++, createReadOnlyItem(QString::number(gameId)));
}

void PlayerDataTable::calculateAndShowAverages()
{
    // 由于移除了统计概览区域，此函数暂时保留为空
    // 如果需要在其他地方显示统计信息，可以在此实现
}

void PlayerDataTable::exportToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("保存统计数据"),
        QString("%1_技术统计.csv").arg(m_playerName),
        tr("CSV 文件 (*.csv)"));
        
    if (fileName.isEmpty())
        return;
        
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("导出失败"),
            tr("无法创建文件：%1").arg(fileName));
        return;
    }
    
    QTextStream out(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    out.setCodec("UTF-8");
#endif
    
    // 写入标题
    out << tr("\"%1 的技术统计表\"\n").arg(m_playerName);
    
    // 写入表头
    QStringList headers;
    for (int i = 0; i < m_tableWidget->columnCount(); ++i) {
        headers << m_tableWidget->horizontalHeaderItem(i)->text();
    }
    out << "\"" << headers.join("\",\"") << "\"\n";
    
    // 写入数据
    for (int row = 0; row < m_tableWidget->rowCount(); ++row) {
        QStringList rowData;
        for (int col = 0; col < m_tableWidget->columnCount(); ++col) {
            rowData << m_tableWidget->item(row, col)->text();
        }
        out << "\"" << rowData.join("\",\"") << "\"\n";
    }
    
    file.close();
    
    QMessageBox::information(this, tr("导出成功"),
        tr("数据已成功导出到文件：%1").arg(fileName));
}

void PlayerDataTable::setDataManager(DataManage* dataManager)
{
    m_dataManager = dataManager;
}

void PlayerDataTable::deleteSelectedRecord()
{
    int currentRow = m_tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, tr("提示"), tr("请先选择要删除的记录"));
        return;
    }
    
    if (!m_dataManager) {
        QMessageBox::warning(this, tr("错误"), tr("数据管理器未设置"));
        return;
    }
    
    // 获取比赛ID (现在在第7列，因为没有头像)
    QTableWidgetItem* gameIdItem = m_tableWidget->item(currentRow, 7);
    if (!gameIdItem) {
        QMessageBox::warning(this, tr("错误"), tr("无法获取比赛ID"));
        return;
    }
    
    int gameId = gameIdItem->text().toInt();
    QString date = m_tableWidget->item(currentRow, 0)->text();
    
    int ret = QMessageBox::warning(this, tr("确认删除"),
        tr("确定要删除 %1 在 %2 的比赛记录吗？此操作不可恢复！")
        .arg(m_playerName).arg(date),
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        if (m_dataManager->deleteGameStat(gameId, m_playerName)) {
            // 从表格中删除该行
            m_tableWidget->removeRow(currentRow);
            
            // 发出数据变化信号
            emit dataChanged();
            
            QMessageBox::information(this, tr("删除成功"),
                tr("已成功删除该场比赛记录"));
        } else {
            QMessageBox::warning(this, tr("删除失败"),
                tr("删除比赛记录时发生错误"));
        }
    }
}

void PlayerDataTable::showChartAnalysis()
{
    if (!m_chartWidget) {
        m_chartWidget = new PlayerChartWidget(m_playerName);
        
        // 计算球员的平均数据用于图表显示
        if (m_tableWidget && m_tableWidget->rowCount() > 0) {
            double totalPoints = 0, totalRebounds = 0, totalAssists = 0, totalSteals = 0, totalDunks = 0;
            int gameCount = m_tableWidget->rowCount();
            
            for (int row = 0; row < gameCount; ++row) {
                totalPoints += m_tableWidget->item(row, 2) ? m_tableWidget->item(row, 2)->text().toDouble() : 0;
                totalRebounds += m_tableWidget->item(row, 4) ? m_tableWidget->item(row, 4)->text().toDouble() : 0;
                totalAssists += m_tableWidget->item(row, 5) ? m_tableWidget->item(row, 5)->text().toDouble() : 0;
                totalSteals += m_tableWidget->item(row, 6) ? m_tableWidget->item(row, 6)->text().toDouble() : 0;
                totalDunks += m_tableWidget->item(row, 5) ? m_tableWidget->item(row, 5)->text().toDouble() : 0; // 扣篮数据
            }
            
            // 设置图表数据
            std::vector<ChartData> chartData = {
                {"得分", totalPoints / gameCount, QColor(255, 99, 132)},
                {"篮板", totalRebounds / gameCount, QColor(54, 162, 235)},
                {"助攻", totalAssists / gameCount, QColor(255, 205, 86)},
                {"抢断", totalSteals / gameCount, QColor(75, 192, 192)},
                {"扣篮", totalDunks / gameCount, QColor(153, 102, 255)}
            };
            
            m_chartWidget->setChartData(chartData);
        }
    }
    
    m_chartWidget->show();
    m_chartWidget->raise();
    m_chartWidget->activateWindow();
}
