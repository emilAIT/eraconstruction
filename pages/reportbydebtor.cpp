#include "reportbydebtor.h"
#include "ui_reportbydebtor.h"
#include "../enums/ReportsE.h"

ReportByDebtor::ReportByDebtor(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::ReportByDebtor)
{
    ui->setupUi(this);
    this->nav = nav;

    ui->Header->setText("По должнику");
}

ReportByDebtor::~ReportByDebtor()
{
    delete ui;
}

void ReportByDebtor::setReports(ReportsE table, int id)
{
    this->selectedColumn = -1;
    this->table = table;
    this->id = id;
    ui->allCostButton->setText("");
    ui->remainingButton->setText("");
    ui->paidButton->setText("");
    ui->monthlyButton->setText("");
    ui->paidMonthsButton->setText("");
    ui->missedMonthsButton->setText("");
    ui->arrearsButton->setText("");
    ui->currencyButton->setText("");
    setHeader();
    setTable();
    setBottomTable();
    setTableSizes();
}

void ReportByDebtor::setHeader()
{
    switch (this->table)
    {
    case ReportsE::Apartments:
        ui->Header->setText("по квартире:");
        ui->allCostLabel->setText("Клиент:");
        ui->remainingLabel->setText("Объект:");
        ui->paidLabel->setText("Кол-во комнат:");
        ui->monthlyLabel->setText("Санузел:");
        ui->paidMonthsLabel->setText("Площадь:");
        ui->missedMonthsLabel->setText("Этаж:");
        ui->arrearsLabel->setText("Стоимость:");
        ui->currencyLabel->setText("Валюта:");
        ui->SettingsButton->show();
        break;
    case ReportsE::Debts:
        ui->Header->setText("по должнику");
        ui->allCostLabel->setText("Общая стоимость:");
        ui->remainingLabel->setText("Осталось:");
        ui->paidLabel->setText("Оплачено:");
        ui->monthlyLabel->setText("Оплата за месяц:");
        ui->paidMonthsLabel->setText("Оплаченные месяцы:");
        ui->missedMonthsLabel->setText("Пропущенные месяцы:");
        ui->arrearsLabel->setText("Должен:");
        ui->currencyLabel->setText("Валюта:");
        ui->SettingsButton->hide();
        break;
    default:
        break;
    }
}

