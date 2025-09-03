#include "addgamestat_log.h"
#include "ui_addgamestat_log.h"

AddGameStat_Log::AddGameStat_Log(DataManage* dataManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddGameStat_Log)
    , m_dataManager(dataManager)
    , m_currentGameId(m_dataManager->getNextGameId())
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
    
    // 设置窗口标题
    setWindowTitle(tr("添加比赛数据"));
    
    // 设置状态标签初始文本
    ui->statusLabel->setText(tr("请输入比赛基本信息和球员数据"));
}

AddGameStat_Log::~AddGameStat_Log()
{
    delete ui;
}

void AddGameStat_Log::setupUI()
{
    // 设置窗口属性
    setMinimumSize(600, 500);
    
    // 设置控件属性
    ui->gameNameEdit->setPlaceholderText(tr("请输入比赛名称"));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setCalendarPopup(true);
    
    // 设置数据输入控件的范围限制
    ui->threePointsSpinBox->setRange(0, 30);    // 三分球通常不会超过30个
    ui->reboundsSpinBox->setRange(0, 40);       // 篮板数通常不会超过40个
    ui->dunksSpinBox->setRange(0, 20);          // 扣篮数通常不会超过20个
    ui->stealsSpinBox->setRange(0, 20);         // 抢断数通常不会超过20个
    ui->pointsSpinBox->setRange(0, 150);        // 得分通常不会超过150分
    
    // 从数据管理器获取现有队伍列表
    QStringList teams = m_dataManager->getAllTeams();
    ui->teamComboBox->addItems(teams);
    ui->teamComboBox->setEditable(true);
    
    // 设置表格属性
    ui->playersTable->setColumnCount(8);
    ui->playersTable->setHorizontalHeaderLabels(QStringList()
        << tr("姓名") << tr("队伍") << tr("得分") << tr("三分")
        << tr("篮板") << tr("扣篮") << tr("抢断") << tr("状态"));
    ui->playersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->playersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->playersTable->horizontalHeader()->setStretchLastSection(true);
    ui->playersTable->verticalHeader()->setVisible(false);
    
    // 在比赛信息组中添加ID标签
    QLabel* gameIdLabel = new QLabel(QString("比赛ID：%1").arg(m_currentGameId), this);
    QFormLayout* gameInfoLayout = qobject_cast<QFormLayout*>(ui->gameInfoGroup->layout());
    if (gameInfoLayout) {
        gameInfoLayout->insertRow(0, tr(""), gameIdLabel);
    }
    
    // 设置状态标签
    ui->statusLabel->setAlignment(Qt::AlignCenter);
    ui->statusLabel->setText(tr("请输入比赛基本信息和球员数据"));
}

bool AddGameStat_Log::validateGameInfo()
{
    if (ui->gameNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入比赛名称"));
        return false;
    }
    
    if (!ui->dateEdit->date().isValid()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请选择有效的比赛日期"));
        return false;
    }
    
    return true;
}

bool AddGameStat_Log::validateInput()
{
    if (ui->nameLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入球员姓名"));
        return false;
    }
    
    if (ui->teamComboBox->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("输入错误"), tr("请输入队伍名称"));
        return false;
    }
    
    // 检查数值是否合法
    int threePoints = ui->threePointsSpinBox->value();
    if (threePoints < 0) {
        QMessageBox::warning(this, tr("输入错误"), tr("三分球数据无效"));
        return false;
    }
    
    int rebounds = ui->reboundsSpinBox->value();
    if (rebounds < 0) {
        QMessageBox::warning(this, tr("输入错误"), tr("篮板球数据无效"));
        return false;
    }
    
    int dunks = ui->dunksSpinBox->value();
    if (dunks < 0) {
        QMessageBox::warning(this, tr("输入错误"), tr("扣篮数据无效"));
        return false;
    }
    
    int steals = ui->stealsSpinBox->value();
    if (steals < 0) {
        QMessageBox::warning(this, tr("输入错误"), tr("抢断数据无效"));
        return false;
    }
    
    int points = ui->pointsSpinBox->value();
    if (points < 0) {
        QMessageBox::warning(this, tr("输入错误"), tr("得分数据无效"));
        return false;
    }
    
    return true;
}

