#include "generalreports.h"
#include "qsortfilterproxymodel.h"
#include "ui_generalreports.h"
#include <QMessageBox>
#include <QtMath>

GeneralReports::GeneralReports(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GeneralReports)
{
    ui->setupUi(this);
    this->nav = nav;

    this->fromDate = QDate::currentDate().addDays(-1);
    this->toDate = QDate::currentDate();

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");

    ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));

    ui->objectBox->setCurrentIndex(-1);
    ui->objectBox->setProperty("type", "nav");
}

GeneralReports::~GeneralReports()
{
    delete ui;
}

void GeneralReports::setGeneralReports(GeneralReportsE table, QDate from, QDate to) {
    this->table = table;

    setHeader();
    setTable();
    setBottomTable();
    setTableSizes();

    ui->objectBox->clear();
    objects.clear();

    QVariantList objectsList = Operations::selectAllObjects();
    ui->objectBox->addItem("Все объекты");
    foreach (QVariant object, objectsList)
    {
        QVariantList data = object.toList();
        QString show = data[1].toString();
        objects.insert(show, data[0].toInt());
        ui->objectBox->addItem(show);
    }
}

void GeneralReports::setHeader() {
    QString type = "type";
    QString header = "header";
    switch (this->table)
    {
    case GeneralReportsE::PaymentTypes:
        type = "ОТЧЕТ";
        header = "ПО ТИПАМ ОПЛАТ";
        ui->ReportButton->setText("ОТЧЕТ ПО ТИПУ ОПЛАТЫ");
        ui->objectBox->hide();
        ui->fromLabel->show();
        ui->toLabel->show();
        ui->FromDateButton->show();
        ui->ToDateButton->show();
        ui->SettingsButton->show();
        break;

    case GeneralReportsE::Objects:
        type = "ОТЧЕТ";
        header = "ПО ОБЪЕКТАМ";
        ui->ReportButton->setText("ОТЧЕТ ПО ОБЪЕКТУ");
        ui->objectBox->hide();
        ui->fromLabel->show();
        ui->toLabel->show();
        ui->FromDateButton->show();
        ui->ToDateButton->show();
        ui->SettingsButton->show();
        break;

    case GeneralReportsE::Expenses:
        type = "ОТЧЕТ";
        header = "ПО РАССХОДАМ";
        ui->ReportButton->setText("ОТЧЕТ ПО РАСХОДУ");
        ui->objectBox->hide();
        ui->fromLabel->show();
        ui->toLabel->show();
        ui->FromDateButton->show();
        ui->ToDateButton->show();
        ui->SettingsButton->show();
        break;

    case GeneralReportsE::Clients:
        type = "ОТЧЕТ";
        header = "ПО КЛИЕНТАМ";
        ui->ReportButton->setText("ОТЧЕТ ПО КЛИЕНТУ");
        ui->objectBox->hide();
        ui->fromLabel->show();
        ui->toLabel->show();
        ui->FromDateButton->show();
        ui->ToDateButton->show();
        ui->SettingsButton->show();
        break;

    case GeneralReportsE::Apartments:
        type = "ОТЧЕТ";
        header = "ПО КВАРТИРАМ";
        ui->ReportButton->setText("ОТЧЕТ ПО КВАРТИРЕ");
        ui->objectBox->show();
        ui->fromLabel->show();
        ui->toLabel->show();
        ui->FromDateButton->show();
        ui->ToDateButton->show();
        ui->SettingsButton->show();
        break;

    case GeneralReportsE::Debts:
        type = "ОТЧЕТ";
        header = "ПО ДОЛГАМ";
        ui->ReportButton->setText("ОТЧЕТ ПО ДОЛГУ");
        ui->objectBox->hide();
        ui->fromLabel->hide();
        ui->toLabel->hide();
        ui->FromDateButton->hide();
        ui->ToDateButton->hide();
        ui->SettingsButton->hide();
        break;
    }
    ui->EcoTaxi->setText(type);
    ui->Header->setText(header);
}


