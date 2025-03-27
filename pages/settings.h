#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "QStandardItemModel"



#include "../enums/SettingsE.h"
#include "../managers/nm.h"
#include "../managers/operations.h"
#include "../pages/customdialog.h"

#include "addupdatesettings.h"


namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(nm *nav, QWidget *parent = nullptr);
    ~Settings();

    void setSettings(SettingsE settings);

private slots:
    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void on_backButton_clicked();
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();

    void onYes();
    void onNo();

private:
    Ui::Settings *ui;
    nm *nav;
    SettingsE table;

    void setHeader();
    void setTable();

    void adjustColumnWidths();
};

#endif // SETTINGS_H
