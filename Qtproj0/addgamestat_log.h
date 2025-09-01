#ifndef ADDGAMESTAT_LOG_H
#define ADDGAMESTAT_LOG_H

#include <QDialog>
#include <QDate>
#include "datamanage.h"

namespace Ui {
class AddGameStat_Log;
}

class AddGameStat_Log : public QDialog
{
    Q_OBJECT

public:
    explicit AddGameStat_Log(DataManage* dataManager, QWidget *parent = nullptr);
    ~AddGameStat_Log();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::AddGameStat_Log *ui;
    DataManage* m_dataManager;
    
    bool validateInput();
    void setupUI();
};

#endif // ADDGAMESTAT_LOG_H