void GeneralReports::setTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);

    switch (this->table) {
    case GeneralReportsE::PaymentTypes:
        model->setHorizontalHeaderLabels({"ID", "Тип оплаты", "Валюта", "Сумма"});
        for (const QVariant &paymentType : ReportOperations::getPaymentTypesReport(this->fromDate, this->toDate)) {
            QVariantList values = paymentType.toList();
            QList<QStandardItem*> row;

            row << new QStandardItem(values[0].toString());
            row << new QStandardItem(values[1].toString());
            row << new QStandardItem(values[3].toString());

            double amount = values[2].toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(amountStr, Qt::DisplayRole);
            amountItem->setData(amount, Qt::UserRole);
            row << amountItem;

            model->appendRow(row);
        }
        break;

    case GeneralReportsE::Objects:
        model->setHorizontalHeaderLabels({"ID", "Объект", "Продано", "Не Продано", "Рассрочка", "Полный расчет", "Сумма KGS", "Сумма USD", "Долг KGS", "Долг USD", "Приход KGS", "Приход USD"});

        for (const QVariant &object : ReportOperations::getObjectsReport(this->fromDate, this->toDate)) {
            QVariantList values = object.toList();
            qDebug() << "Data row:" << values; // Отладочный вывод
            QList<QStandardItem*> row;

            // ID объекта
            row << new QStandardItem(values[0].toString());

            // Название объекта
            row << new QStandardItem(values[1].toString());

            int sold = values[2].toInt();
            QStandardItem *soldItem = new QStandardItem();
            soldItem->setData(sold, Qt::DisplayRole);
            soldItem->setData(sold, Qt::UserRole);
            row << soldItem;

            int notSold = values[3].toInt();
            QStandardItem *notSoldItem = new QStandardItem();
            notSoldItem->setData(notSold, Qt::DisplayRole);
            notSoldItem->setData(notSold, Qt::UserRole);
            row << notSoldItem;

            int installment = values[4].toInt();
            QStandardItem *installmentItem = new QStandardItem();
            installmentItem->setData(installment, Qt::DisplayRole);
            installmentItem->setData(installment, Qt::UserRole);
            row << installmentItem;

            int fullPayment = values[5].toInt();
            QStandardItem *fullPaymentItem = new QStandardItem();
            fullPaymentItem->setData(fullPayment, Qt::DisplayRole);
            fullPaymentItem->setData(fullPayment, Qt::UserRole);
            row << fullPaymentItem;

            // Сумма KGS
            double sumKGS = values[6].toDouble();
            QStandardItem *sumKGSItem = new QStandardItem();
            sumKGSItem->setData(locale.toString(sumKGS, 'f', 2), Qt::DisplayRole);
            sumKGSItem->setData(sumKGS, Qt::UserRole);
            row << sumKGSItem;

            // Сумма USD
            double sumUSD = values[7].toDouble();
            QStandardItem *sumUSDItem = new QStandardItem();
            sumUSDItem->setData(locale.toString(sumUSD, 'f', 2), Qt::DisplayRole);
            sumUSDItem->setData(sumUSD, Qt::UserRole);
            row << sumUSDItem;

            // Долг KGS
            double debtKGS = values[8].toDouble();
            QStandardItem *debtKGSItem = new QStandardItem();
            debtKGSItem->setData(locale.toString(debtKGS, 'f', 2), Qt::DisplayRole);
            debtKGSItem->setData(debtKGS, Qt::UserRole);
            row << debtKGSItem;

            // Долг USD
            double debtUSD = values[9].toDouble();
            QStandardItem *debtUSDItem = new QStandardItem();
            debtUSDItem->setData(locale.toString(debtUSD, 'f', 2), Qt::DisplayRole);
            debtUSDItem->setData(debtUSD, Qt::UserRole);
            row << debtUSDItem;

            // Приход KGS
            double incomeKGS = values[10].toDouble();
            QStandardItem *incomeKGSItem = new QStandardItem();
            incomeKGSItem->setData(locale.toString(incomeKGS, 'f', 2), Qt::DisplayRole);
            incomeKGSItem->setData(incomeKGS, Qt::UserRole);
            row << incomeKGSItem;

            // Приход USD
            double incomeUSD = values[11].toDouble();
            QStandardItem *incomeUSDItem = new QStandardItem();
            incomeUSDItem->setData(locale.toString(incomeUSD, 'f', 2), Qt::DisplayRole);
            incomeUSDItem->setData(incomeUSD, Qt::UserRole);
            row << incomeUSDItem;

            model->appendRow(row);
        }

        break;

    case GeneralReportsE::Expenses:
        model->setHorizontalHeaderLabels({"ID", "Тип", "Сумма KGS", "Сумма USD"});
        for (const QVariant &expens : ReportOperations::getExpensesReport(this->fromDate, this->toDate))
        {
            QVariantList services = expens.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(services[0].toString()));  // ID

            QStandardItem *typeItem = new QStandardItem(services[1].toString());
            typeItem->setTextAlignment(Qt::AlignCenter);
            row.append(typeItem);

            QStandardItem *amountKGSItem = new QStandardItem(locale.toString(services[2].toDouble(), 'f', 2));
            amountKGSItem->setTextAlignment(Qt::AlignCenter);
            row.append(amountKGSItem);

            QStandardItem *amountUSDItem = new QStandardItem(locale.toString(services[3].toDouble(), 'f', 2));
            amountUSDItem->setTextAlignment(Qt::AlignCenter);
            row.append(amountUSDItem);

            model->appendRow(row);
        }
        break;

    case GeneralReportsE::Clients:
        model->setHorizontalHeaderLabels({"ID", "Клиент", "Валюта", "Сумма", "Оплатил", "Осталось"});
        for (const QVariant &client : ReportOperations::getClientsReport(this->fromDate, this->toDate)) {
            QVariantList values = client.toList();
            QList<QStandardItem*> row;

            row << new QStandardItem(values[0].toString());  // ID клиента
            row << new QStandardItem(values[1].toString());  // Имя клиента
            row << new QStandardItem(values[2].toString());  // currency name

            // Форматирование общей суммы сделки
            double totalAmount = values[3].toDouble();
            QString totalAmountStr = locale.toString(totalAmount, 'f', 2);
            QStandardItem *totalAmountItem = new QStandardItem();
            totalAmountItem->setData(totalAmountStr, Qt::DisplayRole);
            totalAmountItem->setData(totalAmount, Qt::UserRole);
            row << totalAmountItem;

            // Форматирование оплаченной суммы
            double paid = values[4].toDouble();
            QString paidStr = locale.toString(paid, 'f', 2);
            QStandardItem *paidItem = new QStandardItem();
            paidItem->setData(paidStr, Qt::DisplayRole);
            paidItem->setData(paid, Qt::UserRole);
            row << paidItem;

            // Форматирование оставшейся суммы
            double remaining = values[5].toDouble();
            QString remainingStr = locale.toString(remaining, 'f', 2);
            QStandardItem *remainingItem = new QStandardItem();
            remainingItem->setData(remainingStr, Qt::DisplayRole);
            remainingItem->setData(remaining, Qt::UserRole);
            row << remainingItem;

            model->appendRow(row);
        }
        break;

    case GeneralReportsE::Apartments:
        model->setHorizontalHeaderLabels({"ID", "Клиент", "Объект", "Квартира", "Цена", "Скидка м²", "Итого","Валюта", "Тип оплаты", "Статус", "Оплачено", "Осталось", "Плата в месяц", "Опл. месяцы", "Неопл. месяцы"});

        for (const QVariant &apartment : ReportOperations::getApartmentsReport(this->objectId, this->fromDate, this->toDate)) {
            QVariantList values = apartment.toList();
            QList<QStandardItem*> row;

            // Добавляем ID перед клиентом
            row << new QStandardItem(values[0].toString());  // ID

            // Клиент
            row << new QStandardItem(values[1].toString());  // Клиент
            row << new QStandardItem(values[2].toString());  // Объект
            row << new QStandardItem(values[3].toString());  // Квартира

            double apartmentCost = values[4].toDouble();
            QString apartmentCostStr = locale.toString(apartmentCost, 'f', 2);
            QStandardItem *apartmentCostItem = new QStandardItem();
            apartmentCostItem->setData(apartmentCostStr, Qt::DisplayRole);
            apartmentCostItem->setData(apartmentCost, Qt::UserRole);
            row << apartmentCostItem;

            double discount = values[5].toDouble();
            QString discountStr = locale.toString(discount, 'f', 2);
            QStandardItem *discountItem = new QStandardItem();
            discountItem->setData(discountStr, Qt::DisplayRole);
            discountItem->setData(discount, Qt::UserRole);
            row << discountItem;

            double totalAmount = values[6].toDouble();
            QString totalAmountStr = locale.toString(totalAmount, 'f', 2);
            QStandardItem *totalAmountItem = new QStandardItem();
            totalAmountItem->setData(totalAmountStr, Qt::DisplayRole);
            totalAmountItem->setData(totalAmount, Qt::UserRole);
            row << totalAmountItem;

            row << new QStandardItem(values[7].toString());
            row << new QStandardItem(values[8].toString());  // Тип оплаты

            QString status = values[9].toString();
            row << new QStandardItem(status); // Статус

            double paid = 0.0;
            double remaining = 0.0;
            int paidMonths = 0;
            int remainingMonths = 0;
            int summa = 0;
            int passedMonths = 0;
            double monthlyPayment = values[12].toDouble();

            if (status == "Полный расчет") {
                paid = values[6].toDouble();
                remaining = 0.0;
                monthlyPayment = 0.0;
                paidMonths = 0;
                remainingMonths = 0;
            } else {
                paid = values[10].toDouble();
                remaining = values[11].toDouble();

                summa = (values[13].isNull() || values[13].toString().isEmpty()) ? 0 : values[13].toInt();
                passedMonths = (values[14].isNull() || values[14].toString().isEmpty()) ? 0 : values[14].toInt();
                paidMonths = (monthlyPayment != 0) ? floor((double)summa / monthlyPayment) : 0;
                remainingMonths = (passedMonths > 0 && passedMonths > paidMonths) ? (passedMonths - paidMonths) : 0;
            }

            QString paidStr = locale.toString(paid, 'f', 2);
            QStandardItem *paidItem = new QStandardItem();
            paidItem->setData(paidStr, Qt::DisplayRole);
            paidItem->setData(paid, Qt::UserRole);
            row << paidItem;

            QString remainingStr = locale.toString(remaining, 'f', 2);
            QStandardItem *remainingItem = new QStandardItem();
            remainingItem->setData(remainingStr, Qt::DisplayRole);
            remainingItem->setData(remaining, Qt::UserRole);
            row << remainingItem;

            QString monthlyPaymentStr = locale.toString(monthlyPayment, 'f', 2);
            QStandardItem *monthlyPaymentItem = new QStandardItem();
            monthlyPaymentItem->setData(monthlyPaymentStr, Qt::DisplayRole);
            monthlyPaymentItem->setData(monthlyPayment, Qt::UserRole);
            row << monthlyPaymentItem;

            QString paidMonthsStr = locale.toString(paidMonths);
            QStandardItem *paidMonthsItem = new QStandardItem();
            paidMonthsItem->setData(paidMonthsStr, Qt::DisplayRole);
            paidMonthsItem->setData(paidMonths, Qt::UserRole);
            row << paidMonthsItem;

            QString remainingMonthsStr = locale.toString(remainingMonths);
            QStandardItem *remainingMonthsItem = new QStandardItem();
            remainingMonthsItem->setData(remainingMonthsStr, Qt::DisplayRole);
            remainingMonthsItem->setData(remainingMonths, Qt::UserRole);
            row << remainingMonthsItem;

            // row << new QStandardItem(QString::number(paidMonths));
            // row << new QStandardItem(QString::number(remainingMonths));


            model->appendRow(row);
        }
        break;

    case GeneralReportsE::Debts:
        model->setHorizontalHeaderLabels({"ID", "Клиент", "Объект", "Квартира", "Не оплатил", "Валюта", "Долг"});

        for (const QVariant &debt : ReportOperations::getDebtsReport()) {
            QVariantList values = debt.toList();
            QList<QStandardItem*> row;

            // ID
            row << new QStandardItem(values[0].toString());
            // Клиент
            row << new QStandardItem(values[1].toString());
            // Объект (индекс 3)
            row << new QStandardItem(values[3].toString());
            // Валюта (индекс 4)
            row << new QStandardItem(values[2].toString());
            // Квартира (индекс 2)

            int unpaidMonths = values[5].toInt();
            QString unpaidMonthsStr = locale.toString(unpaidMonths);
            QStandardItem *unpaidMonthsItem = new QStandardItem(unpaidMonthsStr);
            unpaidMonthsItem->setData(unpaidMonths, Qt::UserRole);
            row << unpaidMonthsItem;

            row << new QStandardItem(values[4].toString());

            double debtAmount = values[6].toDouble();
            QString debtAmountStr = locale.toString(debtAmount, 'f', 2);
            QStandardItem *debtItem = new QStandardItem(debtAmountStr);
            debtItem->setData(debtAmount, Qt::UserRole);
            row << debtItem;

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
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    if (this->selectedColumn != -1)
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);
}