void ReportByDebtor::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void ReportByDebtor::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);

    switch (this->table)
    {
    case ReportsE::Apartments:
    {
        QVariantList apartmentInfo = ReportOperations::getApartmentInformation(this->id);

        if (!apartmentInfo.isEmpty())
        {
            QVariantList apartmentData = apartmentInfo.first().toList();

            ui->allCostButton->setText(apartmentData[0].toString());
            ui->remainingButton->setText(apartmentData[1].toString());
            ui->paidButton->setText(QString::number(apartmentData[2].toInt()));
            ui->monthlyButton->setText(QString::number(apartmentData[3].toInt()));
            ui->paidMonthsButton->setText(QString::number(apartmentData[4].toDouble()));
            ui->missedMonthsButton->setText(QString::number(apartmentData[5].toInt()));
            ui->currencyButton->setText(apartmentData[8].toString());
            double amount = apartmentData[7].toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            ui->arrearsButton->setText(amountStr);
        }
        else
        {
            ui->allCostButton->setText("");
            ui->remainingButton->setText("");
            ui->paidButton->setText("");
            ui->monthlyButton->setText("");
            ui->paidMonthsButton->setText("");
            ui->missedMonthsButton->setText("");
            ui->arrearsButton->setText("");
        }

        model->setHorizontalHeaderLabels({"Дата", "Тип оплаты", "Оплатил", "Описание"});
        for (const QVariant &payment : ReportOperations::getApartmentPayments(this->id))
        {
            QVariantList values = payment.toList();
            QList<QStandardItem *> rowItems;

            rowItems << new QStandardItem(values[0].toString()); // Дата
            rowItems << new QStandardItem(values[1].toString()); // Тип оплаты

            double amount = values[2].toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            rowItems << new QStandardItem(amountStr); // Оплатил

            rowItems << new QStandardItem(values[4].toString()); // Описание

            model->appendRow(rowItems);
            for (QStandardItem *item : rowItems) {
                item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            }
        }
    }
    break;

    case ReportsE::Debts:
    {
        isEnd = 0;
        monthQuantity = 0;
        QLocale locale(QLocale::Russian);
        QVariantList clientInfo = ReportOperations::getClientInformation(this->id);

        // Отображение информации о клиенте
        if (!clientInfo.isEmpty() && clientInfo[0].canConvert<QVariantList>())
        {
            QVariantList row = clientInfo[0].toList();

            if (row.size() >= 7)
            {
                QString remainingAmount = locale.toString(row[1].toDouble(), 'f', 2);
                QString monthlyPayment = locale.toString(row[3].toDouble(), 'f', 2);
                QString missedMonths = locale.toString(row[5].toDouble(), 'f', 0);
                QString paid = locale.toString(row[2].toDouble(), 'f', 2);
                double arrearsAmount = row[5].toInt() > 0 ? row[6].toDouble() : 0;
                isEnd = row[7].toInt();
                monthQuantity = row[8].toInt();

                ui->allCostButton->setText(locale.toString(row[0].toDouble(), 'f', 2));
                ui->remainingButton->setText(remainingAmount);
                ui->paidButton->setText(paid);
                ui->monthlyButton->setText(monthlyPayment);
                ui->paidMonthsButton->setText(QString::number(row[4].toInt()));
                ui->missedMonthsButton->setText(missedMonths);
                ui->arrearsButton->setText(locale.toString(arrearsAmount, 'f', 2));
                ui->currencyButton->setText(row[9].toString());
            }
            else
            {
                qDebug() << "Row size is less than 7!";
            }
        }
        else
        {
            qDebug() << "clientInfo is empty or not in expected format!";
        }

        // Настройка модели таблицы
        model->setHorizontalHeaderLabels({"Дата", "Оплата"});

        QList<QVariant> payments = ReportOperations::getClientPayments(this->id);
        QList<QDate> processedMonths;

        double remaining = 0;

        for (int i = 0; i < payments.size(); ++i)
        {
            QVariantList values = payments[i].toList();

            QDate paymentDate = values[2].toDate();
            QDate saleDate = values[0].toDate();
            QDate currentDate = saleDate.addMonths(1);

            double amount = values[3].toDouble() + remaining;
            double monthlyPayment = values[1].toDouble();

            while (monthlyPayment <= amount)
            {
                QString displayValue;

                if (processedMonths.contains(currentDate))
                {
                    currentDate = currentDate.addMonths(1);
                    continue;
                }

                if (monthQuantity == 0)
                {
                    break;
                }

                if (paymentDate.isValid() && paymentDate > currentDate)
                {
                    displayValue = locale.toString(monthlyPayment, 'f', 2) + " (С опозданием)";
                }
                else if (paymentDate.isValid() && paymentDate < currentDate)
                {
                    displayValue = locale.toString(monthlyPayment, 'f', 2) + " (Досрочно)";
                }
                else
                {
                    displayValue = locale.toString(monthlyPayment, 'f', 2);
                }

                QList<QStandardItem *> rowItems;
                rowItems << new QStandardItem(currentDate.toString("yyyy-MM-dd"));
                rowItems << new QStandardItem(displayValue);
                model->appendRow(rowItems);
                for (QStandardItem *item : rowItems) {
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                }
                processedMonths.append(currentDate);
                currentDate = currentDate.addMonths(1);
                monthQuantity--;
                amount -= monthlyPayment;
            }

            remaining = 0;
            if (i < payments.size() - 1)
            {
                remaining += amount;
            }
            else
            {
                QString display;
                if (currentDate < paymentDate && isEnd == 1)
                {
                    display = locale.toString(amount, 'f', 2) + " (С опозданием)";
                }
                else if (currentDate < paymentDate && isEnd == 1)
                {
                    display = locale.toString(amount, 'f', 2) + " (Досрочно)";
                }
                else if (currentDate < paymentDate && amount != 0)
                {
                    display = locale.toString(amount, 'f', 2) + " (Недостаточно)";
                }
                else if (currentDate < paymentDate && amount != 0 && isEnd == 1)
                {
                    display = locale.toString(amount, 'f', 2) + " (С опозданием)";
                }
                else
                {
                    display = locale.toString(amount, 'f', 2) + " (Ожидается)";
                }
                if (amount != 0)
                {
                    QList<QStandardItem *> rowItems;
                    rowItems << new QStandardItem(currentDate.toString("yyyy-MM-dd"));
                    rowItems << new QStandardItem(display);
                    model->appendRow(rowItems);
                    for (QStandardItem *item : rowItems) {
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    }
                    amount = 0;
                    currentDate = currentDate.addMonths(1);
                    monthQuantity--;
                }

                while (monthQuantity != 0)
                {
                    if (processedMonths.contains(currentDate))
                    {
                        currentDate = currentDate.addMonths(1);
                        continue;
                    }

                    QList<QStandardItem *> rowItems;
                    display = (currentDate < QDate::currentDate()) ? "Пропущен" : "Ожидается";
                    rowItems << new QStandardItem(currentDate.toString("yyyy-MM-dd"));
                    rowItems << new QStandardItem(display);
                    model->appendRow(rowItems);
                    for (QStandardItem *item : rowItems) {
                        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                    }

                    processedMonths.append(currentDate);
                    currentDate = currentDate.addMonths(1);

                    qDebug() << "whattttttttttttttttttt" << monthQuantity;

                    monthQuantity--;
                    qDebug() << "whattttttttttttttttttt" << monthQuantity;
                }
            }
        }

        // Обработка платежей
        // QList<QVariant> payments = ReportOperations::getClientPayments(this->id);
        // QList<QDate> processedMonths;

        // QMap<QDate, double> paymentMap;
        // for (const QVariant& payment : payments) {
        //     QVariantList values = payment.toList();
        //     if (values.size() < 4) continue;

        //     QDate payDate = values[2].toDate();
        //     double amount = values[3].toDouble();

        //     if (paymentMap.contains(payDate)) {
        //         paymentMap[payDate] += amount;
        //     } else {
        //         paymentMap.insert(payDate, amount);
        //     }
        // }

        // qDebug() << payments;
        // for (int i = 0; i < payments.size(); ++i) {
        //     QVariantList values = payments[i].toList();
        //     qDebug() << "Payment data for iteration" << i << ":" << values;

        //     if (values.size() < 4) {
        //         qDebug() << "Invalid payment data format!";
        //         continue;
        //     }

        //     QDate saleDate = values[0].toDate();
        //     double monthlyPayment = values[1].toDouble();
        //     QDate paymentDate = values[2].toDate();
        //     double paymentAmount = values[3].toDouble();

        //     qDebug() << "Sale Date:" << saleDate;
        //     qDebug() << "Monthly Payment:" << monthlyPayment;
        //     qDebug() << "Payment Date:" << paymentDate;
        //     qDebug() << "Payment Amount:" << paymentAmount;
        //     // double remainingPayment;

        //     // // Начало платежей через месяц после покупки
        //     QDate currentDate = saleDate.addMonths(1);
        //     // for (int j = 0; j < payments.size(); ++j) {
        //     //     QVariantList additionalValues = payments[j].toList();
        //     //     if (additionalValues.size() < 4) continue;

        //     //     QDate additionalPaymentDate = additionalValues[2].toDate();
        //     //     double additionalPaymentAmount = additionalValues[3].toDouble();

        //     //     // Собираем все платежи, которые относятся к одному клиенту
        //     //     if (paymentDate == additionalPaymentDate) {
        //     //         remainingPayment += additionalPaymentAmount;
        //     //     }
        //     // }

        //     double remainingPayment = paymentAmount;  // Начинаем с суммы первого платежа

        //     qDebug() << "Total Remaining Payment:" << remainingPayment;

        //     // Обработка всех месяцев
        //     while (remainingPayment > 0 && monthQuantity > 0) {
        //         QString displayValue;

        //         if (remainingPayment >= monthlyPayment) {
        //             if (paymentDate.isValid() && paymentDate > currentDate) {
        //                 displayValue = locale.toString(monthlyPayment, 'f', 2) + " (С опозданием)";
        //             } else if (paymentDate.isValid() && paymentDate < currentDate) {
        //                 displayValue = locale.toString(monthlyPayment, 'f', 2) + " (Досрочно)";
        //             } else {
        //                 displayValue = locale.toString(monthlyPayment, 'f', 2);
        //             }
        //             remainingPayment -= monthlyPayment;
        //             qDebug() << "Remaining Payment after subtraction:" << remainingPayment;
        //         }
        //         else if (paymentDate.isValid() && paymentDate > currentDate && isEnd == 1) {
        //             displayValue = locale.toString(remainingPayment, 'f', 2) + " (С опозданием)";
        //             remainingPayment = 0;
        //             qDebug() << "Remaining Payment after finalization:" << remainingPayment;
        //         }
        //         else if (paymentDate.isValid() && paymentDate < currentDate && isEnd == 1) {
        //             displayValue = locale.toString(remainingPayment, 'f', 2) + " (Досрочно)";
        //             remainingPayment = 0;
        //             qDebug() << "Remaining Payment after finalization:" << remainingPayment;
        //         }
        //         else if (remainingPayment > 0 && currentDate < paymentDate) {
        //             displayValue = locale.toString(remainingPayment, 'f', 2) + " (Недостаточно)";
        //             remainingPayment = 0;
        //             qDebug() << "Remaining Payment after finalization:" << remainingPayment;
        //         }
        //         else if (remainingPayment > 0 && currentDate > paymentDate) {
        //             displayValue = locale.toString(remainingPayment, 'f', 2) + " (Ожидается)";
        //             remainingPayment = 0;
        //             qDebug() << "Remaining Payment after finalization:" << remainingPayment;
        //         }
        //         else {
        //             displayValue = (currentDate < QDate::currentDate()) ? "Пропущен" : "Ожидается";
        //             qDebug() << "No payment made for this month.";
        //         }

        //         // Добавляем строку в таблицу
        //         QList<QStandardItem*> rowItems;
        //         rowItems << new QStandardItem(currentDate.toString("yyyy-MM-dd"));
        //         rowItems << new QStandardItem(displayValue);
        //         model->appendRow(rowItems);

        //         // Увеличиваем дату на месяц
        //         currentDate = currentDate.addMonths(1);
        //         monthQuantity--;
        //     }}
    }
    break;
    default:
        break;
    }

    auto *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setSortRole(Qt::UserRole);

    ui->tableView->setModel(proxyModel);
    // ui->tableView->setColumnHidden(0, true);  // Скрываем ID
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    if (this->selectedColumn != -1)
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);
}

