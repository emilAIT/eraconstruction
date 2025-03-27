#include "reportbyClient.h"
#include "pages/ui_reportbyClient.h"
// #include "ui_reportbyClient.h"
#include "../enums/ReportsE.h"
#include "qsortfilterproxymodel.h"

ReportByClient::ReportByClient(nm *nav,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReportByClient)
{

    ui->setupUi(this);
    this->nav = nav;

    // this->fromDate = QDate::currentDate().addDays(-1);
    // this->toDate = QDate::currentDate();

    // ui->FromDateButton->setProperty("color", "gray");
    // ui->ToDateButton->setProperty("color", "gray");

    // ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    // ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
}

ReportByClient::~ReportByClient()
{
    delete ui;
}

void ReportByClient::setReportType(int id, QDate from, QDate to)
{
    ui->Header->setText("по клиенту");
    // ui->ToEXCELButton->setDisabled(true);

    this->selectedColumn = -1;

    // if (!from.isNull())
    // {
    //     this->fromDate = from;
    //     this->toDate = to;

    //     ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    //     ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
    // }
    this->id = id;
    this->saleId = 0;
    setSideTable();
    setTable();
    setBottomTable();
    setTableSizes();
    setTotalTable();

}

void ReportByClient::setTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);
    model->setHorizontalHeaderLabels({"ID", "Дата", "Объект", "Квартира", "Валюта", "Стоимость", "Скидка", "Итого", "Оплачено", "Статус", "Площадь м²", "Стоимость(м²)", "Кол-во комнат", "Кол-во санузел", "Этаж", "Долг"});

    for (const QVariant &rowData : ReportOperations::getClientApartments(this->id)) {
        QVariantList data = rowData.toList();
        QList<QStandardItem*> row;

        row << new QStandardItem(data[0].toString());


        QStandardItem *dateItem = new QStandardItem();
        QDate date = data[1].toDate();
        QString formattedDate = date.toString("dd.MM.yyyy"); // формат отображения, можно изменить
        dateItem->setData(formattedDate, Qt::DisplayRole);
        dateItem->setData(date, Qt::UserRole);
        row.append(dateItem);

        row << new QStandardItem(data[2].toString());
        row << new QStandardItem(data[3].toString());
        row << new QStandardItem(data[4].toString());
        row << new QStandardItem(locale.toString(data[5].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(data[6].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(data[7].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(data[8].toDouble(), 'f', 2));
        row << new QStandardItem(data[9].toString());
        row << new QStandardItem(locale.toString(data[10].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(data[11].toDouble(), 'f', 2));
        row << new QStandardItem(data[12].toString());
        row << new QStandardItem(data[13].toString());
        row << new QStandardItem(data[14].toString());
        row << new QStandardItem(locale.toString(data[15].toDouble(), 'f', 2));

        model->appendRow(row);
        for (QStandardItem *item : row) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }

    }

    auto *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::UserRole);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);
    ui->tableView->setSortingEnabled(true);

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ReportByClient::selectionTableChanged);

    if (this->selectedColumn != -1) {
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);
    }
}

void ReportByClient::setBottomTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);
    model->setHorizontalHeaderLabels({"Дата", "Тип оплаты", "Оплатил", "Описание"});

    QVariantList payments = ReportOperations::getClientsPaymentsForReportByClient(this->saleId);

    for (const QVariant &paymentData : payments) {
        QVariantList payment = paymentData.toList();

        QList<QStandardItem*> row;
        row.append(new QStandardItem(payment[0].toString())); // Дата
        row.append(new QStandardItem(payment[1].toString())); // Тип оплаты
        row << new QStandardItem(locale.toString(payment[2].toDouble(), 'f', 2)); // Оплатил
        row.append(new QStandardItem(payment[3].toString())); // Описание

        model->appendRow(row);
        for (QStandardItem *item : row) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }

    auto *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::UserRole);

    ui->bottomTable->setModel(model);
    ui->bottomTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->bottomTable->horizontalHeader()->setStretchLastSection(true);
    ui->bottomTable->horizontalHeader()->setCascadingSectionResizes(true);
    ui->bottomTable->setSortingEnabled(true);
}


void ReportByClient::setTotalTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);
    model->setHorizontalHeaderLabels({"Общая сумма"});
    for (const QVariant &paymentType : ReportOperations::getApartmentAllPayments(this->saleId)) {
        QVariantList data = paymentType.toList();

        QList<QStandardItem *> row;

        QStandardItem *item = new QStandardItem(locale.toString(data[0].toDouble(), 'f', 2));
        item->setTextAlignment(Qt::AlignCenter); // Выравнивание по центру
        item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Запрет на изменение
        row << item;
        model->appendRow(row);
    }

    ui->totalTable->setModel(model);
    ui->totalTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}



