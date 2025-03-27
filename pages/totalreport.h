#ifndef TOTALREPORT_H
#define TOTALREPORT_H

#include <QWidget>
#include <qdatetime.h>
#include <QDate>
#include <QMessageBox>

#include "../managers/nm.h"
#include "../managers/reportoperations.h"
#include "../managers/reportoperations.h"
#include "../managers/verticalpdfmanager.h"
#include "../managers/verticalexcelmanager.h"

#include "../pages/calendarpage.h"
#include "../pages/generalreports.h"

namespace Ui {
class TotalReport;
}

class TotalReport : public QWidget
{
    Q_OBJECT

public:
    explicit TotalReport(nm *nav, QWidget *parent = nullptr);
    ~TotalReport();

    void setTotalReport(QDate from = QDate(), QDate to = QDate());

private slots:

    void on_BackButton_clicked();
    void updateData();
    void on_ToDateButton_clicked();
    void on_FromDateButton_clicked();

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void on_ToPDFButton_clicked();
    void on_ToEXCELButton_clicked();
    void on_detailButton_clicked();

signals:
    void datesChangedtotal(GeneralReportsE table, QDate from, QDate to);

private:
    Ui::TotalReport *ui;
    nm *nav;
    int selectedColumn;

    QDate fromDate;
    QDate toDate;

    void setTable();
    void setTableSizes();

    void setFromDate(QDate date);
    void setToDate(QDate date);

    void adjustColumnWidths();
};

#endif // TOTALREPORT_H
