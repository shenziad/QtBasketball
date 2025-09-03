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

private:
    QVector<PlayerStats> m_gameStats;          // 所有比赛数据
    QMap<QString, PlayerStatsSummary> m_summaryStats;  // 汇总数据，按球员名字索引

    void updateSummaryStats(const PlayerStats& stats);
    static bool compareByCategory(const PlayerStatsSummary& a, 
                                const PlayerStatsSummary& b,
                                const QString& category);
};

#endif // DATAMANAGE_H