void GeneralReports::setTableSizes() {
    switch (this->table)
    {
    case GeneralReportsE::Expenses:
    case GeneralReportsE::PaymentTypes:
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 500);
        ui->tableView->setColumnWidth(2, 500);

        ui->bottomTable->setColumnWidth(0, 700);
        ui->bottomTable->setColumnWidth(1, 700);

        // ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        break;
    case GeneralReportsE::Debts:
        ui->tableView->setColumnWidth(1, 215);
        ui->tableView->setColumnWidth(2, 215);
        ui->tableView->setColumnWidth(3, 215);
        ui->tableView->setColumnWidth(4, 215);
        ui->tableView->setColumnWidth(5, 215);
        ui->tableView->setColumnWidth(6, 215);

        ui->bottomTable->setColumnWidth(0, 700);
        ui->bottomTable->setColumnWidth(1, 700);
        break;
    case GeneralReportsE::Clients:
        ui->tableView->setColumnWidth(1, 300);
        ui->tableView->setColumnWidth(2, 300);
        ui->tableView->setColumnWidth(3, 300);
        ui->tableView->setColumnWidth(4, 300);

        ui->bottomTable->setColumnWidth(0, 230);
        ui->bottomTable->setColumnWidth(1, 230);
        ui->bottomTable->setColumnWidth(2, 230);
        ui->bottomTable->setColumnWidth(3, 230);
        ui->bottomTable->setColumnWidth(4, 230);
        ui->bottomTable->setColumnWidth(5, 230);
        break;
    case GeneralReportsE::Apartments:
        ui->tableView->setColumnWidth(1, 190);
        ui->tableView->setColumnWidth(2, 190);
        ui->tableView->setColumnWidth(3, 190);
        ui->tableView->setColumnWidth(4, 190);
        ui->tableView->setColumnWidth(5, 190);
        ui->tableView->setColumnWidth(6, 190);
        ui->tableView->setColumnWidth(7, 190);
        ui->tableView->setColumnWidth(8, 190);
        ui->tableView->setColumnWidth(9, 190);
        ui->tableView->setColumnWidth(10, 190);
        ui->tableView->setColumnWidth(11, 190);
        ui->tableView->setColumnWidth(12, 190);
        ui->tableView->setColumnWidth(13, 190);
        ui->tableView->setColumnWidth(14, 190);

        ui->bottomTable->setColumnWidth(0, 175);
        ui->bottomTable->setColumnWidth(1, 175);
        ui->bottomTable->setColumnWidth(2, 175);
        ui->bottomTable->setColumnWidth(3, 175);
        ui->bottomTable->setColumnWidth(4, 175);
        ui->bottomTable->setColumnWidth(5, 175);
        ui->bottomTable->setColumnWidth(6, 175);
        ui->bottomTable->setColumnWidth(7, 175);
        break;
    case GeneralReportsE::Objects:
        ui->tableView->setColumnWidth(0, 150);
        ui->tableView->setColumnWidth(1, 150);
        ui->tableView->setColumnWidth(2, 150);
        ui->tableView->setColumnWidth(3, 170);
        ui->tableView->setColumnWidth(4, 150);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 150);
        ui->tableView->setColumnWidth(7, 150);
        ui->tableView->setColumnWidth(8, 150);
        ui->tableView->setColumnWidth(9, 150);
        ui->tableView->setColumnWidth(10, 170);
        ui->tableView->setColumnWidth(11, 170);

        ui->bottomTable->setColumnWidth(0, 136);
        ui->bottomTable->setColumnWidth(1, 136);
        ui->bottomTable->setColumnWidth(2, 136);
        ui->bottomTable->setColumnWidth(3, 180);
        ui->bottomTable->setColumnWidth(4, 136);
        ui->bottomTable->setColumnWidth(5, 136);
        ui->bottomTable->setColumnWidth(6, 136);
        ui->bottomTable->setColumnWidth(7, 136);
        ui->bottomTable->setColumnWidth(8, 144);
        ui->bottomTable->setColumnWidth(9, 128);

        break;
    default:
        break;
    }
}



