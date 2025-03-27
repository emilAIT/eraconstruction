#ifndef REPORTBYDEBTOR_H
#define REPORTBYDEBTOR_H

#include <QWidget>
#include <QDate>
#include "../managers/nm.h"
#include "../enums/ReportsE.h"
#include "qstandarditemmodel.h"
#include "../managers/reportoperations.h"
#include "qsortfilterproxymodel.h"
#include "../managers/operations.h"
#include "../managers/excelfordetailedpages.h"
#include "../managers/pdffordetailedpages.h"

namespace Ui {
class ReportByDebtor;
}

class ReportByDebtor : public QWidget
{
    Q_OBJECT

public:
    explicit ReportByDebtor(nm *nav,QWidget *parent = nullptr);
    ~ReportByDebtor();

    void setReports(ReportsE table, int id);

private slots:
    void on_BackButton_clicked();
    void on_SettingsButton_clicked();
    void on_ToEXCELButton_clicked();
    void on_ToPDFButton_clicked();

private:
    Ui::ReportByDebtor *ui;
    nm *nav;
    ReportsE table;
    int id;
    int selectedColumn;
    int monthQuantity;
    double monthlypayment;
    int isEnd;

    void setTable();
    void setHeader();
    void setBottomTable();
    void setTableSizes();
    Qt::SortOrder sortOrder;
};

#endif // REPORTBYDEBTOR_H
