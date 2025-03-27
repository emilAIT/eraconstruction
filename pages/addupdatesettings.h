#ifndef ADDUPDATESETTINGS_H
#define ADDUPDATESETTINGS_H

#include <QWidget>
#include "qdatetime.h"
#include <QMessageBox>

#include "../scripts/hash.h"
#include "../enums/AddUpdateSettingsE.h"
#include "../managers/operations.h"
#include "../pages/calendarpage.h"

namespace Ui {
class AddUpdateSettings;
}

class AddUpdateSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AddUpdateSettings(AddUpdateSettingsE mode, int id, QWidget *parent = nullptr);
    ~AddUpdateSettings();

signals:
    void closed();
    void objectAdded(int clientId);


private slots:
    void on_addButton_clicked();
    void calculateTotalAmount();
    bool checkFill();
    void resetInputColor();

    void on_DateOfBirthButton_clicked();
    void changeDateOfBirthDate(QDate date);

    void on_dateOfIssueButton_clicked();
    void changedateOfIssueDate(QDate date);

private:
    Ui::AddUpdateSettings *ui;
    AddUpdateSettingsE mode;
    QDate from;
    QDate to;

    QDate Bdate;
    QDate Issuedate;

    int id;

    void addRecord();
    // void resetInputColor();
    // bool checkFill();

    QMap<QString, int> types;
    QMap<QString, int> objects;
    // QMap<QString, int> clients;
    // QMap<QString, int> marks;
};

#endif // ADDUPDATESETTINGS_H
