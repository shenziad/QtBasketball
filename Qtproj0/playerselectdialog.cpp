#include "playerselectdialog.h"
#include <QMessageBox>
#include <QInputDialog>

PlayerSelectDialog::PlayerSelectDialog(DataManage* dataManager, QWidget *parent)
    : QDialog(parent), m_dataManager(dataManager)
{
    setWindowTitle("选择预设球员");
    setModal(true);
    resize(600, 500);
    
    setupUI();
    setupPresetPlayers();
}

void PlayerSelectDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // 创建标题
    QLabel* titleLabel = new QLabel("请选择要添加到比赛中的球员：");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px; margin: 10px;");
    m_mainLayout->addWidget(titleLabel);
    
    // 创建滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollWidget = new QWidget();
    m_playersLayout = new QGridLayout(m_scrollWidget);
    
    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    m_mainLayout->addWidget(m_scrollArea);
    
    // 创建按钮区域
    m_buttonLayout = new QHBoxLayout();
    
    m_selectAllBtn = new QPushButton("全选");
    m_clearAllBtn = new QPushButton("清空");
    m_selectByTeamBtn = new QPushButton("按队伍选择");
    
    m_buttonLayout->addWidget(m_selectAllBtn);
    m_buttonLayout->addWidget(m_clearAllBtn);
    m_buttonLayout->addWidget(m_selectByTeamBtn);
    m_buttonLayout->addStretch();
    
    m_okBtn = new QPushButton("确定");
    m_cancelBtn = new QPushButton("取消");
    m_okBtn->setDefault(true);
    
    m_buttonLayout->addWidget(m_okBtn);
    m_buttonLayout->addWidget(m_cancelBtn);
    
    m_mainLayout->addLayout(m_buttonLayout);
    
    // 连接信号槽
    connect(m_selectAllBtn, &QPushButton::clicked, this, &PlayerSelectDialog::selectAll);
    connect(m_clearAllBtn, &QPushButton::clicked, this, &PlayerSelectDialog::clearAll);
    connect(m_selectByTeamBtn, &QPushButton::clicked, this, &PlayerSelectDialog::selectByTeam);
    connect(m_okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void PlayerSelectDialog::setupPresetPlayers()
{
    // 预设球员数据
    m_playerTeamMap = {
        {"斯蒂芬·库里", "勇士队"}, {"克莱·汤普森", "勇士队"}, 
        {"德雷蒙德·格林", "勇士队"}, {"安德鲁·威金斯", "勇士队"},
        {"勒布朗·詹姆斯", "湖人队"}, {"安东尼·戴维斯", "湖人队"}, 
        {"奥斯汀·里夫斯", "湖人队"},
        {"杰森·塔图姆", "凯尔特人队"}, {"杰伦·布朗", "凯尔特人队"}, 
        {"克里斯塔普斯·波尔津吉斯", "凯尔特人队"},
        {"卢卡·东契奇", "独行侠队"}, {"凯里·欧文", "独行侠队"},
        {"阿尔佩伦·申京", "火箭队"}, {"弗雷德·范弗利特", "火箭队"},
        {"吉米·巴特勒", "热火队"}, {"泰勒·希罗", "热火队"},
        {"德玛尔·德罗赞", "公牛队"}, {"扎克·拉文", "公牛队"},
        {"乔尔·恩比德", "76人队"}, {"泰瑞斯·马克西", "76人队"}
    };
    
    // 按队伍分组球员
    for (auto it = m_playerTeamMap.begin(); it != m_playerTeamMap.end(); ++it) {
        QString playerName = it.key();
        QString teamName = it.value();
        m_teamPlayersMap[teamName].append(playerName);
    }
    
    // 创建按队伍分组的复选框
    int row = 0;
    for (auto teamIt = m_teamPlayersMap.begin(); teamIt != m_teamPlayersMap.end(); ++teamIt) {
        QString teamName = teamIt.key();
        QStringList players = teamIt.value();
        
        // 创建队伍标签
        QLabel* teamLabel = new QLabel(teamName);
        teamLabel->setStyleSheet("font-weight: bold; color: #2E86C1; font-size: 13px; padding: 5px;");
        m_playersLayout->addWidget(teamLabel, row, 0, 1, 3);
        row++;
        
        // 创建该队伍的球员复选框
        int col = 0;
        for (const QString& playerName : players) {
            QCheckBox* checkBox = new QCheckBox(playerName);
            checkBox->setStyleSheet("padding: 3px; margin: 2px;");
            m_playerCheckBoxes[playerName] = checkBox;
            
            m_playersLayout->addWidget(checkBox, row, col);
            col++;
            if (col >= 3) { // 每行最多3个球员
                col = 0;
                row++;
            }
        }
        if (col != 0) {
            row++;
        }
        
        // 添加间隔
        row++;
    }
    
    m_playersLayout->setSpacing(5);
    m_playersLayout->setContentsMargins(10, 10, 10, 10);
}

QVector<PlayerStats> PlayerSelectDialog::getSelectedPlayers() const
{
    QVector<PlayerStats> selectedPlayers;
    for (auto it = m_playerCheckBoxes.begin(); it != m_playerCheckBoxes.end(); ++it) {
        if (it.value()->isChecked()) {
            QString playerName = it.key();
            QString teamName = m_playerTeamMap[playerName];
            
            // Create a basic PlayerStats object
            PlayerStats player;
            player.setName(playerName);
            player.setTeam(teamName);
            // Initialize other stats to 0
            player.setPoints(0);
            player.setThreePoints(0);
            player.setRebounds(0);
            player.setDunks(0);
            player.setSteals(0);
            
            selectedPlayers.append(player);
        }
    }
    return selectedPlayers;
}

void PlayerSelectDialog::setSelectedPlayers(const QVector<PlayerStats>& selectedPlayers)
{
    // 先清空所有选择
    for (auto it = m_playerCheckBoxes.begin(); it != m_playerCheckBoxes.end(); ++it) {
        it.value()->setChecked(false);
    }
    
    // 设置指定的球员为选中状态
    for (const PlayerStats& player : selectedPlayers) {
        QString playerName = player.getName();
        if (m_playerCheckBoxes.contains(playerName)) {
            m_playerCheckBoxes[playerName]->setChecked(true);
        }
    }
}

void PlayerSelectDialog::selectAll()
{
    for (auto it = m_playerCheckBoxes.begin(); it != m_playerCheckBoxes.end(); ++it) {
        it.value()->setChecked(true);
    }
}

void PlayerSelectDialog::clearAll()
{
    for (auto it = m_playerCheckBoxes.begin(); it != m_playerCheckBoxes.end(); ++it) {
        it.value()->setChecked(false);
    }
}

void PlayerSelectDialog::selectByTeam()
{
    QStringList teams = m_teamPlayersMap.keys();
    
    bool ok;
    QString selectedTeam = QInputDialog::getItem(this, "按队伍选择", 
                                                "请选择队伍：", teams, 0, false, &ok);
    
    if (ok && !selectedTeam.isEmpty()) {
        // 先清空所有选择
        clearAll();
        
        // 选中指定队伍的所有球员
        QStringList teamPlayers = m_teamPlayersMap[selectedTeam];
        for (const QString& playerName : teamPlayers) {
            if (m_playerCheckBoxes.contains(playerName)) {
                m_playerCheckBoxes[playerName]->setChecked(true);
            }
        }
    }
}
