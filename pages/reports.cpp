#include "reports.h"
#include "pages/ui_reports.h"
#include "qsortfilterproxymodel.h"
// #include "ui_reports.h"

Reports::Reports(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reports)
{
    ui->setupUi(this);
    this->nav = nav;

    this->fromDate = QDate::currentDate().addDays(-1);
    this->toDate = QDate::currentDate();

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");

    ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));

}

Reports::~Reports()
{
    delete ui;
}



void Reports::setReports(ReportsE table, int id, QDate from, QDate to) {
    this->selectedColumn = -1;

    this->table = table;

    if (!from.isNull())
    {
        this->fromDate = from;
        this->toDate = to;

        ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
        ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
    }

    if (id > -2)
    {
        this->id = id;
    }
    setHeader();
    setSideTable();

    if (id != 0)
    {
        setTable();
        setBottomTable();
        setTableSizes();
    }
    else
    {
        QStandardItemModel *model = new QStandardItemModel();
        ui->tableView->setModel(model);
    }
}



void Reports::setHeader() {
    QString header = "header";
    switch (this->table) {
    case ReportsE::PaymentTypes:
        header = "ПО ТИПУ ОПЛАТЫ";
        break;
    case ReportsE::Objects:
        header = "ПО ОБЪЕКТУ";
        break;
    case ReportsE::Expenses:
        header = "ПО РАСХОДУ";
        break;
    default:
        break;
    }
    ui->Header->setText(header);
}




