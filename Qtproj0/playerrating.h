#ifndef PLAYERRATING_H
#define PLAYERRATING_H

#include <QString>
#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>

// 球员评级结构
struct PlayerRating {
    QString playerName;
    double overallRating;      // 综合评分 (0-100)
    double offensiveRating;    // 进攻评分
    double defensiveRating;    // 防守评分
    double efficiencyRating;   // 效率评分
    double stabilityRating;    // 稳定性评分
    QString grade;             // 等级 (S+, S, A+, A, B+, B, C+, C, D)
    QString comment;           // 评价文字
};

class PlayerRatingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerRatingWidget(const QString& playerName, QWidget* parent = nullptr);
    
    // 计算球员评级
    PlayerRating calculateRating(const QString& playerName);
    
    // 更新显示
    void updateRatingDisplay();

private:
    void setupUI();
    void createRatingBar(const QString& label, double value, QVBoxLayout* layout);
    QString getRatingGrade(double score);
    QString getRatingComment(double score, const QString& grade);
    QColor getRatingColor(double score);

private:
    QString m_playerName;
    PlayerRating m_rating;
    
    QVBoxLayout* m_mainLayout;
    QLabel* m_titleLabel;
    QLabel* m_gradeLabel;
    QLabel* m_commentLabel;
    
    QProgressBar* m_overallBar;
    QProgressBar* m_offensiveBar;
    QProgressBar* m_defensiveBar;
    QProgressBar* m_efficiencyBar;
    QProgressBar* m_stabilityBar;
};

#endif // PLAYERRATING_H
