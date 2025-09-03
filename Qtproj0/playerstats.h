#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include <QString>
#include <QDate>
#include <QDataStream>

class PlayerStats
{
public:
    PlayerStats();
    PlayerStats(const QString& name, const QString& team,
                int threePoints, int rebounds, int dunks,
                int steals, int points, const QDate& date,
                int gameId = 0, const QString& gameName = QString());

    // Getters
    QString getName() const { return m_name; }
    QString getTeam() const { return m_team; }
    QString getGameName() const { return m_gameName; }
    int getGameId() const { return m_gameId; }
    int getThreePoints() const { return m_threePoints; }
    int getRebounds() const { return m_rebounds; }
    int getDunks() const { return m_dunks; }
    int getSteals() const { return m_steals; }
    int getPoints() const { return m_points; }
    QDate getDate() const { return m_date; }

    // Setters
    void setName(const QString& name) { m_name = name; }
    void setTeam(const QString& team) { m_team = team; }
    void setGameName(const QString& gameName) { m_gameName = gameName; }
    void setGameId(int gameId) { m_gameId = gameId; }
    void setThreePoints(int threePoints) { m_threePoints = threePoints; }
    void setRebounds(int rebounds) { m_rebounds = rebounds; }
    void setDunks(int dunks) { m_dunks = dunks; }
    void setSteals(int steals) { m_steals = steals; }
    void setPoints(int points) { m_points = points; }
    void setDate(const QDate& date) { m_date = date; }

    // 序列化支持
    friend QDataStream& operator<<(QDataStream& out, const PlayerStats& stats);
    friend QDataStream& operator>>(QDataStream& in, PlayerStats& stats);

private:
    QString m_name;        // 球员姓名
    QString m_team;        // 所属队名
    QString m_gameName;    // 比赛名称
    int m_gameId;         // 比赛ID
    int m_threePoints;    // 三分球个数
    int m_rebounds;       // 篮板球个数
    int m_dunks;          // 扣篮成功次数
    int m_steals;         // 抢断次数
    int m_points;         // 得分
    QDate m_date;         // 比赛日期
};

// 定义用于汇总的数据结构
class PlayerStatsSummary {
public:
    PlayerStatsSummary();
    void addGameStats(const PlayerStats& stats);

    QString name;           // 姓名
    QString team;          // 所属队名
    int totalThreePoints;  // 三分球总数
    int totalRebounds;     // 篮板球总数
    int totalDunks;        // 扣篮成功总次数
    int totalSteals;       // 抢断总次数
    int totalPoints;       // 得分总数
    int gamesPlayed;       // 比赛场数

    // 获取平均数据
    double getAverageThreePoints() const { return gamesPlayed ? (double)totalThreePoints / gamesPlayed : 0; }
    double getAverageRebounds() const { return gamesPlayed ? (double)totalRebounds / gamesPlayed : 0; }
    double getAverageDunks() const { return gamesPlayed ? (double)totalDunks / gamesPlayed : 0; }
    double getAverageSteals() const { return gamesPlayed ? (double)totalSteals / gamesPlayed : 0; }
    double getAveragePoints() const { return gamesPlayed ? (double)totalPoints / gamesPlayed : 0; }

    // 序列化支持
    friend QDataStream& operator<<(QDataStream& out, const PlayerStatsSummary& summary);
    friend QDataStream& operator>>(QDataStream& in, PlayerStatsSummary& summary);
};

#endif // PLAYERSTATS_H
