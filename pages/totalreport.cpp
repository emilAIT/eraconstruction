#include "totalreport.h"
#include "ui_totalreport.h"

TotalReport::TotalReport(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TotalReport)
{
    ui->setupUi(this);
    this->nav = nav;

    this->fromDate = QDate::currentDate().addDays(-1);
    this->toDate = QDate::currentDate();

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");

    ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));

    ui->FilterButton->setDisabled(true);

    connect(ui->tableView->verticalHeader(), &QHeaderView::sectionResized, this, &TotalReport::onSectionResized);

    setTotalReport(this->fromDate, this->toDate);
}

TotalReport::~TotalReport()
{
    delete ui;
}

void TotalReport::setTotalReport(QDate from, QDate to) {
    if (!from.isNull()) {
        this->fromDate = from;
        this->toDate = to;

        ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
        ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
    }

    setTable();
    setTableSizes();
}

void TotalReport::setTable() {
    auto *model = new QStandardItemModel(this);
    QLocale locale(QLocale::Russian);

    QVariantList clientsReport = ReportOperations::getTotalReport(this->fromDate, this->toDate);

    qDebug() << clientsReport;

    for (int i = 0; i < clientsReport.size(); ++i) {
        QVariantList clients = clientsReport[i].toList();

        if (clients[1].toString().isEmpty()) {
            model->appendRow(new QStandardItem(""));
        }
        else {
            model->appendRow(new QStandardItem(locale.toString(clients[1].toDouble(), 'f', 2))); // Отформатированная сумма
        }

        model->setVerticalHeaderItem(i, new QStandardItem(clients[0].toString()));
    }

    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TotalReport::setTableSizes() {
    // ui->tableView->setColumnWidth(0, 150);
    // ui->tableView->setColumnWidth(1, 150);
    // ui->tableView->setColumnWidth(2, 150);
    // ui->tableView->setColumnWidth(3, 150);
    // ui->tableView->setColumnWidth(4, 150);
    // ui->tableView->setColumnWidth(5, 150);
    // ui->tableView->setColumnWidth(6, 150);
    // ui->tableView->setColumnWidth(7, 150);
    // ui->tableView->setColumnWidth(8, 150);
    // ui->tableView->setColumnWidth(9, 180);
}

void TotalReport::updateData()
{
    setTable();
    setTableSizes();
}

void TotalReport::on_BackButton_clicked()
{
    nav->changeWindow(0);
}



void TotalReport::on_ToPDFButton_clicked()
{
    QAbstractItemModel *model = ui->tableView->model();

    if (!model) {
        QMessageBox::warning(this, "Ошибка", "Данные для экспорта отсутствуют.");
        return;
    }

    VerticalPDFmanager pdfManager;

    // Заголовок и даты отчета
    QString title = "Общий отчет";
    QString dates = QString("%1 - %2")
                        .arg(this->fromDate.toString("dd.MM.yyyy"))
                        .arg(this->toDate.toString("dd.MM.yyyy"));

    // Экспортируем данные таблицы в PDF
    pdfManager.ToVerticalPDF(title, dates, model);
}


void TotalReport::on_ToEXCELButton_clicked()
{
    QAbstractItemModel *model = ui->tableView->model();

    if (!model) {
        QMessageBox::warning(this, "Ошибка", "Данные для экспорта отсутствуют.");
        return;
    }

    // Создаем объект "вертикального" Excel-менеджера
    VerticalExcelManager excelManager;

    // Заголовок и даты отчета
    QString title = "Общий отчет";
    QString dates = QString("%1 - %2")
                        .arg(this->fromDate.toString("dd.MM.yyyy"))
                        .arg(this->toDate.toString("dd.MM.yyyy"));

    // Передаем модель
    excelManager.exportToExcelVertical(title, dates, model);
}


void TotalReport::on_detailButton_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите строку для перехода.");
        return;
    }

    QString description = ui->tableView->model()->headerData(currentIndex.row(), Qt::Vertical).toString();
    qDebug() << description;

    if (description == "Общий приход KGS:") {
        nav->openEvents(2);
    } else if (description == "Общий расход KGS:") {
        nav->openEvents(1);
    } else if (description == "Итого KGS:") {
        nav->openEvents(2);
    } else if (description == "Общий приход USD:") {
        nav->openEvents(2);
    } else if (description == "Общий расход USD:") {
        nav->openEvents(1);
    } else if (description == "Итого USD:") {
        nav->openEvents(2);
    } else {
        nav->openReports(0, 0, this->fromDate, this->toDate); // open reports by payment type
    }
}







void TotalReport::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromDate);

    connect(c, &CalendarPage::changeDate, this, &TotalReport::setFromDate);

    c->show();
}

void TotalReport::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->toDate);

    connect(c, &CalendarPage::changeDate, this, &TotalReport::setToDate);

    c->show();
}

void TotalReport::setFromDate(QDate date)
{
    this->fromDate = date;
    ui->FromDateButton->setText(date.toString("dd.MM.yyyy"));
    updateData();
}

void TotalReport::setToDate(QDate date)
{
    this->toDate = date;
    ui->ToDateButton->setText(date.toString("dd.MM.yyyy"));
    updateData();
}

void TotalReport::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void TotalReport::adjustColumnWidths()
{
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i)
    {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 1130 - totalWidth;
    if (lastColumnWidth > 150)
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    }
    else
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}

































