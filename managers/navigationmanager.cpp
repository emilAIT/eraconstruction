#include "navigationmanager.h"
#include "ui_navigationmanager.h"

navigationManager::navigationManager(QWidget *parent)
    : QWidget(parent), nm()
    , ui(new Ui::navigationManager)
{
    ui->setupUi(this);
    this->MainPage = new MainWindow(this);
    this->SettingsPage = new Settings(this);
    this->SettingsPage2 = new Settings2(this);
    this->AddPaymentsPagee = new AddPaymentsPage(this);
    this->EventsPage = new Events(this);
    this->GeneralReportsPage = new GeneralReports(this);
    this->ReportsPage = new Reports(this);
    this->TotalReportPage = new TotalReport(this);
    this->ReportByClientPage = new ReportByClient(this);
    this->ReportByDebtorPage = new ReportByDebtor(this);


    ui->pages->insertWidget(0, this->MainPage);
    ui->pages->insertWidget(1, this->SettingsPage);
    ui->pages->insertWidget(2, this->SettingsPage2);
    ui->pages->insertWidget(3, this->AddPaymentsPagee);
    ui->pages->insertWidget(4, this->EventsPage);
    ui->pages->insertWidget(5, this->GeneralReportsPage);
    ui->pages->insertWidget(6, this->ReportsPage);
    ui->pages->insertWidget(7, this->TotalReportPage);
    ui->pages->insertWidget(8,this->ReportByClientPage);
    ui->pages->insertWidget(9,this->ReportByDebtorPage);
}

navigationManager::~navigationManager()
{
    delete ui;
}


void navigationManager::changeWindow(int id)
{
    if (id == 0)
    {
        lastFunc = [this]() { this->MainPage->openWindow(); };
    }
    lastFunc();
    ui->pages->setCurrentIndex(id);
}

bool navigationManager::openSettings(int id) {
    switch (id) {
    case 0:
        lastFunc = [this]() { this->SettingsPage2->setSettings(SettingsE::Objects); };
        changeWindow(2);
        break;
    case 1:
        lastFunc = [this]() { this->SettingsPage2->setSettings(SettingsE::Expenses); };
        changeWindow(2);
        break;
    case 2:
        lastFunc = [this]() { this->SettingsPage->setSettings(SettingsE::Clients); };
        changeWindow(1);
        break;
    case 3:
        lastFunc = [this]() { this->SettingsPage->setSettings(SettingsE::PaymentTypes); };
        changeWindow(1);
        break;
    case 4:
        lastFunc = [this]() { this->SettingsPage->setSettings(SettingsE::Users); };
        changeWindow(1);
        break;
    default:
        return false;
        break;
    }
    lastFunc();
    return true;
}




bool navigationManager::openEvents(int id) {
    switch (id)
    {
    case 0:
        lastFunc = [this]() { this->EventsPage->setEvents(EventsE::Payments); };
        break;
    case 1:
        lastFunc = [this]() { this->EventsPage->setEvents(EventsE::Expenses); };
        break;
    case 2:
        lastFunc = [this]() { this->EventsPage->setEvents(EventsE::Sales); };
        break;
    case 3:
        lastFunc = [this]() {this->AddPaymentsPagee->openWindow();};
        changeWindow(3);
        return true;
        break;
    default:
        return false;
        break;
    };
    lastFunc();
    changeWindow(4);
    return true;
}



bool navigationManager::openGeneralReports(int id) {
    switch (id) {
    case 0:
        lastFunc = [this]() { this->GeneralReportsPage->setGeneralReports(GeneralReportsE::PaymentTypes);};
        changeWindow(5);
        break;
    case 1:
        lastFunc = [this]() { this->GeneralReportsPage->setGeneralReports(GeneralReportsE::Objects);};
        changeWindow(5);
        break;
    case 2:
        lastFunc = [this]() { this->GeneralReportsPage->setGeneralReports(GeneralReportsE::Expenses);};
        changeWindow(5);
        break;
    case 3:
        lastFunc = [this]() { this->GeneralReportsPage->setGeneralReports(GeneralReportsE::Clients);};
        changeWindow(5);
        break;
    case 4:
        lastFunc = [this]() { this->GeneralReportsPage->setGeneralReports(GeneralReportsE::Apartments);};
        changeWindow(5);
        break;
    case 5:
        lastFunc = [this]() { this->GeneralReportsPage->setGeneralReports(GeneralReportsE::Debts);};
        changeWindow(5);
        break;
    case 6:
        lastFunc = [this]() {this->TotalReportPage->setTotalReport();};
        changeWindow(7);
        break;
    default:
        return false;
    } lastFunc();  // Вызываем последнюю функцию
    return true;
}



bool navigationManager::openReports(int index, int id, QDate from, QDate to) {
    switch (index) {
    case 0:
        this->ReportsPage->setReports(ReportsE::PaymentTypes, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReports(ReportsE::PaymentTypes, id, from, to); };
        changeWindow(6);
        break;
    case 1:
        this->ReportsPage->setReports(ReportsE::Objects, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReports(ReportsE::Objects, id, from, to); };
        changeWindow(6);
        break;
    case 2:
        this->ReportsPage->setReports(ReportsE::Expenses, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReports(ReportsE::Expenses, id, from, to); };
        changeWindow(6);
        break;
    case 3:
        this->ReportByClientPage->setReportType(id, from, to);
        // lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->ReportByApartmentPage->setReportType(from, to); };
        changeWindow(8);
        break;
    case 4:
        this->ReportByDebtorPage->setReports(ReportsE::Apartments, id);
        lastFunc = [this, id](int newId = -2) { this->ReportByDebtorPage->setReports(ReportsE::Apartments, (newId == -2 ? id : newId)); };
        changeWindow(9);
        break;
    case 5:
        this->ReportByDebtorPage->setReports(ReportsE::Debts, id);
        lastFunc = [this, id](int newId = -2) { this->ReportByDebtorPage->setReports(ReportsE::Debts, (newId == -2 ? id : newId)); };

        changeWindow(9);
        break;
    default:
        return false;
        break;
    }
    // lastFunc();
    return true;
}



void navigationManager::disableReportsButton(bool isAdmin)
{
    if (!isAdmin)
        this->MainPage->setReportsAvailability(false);
}
