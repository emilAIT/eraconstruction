#ifndef SETTINGS2_H
#define SETTINGS2_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelection>
#include <QSortFilterProxyModel>

#include "../managers/nm.h"
#include "../enums/SettingsE.h"
#include "../pages/addupdatesettings.h"
#include "../pages/customdialog.h"

namespace Ui {
class Settings2;
}

class Settings2 : public QWidget
{
    Q_OBJECT

public:
    explicit Settings2(nm *nav, QWidget *parent = nullptr);
    ~Settings2();

    void setSettings(SettingsE settings);

private slots:
    void onItemAdded(int itemId);

    void on_backButton_clicked();
    void on_AddButton_clicked();
    void on_addSubButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();

    void onYes();
    void onNo();

    void onTableViewSelectionChanged();
    void onSideTableSelectionChanged();

    void onSectionResized(int logicalIndex, int oldSize, int newSize);
private:
    Ui::Settings2 *ui;
    nm *nav;
    SettingsE table;
    int id;

    void setHeader();
    void setSideTable();
    void setTable();

    void adjustColumnWidths();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

};

#endif // SETTINGS2_H