void Reports::setTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);

    switch (this->table) {
    case ReportsE::PaymentTypes:
        model->setHorizontalHeaderLabels({"Дата", "Источник", "Валюта", "Сумма", "Описание"});
        for (const QVariant &mark : ReportOperations::getPaymentTypesReportByItem(this->id, this->fromDate, this->toDate)) {
            QVariantList data = mark.toList();
            QList<QStandardItem*> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(data[1].toDate(), Qt::DisplayRole);
            row << dateItem;

            row << new QStandardItem(data[4].toString());
            row << new QStandardItem(data[6].toString());

            double amount = data[3].toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(amountStr, Qt::DisplayRole);
            amountItem->setData(amount, Qt::UserRole);
            row << amountItem;

            row << new QStandardItem(data[5].toString());

            model->appendRow(row);
        }
        break;

    case ReportsE::Objects:
        model->setHorizontalHeaderLabels({"Клиент", "Номер квартиры", "Валюта", "Стоимость", "Скидка", "Итого",
                                          "Статус", "Приход", "Площадь", "Стоимость (м²)",
                                          "Количество комнат", "Количество санузлов", "Этаж"});

        for (const QVariant &row : ReportOperations::getObjectsReportByItem(this->id, this->fromDate, this->toDate)) {
            QVariantList data = row.toList();
            QList<QStandardItem*> items;

            items << new QStandardItem(data[0].toString());  // Клиент
            items << new QStandardItem(data[1].toString());  // Номер квартиры
            items << new QStandardItem(data[12].toString());

            double price = data[2].toDouble();
            QString priceStr = locale.toString(price, 'f', 2);
            QStandardItem *priceItem = new QStandardItem();
            priceItem->setData(priceStr, Qt::DisplayRole);
            priceItem->setData(price, Qt::UserRole);
            items << priceItem;

            double discount = data[3].toDouble();
            QString discountStr = locale.toString(discount, 'f', 2);
            QStandardItem *discountItem = new QStandardItem();
            discountItem->setData(discountStr, Qt::DisplayRole);
            discountItem->setData(discount, Qt::UserRole);
            items << discountItem;

            double total = data[4].toDouble();
            QString totalStr = locale.toString(total, 'f', 2);
            QStandardItem *totalItem = new QStandardItem();
            totalItem->setData(totalStr, Qt::DisplayRole);
            totalItem->setData(total, Qt::UserRole);
            items << totalItem;

            items << new QStandardItem(data[5].toString());

            double payment = data[6].toDouble();
            QString paymentStr = locale.toString(payment, 'f', 2);
            QStandardItem *paymentItem = new QStandardItem();
            paymentItem->setData(paymentStr, Qt::DisplayRole);
            paymentItem->setData(payment, Qt::UserRole);
            items << paymentItem;

            double area = data[7].toDouble();
            QString areaStr = locale.toString(area, 'f', 2);
            QStandardItem *areaItem = new QStandardItem();
            areaItem->setData(areaStr, Qt::DisplayRole);
            areaItem->setData(area, Qt::UserRole);
            items << areaItem;

            double pricePerM2 = data[8].toDouble();
            QString pricePerM2Str = locale.toString(pricePerM2, 'f', 2);
            QStandardItem *pricePerM2Item = new QStandardItem();
            pricePerM2Item->setData(pricePerM2Str, Qt::DisplayRole);
            pricePerM2Item->setData(pricePerM2, Qt::UserRole);
            items << pricePerM2Item;

            int rooms = data[9].toInt();
            QStandardItem *roomsItem = new QStandardItem();
            roomsItem->setData(rooms, Qt::DisplayRole);
            roomsItem->setData(rooms, Qt::UserRole);
            items << roomsItem;

            int bathrooms = data[10].toInt();
            QStandardItem *bathroomsItem = new QStandardItem();
            bathroomsItem->setData(bathrooms, Qt::DisplayRole);
            items << bathroomsItem;

            int floor = data[11].toInt();
            QStandardItem *floorItem = new QStandardItem();
            floorItem->setData(floor, Qt::DisplayRole);
            items << floorItem;

            model->appendRow(items);
        }
        break;

    case ReportsE::Expenses:
        model->setHorizontalHeaderLabels({"Дата", "Кто/Что", "Сумма KGS", "Сумма USD", "Описание"});
        for (const QVariant &mark : ReportOperations::getExpensesReportByItem(this->id, this->fromDate, this->toDate)) {
            QVariantList data = mark.toList();
            QList<QStandardItem*> row;

            // Дата
            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(data[1].toDate(), Qt::DisplayRole);
            row << dateItem;

            // Кто/Что
            row << new QStandardItem(data[3].toString());

            // Сумма KGS
            double amountKGS = data[5].toDouble();
            QStandardItem *amountKGSItem = new QStandardItem(locale.toString(amountKGS, 'f', 2));
            amountKGSItem->setTextAlignment(Qt::AlignCenter);
            row << amountKGSItem;

            // Сумма USD
            double amountUSD = data[6].toDouble();
            QStandardItem *amountUSDItem = new QStandardItem(locale.toString(amountUSD, 'f', 2));
            amountUSDItem->setTextAlignment(Qt::AlignCenter);
            row << amountUSDItem;

            // Описание
            row << new QStandardItem(data[7].toString());

            model->appendRow(row);
        }
        break;

    default:
        break;
    }

    auto *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::UserRole);

    ui->tableView->setModel(proxyModel);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    if (this->selectedColumn != -1)
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);
}



