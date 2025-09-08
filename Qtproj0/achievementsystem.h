#ifndef ACHIEVEMENTSYSTEM_H
#define ACHIEVEMENTSYSTEM_H

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <vector>
#include <QString>
#include <QPixmap>

// 成就类型枚举
enum class AchievementType {
    Scoring,        // 得分类
    Rebounding,     // 篮板类
    Assists,        // 助攻类  
    Defense,        // 防守类
    Efficiency,     // 效率类
    Consistency,    // 稳定性类
    Milestone       // 里程碑类
};

// 成就结构
struct Achievement {
    QString id;                 // 成就ID
    QString name;               // 成就名称
    QString description;        // 成就描述
    AchievementType type;       // 成就类型
    int targetValue;           // 目标值
    int currentProgress;       // 当前进度
    bool unlocked;             // 是否已解锁
    QString iconPath;          // 成就图标路径
    int points;                // 成就积分
};

class AchievementWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AchievementWidget(const Achievement& achievement, QWidget* parent = nullptr);
    
    void updateProgress(int newProgress);
    bool isUnlocked() const { return m_achievement.unlocked; }

signals:
    void achievementUnlocked(const QString& achievementId);

private:
    void setupUI();
    void checkUnlock();

private:
    Achievement m_achievement;
    QLabel* m_iconLabel;
    QLabel* m_nameLabel;
    QLabel* m_descLabel;
    QProgressBar* m_progressBar;
    QLabel* m_pointsLabel;
};

class AchievementSystem : public QWidget
{
    Q_OBJECT

public:
    explicit AchievementSystem(QWidget* parent = nullptr);
    
    // 检查球员成就
    void checkPlayerAchievements(const QString& playerName);
    
    // 获取所有成就
    std::vector<Achievement> getAllAchievements() const;
    
    // 获取已解锁成就数量
    int getUnlockedCount() const;
    
    // 获取总积分
    int getTotalPoints() const;

private slots:
    void onAchievementUnlocked(const QString& achievementId);
    void onFilterChanged(int typeFilter);

private:
    void setupUI();
    void initializeAchievements();
    void updateDisplay();
    Achievement createAchievement(const QString& id, const QString& name, 
                                const QString& description, AchievementType type, 
                                int target, const QString& icon = "", int points = 10);

private:
    std::vector<Achievement> m_achievements;
    QScrollArea* m_scrollArea;
    QGridLayout* m_gridLayout;
    QWidget* m_contentWidget;
    QPushButton* m_filterButton;
    QLabel* m_statsLabel;
    
    int m_currentFilter; // -1 = 全部, 其他 = AchievementType
};

#endif // ACHIEVEMENTSYSTEM_H