void GeneralReports::setBottomTable() {
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);

    switch (this->table) {
    case GeneralReportsE::PaymentTypes:
    {
        model->setHorizontalHeaderLabels({"Общая Сумма KGS", "Общая Сумма USD"});  // Два столбца для KGS и USD

        // Инициализируем суммы для KGS и USD
        double totalKGS = 0.0;
        double totalUSD = 0.0;

        // Получаем данные из отчета
        for (const QVariant &data : ReportOperations::getAllPaymentTypesReport(this->fromDate, this->toDate)) {
            QVariantMap paymentType = data.toMap();  // Используем toMap(), так как данные в формате QVariantMap

            // Суммируем суммы по валютам
            if (paymentType["currency"].toString() == "KGS") {
                totalKGS += paymentType["total_amount"].toDouble();
            } else if (paymentType["currency"].toString() == "USD") {
                totalUSD += paymentType["total_amount"].toDouble();
            }
        }

        // Создаем строку для модели
        QList<QStandardItem *> row;

        // Добавляем сумму для KGS в первый столбец
        QStandardItem *itemKGS = new QStandardItem(locale.toString(totalKGS, 'f', 2));  // Сумма KGS
        itemKGS->setTextAlignment(Qt::AlignCenter);  // Центровка текста
        row << itemKGS;

        // Добавляем сумму для USD во второй столбец
        QStandardItem *itemUSD = new QStandardItem(locale.toString(totalUSD, 'f', 2));  // Сумма USD
        itemUSD->setTextAlignment(Qt::AlignCenter);  // Центровка текста
        row << itemUSD;

        // Добавляем строку в модель
        model->appendRow(row);
    }
        break;

    case GeneralReportsE::Objects:
    {
        model->setHorizontalHeaderLabels({
            "Продано", "Не продано", "Рассрочка", "Полный расчет",
            "Сумма KGS", "Сумма USD", "Долг KGS", "Долг USD",
            "Приход KGS", "Приход USD"
        });

        for (const QVariant &report : ReportOperations::getAllObjectsReport(this->fromDate, this->toDate)) {
            QVariantList data = report.toList();
            QList<QStandardItem *> row;

            row << new QStandardItem(QString::number(data[0].toInt()));
            row << new QStandardItem(QString::number(data[1].toInt()));
            row << new QStandardItem(QString::number(data[2].toInt()));
            row << new QStandardItem(QString::number(data[3].toInt()));

            row << new QStandardItem(locale.toString(data[4].toDouble(), 'f', 2));
            row << new QStandardItem(locale.toString(data[5].toDouble(), 'f', 2));
            row << new QStandardItem(locale.toString(data[6].toDouble(), 'f', 2));
            row << new QStandardItem(locale.toString(data[7].toDouble(), 'f', 2));
            row << new QStandardItem(locale.toString(data[8].toDouble(), 'f', 2));
            row << new QStandardItem(locale.toString(data[9].toDouble(), 'f', 2));

            model->appendRow(row);
        }
    }
        break;

    case GeneralReportsE::Expenses:
    {
        model->setHorizontalHeaderLabels({"Общая сумма KGS", "Общая сумма USD"});
        for (const QVariant &client : ReportOperations::getAllExpensesReport(this->fromDate, this->toDate)) {
            QVariantList extrServices = client.toList();

            QList<QStandardItem *> row;

            QStandardItem *amountKGSItem = new QStandardItem(locale.toString(extrServices[0].toDouble(), 'f', 2));
            amountKGSItem->setTextAlignment(Qt::AlignCenter);
            row.append(amountKGSItem);

            QStandardItem *amountUSDItem = new QStandardItem(locale.toString(extrServices[1].toDouble(), 'f', 2));
            amountUSDItem->setTextAlignment(Qt::AlignCenter);
            row.append(amountUSDItem);

            model->appendRow(row);
        }
    }
        break;

    case GeneralReportsE::Clients:
    {
        model->setHorizontalHeaderLabels({"Общая сумма KGS", "Общая сумма USD", "Общая оплата KGS", "Общая оплата USD", "Осталось KGS", "Осталось USD"});
        for (const QVariant &debt : ReportOperations::getAllClientsReport(this->fromDate, this->toDate)) {
            QVariantList debts = debt.toList();

            QList<QStandardItem *> row;

            // Общая сумма KGS
            QStandardItem *totalAmountKGSItem = new QStandardItem(locale.toString(debts[0].toDouble(), 'f', 2));
            totalAmountKGSItem->setTextAlignment(Qt::AlignCenter);
            row << totalAmountKGSItem;

            // Общая сумма USD123
            QStandardItem *totalAmountUSDItem = new QStandardItem(locale.toString(debts[1].toDouble(), 'f', 2));
            totalAmountUSDItem->setTextAlignment(Qt::AlignCenter);
            row << totalAmountUSDItem;

            // Общая оплата KGS
            QStandardItem *paidKGSItem = new QStandardItem(locale.toString(debts[2].toDouble(), 'f', 2));
            paidKGSItem->setTextAlignment(Qt::AlignCenter);
            row << paidKGSItem;

            // Общая оплата USD
            QStandardItem *paidUSDItem = new QStandardItem(locale.toString(debts[3].toDouble(), 'f', 2));
            paidUSDItem->setTextAlignment(Qt::AlignCenter);
            row << paidUSDItem;

            // Осталось KGS
            QStandardItem *remainingKGSItem = new QStandardItem(locale.toString(debts[4].toDouble(), 'f', 2));
            remainingKGSItem->setTextAlignment(Qt::AlignCenter);
            row << remainingKGSItem;

            // Осталось USD
            QStandardItem *remainingUSDItem = new QStandardItem(locale.toString(debts[5].toDouble(), 'f', 2));
            remainingUSDItem->setTextAlignment(Qt::AlignCenter);
            row << remainingUSDItem;

            model->appendRow(row);
        }
    }
        break;

    case GeneralReportsE::Apartments:
    {
        model->setHorizontalHeaderLabels({
            "Скидка KGS", "Скидка USD",
            "Сумма KGS", "Сумма USD",
            "Оплачено KGS", "Оплачено USD",
            "Долг KGS", "Долг USD"
        });

        QVariantMap report = ReportOperations::getAllApartmentsReport(this->objectId, this->fromDate, this->toDate);

        QList<QStandardItem *> row;
        row << new QStandardItem(locale.toString(report["totalDiscount_KGS"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalDiscount_USD"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalAmount_KGS"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalAmount_USD"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalPaid_KGS"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalPaid_USD"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalRemaining_KGS"].toDouble(), 'f', 2));
        row << new QStandardItem(locale.toString(report["totalRemaining_USD"].toDouble(), 'f', 2));

        for (QStandardItem *item : row) {
            item->setTextAlignment(Qt::AlignCenter);
        }
        model->appendRow(row);
    }
    break;

    case GeneralReportsE::Debts:
    {
        model->setHorizontalHeaderLabels({"Общий долг KGS", "Общий долг USD"});
        for (const QVariant &debt : ReportOperations::getAllDebtsReport()) {
            QVariantList debts = debt.toList();
            QList<QStandardItem *> row;

            QStandardItem *totalDebtKGSItem = new QStandardItem(locale.toString(debts[0].toDouble(), 'f', 2));
            totalDebtKGSItem->setTextAlignment(Qt::AlignCenter);
            row << totalDebtKGSItem;

            QStandardItem *totalDebtUSDItem = new QStandardItem(locale.toString(debts[1].toDouble(), 'f', 2));
            totalDebtUSDItem->setTextAlignment(Qt::AlignCenter);
            row << totalDebtUSDItem;

            model->appendRow(row);
        }
    }
        break;


    default:
        break;
    }

    ui->bottomTable->setModel(model);
    ui->bottomTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->bottomTable->horizontalHeader()->setStretchLastSection(true);
    ui->bottomTable->horizontalHeader()->setCascadingSectionResizes(true);


}





