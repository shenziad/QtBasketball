#include "datamanage.h"
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <algorithm>

DataManage::DataManage(QObject *parent)
    : QObject(parent)
{
}

DataManage::~DataManage()
{
}

bool DataManage::addGameStats(const PlayerStats& stats)
{
    // 添加到比赛记录
    m_gameStats.append(stats);
    
    // 更新汇总数据
    updateSummaryStats(stats);
    
    // 发出数据变化信号
    emit dataChanged();
    
    return true;
}

void DataManage::updateSummaryStats(const PlayerStats& stats)
{
    QString playerName = stats.getName();
    if (!m_summaryStats.contains(playerName)) {
        m_summaryStats[playerName] = PlayerStatsSummary();
    }
    m_summaryStats[playerName].addGameStats(stats);
}

bool DataManage::saveGameStats(const QString& filename)
{
    // 确保目录存在
    QDir dir = QFileInfo(filename).dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);  // 设置版本以确保兼容性
    out << m_gameStats;
    file.close();
    return true;
}

bool DataManage::saveSummaryStats(const QString& filename)
{
    // 确保目录存在
    QDir dir = QFileInfo(filename).dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);  // 设置版本以确保兼容性
    out << m_summaryStats;
    file.close();
    return true;
}

bool DataManage::loadGameStats(const QString& filename)
{
    QFile file(filename);
    if (!file.exists()) {
        // 如果文件不存在，创建空文件
        m_gameStats.clear();
        return saveGameStats(filename);
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);  // 设置版本以确保兼容性
    
    m_gameStats.clear();
    in >> m_gameStats;
    
    // 重新计算汇总数据
    m_summaryStats.clear();
    for (const PlayerStats& stats : m_gameStats) {
        updateSummaryStats(stats);
    }
    
    file.close();
    return true;
}

bool DataManage::loadSummaryStats(const QString& filename)
{
    QFile file(filename);
    if (!file.exists()) {
        // 如果文件不存在，创建空文件
        m_summaryStats.clear();
        return saveSummaryStats(filename);
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);  // 设置版本以确保兼容性
    
    m_summaryStats.clear();
    in >> m_summaryStats;
    file.close();
    return true;
}

QVector<PlayerStatsSummary> DataManage::getAllPlayersSummary() const
{
    QVector<PlayerStatsSummary> result;
    for (const auto& summary : m_summaryStats) {
        result.append(summary);
    }
    
    // 按总得分降序排序
    std::sort(result.begin(), result.end(),
              [](const PlayerStatsSummary& a, const PlayerStatsSummary& b) {
                  return a.totalPoints > b.totalPoints;
              });
              
    return result;
}

const PlayerStatsSummary* DataManage::getPlayerSummary(const QString& name) const
{
    auto it = m_summaryStats.find(name);
    if (it != m_summaryStats.end()) {
        return &it.value();
    }
    return nullptr;
}

bool DataManage::compareByCategory(const PlayerStatsSummary& a, 
                                 const PlayerStatsSummary& b,
                                 const QString& category)
{
    if (category == "threePoints")
        return a.getAverageThreePoints() > b.getAverageThreePoints();
    else if (category == "rebounds")
        return a.getAverageRebounds() > b.getAverageRebounds();
    else if (category == "dunks")
        return a.getAverageDunks() > b.getAverageDunks();
    else if (category == "steals")
        return a.getAverageSteals() > b.getAverageSteals();
    else if (category == "points")
        return a.getAveragePoints() > b.getAveragePoints();
    return false;
}

QVector<PlayerStatsSummary> DataManage::getTopThreeByAverage(const QString& category) const
{
    QVector<PlayerStatsSummary> result = getAllPlayersSummary();
    
    std::sort(result.begin(), result.end(), 
              [category](const PlayerStatsSummary& a, const PlayerStatsSummary& b) {
                  return compareByCategory(a, b, category);
              });
    
    while (result.size() > 3) {
        result.removeLast();
    }
    
    return result;
}

QVector<PlayerStatsSummary> DataManage::getTopThreeInTeam(const QString& team) const
{
    QVector<PlayerStatsSummary> teamPlayers;
    
    // 获取指定队伍的所有球员
    for (const auto& summary : m_summaryStats) {
        if (summary.team == team) {
            teamPlayers.append(summary);
        }
    }
    
    // 按总得分排序
    std::sort(teamPlayers.begin(), teamPlayers.end(),
              [](const PlayerStatsSummary& a, const PlayerStatsSummary& b) {
                  return a.totalPoints > b.totalPoints;
              });
    
    // 只保留前三名
    while (teamPlayers.size() > 3) {
        teamPlayers.removeLast();
    }
    
    return teamPlayers;
}

QStringList DataManage::getAllTeams() const
{
    QStringList teams;
    for (const auto& summary : m_summaryStats) {
        if (!teams.contains(summary.team)) {
            teams.append(summary.team);
        }
    }
    return teams;
}
