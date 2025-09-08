#include "addgamestat_log.h"
#include "ui_addgamestat_log.h"
#include "playerselectdialog.h"
#include <QMessageBox>
#include <QCompleter>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

AddGameStat_Log::AddGameStat_Log(DataManage* dataManager, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddGameStat_Log)
    , m_dataManager(dataManager)
    , m_currentGameId(m_dataManager->getNextGameId())
{
    ui->setupUi(this);
    setupUI();
    setupConnections();
}

AddGameStat_Log::~AddGameStat_Log()
{
    delete ui;
}

void AddGameStat_Log::setupUI()
{
    setFixedSize(800, 600);
    setWindowTitle("添加比赛统计");
    
    ui->playersTable->setColumnCount(7);
    QStringList headers;
    headers << "姓名" << "队伍" << "得分" << "三分球" 
            << "篮板" << "扣篮" << "抢断";
    ui->playersTable->setHorizontalHeaderLabels(headers);
    ui->playersTable->horizontalHeader()->setStretchLastSection(true);
    
    // 使用预设队伍数据
    QStringList teams = m_dataManager->getPresetTeams();
    ui->teamComboBox->addItems(teams);
    ui->teamComboBox->setEditable(true); // 设置为可编辑，允许输入新队伍
    ui->teamComboBox->setInsertPolicy(QComboBox::NoInsert); // 不自动插入，手动处理
    
    QStringList playerNames = m_dataManager->getAllPlayerNames();
    QCompleter *completer = new QCompleter(playerNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    ui->nameLineEdit->setCompleter(completer);
    
    // 当用户从自动完成列表中选择时也触发队伍设置
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
            this, &AddGameStat_Log::onPlayerNameChanged);
    
    ui->statusLabel->setText("准备添加球员统计数据");
}

void AddGameStat_Log::setupConnections()
{
    connect(ui->selectPresetButton, &QPushButton::clicked, this, &AddGameStat_Log::selectPresetPlayers);
    connect(ui->addPlayerButton, &QPushButton::clicked, this, &AddGameStat_Log::addPlayer);
    connect(ui->clearInputsButton, &QPushButton::clicked, this, &AddGameStat_Log::clearInputs);
    connect(ui->finishButton, &QPushButton::clicked, this, &AddGameStat_Log::saveGameData);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AddGameStat_Log::reject);
    
    // 连接球员姓名改变信号，自动设置队伍
    connect(ui->nameLineEdit, &QLineEdit::textChanged, this, &AddGameStat_Log::onPlayerNameChanged);
}

void AddGameStat_Log::addPlayer()
{
    QString name = ui->nameLineEdit->text().trimmed();
    QString team = ui->teamComboBox->currentText().trimmed();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入球员姓名！");
        return;
    }
    
    if (team.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择或输入球队名称！");
        return;
    }
    
    // 如果输入了新的队伍名称，添加到下拉框中（避免重复）
    if (ui->teamComboBox->findText(team) == -1) {
        ui->teamComboBox->addItem(team);
    }
    
    // Check if player already exists in current game
    for (int i = 0; i < ui->playersTable->rowCount(); ++i) {
        if (ui->playersTable->item(i, 0) && 
            ui->playersTable->item(i, 0)->text() == name) {
            // Update existing player
            ui->playersTable->setItem(i, 1, new QTableWidgetItem(team));
            ui->playersTable->setItem(i, 2, new QTableWidgetItem(QString::number(ui->pointsSpinBox->value())));
            ui->playersTable->setItem(i, 3, new QTableWidgetItem(QString::number(ui->threePointsSpinBox->value())));
            ui->playersTable->setItem(i, 4, new QTableWidgetItem(QString::number(ui->reboundsSpinBox->value())));
            ui->playersTable->setItem(i, 5, new QTableWidgetItem(QString::number(ui->dunksSpinBox->value())));
            ui->playersTable->setItem(i, 6, new QTableWidgetItem(QString::number(ui->stealsSpinBox->value())));
            
            ui->statusLabel->setText(QString("已更新球员: %1").arg(name));
            clearInputs();
            return;
        }
    }
    
    // Add new row to table
    int row = ui->playersTable->rowCount();
    ui->playersTable->insertRow(row);
    
    // Set player data
    ui->playersTable->setItem(row, 0, new QTableWidgetItem(name));
    ui->playersTable->setItem(row, 1, new QTableWidgetItem(team));
    ui->playersTable->setItem(row, 2, new QTableWidgetItem(QString::number(ui->pointsSpinBox->value())));
    ui->playersTable->setItem(row, 3, new QTableWidgetItem(QString::number(ui->threePointsSpinBox->value())));
    ui->playersTable->setItem(row, 4, new QTableWidgetItem(QString::number(ui->reboundsSpinBox->value())));
    ui->playersTable->setItem(row, 5, new QTableWidgetItem(QString::number(ui->dunksSpinBox->value())));
    ui->playersTable->setItem(row, 6, new QTableWidgetItem(QString::number(ui->stealsSpinBox->value())));
    
    // Update status
    ui->statusLabel->setText(QString("已添加球员: %1. 总计球员: %2")
                            .arg(name).arg(ui->playersTable->rowCount()));
    clearInputs();
}

