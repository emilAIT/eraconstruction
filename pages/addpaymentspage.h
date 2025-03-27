#ifndef ADDPAYMENTSPAGE_H
#define ADDPAYMENTSPAGE_H

#include <QWidget>
#include <QDate>
#include <QTimer>

#include "../managers/nm.h"
#include "../enums/EventsE.h"
#include "calendarpage.h"
#include "../managers/operations.h"

namespace Ui {
class AddPaymentsPage;
}

class AddPaymentsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AddPaymentsPage(nm *nav, QWidget *parent = nullptr);
    ~AddPaymentsPage();

    void openWindow();

private slots:
    void on_oplatyButton_clicked();

    void on_backButton_clicked();

    void on_objectBox_currentIndexChanged(int index);
    void on_apartmentBox_currentIndexChanged(int index);

    void on_addEventButton_clicked();

    void on_dateButton_clicked();
    void onChangeDate(QDate date);

    void on_clientBox_currentIndexChanged(int index);

private:
    Ui::AddPaymentsPage *ui;
    nm *nav;
    EventsE table;
    QDate date;

    QMap<QString, int> clients;
    QMap<QString, int> objects;
    QMap<QString, int> apartments;
    QMap<QString, int> paymentsType;

    bool checkEventFill(int clientId, int objectId, int apartmentId);

    void updateMonthlyPayment();
    void updatePaymentDetails();
    void setComboBoxesData();
    void clearAllInputsEvent();
    void resetInputColor();
};

#endif // ADDPAYMENTSPAGE_H