void ReportByDebtor::setBottomTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    QLocale locale(QLocale::Russian);
    switch (this->table)
    {
    case ReportsE::Apartments:
        model->setHorizontalHeaderLabels({"Общая сумма"});
        for (const QVariant &paymentType : ReportOperations::getApartmentAllPayments(this->id))
        {
            QVariantList data = paymentType.toList();

            QList<QStandardItem *> row;

            QStandardItem *item = new QStandardItem(locale.toString(data[0].toDouble(), 'f', 2));
            item->setTextAlignment(Qt::AlignCenter); // Выравнивание по центру
            item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Запрет на изменения
            row << item;
            model->appendRow(row);
        }
        break;

    case ReportsE::Debts:
        ui->bottomTable->hide();
        break;

    default:
        break;
    }

    ui->bottomTable->setModel(model);
    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void ReportByDebtor::setTableSizes()
{
    switch (this->table)
    {
    case ReportsE::Debts:
        break;
    case ReportsE::Apartments:
        ui->tableView->setColumnWidth(0, 190);
        ui->tableView->setColumnWidth(1, 190);
        ui->tableView->setColumnWidth(2, 190);
        ui->tableView->setColumnWidth(3, 190);
        break;

    default:
        break;
    }
}

void ReportByDebtor::on_SettingsButton_clicked()
{
    nav->openSettings(0);
}