void AddGameStat_Log::clearInputs()
{
    ui->nameLineEdit->clear();
    ui->teamComboBox->setCurrentIndex(0);
    ui->pointsSpinBox->setValue(0);
    ui->threePointsSpinBox->setValue(0);
    ui->reboundsSpinBox->setValue(0);
    ui->dunksSpinBox->setValue(0);
    ui->stealsSpinBox->setValue(0);
    ui->nameLineEdit->setFocus();
}

void AddGameStat_Log::saveGameData()
{
    QString gameName = ui->gameNameEdit->text().trimmed();
    
    if (gameName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入比赛名称！");
        ui->gameNameEdit->setFocus();
        return;
    }
    
    if (ui->playersTable->rowCount() == 0) {
        QMessageBox::warning(this, "警告", "请至少添加一名球员！");
        return;
    }
    
    // Validate and save data
    QVector<PlayerStats> gameData;
    
    for (int row = 0; row < ui->playersTable->rowCount(); ++row) {
        PlayerStats stats;
        
        // Set game info using setters
        stats.setGameId(m_currentGameId);
        stats.setGameName(gameName);
        stats.setDate(ui->dateEdit->date());
        
        // Get player data using setters
        if (ui->playersTable->item(row, 0)) {
            stats.setName(ui->playersTable->item(row, 0)->text());
        }
        
        if (ui->playersTable->item(row, 1)) {
            stats.setTeam(ui->playersTable->item(row, 1)->text());
        }
        
        // Get statistics using setters
        bool ok;
        if (ui->playersTable->item(row, 2)) {
            int points = ui->playersTable->item(row, 2)->text().toInt(&ok);
            stats.setPoints(ok ? points : 0);
        }
        
        if (ui->playersTable->item(row, 3)) {
            int threePoints = ui->playersTable->item(row, 3)->text().toInt(&ok);
            stats.setThreePoints(ok ? threePoints : 0);
        }
        
        if (ui->playersTable->item(row, 4)) {
            int rebounds = ui->playersTable->item(row, 4)->text().toInt(&ok);
            stats.setRebounds(ok ? rebounds : 0);
        }
        
        if (ui->playersTable->item(row, 5)) {
            int dunks = ui->playersTable->item(row, 5)->text().toInt(&ok);
            stats.setDunks(ok ? dunks : 0);
        }
        
        if (ui->playersTable->item(row, 6)) {
            int steals = ui->playersTable->item(row, 6)->text().toInt(&ok);
            stats.setSteals(ok ? steals : 0);
        }
        
        gameData.append(stats);
    }
    
    // Save to data manager
    if (m_dataManager->addGameStats(gameData)) {
        QMessageBox::information(this, "成功", 
                               QString("比赛数据保存成功！\n比赛: %1\n球员数: %2")
                               .arg(gameName).arg(gameData.size()));
        accept();
    } else {
        QMessageBox::critical(this, "错误", "保存比赛数据失败！");
    }
}