void Reports::setBottomTable()
{
    if (this->id > 0) {
        QStandardItemModel *model = new QStandardItemModel();
        QLocale locale(QLocale::Russian);
        switch (this->table)
        {
        case ReportsE::PaymentTypes:
            model->setHorizontalHeaderLabels({"Общая Сумма KGS", "Общая Сумма USD"});  // Два столбца для KGS и USD
            for (const QVariant &paymentType : ReportOperations::getAllPaymentTypesReportByItem(this->id, this->fromDate, this->toDate)) {
                QVariantList data = paymentType.toList();

                QList<QStandardItem *> row;

                // Добавляем сумму для KGS в первый столбец
                QStandardItem *itemKGS = new QStandardItem(locale.toString(data[0].toDouble(), 'f', 2));  // Сумма KGS
                itemKGS->setTextAlignment(Qt::AlignCenter);  // Выравнивание по центру
                row << itemKGS;

                // Добавляем сумму для USD во второй столбец
                QStandardItem *itemUSD = new QStandardItem(locale.toString(data[1].toDouble(), 'f', 2));  // Сумма USD
                itemUSD->setTextAlignment(Qt::AlignCenter);  // Выравнивание по центру
                row << itemUSD;

                model->appendRow(row);  // Добавляем строку в модель
            }
            break;

        case ReportsE::Objects:
        {
            model->setHorizontalHeaderLabels({
                "Скидка KGS", "Скидка USD",
                "Итог KGS", "Итог USD",
                "Приход KGS", "Приход USD"
            });

            double totalCostKGS = 0, totalCostUSD = 0;
            double totalDiscountKGS = 0, totalDiscountUSD = 0;
            double totalFinalKGS = 0, totalFinalUSD = 0;
            double totalIncomeKGS = 0, totalIncomeUSD = 0;

            for (const QVariant &objectReport : ReportOperations::getAllObjectsReportByItem(this->id, this->fromDate, this->toDate)) {
                QVariantList data = objectReport.toList();
                if (data.size() < 5) continue; // Проверяем, что данные корректные

                QString currency = data[0].toString();
                double totalCost = data[1].toDouble();
                double totalDiscount = data[2].toDouble();
                double totalFinal = data[3].toDouble();
                double totalIncome = data[4].toDouble();

                if (currency == "KGS") {
                    totalCostKGS = totalCost;
                    totalDiscountKGS = totalDiscount;
                    totalFinalKGS = totalFinal;
                    totalIncomeKGS = totalIncome;
                } else if (currency == "USD") {
                    totalCostUSD = totalCost;
                    totalDiscountUSD = totalDiscount;
                    totalFinalUSD = totalFinal;
                    totalIncomeUSD = totalIncome;
                }
            }

            QList<QStandardItem *> row;
            // row << new QStandardItem(locale.toString(totalCostKGS, 'f', 2));
            // row << new QStandardItem(locale.toString(totalCostUSD, 'f', 2));
            row << new QStandardItem(locale.toString(totalDiscountKGS, 'f', 2));
            row << new QStandardItem(locale.toString(totalDiscountUSD, 'f', 2));
            row << new QStandardItem(locale.toString(totalFinalKGS, 'f', 2));
            row << new QStandardItem(locale.toString(totalFinalUSD, 'f', 2));
            row << new QStandardItem(locale.toString(totalIncomeKGS, 'f', 2));
            row << new QStandardItem(locale.toString(totalIncomeUSD, 'f', 2));

            for (QStandardItem *item : row) {
                item->setTextAlignment(Qt::AlignCenter);
            }

            model->appendRow(row);
        }
            break;

        case ReportsE::Expenses:
            model->setHorizontalHeaderLabels({"Общая сумма KGS", "Общая сумма USD"});
            for (const QVariant &expense : ReportOperations::getAllExpensesReportByItem(this->id, this->fromDate, this->toDate)) {
                QVariantList data = expense.toList();
                QList<QStandardItem *> row;

                // Сумма KGS
                QStandardItem *amountKGSItem = new QStandardItem(locale.toString(data[0].toDouble(), 'f', 2));
                amountKGSItem->setTextAlignment(Qt::AlignCenter);
                row << amountKGSItem;

                // Сумма USD
                QStandardItem *amountUSDItem = new QStandardItem(locale.toString(data[1].toDouble(), 'f', 2));
                amountUSDItem->setTextAlignment(Qt::AlignCenter);
                row << amountUSDItem;

                model->appendRow(row);
            }
            break;

        default:
            break;
        }

        ui->bottomTable->setModel(model);
        ui->bottomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        // ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->bottomTable->horizontalHeader()->setStretchLastSection(true);
        ui->bottomTable->horizontalHeader()->setCascadingSectionResizes(true);

    }
    else
    {
        ui->bottomTable->setModel(new QStandardItemModel());
    }
}



