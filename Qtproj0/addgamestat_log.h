#ifndef ADDGAMESTAT_LOG_H
#define ADDGAMESTAT_LOG_H

#include <QtCore>
#include <QtWidgets>
#include <QCompleter>
#include "datamanage.h"
#include "playerselectdialog.h"

namespace Ui {
class AddGameStat_Log;
}

class AddGameStat_Log : public QDialog
{
    Q_OBJECT

public:
    explicit AddGameStat_Log(DataManage* dataManager, QWidget *parent = nullptr);
    ~AddGameStat_Log();

private slots:
    void addPlayer();             // 添加当前球员数据
    void saveGameData();          // 保存比赛数据
    void clearInputs();           // 清空输入框
    void selectPresetPlayers();   // 选择预设球员

private:
    Ui::AddGameStat_Log *ui;
    DataManage* m_dataManager;
    int m_currentGameId;                // 当前比赛ID
    
    void setupUI();                     // 设置界面
    void setupConnections();            // 设置信号连接
};

#endif // ADDGAMESTAT_LOG_H
