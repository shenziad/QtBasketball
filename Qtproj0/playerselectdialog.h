#ifndef PLAYERSELECTDIALOG_H
#define PLAYERSELECTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QMap>
#include <QStringList>
#include "datamanage.h"
#include "playerstats.h"

class PlayerSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerSelectDialog(DataManage* dataManager, QWidget *parent = nullptr);
    
    // 获取选中的球员列表
    QVector<PlayerStats> getSelectedPlayers() const;
    
    // 设置已选中的球员
    void setSelectedPlayers(const QVector<PlayerStats>& selectedPlayers);

private slots:
    void selectAll();
    void clearAll();
    void selectByTeam();

private:
    void setupUI();
    void setupPresetPlayers();
    
    DataManage* m_dataManager;
    QVBoxLayout* m_mainLayout;
    QScrollArea* m_scrollArea;
    QWidget* m_scrollWidget;
    QGridLayout* m_playersLayout;
    
    QHBoxLayout* m_buttonLayout;
    QPushButton* m_selectAllBtn;
    QPushButton* m_clearAllBtn;
    QPushButton* m_selectByTeamBtn;
    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
    
    // 球员复选框，key为球员姓名，value为对应的复选框
    QMap<QString, QCheckBox*> m_playerCheckBoxes;
    
    // 球员队伍映射
    QMap<QString, QString> m_playerTeamMap;
    
    // 按队伍分组的球员
    QMap<QString, QStringList> m_teamPlayersMap;
};

#endif // PLAYERSELECTDIALOG_H