void GeneralReports::on_BackButton_clicked()
{
    nav->changeWindow(0);
}


void GeneralReports::on_ReportButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (select->hasSelection()) {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->table) {
        case GeneralReportsE::PaymentTypes:
            nav->openReports(0, id, fromDate, toDate);
            break;
        case GeneralReportsE::Objects:
            nav->openReports(1, id, fromDate, toDate);
            break;
        case GeneralReportsE::Expenses:
            nav->openReports(2, id, fromDate, toDate);
            break;

        case GeneralReportsE::Clients:
            nav->openReports(3, id, fromDate, toDate);
            break;
        case GeneralReportsE::Apartments:
            nav->openReports(4, id, fromDate, toDate);
            break;
        case GeneralReportsE::Debts:
            nav->openReports(5, id, fromDate, toDate);
            break;

        default:
            break;
        }
    } else {
        switch (this->table) {
        case GeneralReportsE::PaymentTypes:
            nav->openReports(0,0, fromDate, toDate);
            break;
        case GeneralReportsE::Objects:
            nav->openReports(1,0, fromDate, toDate);
            break;
        case GeneralReportsE::Expenses:
            nav->openReports(2,0, fromDate, toDate);
            break;

        case GeneralReportsE::Clients:
            nav->openReports(3,0,fromDate, toDate);
            break;
        case GeneralReportsE::Apartments:
            QMessageBox::critical(this, "Ошибка", "Выберите квартиру для перехода ");
            break;
        case GeneralReportsE::Debts:
            QMessageBox::critical(this, "Ошибка", "Выберите должника для перехода ");
            break;
        default:
            break;
        }
    }
}



