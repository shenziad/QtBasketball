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

    // 保存数据到文件
    bool saveGameStats(const QString& filename);
    bool saveSummaryStats(const QString& filename);

    // 从文件加载数据
    bool loadGameStats(const QString& filename);
    bool loadSummaryStats(const QString& filename);

    // 获取汇总数据
    QVector<PlayerStatsSummary> getAllPlayersSummary() const;
    
    // 获取指定技术项目平均每场数据最高的前三名
    QVector<PlayerStatsSummary> getTopThreeByAverage(const QString& category) const;
    
    // 获取某个队中个人数据最高的前三名
    QVector<PlayerStatsSummary> getTopThreeInTeam(const QString& team) const;

    // 获取所有队伍名称
    QStringList getAllTeams() const;

private:
    QVector<PlayerStats> m_gameStats;          // 所有比赛数据
    QMap<QString, PlayerStatsSummary> m_summaryStats;  // 汇总数据，按球员名字索引

    void updateSummaryStats(const PlayerStats& stats);
    static bool compareByCategory(const PlayerStatsSummary& a, 
                                const PlayerStatsSummary& b,
                                const QString& category);
};

#endif // DATAMANAGE_H
