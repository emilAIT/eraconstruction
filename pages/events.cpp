#include "events.h"
#include "qsortfilterproxymodel.h"
#include "ui_events.h"
#include "../managers/nakladnayapaymentspdf.h"
#include <QStandardItemModel>
#include <QVariant>
#include <QDebug>
#include <QLocale>

Events::Events(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Events)
{
    ui->setupUi(this);
    this->nav = nav;

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");
    this->date = QDate::currentDate();
    this->todate = QDate::currentDate();
    this->fromdate = QDate::currentDate().addDays(-1);
    ui->ToDateButton->setText(todate.toString("dd.MM.yyyy"));
    ui->FromDateButton->setText(fromdate.toString("dd.MM.yyyy"));


    ui->comboBox->setCurrentIndex(-1);
    ui->comboBox->setProperty("type", "nav");



    apartments.clear();
    QVariantList apartmentsList = Operations::selectAllApartmentsWithoutId();
    foreach (QVariant apartment, apartmentsList) {
        QVariantList data = apartment.toList();
        QString apartmentNumber = data[2].toString(); // Номер квартиры
        int apartmentId = data[0].toInt(); // ID квартиры
        apartments.insert(apartmentNumber, apartmentId);
    }

    clients.clear();
    QVariantList clientsList = Operations::selectAllClients();
    foreach (QVariant client, clientsList)
    {
        QVariantList data = client.toList();
        QString clientName = data[1].toString(); // name
        int clientId = data[0].toInt(); // ID
        clients.insert(clientName, clientId);
    }

    objects.clear();
    QVariantList objectsList = Operations::selectAllObjects();
    foreach (QVariant client, objectsList)
    {
        QVariantList data = client.toList();
        QString objectName = data[1].toString(); // name
        int objectId = data[0].toInt(); // ID
        objects.insert(objectName, objectId);
    }
}

Events::~Events()
{
    delete ui;
}


void Events::setEvents(EventsE table) {
    this->table = table;
    setHeader();
    setTable();
}

void Events::on_BackButton_clicked()
{
    nav->changeWindow(0);
}


void Events::setHeader() {
    QString header = "header";
    switch (this->table)
    {
    case EventsE::Payments:
        header = "Оплаты";
        ui->ReportButton->setDisabled(true);
        ui->AddButton->setHidden(true);
        ui->CreateContractButton->setHidden(true);
        ui->NakladnayaButton->setHidden(false);
        break;

    case EventsE::Expenses:
        header = "Расходы";
        ui->ReportButton->setDisabled(false);
        ui->AddButton->setHidden(false);
        ui->CreateContractButton->setHidden(true);
        ui->NakladnayaButton->setHidden(false);
        break;

    case EventsE::Sales:
        header = "Продажи";
        ui->ReportButton->setDisabled(true);
        ui->AddButton->setHidden(true);
        ui->NakladnayaButton->setHidden(true);
        ui->CreateContractButton->setHidden(false);
        break;
    }
    ui->EcoTaxi->setText(header);
}



