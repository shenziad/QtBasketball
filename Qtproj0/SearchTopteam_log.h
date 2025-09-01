#ifndef SEARCHTOPTEAM_LOG_H
#define SEARCHTOPTEAM_LOG_H

#include <QDialog>
#include "datamanage.h"

namespace Ui {
class SearchTopteam_Log;
}

class SearchTopteam_Log : public QDialog
{
    Q_OBJECT

public:
    explicit SearchTopteam_Log(DataManage* dataManager, QWidget *parent = nullptr);
    ~SearchTopteam_Log();

private slots:
    void on_teamComboBox_currentIndexChanged(const QString &team);
    void updateResults();

private:
    Ui::SearchTopteam_Log *ui;
    DataManage* m_dataManager;
    
    void setupUI();
};

#endif // SEARCHTOPTEAM_LOG_H