void GeneralReports::on_SettingsButton_clicked()
{
    switch (this->table) {
    case GeneralReportsE::PaymentTypes:
        nav->openSettings(3);
        break;
    case GeneralReportsE::Objects:
        nav->openSettings(0);
        break;
    case GeneralReportsE::Expenses:
        nav->openSettings(1);
        break;
    case GeneralReportsE::Clients:
        nav->openSettings(2);
        break;
    case GeneralReportsE::Apartments:
        nav->openSettings(0);
        break;
    default:
        break;
    }
}



void GeneralReports::on_ToPDFButton_clicked()
{
    QString title;
    switch (this->table) {
    case GeneralReportsE::PaymentTypes:
        title = "Отчет по типам оплат";
        break;
    case GeneralReportsE::Objects:
        title = "Отчет по объектам";
        break;
    case GeneralReportsE::Expenses:
        title = "Отчет по расходам";
        break;
    case GeneralReportsE::Clients:
        title = "Отчет по клиентам";
        break;
    case GeneralReportsE::Apartments:
        title = "Отчет по квартирам";
        break;
    case GeneralReportsE::Debts:
        title = "Отчет по долгам";
        break;
    default:
        break;
    }
    QString dates = this->fromDate.toString("dd.MM.yyyy") + " - " + this->toDate.toString("dd.MM.yyyy");


    // уменьшение текста для отчета по КВАРТИРАМ =====================
    if (this->table == GeneralReportsE::Apartments) {
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



        PDFmanager::exportToPDF(title, dates, { proxyTop, proxyBottom });
    } else {
        PDFmanager::exportToPDF(title, dates, { ui->tableView->model(), ui->bottomTable->model() });
    }
}