void Events::setTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);

    switch (this->table) {
    case EventsE::Payments:
        model->setHorizontalHeaderLabels({"id", "Дата", "Клиент", "Объект", "Квартира", "Тип оплаты", "Валюта", "Оплатил", "Описание"});
        for (const QVariant &payment : Operations::selectAllPayments(this->fromdate, this->todate)){
            QVariantList values = payment.toList();
            qDebug() << "dkfjkdkfjkd" << values;
            QList<QStandardItem*> rowItems;

            rowItems << new QStandardItem(values[0].toString());  // id
            rowItems << new QStandardItem(values[1].toString());  // Дата
            rowItems << new QStandardItem(values[2].toString());  // Клиент
            rowItems << new QStandardItem(values[3].toString());  // Объект
            rowItems << new QStandardItem(values[4].toString());  // Квартира
            rowItems << new QStandardItem(values[5].toString());  // Тип оплаты
            rowItems << new QStandardItem(values[6].toString());
            aparId = values[9].toInt();

            double amount = values[7].toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(amountStr, Qt::DisplayRole);
            amountItem->setData(amount, Qt::UserRole);
            rowItems << amountItem;

            rowItems << new QStandardItem(values[8].toString());  // Описание

            model->appendRow(rowItems);
        }
        break;

    case EventsE::Expenses:
        model->setHorizontalHeaderLabels({"id", "Дата", "Тип", "Кто/Что", "Тип оплаты", "Валюта", "Сумма", "Описание"});
        for (const QVariant &expense : Operations::selectAllExpenses(this->fromdate, this->todate)) {
            QVariantList values = expense.toList();
            QList<QStandardItem*> rowItems;

            rowItems << new QStandardItem(values[0].toString());  // id
            rowItems << new QStandardItem(values[1].toString());  // Дата
            rowItems << new QStandardItem(values[2].toString());  // Тип
            rowItems << new QStandardItem(values[3].toString());  // Кто/Что
            rowItems << new QStandardItem(values[4].toString());  // Тип оплаты
            rowItems << new QStandardItem(values[7].toString());

            double amount = values[5].toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(amountStr, Qt::DisplayRole);
            amountItem->setData(amount, Qt::UserRole);
            rowItems << amountItem;

            rowItems << new QStandardItem(values[6].toString());  // Описание

            model->appendRow(rowItems);
        }
        break;

    case EventsE::Sales:
        model->setHorizontalHeaderLabels({
            "id", "Дата", "Клиент", "Объект", "Квартира",
            "Стоимость", "Скидка", "Итого", "Тип оплаты", "Валюта",
            "Рассрочка", "Первоначальный взнос", "Кол-во месяцев", "Ежемесячный платеж", "Описание"
        });

        for (const QVariant &sale : Operations::selectAllSales(this->fromdate, this->todate)) {
            QVariantList values = sale.toList();
            QList<QStandardItem*> rowItems;
            qDebug() << "fkj;adjkf" << values[12];

            rowItems << new QStandardItem(values[0].toString());  // id

            // rowItems << new QStandardItem(values[1].toString());  // Дата
            QStandardItem *dateItem = new QStandardItem();
            QDate date = values[1].toDate();
            QString formattedDate = date.toString("dd.MM.yyyy"); // формат отображения, можно изменить
            dateItem->setData(formattedDate, Qt::DisplayRole);
            dateItem->setData(date, Qt::UserRole);
            rowItems.append(dateItem);

            rowItems << new QStandardItem(values[2].toString());  // Клиент
            rowItems << new QStandardItem(values[3].toString());  // Объект
            rowItems << new QStandardItem(values[4].toString());  // Квартира

            double apartmentCost = values[5].toDouble();
            QString apartmentCostStr = locale.toString(apartmentCost, 'f', 2);
            QStandardItem *apartmentCostItem = new QStandardItem();
            apartmentCostItem->setData(apartmentCostStr, Qt::DisplayRole);
            apartmentCostItem->setData(apartmentCost, Qt::UserRole);
            rowItems << apartmentCostItem;

            double discount = values[6].toDouble();
            QString discountStr = locale.toString(discount, 'f', 2);
            QStandardItem *discountItem = new QStandardItem();
            discountItem->setData(discountStr, Qt::DisplayRole);
            discountItem->setData(discount, Qt::UserRole);
            rowItems << discountItem;

            double totalAmount = values[7].toDouble();
            QString totalAmountStr = locale.toString(totalAmount, 'f', 2);
            QStandardItem *totalAmountItem = new QStandardItem();
            totalAmountItem->setData(totalAmountStr, Qt::DisplayRole);
            totalAmountItem->setData(totalAmount, Qt::UserRole);
            rowItems << totalAmountItem;

            rowItems << new QStandardItem(values[8].toString());  // Тип оплаты
            rowItems << new QStandardItem(values[14].toString()); // currency

            bool isInstallmentPlan = values[9].toBool();
            rowItems << new QStandardItem(isInstallmentPlan ? "Да" : "Нет");  // Рассрочка

            double installment = values[10].toDouble();
            QString installmentStr = locale.toString(installment, 'f', 2);
            QStandardItem *installmentItem = new QStandardItem();
            installmentItem->setData(installmentStr, Qt::DisplayRole);
            installmentItem->setData(installment, Qt::UserRole);
            rowItems << installmentItem;

            int monthQuantity = values[11].toInt();
            QStandardItem *monthQuantityItem = new QStandardItem();
            monthQuantityItem->setData(QString::number(monthQuantity), Qt::DisplayRole);
            monthQuantityItem->setData(monthQuantity, Qt::UserRole);
            rowItems << monthQuantityItem;  // Количество месяцев

            double monthlyPayment = values[12].toDouble();
            QString monthlyPaymentStr = locale.toString(monthlyPayment, 'f', 2);
            QStandardItem *monthlyPaymentItem = new QStandardItem();
            monthlyPaymentItem->setData(monthlyPaymentStr, Qt::DisplayRole);
            monthlyPaymentItem->setData(monthlyPayment, Qt::UserRole);
            rowItems << monthlyPaymentItem;

            rowItems << new QStandardItem(values[13].toString());  // Описание

            model->appendRow(rowItems);
        }
        break;

    default:
        break;
    }

    auto *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::UserRole);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setColumnHidden(0, true);  // Скрываем ID
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);
    ui->tableView->setSortingEnabled(true);

    switch(this->table){
    case EventsE::Expenses:
        ui->tableView->setColumnWidth(1, 165);
        ui->tableView->setColumnWidth(2, 190);
        ui->tableView->setColumnWidth(3, 195);
        ui->tableView->setColumnWidth(4, 195);
        ui->tableView->setColumnWidth(5, 195);
        ui->tableView->setColumnWidth(6, 195);
        ui->tableView->setColumnWidth(7, 175);
        break;
    case EventsE::Payments:
        ui->tableView->setColumnWidth(1, 165);
        ui->tableView->setColumnWidth(2, 190);
        ui->tableView->setColumnWidth(3, 195);
        ui->tableView->setColumnWidth(4, 195);
        ui->tableView->setColumnWidth(5, 195);
        ui->tableView->setColumnWidth(6, 175);
        break;
    }
}



