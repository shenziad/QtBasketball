#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include "datamanage.h"

class TestAutoTeamSelection : public QWidget
{
    Q_OBJECT

public:
    TestAutoTeamSelection() 
    {
        setupUI();
        setupData();
        connect(nameEdit, &QLineEdit::textChanged, this, &TestAutoTeamSelection::onNameChanged);
    }

private slots:
    void onNameChanged() {
        QString name = nameEdit->text().trimmed();
        if (name.isEmpty()) {
            statusLabel->setText("请输入球员姓名");
            return;
        }
        
        // 检查预设球员
        QVector<QPair<QString, QString>> presetPlayers = dataManager->getPresetPlayers();
        for (const auto& player : presetPlayers) {
            if (player.first == name) {
                teamCombo->setCurrentText(player.second);
                statusLabel->setText(QString("找到预设球员: %1 (%2)").arg(name).arg(player.second));
                return;
            }
        }
        
        // 检查数据库中的球员
        if (dataManager->isPlayerExists(name)) {
            const PlayerStatsSummary* summary = dataManager->getPlayerSummary(name);
            if (summary) {
                teamCombo->setCurrentText(summary->team);
                statusLabel->setText(QString("找到已存在球员: %1 (%2)").arg(name).arg(summary->team));
                return;
            }
        }
        
        statusLabel->setText(QString("新球员: %1").arg(name));
    }

private:
    void setupUI() {
        setWindowTitle("测试自动队伍选择");
        resize(400, 200);
        
        QVBoxLayout* layout = new QVBoxLayout(this);
        
        layout->addWidget(new QLabel("球员姓名:"));
        nameEdit = new QLineEdit;
        layout->addWidget(nameEdit);
        
        layout->addWidget(new QLabel("所属队伍:"));
        teamCombo = new QComboBox;
        teamCombo->setEditable(true);
        layout->addWidget(teamCombo);
        
        statusLabel = new QLabel("请输入球员姓名");
        layout->addWidget(statusLabel);
    }
    
    void setupData() {
        dataManager = new DataManage(this);
        
        // 设置预设队伍
        QStringList teams = {"勇士队", "湖人队", "凯尔特人队", "独行侠队", "火箭队", "热火队", "公牛队", "76人队"};
        dataManager->setPresetTeams(teams);
        teamCombo->addItems(teams);
        
        // 设置预设球员
        QVector<QPair<QString, QString>> players = {
            {"斯蒂芬·库里", "勇士队"},
            {"勒布朗·詹姆斯", "湖人队"},
            {"杰森·塔图姆", "凯尔特人队"},
            {"卢卡·东契奇", "独行侠队"}
        };
        dataManager->setPresetPlayers(players);
        
        // 添加一些测试数据到数据库
        PlayerStats testStat("测试球员", "测试队", 3, 8, 2, 1, 25, QDate::currentDate(), 1);
        dataManager->addGameStats(testStat);
    }

private:
    QLineEdit* nameEdit;
    QComboBox* teamCombo;
    QLabel* statusLabel;
    DataManage* dataManager;
};

#include "test_auto_team.moc"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    TestAutoTeamSelection test;
    test.show();
    
    return app.exec();
}
