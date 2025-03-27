#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H



#include <QWidget>

#include "nm.h"
#include "../mainwindow.h"

#include "../pages/settings.h"
#include "../pages/settings2.h"
#include "../pages/addpaymentspage.h"
#include "../pages/events.h"
#include "../pages/generalreports.h"
#include "../pages/reports.h"
#include "../pages/totalreport.h"
#include "../pages/reportbyClient.h"
#include "../pages/reportbydebtor.h"

namespace Ui {
class navigationManager;
}

class navigationManager : public QWidget, public nm
{
    Q_OBJECT

public:
    explicit navigationManager(QWidget *parent = nullptr);
    ~navigationManager();

    void changeWindow(int id);
    bool openSettings(int id);
    bool openEvents(int id);
    bool openGeneralReports(int id);
    bool openReports(int index, int id = 0, QDate = QDate(), QDate = QDate());

    void disableReportsButton(bool isAdmin);


private:
    Ui::navigationManager *ui;
    std::function<void()> lastFunc;

    MainWindow *MainPage;
    Settings *SettingsPage;
    Settings2 *SettingsPage2;
    AddPaymentsPage *AddPaymentsPagee;
    Events *EventsPage;
    GeneralReports *GeneralReportsPage;
    Reports *ReportsPage;
    TotalReport *TotalReportPage;
    ReportByClient *ReportByClientPage;
    ReportByDebtor *ReportByDebtorPage;
};

#endif // NAVIGATIONMANAGER_H
