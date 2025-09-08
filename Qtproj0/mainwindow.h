#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include <QVector>
#include <QPair>
#include <QDir>
#include <QMessageBox>
#include "avatargenerator.h"

QT_BEGIN_NAMESPACE
class QTableWidget;
class QTableWidgetItem;
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class DataManage;  // 前向声明

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 文件菜单
    void on_actionNewGame_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    
    // 查询菜单
    void on_actionShowSummary_triggered();
    void on_actionTopThree_triggered();
    void on_actionTeamTopThree_triggered();

private slots:
    void onDataChanged();  // 响应数据变化的槽函数

private:
    // 静态常量
    static inline const QString GAME_STATS_FILE = QStringLiteral("game_stats.dat");
    static inline const QString SUMMARY_STATS_FILE = QStringLiteral("summary_stats.dat");
    
    // 成员变量
    DataManage* m_dataManager;            // 数据管理器
    Ui::MainWindow *ui;                   // UI对象
    QVector<QPair<QString, QString>> m_presetPlayers; // 预设球员列表 <姓名, 队伍>
    
    // 成员函数
    void createMenus();
    void setupConnections();
    void updateDisplay();
    void initializeTables();              // 初始化表格
    void showPlayerDetailedStats(const QString& playerName); // 显示球员详细统计
    void initializePresetData();          // 初始化预设球员和队伍
    
    // 表格操作辅助函数
    QTableWidgetItem* createReadOnlyItem(const QString& text);
    void setupTableWidget(QTableWidget* table);
};

#endif // MAINWINDOW_H