void Events::on_AddButton_clicked()
{
    AddUpdateOtherEvents *w = new AddUpdateOtherEvents(this->table, 1, 0);
    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
}

void Events::on_EditButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        QWidget *w = nullptr;  // Используем общий тип для виджетов

        switch (this->table) {
        case EventsE::Expenses:
            w = new AddUpdateOtherEvents(this->table, -1, id);
            break;
        case EventsE::Sales:
        case EventsE::Payments:
            w = new AddUpdateEvents(id, this->table);
            break;
        default:
            break;
        }

        if (w) {  // Проверяем, что объект был создан
            w->resize(w->minimumSizeHint());
            w->show();

            QEventLoop loop;
            connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
            loop.exec();

            setTable();
        }
    }
}


void Events::on_DeleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления.");
        return;
    }

    int row = select->selectedRows().at(0).row();
    int id = ui->tableView->model()->index(row, 0).data().toInt();
    CustomDialog *d = nullptr;

    switch (this->table) {
        case EventsE::Sales: {
            QVariantList sale = Operations::getSale(id);
            QVariantList data = sale[0].toList();
            QString client = data[1].toString();
            QString object = data[2].toString();
            d = new CustomDialog("Вы уверены, что хотите удалить продажу с клиентом " + client + " на объекте " + object + "?");
            break;
        }

        case EventsE::Payments: {
            QVariantList payment = Operations::getPayment(id);
            QVariantList data = payment[0].toList();
            QString client = data[1].toString();
            QString object = data[2].toString();
            QString apartment = data[3].toString();
            float amount = data[5].toFloat();
            d = new CustomDialog("Вы уверены, что хотите удалить платеж для клиента " + client + " на объекте " + object + " (квартира: " + apartment + ") на сумму " + QString::number(amount) + "?");
            break;
        }

        case EventsE::Expenses: {
            QVariantList expense = Operations::getExpense(id);
            QVariantList data = expense[0].toList();
            QString type = data[1].toString();
            QString whoWhat = data[2].toString();
            float amount = data[4].toFloat();
            d = new CustomDialog("Вы уверены, что хотите удалить расход для " + whoWhat + " на сумму " + QString::number(amount) + " (Тип: " + type + ")?");
            break;
        }

        default:
            break;
    }
    if (d) {
        connect(d, &CustomDialog::yesClicked, this, &Events::onYes);
        connect(d, &CustomDialog::noClicked, this, &Events::onNo);
        d->show();
    }
}


