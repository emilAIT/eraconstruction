#ifndef ADDUPDATEOTHEREVENTS_H
#define ADDUPDATEOTHEREVENTS_H

#include <QWidget>
#include "../enums/EventsE.h"
#include "../pages/calendarpage.h"
#include "../managers/operations.h"

namespace Ui {
class AddUpdateOtherEvents;
}

class AddUpdateOtherEvents : public QWidget
{
    Q_OBJECT

public:
    explicit AddUpdateOtherEvents(EventsE eventType, int typeOperation, int id, QWidget *parent = nullptr);
    ~AddUpdateOtherEvents();

signals:
    void closed();

private slots:
    void on_dateButton_clicked();
    void onChangeDate(QDate date);

    void on_typeBox_currentIndexChanged(int index);

    void on_addButton_clicked();
    bool checkFill();
    void resetInputColor();
private:
    Ui::AddUpdateOtherEvents *ui;
    EventsE eventType;
    int id;
    int typeOperation;
    QDate date;

    QMap<QString, int> types;
    QMap<QString, int> whoWhats;
    QMap<QString, int> paymentTypes;

    void loadExpensesData();
};

#endif // ADDUPDATEOTHEREVENTS_H
