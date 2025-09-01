#ifndef SEARCHAVE_LOG_H
#define SEARCHAVE_LOG_H

#include <QDialog>
#include "datamanage.h"

namespace Ui {
class SearchAve_Log;
}

class SearchAve_Log : public QDialog
{
    Q_OBJECT

public:
    explicit SearchAve_Log(DataManage* dataManager, QWidget *parent = nullptr);
    ~SearchAve_Log();

private slots:
    void on_categoryComboBox_currentIndexChanged(const QString &category);
    void updateResults();

private:
    Ui::SearchAve_Log *ui;
    DataManage* m_dataManager;
    
    void setupUI();
    QString getCategoryCode(const QString& displayName) const;
};

#endif // SEARCHAVE_LOG_H
