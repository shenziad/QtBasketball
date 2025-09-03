#include "playerdatatable.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFont>

PlayerDataTable::PlayerDataTable(const QString& playerName, QWidget* parent)
    : QWidget(parent)
    , m_playerName(playerName)
    , m_tableWidget(nullptr)
    , m_titleLabel(nullptr)
    , m_exportButton(nullptr)
    , m_averagesLabel(nullptr)
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
    
    // 标题
    m_titleLabel = new QLabel(tr("%1 的技术统计表").arg(m_playerName), this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    
    // 表格
    m_tableWidget = new QTableWidget(this);
    
    // 平均值标签
    m_averagesLabel = new QLabel(this);
    m_averagesLabel->setAlignment(Qt::AlignCenter);
    
    // 创建按钮布局
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    
    // 导出按钮
    m_exportButton = new QPushButton(tr("导出到文件"), this);
    connect(m_exportButton, &QPushButton::clicked, this, &PlayerDataTable::exportToFile);
    
    // 返回按钮
    QPushButton* returnButton = new QPushButton(tr("返回主界面"), this);
    connect(returnButton, &QPushButton::clicked, this, &QWidget::close);
    
    buttonLayout->addWidget(m_exportButton);
    buttonLayout->addWidget(returnButton);
    
    mainLayout->addWidget(m_titleLabel);
    mainLayout->addWidget(m_tableWidget);
    mainLayout->addWidget(m_averagesLabel);
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
    setWindowTitle(tr("%1 的技术统计").arg(m_playerName));
}

void PlayerDataTable::initializeTable()
{
    QStringList headers;
    headers << tr("日期") << tr("球队") << tr("得分") << tr("三分")
            << tr("篮板") << tr("扣篮") << tr("抢断");
    
    m_tableWidget->setColumnCount(headers.size());
    m_tableWidget->setHorizontalHeaderLabels(headers);
    
    // 设置表格属性
    m_tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    
    // 设置列宽
    m_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents); // 日期
    m_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents); // 球队
    
    // 数据列固定宽度
    for (int i = 2; i < headers.size(); ++i) {
        m_tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Fixed);
        m_tableWidget->setColumnWidth(i, 70);
    }
}

QTableWidgetItem* PlayerDataTable::createReadOnlyItem(const QString& text) const
{
    QTableWidgetItem* item = new QTableWidgetItem(text);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    return item;
}

void PlayerDataTable::addGameRecord(const QDate& date, const QString& team,
                                  int points, int threePoints, int rebounds,
                                  int dunks, int steals)
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
}

void PlayerDataTable::calculateAndShowAverages()
{
    if (m_tableWidget->rowCount() == 0) return;
    
    double totalPoints = 0, totalThrees = 0, totalRebounds = 0,
           totalDunks = 0, totalSteals = 0;
    
    for (int row = 0; row < m_tableWidget->rowCount(); ++row) {
        totalPoints += m_tableWidget->item(row, 2)->text().toDouble();
        totalThrees += m_tableWidget->item(row, 3)->text().toDouble();
        totalRebounds += m_tableWidget->item(row, 4)->text().toDouble();
        totalDunks += m_tableWidget->item(row, 5)->text().toDouble();
        totalSteals += m_tableWidget->item(row, 6)->text().toDouble();
    }
    
    int games = m_tableWidget->rowCount();
    
    double avgPoints = totalPoints / games;
    double avgThrees = totalThrees / games;
    double avgRebounds = totalRebounds / games;
    double avgDunks = totalDunks / games;
    double avgSteals = totalSteals / games;
    
    QString averagesText = QString("场均数据：得分 %1  三分 %2  篮板 %3  扣篮 %4  抢断 %5  （共 %6 场比赛）")
        .arg(avgPoints, 0, 'f', 1)
        .arg(avgThrees, 0, 'f', 1)
        .arg(avgRebounds, 0, 'f', 1)
        .arg(avgDunks, 0, 'f', 1)
        .arg(avgSteals, 0, 'f', 1)
        .arg(games);
        
    m_averagesLabel->setText(averagesText);
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
    
    // 写入平均值
    out << "\n" << m_averagesLabel->text() << "\n";
    
    file.close();
    
    QMessageBox::information(this, tr("导出成功"),
        tr("数据已成功导出到文件：%1").arg(fileName));
}
