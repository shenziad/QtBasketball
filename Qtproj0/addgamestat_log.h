#ifndef ADDGAMESTAT_LOG_H
#define ADDGAMESTAT_LOG_H

#include <QtCore>
#include <QtWidgets>
#include "datamanage.h"

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
    void finishGame();            // 完成本场比赛
    void cancelGame();            // 取消添加
    void clearPlayerInputs();     // 清空球员输入框

private:
    Ui::AddGameStat_Log *ui;
    DataManage* m_dataManager;
    int m_currentGameId;                // 当前比赛ID
    QVector<PlayerStats> m_gameStats;   // 存储本场比赛所有球员数据
    
    bool validateInput();               // 验证球员数据输入
    bool validateGameInfo();            // 验证比赛基本信息
    void setupUI();                     // 设置界面
    void setupConnections();            // 设置信号连接
};

#endif // ADDGAMESTAT_LOG_H