void ReportByClient::setSideTable() {
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderLabels({"id", "Клиенты"});

    QVariantList clients = ReportOperations::getClients();

    QModelIndex selectedIndex;  // Объявляем переменную для хранения индекса выбранной строки

    for (int row = 0; row < clients.size(); ++row) {
        QVariantList client = clients[row].toList();

        QList<QStandardItem*> rowItems;
        rowItems.append(new QStandardItem(client[0].toString())); // ID клиента
        rowItems.append(new QStandardItem(client[1].toString())); // Имя клиента
        model->appendRow(rowItems);

        // Если id клиента совпадает с выбранным, сохраняем индекс этой строки
        if (client[0].toInt() == this->id) {
            selectedIndex = model->index(row, 0); // Получаем индекс строки
        }
    }

    ui->sideTable->setModel(model);
    ui->sideTable->setColumnHidden(0, true); // Скрываем ID
    ui->sideTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Если индекс найден, выделяем строку
    if (selectedIndex.isValid()) {
        ui->sideTable->selectionModel()->select(selectedIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        ui->sideTable->scrollTo(selectedIndex); // Прокручиваем к выбранной строке
    }

    connect(ui->sideTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ReportByClient::selectionChanged);
}


void ReportByClient::setTableSizes() {
    ui->tableView->setColumnWidth(0, 130);
    ui->tableView->setColumnWidth(1, 100);
    ui->tableView->setColumnWidth(2, 100);
    ui->tableView->setColumnWidth(3, 120);
    ui->tableView->setColumnWidth(4, 90);
    ui->tableView->setColumnWidth(5, 180);
    ui->tableView->setColumnWidth(6, 110);
    ui->tableView->setColumnWidth(7, 180);
    ui->tableView->setColumnWidth(8, 180);
    ui->tableView->setColumnWidth(9, 120);
    ui->tableView->setColumnWidth(10, 160);
    ui->tableView->setColumnWidth(11, 180);
    ui->tableView->setColumnWidth(12, 180);
    ui->tableView->setColumnWidth(13, 180);
    ui->tableView->setColumnWidth(14, 80);
    ui->tableView->setColumnWidth(15, 180);
}

void ReportByClient::updateData()
{
    setTable();
    setBottomTable();
    setTableSizes();
    setTotalTable();
}

void ReportByClient::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void ReportByClient::on_SettingsButton_clicked()
{
    nav->openSettings(2);
}

void ReportByClient::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    int row = selected.indexes()[0].row();
    this->id = ui->sideTable->model()->index(row, 0).data().toInt();
    this->saleId = 0;
    updateData();
}

void ReportByClient::selectionTableChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (selected.indexes().isEmpty()) {
        return;
    }

    int row = selected.indexes()[0].row();
    this->saleId = ui->tableView->model()->index(row, 0).data().toInt();
    setBottomTable();
    setTableSizes();
    setTotalTable();
}



void ReportByClient::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order) {
    this->selectedColumn = logicalIndex;
    this->sortOrder = order;
}


void ReportByClient::on_ToPDFButton_clicked()
{
    QString title;
    int start = 1;

    for (QVariant client : Operations::selectAllClients()) {
        QVariantList values = client.toList();
        if (values[0].toInt() == this->id) {
            title = "Отчет по клиенту " + values[1].toString();
            start = 0;
        }
    }

    class CustomProxy : public QSortFilterProxyModel {
    public:
        CustomProxy(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
            QVariant original = QSortFilterProxyModel::data(index, role);
            if (role == Qt::DisplayRole) {
                return QString("<span style='font-size:8px; word-wrap:break-word;'>%1</span>")
                    .arg(original.toString());
            }
            return original;
        }

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
            QVariant original = QSortFilterProxyModel::headerData(section, orientation, role);
            if (role == Qt::DisplayRole) {
                return QString("<span style='font-size:8px; word-wrap:break-word;'>%1</span>")
                    .arg(original.toString());
            }
            return original;
        }
    };

    CustomProxy *proxyTop = new CustomProxy(this);
    proxyTop->setSourceModel(ui->tableView->model());
    CustomProxy *proxyBottom = new CustomProxy(this);
    proxyBottom->setSourceModel(ui->bottomTable->model());

    PdfByClientManager::exportToPDF(title, { proxyTop, proxyBottom }, start);
}





void ReportByClient::on_ToEXCELButton_clicked()
{
    // Создаем экземпляр ExcelManager
    // ExcelManager excelManager;

    QString title;
    int start = 1;
    for (QVariant client : Operations::selectAllClients()) {
        QVariantList values = client.toList();
        if (values[0].toInt() == this->id) {
            title = "Отчет по клиенту " + values[1].toString();
            start = 0;
        }
    }

    // Вызываем метод экспорта в Excel,
    // передавая заголовок, даты, модели таблиц и значение start
    ExcelByClientManager::exportToExcel(title, { ui->tableView->model(), ui->bottomTable->model() }, start);
}
