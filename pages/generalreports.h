#ifndef GENERALREPORTS_H
#define GENERALREPORTS_H

#include <QWidget>
#include "qstandarditemmodel.h"

#include "../managers/nm.h"
#include "../managers/reportoperations.h"
#include "../managers/excelmanager.h"
#include "../managers/pdfmanager.h"

#include "../enums/GeneralReportsE.h"
#include "../pages/calendarpage.h"
#include "../managers/operations.h"

namespace Ui {
class GeneralReports;
}

class GeneralReports : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralReports(nm *nav, QWidget *parent = nullptr);
    ~GeneralReports();

    void setGeneralReports(GeneralReportsE genera, QDate from = QDate(), QDate to = QDate());

private slots:
    void on_BackButton_clicked();
    void on_ReportButton_clicked();
    void on_SettingsButton_clicked();
    void on_FromDateButton_clicked();
    void on_ToDateButton_clicked();
    void on_ToPDFButton_clicked();
    void on_ToEXCELButton_clicked();
    void on_objectBox_currentIndexChanged(int index);

    void onSectionResized(int logicalIndex, int oldSize, int newSize);
    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);
    void updateData();

signals:
    void datesChanged(QDate fromDate, QDate toDate);

private:
    Ui::GeneralReports *ui;
    nm *nav;
    GeneralReportsE table;
    QDate fromDate;
    QDate toDate;
    Qt::SortOrder sortOrder;
    int selectedColumn;

    void setHeader();
    void setTable();
    void setBottomTable();
    void setTableSizes();
    void adjustColumnWidths();

    void setFromDate(QDate date);
    void setToDate(QDate date);

    QMap<QString, int> objects;
    int objectId;
};

#endif // GENERALREPORTS_H
