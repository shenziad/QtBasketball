#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>

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
    Ui::MainWindow *ui;
    DataManage* m_dataManager;
    
    void createMenus();
    void setupConnections();
    void updateDisplay();
    void initializeTables();  // 初始化表格
    
    // 文件名常量
    static const QString GAME_STATS_FILE;
    static const QString SUMMARY_STATS_FILE;
    
    // 表格操作辅助函数
    QTableWidgetItem* createReadOnlyItem(const QString& text);
    void setupTableWidget(QTableWidget* table);
};

#endif // MAINWINDOW_H
