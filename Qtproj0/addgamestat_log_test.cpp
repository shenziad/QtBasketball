#include "addgamestat_log.h"
#include "ui_addgamestat_log.h"
#include "playerselectdialog.h"
#include <QMessageBox>
#include <QCompleter>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

// Test file to check for syntax errors
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
    
    QStringList teams;
    teams << "湖人队" << "勇士队" << "公牛队" << "凯尔特人队" << "热火队" << "马刺队" << "尼克斯队" << "篮网队";
    ui->teamComboBox->addItems(teams);
    
    QStringList playerNames = m_dataManager->getAllPlayerNames();
    QCompleter *completer = new QCompleter(playerNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->nameLineEdit->setCompleter(completer);
    
    ui->statusLabel->setText("准备添加球员统计数据");
}

void AddGameStat_Log::setupConnections()
{
    connect(ui->selectPresetButton, &QPushButton::clicked, this, &AddGameStat_Log::selectPresetPlayers);
    connect(ui->addPlayerButton, &QPushButton::clicked, this, &AddGameStat_Log::addPlayer);
    connect(ui->clearInputsButton, &QPushButton::clicked, this, &AddGameStat_Log::clearInputs);
    connect(ui->finishButton, &QPushButton::clicked, this, &AddGameStat_Log::saveGameData);
    connect(ui->cancelButton, &QPushButton::clicked, this, &AddGameStat_Log::reject);
}
