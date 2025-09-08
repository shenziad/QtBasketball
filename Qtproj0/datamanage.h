#ifndef DATAMANAGE_H
#define DATAMANAGE_H

#include <QObject>
#include <QVector>
#include <QMap>
#include "playerstats.h"

class DataManage : public QObject
{
    Q_OBJECT

public:
    explicit DataManage(QObject *parent = nullptr);
    ~DataManage();

    // 添加新的比赛数据
    bool addGameStats(const PlayerStats& stats);

    // 文件操作
    bool saveGameStats(const QString& filename);
    bool saveSummaryStats(const QString& filename);
    bool loadGameStats(const QString& filename);
    bool loadSummaryStats(const QString& filename);

    // 数据查询
    QVector<PlayerStats> getAllGames() const { return m_gameStats; }
    QVector<PlayerStatsSummary> getAllPlayersSummary() const;
    QVector<PlayerStatsSummary> getTopThreeByAverage(const QString& category) const;
    QVector<PlayerStatsSummary> getTopThreeInTeam(const QString& team) const;
    QStringList getAllTeams() const;
    QStringList getAllPlayerNames() const;
    
    // 获取预设数据
    QStringList getPresetTeams() const { return m_presetTeams; }
    QVector<QPair<QString, QString>> getPresetPlayers() const { return m_presetPlayers; }

    // 数据验证
    bool isPlayerExists(const QString& name) const { return m_summaryStats.contains(name); }
    const PlayerStatsSummary* getPlayerSummary(const QString& name) const;
    int getGameCount() const { return m_gameStats.size(); }
    
signals:
    void dataChanged();  // 当数据发生变化时发出的信号

public:
    // 获取下一个比赛ID
    int getNextGameId() const { return m_gameStats.isEmpty() ? 1 : m_gameStats.last().getGameId() + 1; }

    // 批量添加同一场比赛的数据
    bool addGameStats(const QVector<PlayerStats>& stats);

    // 删除球员所有数据
    bool deletePlayerAllStats(const QString& playerName);
    
    // 删除单条比赛数据
    bool deleteGameStat(int gameId, const QString& playerName);
    
    // 设置预设球队
    void setPresetTeams(const QStringList& teams);
    
    // 获取预设球员列表（球员名，球队）
    void setPresetPlayers(const QVector<QPair<QString, QString>>& players);

private:
    QVector<PlayerStats> m_gameStats;          // 所有比赛数据
    QMap<QString, PlayerStatsSummary> m_summaryStats;  // 汇总数据，按球员名字索引
    QStringList m_presetTeams;                 // 预设球队列表
    QVector<QPair<QString, QString>> m_presetPlayers; // 预设球员列表（姓名，球队）

    void updateSummaryStats(const PlayerStats& stats);
    static bool compareByCategory(const PlayerStatsSummary& a, 
                                const PlayerStatsSummary& b,
                                const QString& category);
};

#endif // DATAMANAGE_H