void Events::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::warning(this, "Ошибка", "Выберите запись для удаления.");
        return;
    }

    int row = select->selectedRows().at(0).row();
    int id = ui->tableView->model()->index(row, 0).data().toInt();


    switch (this->table)
    {
    case EventsE::Sales: {
        QVariantList sale = Operations::getSale(id);
        if (!sale.isEmpty()) {
            QVariantList data = sale.first().toList();
            QString client = data[1].toString();
            QString object = data[2].toString();
        }
        Operations::deleteSale(id);
        break;
    }

    case EventsE::Payments: {
        Operations::deletePayment(id);
        break;
    }

    case EventsE::Expenses: {
        Operations::deleteExpense(id);
        break;
    }

    default:
        break;
    }

    setTable();
}


void Events::onNo() {

}



void Events::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromdate);

    connect(c, &CalendarPage::changeDate, this, &Events::onfromDateChange);

    c->show();
}


void Events::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->todate);

    connect(c, &CalendarPage::changeDate, this, &Events::ontoDateChange);

    c->show();
}

void Events::on_weekButton_clicked()
{
    this->fromdate = this->todate.addDays(-7);
    ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
    setTable();
}


void Events::on_monthButton_clicked()
{

    this->fromdate = this->todate.addMonths(-1);
    ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
    setTable();
}


void Events::on_threeMonthButton_clicked()
{
    this->fromdate = this->todate.addMonths(-3);
    ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
    setTable();

}



void Events::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        this->fromdate = this->todate.addDays(-7);
        ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
        break;
    case 1:
        this->fromdate = this->todate.addMonths(-1);
        ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
        break;
    case 2:
        this->fromdate = this->todate.addMonths(-3);
        ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
        break;
    default:
        break;
    }
    setTable();
    ui->comboBox->setCurrentIndex(-1);
}



void Events::onfromDateChange(QDate date)
{
    this->fromdate = date;
    ui->FromDateButton->setText(this->fromdate.toString("dd.MM.yyyy"));
    setTable();
}

void Events::ontoDateChange(QDate date)
{
    this->todate = date;
    ui->ToDateButton->setText(this->todate.toString("dd.MM.yyyy"));
    setTable();
}


void Events::onSectionResized(int logicalIndex, int oldSize, int newSize) {
    if (logicalIndex != ui->tableView->model()->columnCount() - 1) {
        adjustColumnWidths();
    }
}

