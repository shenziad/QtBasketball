#include "addgamestat_log.h"
#include "ui_addgamestat_log.h"
#include <QMessageBox>

AddGameStat_Log::AddGameStat_Log(DataManage* dataManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddGameStat_Log)
    , m_dataManager(dataManager)
{
    ui->setupUi(this);
    setupUI();
    
    // 设置窗口标题
    setWindowTitle(tr("添加比赛数据"));
}

AddGameStat_Log::~AddGameStat_Log()
{
    delete ui;
}

void AddGameStat_Log::setupUI()
{
    // 设置日期选择器的范围
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setCalendarPopup(true);
    
    // 设置各项数据的范围限制
    ui->threePointsSpinBox->setRange(0, 30);    // 三分球通常不会超过30个
    ui->reboundsSpinBox->setRange(0, 40);       // 篮板数通常不会超过40个
    ui->dunksSpinBox->setRange(0, 20);          // 扣篮数通常不会超过20个
    ui->stealsSpinBox->setRange(0, 20);         // 抢断数通常不会超过20个
    ui->pointsSpinBox->setRange(0, 150);        // 得分通常不会超过150分
    
    // 如果需要，可以从数据管理器获取现有队伍列表
    QStringList teams = m_dataManager->getAllTeams();
    ui->teamComboBox->addItems(teams);
    ui->teamComboBox->setEditable(true);
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

void AddGameStat_Log::on_buttonBox_accepted()
{
    if (!validateInput()) {
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
        ui->dateEdit->date()
    );
    
    // 添加到数据管理器
    if (m_dataManager->addGameStats(stats)) {
        accept();
    } else {
        QMessageBox::critical(this, tr("错误"),
                            tr("添加比赛数据失败"));
    }
}

void AddGameStat_Log::on_buttonBox_rejected()
{
    reject();
}
