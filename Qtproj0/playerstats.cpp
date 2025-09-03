#include "playerstats.h"

PlayerStats::PlayerStats()
    : m_gameId(0), m_threePoints(0), m_rebounds(0), m_dunks(0)
    , m_steals(0), m_points(0)
{
}

PlayerStats::PlayerStats(const QString& name, const QString& team,
                       int threePoints, int rebounds, int dunks,
                       int steals, int points, const QDate& date,
                       int gameId, const QString& gameName)
    : m_name(name), m_team(team)
    , m_gameName(gameName), m_gameId(gameId)
    , m_threePoints(threePoints), m_rebounds(rebounds)
    , m_dunks(dunks), m_steals(steals)
    , m_points(points), m_date(date)
{
}

QDataStream& operator<<(QDataStream& out, const PlayerStats& stats)
{
    out << stats.m_name << stats.m_team << stats.m_gameName << stats.m_gameId
        << stats.m_threePoints << stats.m_rebounds
        << stats.m_dunks << stats.m_steals
        << stats.m_points << stats.m_date;
    return out;
}

QDataStream& operator>>(QDataStream& in, PlayerStats& stats)
{
    in >> stats.m_name >> stats.m_team >> stats.m_gameName >> stats.m_gameId
       >> stats.m_threePoints >> stats.m_rebounds
       >> stats.m_dunks >> stats.m_steals
       >> stats.m_points >> stats.m_date;
    return in;
}

PlayerStatsSummary::PlayerStatsSummary()
    : totalThreePoints(0), totalRebounds(0)
    , totalDunks(0), totalSteals(0)
    , totalPoints(0), gamesPlayed(0)
{
}

void PlayerStatsSummary::addGameStats(const PlayerStats& stats)
{
    if (gamesPlayed == 0) {
        name = stats.getName();
        team = stats.getTeam();
    }
    
    totalThreePoints += stats.getThreePoints();
    totalRebounds += stats.getRebounds();
    totalDunks += stats.getDunks();
    totalSteals += stats.getSteals();
    totalPoints += stats.getPoints();
    gamesPlayed++;
}

QDataStream& operator<<(QDataStream& out, const PlayerStatsSummary& summary)
{
    out << summary.name << summary.team
        << summary.totalThreePoints << summary.totalRebounds
        << summary.totalDunks << summary.totalSteals
        << summary.totalPoints << summary.gamesPlayed;
    return out;
}

QDataStream& operator>>(QDataStream& in, PlayerStatsSummary& summary)
{
    in >> summary.name >> summary.team
       >> summary.totalThreePoints >> summary.totalRebounds
       >> summary.totalDunks >> summary.totalSteals
       >> summary.totalPoints >> summary.gamesPlayed;
    return in;
}
