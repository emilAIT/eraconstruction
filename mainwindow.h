#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QTimer>
#include <QFile>
#include <QMessageBox>

#include "managers/operations.h"
#include "managers/nm.h"
#include "managers/contractwordmanager.h"
#include "pages/calendarpage.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(nm *nav, QWidget *parent = nullptr);
    ~MainWindow();

    void openWindow();
    void setReportsAvailability(bool isEnabled);

private slots:
    void on_settingsButton_currentIndexChanged(int index);
    void on_reportsButton_currentIndexChanged(int index);
    void on_objectBox_currentIndexChanged(int index);
    void on_apartmentBox_currentIndexChanged(int index);

    void on_paymentsButton_clicked();
    void on_expensesButton_clicked();
    void on_salesButton_clicked();

    void on_installmentCheckBox_stateChanged(int state);
    void on_costEdit_textChanged(const QString &arg1);
    void on_discountEdit_textChanged(const QString &arg1);

    void on_dateButton_clicked();
    void onChangeDate(QDate date);

    void on_addEventButton_clicked();

private:
    Ui::MainWindow *ui;
    nm *nav;
    QDate date;

    QMap<QString, int> clients;
    QMap<QString, int> objects;
    QMap<QString, int> apartments;
    QMap<QString, int> paymentsType;

    double currentApartmentPrice = 0.0;
    bool isAdmin = true;

    void setComboBoxesData();
    bool checkEventFill();
    void checkDebts();

    QVariantList getPriceForApartment(const QString &apartmentName);
    void updateTotalPrice();
    void updateMonthlyPayment();
    void clearAllInputsEvent();
    void resetInputColor();
};
#endif // MAINWINDOW_H