void AddGameStat_Log::addPlayer()
{
    if (!validateInput()) {
        return;
    }
    
    // 如果这是第一个球员，先验证比赛基本信息
    if (m_gameStats.isEmpty() && !validateGameInfo()) {
        return;
    }
    
    // 创建新的比赛数据对象
    PlayerStats stats(
        ui->nameLineEdit->text(),
        ui->teamComboBox->currentText(),
        ui->threePointsSpinBox->value(),
        ui->reboundsSpinBox->value(),
        ui->dunksSpinBox->value(),
        ui->stealsSpinBox->value(),
        ui->pointsSpinBox->value(),
        ui->dateEdit->date(),
        m_currentGameId,
        ui->gameNameEdit->text()
    );
    
    // 添加到当前比赛数据列表
    m_gameStats.append(stats);
    
    // 更新表格显示
    int row = ui->playersTable->rowCount();
    ui->playersTable->insertRow(row);
    ui->playersTable->setItem(row, 0, new QTableWidgetItem(stats.getName()));
    ui->playersTable->setItem(row, 1, new QTableWidgetItem(stats.getTeam()));
    ui->playersTable->setItem(row, 2, new QTableWidgetItem(QString::number(stats.getPoints())));
    ui->playersTable->setItem(row, 3, new QTableWidgetItem(QString::number(stats.getThreePoints())));
    ui->playersTable->setItem(row, 4, new QTableWidgetItem(QString::number(stats.getRebounds())));
    ui->playersTable->setItem(row, 5, new QTableWidgetItem(QString::number(stats.getDunks())));
    ui->playersTable->setItem(row, 6, new QTableWidgetItem(QString::number(stats.getSteals())));
    ui->playersTable->setItem(row, 7, new QTableWidgetItem(tr("已录入")));
    
    // 清空输入框，准备输入下一个球员
    clearPlayerInputs();
    
    // 显示成功消息
    ui->statusLabel->setText(QString("已添加球员 %1 的数据").arg(stats.getName()));
}

void AddGameStat_Log::clearPlayerInputs()
{
    ui->nameLineEdit->clear();
    ui->threePointsSpinBox->setValue(0);
    ui->reboundsSpinBox->setValue(0);
    ui->dunksSpinBox->setValue(0);
    ui->stealsSpinBox->setValue(0);
    ui->pointsSpinBox->setValue(0);
    ui->nameLineEdit->setFocus();
}

void AddGameStat_Log::finishGame()
{
    if (m_gameStats.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("请至少添加一名球员的数据"));
        return;
    }
    
    // 批量添加本场比赛的所有数据
    if (m_dataManager->addGameStats(m_gameStats)) {
        QMessageBox::information(this, tr("成功"), 
            QString("已成功添加比赛\"%1\"的所有数据").arg(ui->gameNameEdit->text()));
        accept();
    } else {
        QMessageBox::critical(this, tr("错误"), tr("添加比赛数据失败"));
    }
}

void AddGameStat_Log::setupConnections()
{
    // 连接按钮信号
    connect(ui->addPlayerButton, &QPushButton::clicked, this, &AddGameStat_Log::addPlayer);
    connect(ui->finishButton, &QPushButton::clicked, this, &AddGameStat_Log::finishGame);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AddGameStat_Log::cancelGame);
}

void AddGameStat_Log::cancelGame()
{
    if (!m_gameStats.isEmpty()) {
        int ret = QMessageBox::warning(this, tr("确认取消"),
            QString("已输入 %1 名球员的数据，确定要取消吗？").arg(m_gameStats.size()),
            QMessageBox::Yes | QMessageBox::No);
            
        if (ret == QMessageBox::No) {
            return;
        }
    }
    reject();
}
