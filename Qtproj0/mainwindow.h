#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datamanage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

private:
    Ui::MainWindow *ui;
    DataManage* m_dataManager;
    
    void createMenus();
    void setupConnections();
    void updateDisplay();
    
    // 文件名常量
    const QString GAME_STATS_FILE = "game_stats.dat";
    const QString SUMMARY_STATS_FILE = "summary_stats.dat";
};

#endif // MAINWINDOW_H