void Reports::setSideTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    int row = 0;
    switch (this->table) {
    case ReportsE::PaymentTypes:
        model->setHorizontalHeaderLabels({"id", "Тип оплаты"});
        for (QVariant paymentType : Operations::selectAllPaymentTypes()) {
            QVariantList values = paymentType.toList();
            if (this->id != 0 && values[0].toInt() == this->id)
                row = model->rowCount();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[1].toString()),
            });
        }
        break;

    case ReportsE::Objects:
        model->setHorizontalHeaderLabels({"id", "Объект"});
        for (QVariant object : Operations::selectAllObjects()) {
            QVariantList values = object.toList();
            if (this->id != 0 && values[0].toInt() == this->id)
                row = model->rowCount();
            model->appendRow({
                new QStandardItem(values[0].toString()), // ID объекта
                new QStandardItem(values[1].toString())  // Название объекта
            });
        }
        break;

    case ReportsE::Expenses:
        model->setHorizontalHeaderLabels({"id", "Тип"});
        for (QVariant types : Operations::selectAllTypes()) {
            QVariantList values = types.toList();
            if (this->id != 0 && values[0].toInt() == this->id)
                row = model->rowCount();
            model->appendRow({
                new QStandardItem(values[0].toString()),
                new QStandardItem(values[1].toString())
            });
        }
        break;
    }

    ui->sideTable->setModel(model);

    if (this->id != 0)
        ui->sideTable->selectRow(row);

    ui->sideTable->setColumnHidden(0, true);
    ui->sideTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui->sideTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &Reports::selectionChanged);
}


void Reports::setTableSizes() {
    switch (this->table)
    {
    case ReportsE::PaymentTypes:
        ui->tableView->setColumnWidth(0, 150);
        ui->tableView->setColumnWidth(1, 270);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 210);

        ui->bottomTable->setColumnWidth(0, 550);
        ui->bottomTable->setColumnWidth(1, 550);

        ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        break;
    case ReportsE::Objects:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 200);
        ui->tableView->setColumnWidth(7, 200);
        ui->tableView->setColumnWidth(8, 200);
        ui->tableView->setColumnWidth(9, 200);
        ui->tableView->setColumnWidth(10, 250);
        ui->tableView->setColumnWidth(11, 250);
        ui->tableView->setColumnWidth(12, 150);

        ui->bottomTable->setColumnWidth(0, 180);
        ui->bottomTable->setColumnWidth(1, 180);
        ui->bottomTable->setColumnWidth(2, 180);
        ui->bottomTable->setColumnWidth(3, 180);
        ui->bottomTable->setColumnWidth(4, 180);
        ui->bottomTable->setColumnWidth(5, 180);

        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        break;
    case ReportsE::Expenses:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 210);
        ui->tableView->setColumnWidth(2, 210);
        ui->tableView->setColumnWidth(3, 210);

        ui->bottomTable->setColumnWidth(0, 550);
        ui->bottomTable->setColumnWidth(1, 550);
        break;
    default:
        break;
    }
}




void Reports::on_BackButton_clicked()
{
    nav->changeWindow(0);
}


void Reports::on_SettingsButton_clicked()
{
    switch (this->table) {
    case ReportsE::PaymentTypes:
        nav->openSettings(3);
        break;
    case ReportsE::Objects:
        nav->openSettings(0);
        break;
    case ReportsE::Expenses:
        nav->openSettings(1);
        break;
    default:
        break;
    }
}


