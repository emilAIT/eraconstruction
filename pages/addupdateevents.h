// #ifndef ADDUPDATEEVENTS_H
// #define ADDUPDATEEVENTS_H

// #include "qdatetime.h"
// #include <QWidget>

// #include "../pages/calendarpage.h"
// #include "../managers/operations.h"

// namespace Ui {
// class AddUpdateEvents;
// }

// class AddUpdateEvents : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit AddUpdateEvents(int id, QWidget *parent = nullptr);
//     ~AddUpdateEvents();

// signals:
//     void closed();

// private slots:
//     void on_addButton_clicked();
//     void on_objectBox_currentIndexChanged(int index);

//     void on_dateButton_clicked();
//     void onChangeDate(QDate date);

// private:
//     Ui::AddUpdateEvents *ui;
//     QDate date;
//     int id;

//     void addRecord();

//     QMap<QString, int> clients;
//     QMap<QString, int> objects;
//     QMap<QString, int> apartments;
//     QMap<QString, int> paymentsType;
// };

// #endif // ADDUPDATEEVENTS_H


#ifndef ADDUPDATEEVENTS_H
#define ADDUPDATEEVENTS_H

#include <QWidget>
#include <QDate>
#include <QMap>
#include <QTimer>

#include "../enums/EventsE.h"
#include "../pages/calendarpage.h"
#include "../managers/operations.h"

namespace Ui {
class AddUpdateEvents;
}

class AddUpdateEvents : public QWidget
{
    Q_OBJECT

public:
    explicit AddUpdateEvents(int id, EventsE eventType, QWidget *parent = nullptr);
    ~AddUpdateEvents();

signals:
    void closed();

private slots:
    void on_addButton_clicked();

    void on_clientBox_currentIndexChanged(int index);
    void on_objectBox_currentIndexChanged(int index);

    void on_dateButton_clicked();
    void onChangeDate(QDate date);

    void on_costEdit_textChanged(const QString &arg1);
    void on_discountEdit_textChanged(const QString &arg1);


    void on_apartmentBox_currentIndexChanged(int index);

private:
    Ui::AddUpdateEvents *ui;
    QDate date;
    int id;
    EventsE eventType;

    QMap<QString, int> clients;
    QMap<QString, int> objects;
    QMap<QString, int> apartments;
    QMap<QString, int> paymentsType;

    double currentApartmentPrice = 0.0;

    void loadSalesData();
    void loadPaymentData();

    bool checkEventFill(int clientId, int objectId, int apartmentId);
    void resetInputColor();

    QVariantList getPriceForApartment(int apartmentId);
    void updateTotalPrice();
    void updateMonthlyPayment();

    void setComboBoxesData();
};

#endif // ADDUPDATEEVENTS_H
