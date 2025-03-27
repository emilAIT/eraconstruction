#ifndef REPORTBYCLIENT_H
#define REPORTBYCLIENT_H

#include <QWidget>
#include <QDate>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelection>

#include "../managers/nm.h"
#include "../pages/calendarpage.h"
#include "../managers/reportoperations.h"
#include "../managers/operations.h"
#include "../managers/pdfbyclientmanager.h"
#include "../managers/excelbyclientmanager.h"

namespace Ui {
class ReportByClient;
}

class ReportByClient : public QWidget
{
    Q_OBJECT

public:
    explicit ReportByClient(nm *nav,QWidget *parent = nullptr);
    ~ReportByClient();

    void setReportType(int id, QDate from = QDate(), QDate to = QDate());

private slots:
    void on_BackButton_clicked();
    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void on_SettingsButton_clicked();
    // void on_FromDateButton_clicked();
    // void on_ToDateButton_clicked();

    void updateData();

    void on_ToEXCELButton_clicked();

    void on_ToPDFButton_clicked();

private:
    Ui::ReportByClient *ui;
    nm *nav;
    // QDate fromDate;
    // QDate toDate;
    int selectedColumn;
    int id;
    int saleId;

    void setTable();
    void setBottomTable();
    void setSideTable();
    void setTableSizes();
    void setTotalTable();

    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void selectionTableChanged(const QItemSelection &selected, const QItemSelection &deselected);
    // void setFromDate(QDate date);
    // void setToDate(QDate date);

    Qt::SortOrder sortOrder;
};

#endif // REPORTBYCLIENT_H