void Reports::on_ToPDFButton_clicked()
{
    QString title;
    int start = 1;

    switch (this->table)
    {
    case ReportsE::PaymentTypes:
        for (QVariant paymentType : Operations::selectAllPaymentTypes()) {
            QVariantList values = paymentType.toList();
            if (values[0].toInt() == this->id) {
                title = "Отчет по типу оплаты " + values[1].toString();
                start = 0;
            }
        }
        break;
    case ReportsE::Objects:
        for (QVariant paymentType : Operations::selectAllObjects()) {
            QVariantList values = paymentType.toList();
            if (values[0].toInt() == this->id) {
                title = "Отчет по объекту " + values[1].toString();
                start = 0;
            }
        }
        break;
    case ReportsE::Expenses:
        for (QVariant type : Operations::selectAllTypes()) {
            QVariantList values = type.toList();
            if (values[0].toInt() == this->id) {
                title = "Отчет по расходу " + values[1].toString();
                start = 0;
            }
        }
        break;
    default:
        break;
    }
    QString dates = this->fromDate.toString("dd.MM.yyyy") + " - " + this->toDate.toString("dd.MM.yyyy");

    if (this->table == ReportsE::Objects) {
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

        PDFmanager::exportToPDF(title, dates, { proxyTop, proxyBottom }, start);
    } else {
        PDFmanager::exportToPDF(title, dates, { ui->tableView->model(), ui->bottomTable->model() }, start);
    }
}





void Reports::on_ToEXCELButton_clicked()
{
    // Создаем экземпляр ExcelManager
    ExcelManager excelManager;

    QString title;
    int start = 1;

    switch (this->table)
    {
    case ReportsE::PaymentTypes:
        for (QVariant paymentType : Operations::selectAllPaymentTypes()) {
            QVariantList values = paymentType.toList();
            if (values[0].toInt() == this->id) {
                title = "Отчет по типу оплаты " + values[1].toString();
                start = 0;
            }
        }
        break;
    case ReportsE::Objects:
        for (QVariant object : Operations::selectAllObjects()) {
            QVariantList values = object.toList();
            if (values[0].toInt() == this->id) {
                title = "Отчет по объекту " + values[1].toString();
                start = 0;
            }
        }
        break;
    case ReportsE::Expenses:
        for (QVariant type : Operations::selectAllTypes()) {
            QVariantList values = type.toList();
            if (values[0].toInt() == this->id) {
                title = "Отчет по расходу " + values[1].toString();
                start = 0;
            }
        }
        break;
    default:
        break;
    }

    // Формируем строку дат
    QString dates = this->fromDate.toString("dd.MM.yyyy") + " - " + this->toDate.toString("dd.MM.yyyy");

    // Вызываем метод экспорта в Excel,
    // передавая заголовок, даты, модели таблиц и значение start
    excelManager.exportToExcel(title, dates,
                               { ui->tableView->model(), ui->bottomTable->model() },
                               start);
}






void Reports::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    int row = selected.indexes()[0].row();
    this->id = ui->sideTable->model()->index(row, 0).data().toInt();
    setTable();
    setBottomTable();
    setTableSizes();
}



void Reports::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromDate);

    connect(c, &CalendarPage::changeDate, this, &Reports::setFromDate);

    c->show();
}

void Reports::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->toDate);

    connect(c, &CalendarPage::changeDate, this, &Reports::setToDate);

    c->show();
}



void Reports::setFromDate(QDate date)
{
    this->fromDate = date;
    ui->FromDateButton->setText(date.toString("dd.MM.yyyy"));
    updateData();
}

void Reports::setToDate(QDate date)
{
    this->toDate = date;
    ui->ToDateButton->setText(date.toString("dd.MM.yyyy"));
    updateData();
}

// void Reports::on_FilterButton_clicked()
void Reports::updateData()
{
    setTable();
    setBottomTable();
    setTableSizes();
}




void Reports::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void Reports::adjustColumnWidths()
{
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i)
    {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 860 - totalWidth;
    if (lastColumnWidth > 150)
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    }
    else
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}


void Reports::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order) {
    this->selectedColumn = logicalIndex;
    this->sortOrder = order;
}