void ReportByDebtor::on_ToPDFButton_clicked()
{
    QString title;
    int start = 1;
    // QString dates = QDate::currentDate().toString("dd.MM.yyyy");

    switch (this->table)
    {
    case ReportsE::Apartments:
    {
        QVariantList salesList = Operations::getSale(this->id);
        if (salesList.isEmpty())
        {
            qWarning() << "Ошибка: данные о продаже не найдены!";
            return;
        }

        QVariantList saleData = salesList.first().toList();
        if (saleData.size() <= 3)
        {
            qWarning() << "Ошибка: недостаточно данных в saleData!";
            return;
        }

        title = "Отчет по квартире " + saleData[3].toString();
        start = 0;
    }
    break;

    case ReportsE::Debts:
    {
        QVariantList salesList = Operations::getSale(this->id);
        if (salesList.isEmpty())
        {
            qWarning() << "Ошибка: данные о продаже не найдены!";
            return;
        }

        QVariantList saleData = salesList.first().toList();
        if (saleData.size() <= 3)
        {
            qWarning() << "Ошибка: недостаточно данных в saleData!";
            return;
        }

        title = "Отчет по должнику " + saleData[1].toString();
        start = 0;
    }
    break;
    default:
        return;
    }

    // Собираем дополнительные данные с правой части страницы
    QMap<QString, QString> additionalData;
    additionalData[ui->allCostLabel->text()] = ui->allCostButton->text();
    additionalData[ui->remainingLabel->text()] = ui->remainingButton->text();
    additionalData[ui->paidLabel->text()] = ui->paidButton->text();
    additionalData[ui->monthlyLabel->text()] = ui->monthlyButton->text();
    additionalData[ui->paidMonthsLabel->text()] = ui->paidMonthsButton->text();
    additionalData[ui->missedMonthsLabel->text()] = ui->missedMonthsButton->text();
    additionalData[ui->arrearsLabel->text()] = ui->arrearsButton->text();
    additionalData[ui->currencyLabel->text()] = ui->currencyButton->text();

    // Создаем экземпляр нового PDF менеджера для подробных страниц
    PDFForDetailedPages pdfDetailed;
    pdfDetailed.exportToPDF(title, "", additionalData, {ui->tableView->model(), ui->bottomTable->model()}, start);
}