void GeneralReports::on_ToEXCELButton_clicked()
{
    ExcelManager excelManager;
    QString title;
    switch (this->table) {
    case GeneralReportsE::PaymentTypes:
        title = "Отчет по типам оплат";
        break;
    case GeneralReportsE::Objects:
        title = "Отчет по объектам";
        break;
    case GeneralReportsE::Expenses:
        title = "Отчет по расходам";
        break;
    case GeneralReportsE::Clients:
        title = "Отчет по клиентам";
        break;
    case GeneralReportsE::Apartments:
        title = "Отчет по квартирам";
        break;
    case GeneralReportsE::Debts:
        title = "Отчет по долгам";
        break;
    default:
        break;
    }
    QString dates = this->fromDate.toString("dd.MM.yyyy") + " - " + this->toDate.toString("dd.MM.yyyy");
    QList<QAbstractItemModel *> models = { ui->tableView->model(), ui->bottomTable->model() };
    int start = 0;
    excelManager.exportToExcel(title, dates, models, start);
}




void GeneralReports::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromDate);

    connect(c, &CalendarPage::changeDate, this, &GeneralReports::setFromDate);

    c->show();
}

void GeneralReports::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->toDate);

    connect(c, &CalendarPage::changeDate, this, &GeneralReports::setToDate);

    c->show();
}

void GeneralReports::setFromDate(QDate date)
{
    this->fromDate = date;
    ui->FromDateButton->setText(date.toString("dd.MM.yyyy"));
    updateData();
    emit datesChanged(this->fromDate, this->toDate);
}

void GeneralReports::setToDate(QDate date)
{
    this->toDate = date;
    ui->ToDateButton->setText(date.toString("dd.MM.yyyy"));
    updateData();
    emit datesChanged(this->fromDate, this->toDate);
}

void GeneralReports::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}


void GeneralReports::adjustColumnWidths()
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



void GeneralReports::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order) {
    this->selectedColumn = logicalIndex;
    this->sortOrder = order;
}

void GeneralReports::updateData()
{
    setTable();
    setBottomTable();
    setTableSizes();
}


void GeneralReports::on_objectBox_currentIndexChanged(int index)
{
    if (index == 0) {
        objectId = -1;
    } else {
        QString objectName = ui->objectBox->itemText(index);
        objectId = objects.value(objectName, -1);
    }

    updateData();
}





