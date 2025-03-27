#ifndef EVENTS_H
#define EVENTS_H

#include <QWidget>
#include "QMessageBox"
#include "QStandardItemModel"

#include "../managers/nm.h"
#include "../managers/operations.h"
#include "../managers/contractwordmanager.h"
#include "../enums/EventsE.h"
#include "../pages/addupdateevents.h"
#include "../pages/customdialog.h"
#include "../pages/addupdateotherevents.h"
#include "../managers/nakladnayapaymentspdf.h"
#include "../managers/nakladnayaexpensepdf.h"


namespace Ui {
class Events;
}

class Events : public QWidget
{
    Q_OBJECT

public:
    explicit Events(nm *nav, QWidget *parent = nullptr);
    ~Events();

    void setEvents(EventsE events);


private slots:
    void on_BackButton_clicked();
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();

    void onfromDateChange(QDate date);
    void ontoDateChange(QDate date);
    void on_weekButton_clicked();
    void on_monthButton_clicked();
    void on_threeMonthButton_clicked();
    void on_comboBox_currentIndexChanged(int index);

    void on_FromDateButton_clicked();
    void on_ToDateButton_clicked();

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void onYes();
    void onNo();

    void on_AddButton_clicked();
    void on_ReportButton_clicked();
    void on_CreateContractButton_clicked();


    void on_NakladnayaButton_clicked();

private:
    Ui::Events *ui;
    nm *nav;
    EventsE table;
    QDate date;
    QDate fromdate;
    QDate todate;
    int aparId;

    QStandardItemModel* model;
    QLocale* locale;
    void setHeader();
    void setTable();
    void adjustColumnWidths();

    QMap<QString, int> apartments;
    QMap<QString, int> clients;
    QMap<QString, int> objects;
};

#endif // EVENTS_H
