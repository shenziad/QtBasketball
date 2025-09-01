#ifndef PLAYERDATATABLE_H
#define PLAYERDATATABLE_H

#include <QtWidgets/QWidget>
#include <QtCore/QString>
#include <QtCore/QDate>

QT_BEGIN_NAMESPACE
class QTableWidget;
class QTableWidgetItem;
class QLabel;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

// 球员数据表格窗口类
class PlayerDataTable : public QWidget
{
    Q_OBJECT

signals:
    void dataChanged();

public:
    // 构造函数，需要传入球员姓名和父窗口指针
    explicit PlayerDataTable(const QString& playerName, QWidget* parent = nullptr);
    // 析构函数
    ~PlayerDataTable();

    // 添加一场比赛记录
    void addGameRecord(const QDate& date, const QString& team,
                      int points, int threePoints, int rebounds,
                      int dunks, int steals);
    // 计算并显示平均数据
    void calculateAndShowAverages();

private slots:
    // 导出数据到文件
    void exportToFile();

private:
    // 初始化UI布局
    void setupUI();
    // 初始化表格
    void initializeTable();
    // 创建只读的表格项
    QTableWidgetItem* createReadOnlyItem(const QString& text) const;

private:
    // 成员变量
    QString m_playerName;        // 球员姓名
    QTableWidget* m_tableWidget; // 数据表格
    QLabel* m_titleLabel;        // 标题标签
    QPushButton* m_exportButton; // 导出按钮
    QLabel* m_averagesLabel;     // 平均值显示标签
};

#endif // PLAYERDATATABLE_H