void AddGameStat_Log::selectPresetPlayers()
{
    PlayerSelectDialog dialog(m_dataManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        QVector<PlayerStats> selectedPlayers = dialog.getSelectedPlayers();
        
        for (const PlayerStats& player : selectedPlayers) {
            // Check if player already exists
            bool exists = false;
            for (int i = 0; i < ui->playersTable->rowCount(); ++i) {
                if (ui->playersTable->item(i, 0) && 
                    ui->playersTable->item(i, 0)->text() == player.getName()) {
                    exists = true;
                    break;
                }
            }
            
            if (!exists) {
                int row = ui->playersTable->rowCount();
                ui->playersTable->insertRow(row);
                
                // Set player info
                ui->playersTable->setItem(row, 0, new QTableWidgetItem(player.getName()));
                ui->playersTable->setItem(row, 1, new QTableWidgetItem(player.getTeam()));
                ui->playersTable->setItem(row, 2, new QTableWidgetItem("0")); // points
                ui->playersTable->setItem(row, 3, new QTableWidgetItem("0")); // threePoints
                ui->playersTable->setItem(row, 4, new QTableWidgetItem("0")); // rebounds
                ui->playersTable->setItem(row, 5, new QTableWidgetItem("0")); // dunks
                ui->playersTable->setItem(row, 6, new QTableWidgetItem("0")); // steals
            }
        }
        
        // Update status
        if (!selectedPlayers.isEmpty()) {
            QMessageBox::information(this, "成功", 
                                   QString("已添加 %1 名预设球员")
                                   .arg(selectedPlayers.size()));
        }
    }
}

void AddGameStat_Log::onPlayerNameChanged()
{
    QString currentName = ui->nameLineEdit->text().trimmed();
    
    if (currentName.isEmpty()) {
        ui->statusLabel->setText("准备添加球员统计数据");
        return;
    }
    
    // 首先检查预设球员中是否有这个球员
    QVector<QPair<QString, QString>> presetPlayers = m_dataManager->getPresetPlayers();
    for (const auto& player : presetPlayers) {
        if (player.first == currentName) {
            QString playerTeam = player.second;
            
            // 查找队伍在下拉框中的索引
            int teamIndex = ui->teamComboBox->findText(playerTeam);
            if (teamIndex != -1) {
                ui->teamComboBox->setCurrentIndex(teamIndex);
            } else {
                // 如果队伍不在列表中，添加并选择
                ui->teamComboBox->addItem(playerTeam);
                ui->teamComboBox->setCurrentText(playerTeam);
            }
            
            // 更新状态提示
            ui->statusLabel->setText(QString("预设球员: %1 (%2)").arg(currentName).arg(playerTeam));
            return;
        }
    }
    
    // 然后检查数据库中是否存在这个球员
    if (m_dataManager->isPlayerExists(currentName)) {
        const PlayerStatsSummary* summary = m_dataManager->getPlayerSummary(currentName);
        if (summary) {
            QString playerTeam = summary->team;
            
            // 查找队伍在下拉框中的索引
            int teamIndex = ui->teamComboBox->findText(playerTeam);
            if (teamIndex != -1) {
                ui->teamComboBox->setCurrentIndex(teamIndex);
            } else {
                // 如果队伍不在列表中，添加并选择
                ui->teamComboBox->addItem(playerTeam);
                ui->teamComboBox->setCurrentText(playerTeam);
            }
            
            // 更新状态提示
            ui->statusLabel->setText(QString("已找到球员: %1 (%2) - 已参加 %3 场比赛")
                                   .arg(currentName)
                                   .arg(playerTeam)
                                   .arg(summary->gamesPlayed));
            return;
        }
    }
    
    // 如果没找到匹配的球员，显示新球员提示
    ui->statusLabel->setText(QString("新球员: %1 - 请选择或输入队伍").arg(currentName));
}