void Events::adjustColumnWidths() {
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i) {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 1130 - totalWidth;
    if (lastColumnWidth > 150) {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    } else {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}

void Events::on_ReportButton_clicked()
{
    nav->openGeneralReports(2);
}



void Events::on_CreateContractButton_clicked()
{
    // Обновляем словарь квартир
    // apartments.clear();
    // QVariantList apartmentsList = Operations::selectAllApartmentsWithoutId();
    // foreach (const QVariant &apartment, apartmentsList) {
    //     QVariantList data = apartment.toList();
    //     QString apartmentNumber = data[2].toString(); // номер квартиры
    //     int apartmentId = data[0].toInt();            // ID квартиры
    //     apartments.insert(apartmentNumber, apartmentId);
    // }
    apartments.clear();
    QVariantList apartmentsList = Operations::selectAllApartmentsWithoutId();
    foreach (const QVariant &apartment, apartmentsList) {
        QVariantList data = apartment.toList();
        // Получаем идентификатор объекта и номер квартиры
        int aptObjectId = data[1].toInt();
        QString apartmentNumber = data[2].toString();
        // Формируем составной ключ: objectId_apartmentNumber
        QString compositeKey = QString("%1_%2").arg(aptObjectId).arg(apartmentNumber);
        int apartmentId = data[0].toInt();
        apartments.insert(compositeKey, apartmentId);
    }


    objects.clear();
    QVariantList objectsList = Operations::selectAllObjects();
    foreach (const QVariant &object, objectsList) {
        QVariantList data = object.toList();
        QString objectNumber = data[1].toString();
        int objectId = data[0].toInt();
        objects.insert(objectNumber, objectId);
    }

    clients.clear();
    QVariantList clientsList = Operations::selectAllClients();
    foreach (const QVariant &client, clientsList) {
        QVariantList data = client.toList();
        QString clientNumber = data[1].toString();
        int clientId = data[0].toInt();
        clients.insert(clientNumber, clientId);
    }

    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (!select->hasSelection()) {
        QMessageBox::warning(this, "Ошибка", "Выберите продажу из таблицы.");
        return;
    }

    int row = select->selectedRows().at(0).row();
    int saleId = ui->tableView->model()->index(row, 0).data().toInt();

    QVariantList saleData = Operations::getSale(saleId);
    if (saleData.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить данные о продаже.");
        return;
    }

    QVariantList rowData = saleData[0].toList();
    QString dateStr = rowData[0].toDate().toString("dd.MM.yyyy");
    QString client = rowData[1].toString();
    QString object = rowData[2].toString();
    QString apartment = rowData[3].toString();
    QString apartmentCost = QString::number(rowData[4].toDouble(), 'f', 2);
    QString totalAmount = QString::number(rowData[6].toDouble(), 'f', 2);
    bool isInstallment = rowData[8].toBool();
    QString installment = QString::number(rowData[9].toDouble(), 'f', 2);
    QString monthQuantity = QString::number(rowData[10].toInt());
    QString monthlyPayment = QString::number(rowData[11].toDouble(), 'f', 2);
    double totalAmountNum = rowData[6].toDouble();
    double installmentNum = rowData[9].toDouble();
    double debtNum = totalAmountNum - installmentNum;
    QString debt = QString::number(debtNum, 'f', 2);
    double monthlyPaymentNum = rowData[11].toDouble();
    double remainNum = debtNum - monthlyPaymentNum;
    QString res = QString::number(remainNum, 'f', 2);
    QString currency = rowData[13].toString();


    int objectId = objects.value(object);
    if (objectId == 0) {
        QMessageBox::warning(this, "Ошибка", "Невозможно создать договор: не найден объект");
        return;
    }
    QVariantList objectData = Operations::getObject(objectId);
    QVariantList objectRow = objectData[0].toList();
    QString objectLocation = objectRow[2].toString();



    QString compositeKey = QString("%1_%2").arg(objectId).arg(apartment);
    int apartmentId = apartments.value(compositeKey);
    if (apartmentId == 0) {
        QMessageBox::warning(this, "Ошибка", "Невозможно создать договор: не найдена квартира");
        return;
    }
    QVariantList apartmentData = Operations::getApartment(apartmentId);
    QVariantList apartmentRow = apartmentData[0].toList();
    QString ploashad = apartmentRow[3].toString();
    QString roomsQuantity = apartmentRow[4].toString();
    QString floor = apartmentRow[6].toString();
    QString pricePerMetr = apartmentRow[7].toString();



    int clientId = clients.value(client);
    if (clientId == 0) {
        QMessageBox::warning(this, "Ошибка", "Невозможно создать договор: не найден клиент");
        return;
    }
    QVariantList clientData = Operations::getClient(clientId);
    QVariantList clientRow = clientData[0].toList();
    QString dateOfBirth = clientRow[4].toString();
    QString pasportId = clientRow[6].toString();
    QString authority = clientRow[3].toString();
    QString dateOfIssue = clientRow[5].toString();
    QString placeOfResidence = clientRow[7].toString();



    ContractWordManager contractManager;
    QString filePath = contractManager.createContract(client,
                                                      roomsQuantity,
                                                      ploashad,
                                                      apartment,
                                                      pricePerMetr,
                                                      apartmentCost,
                                                      totalAmount,
                                                      floor,
                                                      dateStr,
                                                      installment,
                                                      monthlyPayment,
                                                      debt,
                                                      res,
                                                      isInstallment,
                                                      dateOfBirth,
                                                      pasportId,
                                                      authority,
                                                      dateOfIssue,
                                                      placeOfResidence,
                                                      objectLocation,
                                                      currency);
    contractManager.showWordViewer(filePath);
    QFile::remove(filePath);
}




void Events::on_NakladnayaButton_clicked()
{
    switch(this->table) {
    case EventsE::Payments: {
        QItemSelectionModel *select = ui->tableView->selectionModel();
        if (!select->hasSelection()) {
            QMessageBox::warning(this, "Ошибка", "Выберите платеж для создания платежного документа.");
            return;
        }

        int row = select->selectedRows().at(0).row();

        QString id = ui->tableView->model()->index(row, 0).data().toString();          // id платежа
        QString dateStr = ui->tableView->model()->index(row, 1).data().toString();       // Дата платежа
        QString client = ui->tableView->model()->index(row, 2).data().toString();        // Клиент
        QString object = ui->tableView->model()->index(row, 3).data().toString();        // Объект
        QString apartment = ui->tableView->model()->index(row, 4).data().toString();     // Квартира
        QString paymentType = ui->tableView->model()->index(row, 5).data().toString();   // Тип оплаты
        QString currency = ui->tableView->model()->index(row, 6).data().toString();      // Валюта
        QString paidAmount = ui->tableView->model()->index(row, 7).data().toString();    // Сумма оплаты
        QString description = ui->tableView->model()->index(row, 8).data().toString();   // Описание

        NakladnayaPaymentsPDF nakladnayaPaymentsPDF;

        QVariantList apartmentData = Operations::getApartment(aparId);
        QVariantList apartmentRow = apartmentData[0].toList();

        QString roomsQuantity = apartmentRow[4].toString();
        QString number = apartmentRow[2].toString();
        QString floor = apartmentRow[6].toString();
        QString ploshad = apartmentRow[3].toString();

        QString pdfPath = nakladnayaPaymentsPDF.createPaymentPDF(
            id,
            client,
            object,
            apartment,
            paymentType,
            currency,
            paidAmount,
            description,
            dateStr,
            roomsQuantity,
            number,
            floor,
            ploshad
            );

        nakladnayaPaymentsPDF.showPdfViewer(pdfPath);

        QFile::remove(pdfPath);
        break;
    }




    case EventsE::Expenses: {
        QItemSelectionModel *select = ui->tableView->selectionModel();
        if (!select->hasSelection()) {
            QMessageBox::warning(this, "Ошибка", "Выберите расход для создания документа.");
            return;
        }
        int row = select->selectedRows().at(0).row();

        // Извлекаем данные для расхода
        QString id = ui->tableView->model()->index(row, 0).data().toString();         // id расхода
        QString dateStr = ui->tableView->model()->index(row, 1).data().toString();      // Дата расхода
        QString type = ui->tableView->model()->index(row, 2).data().toString();         // Тип расхода
        QString whoWhat = ui->tableView->model()->index(row, 3).data().toString();      // Кто/Что
        QString paymentType = ui->tableView->model()->index(row, 4).data().toString();  // Тип оплаты
        QString currency = ui->tableView->model()->index(row, 5).data().toString();     // Валюта
        QString amount = ui->tableView->model()->index(row, 6).data().toString();       // Сумма расхода
        QString description = ui->tableView->model()->index(row, 7).data().toString();  // Описание

        // Создаем PDF для расхода через менеджер документов (предполагается наличие класса NakladnayaExpensesPDF)
        NakladnayaExpensePDF nakladnayaExpensesPDF;

        QString pdfPath = nakladnayaExpensesPDF.createExpensePDF(
            id,
            type,
            whoWhat,
            paymentType,
            currency,
            amount,
            description,
            dateStr
            );

        nakladnayaExpensesPDF.showPdfViewer(pdfPath);
        QFile::remove(pdfPath);
        break;
    }

    default:
        QMessageBox::warning(this, "Ошибка", "Неизвестный тип таблицы.");
        break;
    }
}