void ReportByDebtor::on_ToEXCELButton_clicked()
{
    QString title;
    int start = 1;

    switch (this->table)
    {
    case ReportsE::Apartments:
    {
        QVariantList salesList = Operations::getSale(this->id);
        if (salesList.isEmpty())
        {
            qWarning() << "Ошибка: данные о продаже не найдены!";
            return;
        }

        QVariantList saleData = salesList.first().toList();
        if (saleData.size() <= 3)
        {
            qWarning() << "Ошибка: недостаточно данных в saleData!";
            return;
        }

        title = "Отчет по квартире " + saleData[3].toString();
        start = 0;
    }
    break;
    case ReportsE::Debts:
    {
        QVariantList salesList = Operations::getSale(this->id);
        if (salesList.isEmpty())
        {
            qWarning() << "Ошибка: данные о продаже не найдены!";
            return;
        }

        QVariantList saleData = salesList.first().toList();
        if (saleData.size() <= 3)
        {
            qWarning() << "Ошибка: недостаточно данных в saleData!";
            return;
        }

        title = "Отчет по должнику " + saleData[1].toString();
        start = 0;
    }
    break;
    default:
        return;
    }

    QMap<QString, QString> additionalData;
    additionalData[ui->allCostLabel->text()] = ui->allCostButton->text();
    additionalData[ui->remainingLabel->text()] = ui->remainingButton->text();
    additionalData[ui->paidLabel->text()] = ui->paidButton->text();
    additionalData[ui->monthlyLabel->text()] = ui->monthlyButton->text();
    additionalData[ui->paidMonthsLabel->text()] = ui->paidMonthsButton->text();
    additionalData[ui->missedMonthsLabel->text()] = ui->missedMonthsButton->text();
    additionalData[ui->arrearsLabel->text()] = ui->arrearsButton->text();
    additionalData[ui->currencyLabel->text()] = ui->currencyButton->text();


    ExcelForDetailedPages excelDetailed;
    excelDetailed.exportToExcel(title, "", additionalData,
                                {ui->tableView->model(), ui->bottomTable->model()},
                                start);
}

