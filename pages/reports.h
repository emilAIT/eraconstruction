#ifndef REPORTS_H
#define REPORTS_H

#include <QWidget>
#include <QDate>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelection>

#include "../managers/reportoperations.h"
#include "../managers/operations.h"
#include "../managers/nm.h"
#include "../managers/pdfmanager.h"
#include "../managers/excelmanager.h"

#include "../enums/ReportsE.h"
#include "../pages/calendarpage.h"


namespace Ui {
class Reports;
}

class Reports : public QWidget
{
    Q_OBJECT

public:
    explicit Reports(nm *nav, QWidget *parent = nullptr);
    ~Reports();

    void setReports(ReportsE table, int id, QDate from = QDate(), QDate to = QDate());

private slots:
    void on_BackButton_clicked();
    void on_SettingsButton_clicked();
    void on_FromDateButton_clicked();
    void on_ToDateButton_clicked();
    void on_ToEXCELButton_clicked();
    void on_ToPDFButton_clicked();

    void onSectionResized(int logicalIndex, int oldSize, int newSize);
    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void updateData();

private:
    Ui::Reports *ui;
    nm *nav;
    ReportsE table;
    QDate fromDate;
    QDate toDate;
    int id;
    int selectedColumn;

    void setHeader();
    void setTable();
    void setBottomTable();
    void setSideTable();
    void setTableSizes();

    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void setFromDate(QDate date);
    void setToDate(QDate date);
    void adjustColumnWidths();
    Qt::SortOrder sortOrder;



};

#endif // REPORTS_H
