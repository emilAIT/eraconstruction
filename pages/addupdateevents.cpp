#include "addupdateevents.h"
#include "pages/ui_addupdateevents.h"

AddUpdateEvents::AddUpdateEvents(int id, EventsE eventType, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddUpdateEvents)
{
    ui->setupUi(this);
    this->id = id;
    this->eventType = eventType;

    this->setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setProperty("color", "green");

    ui->costEdit->setValidator(new QDoubleValidator());
    ui->discountEdit->setValidator(new QDoubleValidator());
    ui->initialPaymentEdit->setValidator(new QDoubleValidator());
    ui->monthQuantityEdit->setValidator(new QIntValidator());
    ui->monthlyPaymentEdit->setValidator(new QDoubleValidator());
    ui->totalEdit->setValidator(new QDoubleValidator());

    ui->descriptionLabel->setProperty("color", "white");
    ui->dateButton->setProperty("color", "gray");

    date = QDate::currentDate();
    ui->dateButton->setText(date.toString("dd.MM.yyyy"));

    // Устанавливаем свойства для заголовков
    ui->dateLabel->setProperty("color", "white");
    ui->clientLabel->setProperty("color", "white");
    ui->objectLabel->setProperty("color", "white");
    ui->apartmentLabel->setProperty("color", "white");
    ui->costLabel->setProperty("color", "white");
    ui->discountLabel->setProperty("color", "white");
    ui->totalLabel->setProperty("color", "white");
    ui->paymentTypeLabel->setProperty("color", "white");
    ui->currencyLabel->setProperty("color", "white");
    ui->installmentLabel->setProperty("color", "white");
    ui->installmentCheckBox->setProperty("color", "white");
    ui->initialPaymentLabel->setProperty("color", "white");
    ui->monthQuantityLabel->setProperty("color", "white");
    ui->monthlyPaymentLabel->setProperty("color", "white");
    ui->descriptionLabel->setProperty("color", "white");

    setComboBoxesData();

    // Устанавливаем заголовок и загружаем данные в зависимости от типа события
    switch (this->eventType) {
    case EventsE::Sales:
        ui->Header->setText("Редактировать продажу");
        loadSalesData();
        break;

    case EventsE::Payments:
        ui->Header->setText("Редактировать оплату");
        loadPaymentData();
        break;

    default:
        ui->Header->setText("Редактировать событие");
        break;
    }


    connect(ui->initialPaymentEdit, &QLineEdit::textChanged, this, &AddUpdateEvents::updateMonthlyPayment);
    connect(ui->monthQuantityEdit, &QLineEdit::textChanged, this, &AddUpdateEvents::updateMonthlyPayment);
}

AddUpdateEvents::~AddUpdateEvents() {
    delete ui;
}

void AddUpdateEvents::setComboBoxesData() {

    ui->clientBox->clear();
    ui->objectBox->clear();
    ui->apartmentBox->clear();
    ui->paymentTypeBox->clear();
    clients.clear();
    objects.clear();
    apartments.clear();
    paymentsType.clear();

    switch (this->eventType) {
    case EventsE::Payments:
    {
        QVariantList clientsList = Operations::selectClientsWithPurchase();
        ui->clientBox->addItem("-");

        foreach (QVariant client, clientsList)
        {
            QVariantList data = client.toList();
            QString show = data[1].toString();
            clients.insert(show, data[0].toInt());
            ui->clientBox->addItem(show);
        }
    }
        break;

    case EventsE::Sales:
    {
        QVariantList clientsList = Operations::selectAllClients();
        ui->clientBox->addItem("-");
        foreach (QVariant client, clientsList) {
            QVariantList data = client.toList();
            QString show = data[1].toString();
            clients.insert(show, data[0].toInt());
            ui->clientBox->addItem(show);
        }

        // Загрузка объектов
        QVariantList objectsList = Operations::selectAvailableObjects(this->id);

        ui->objectBox->addItem("-");

        foreach (QVariant object, objectsList)
        {
            QVariantList data = object.toList();
            QString show = data[1].toString();
            objects.insert(show, data[0].toInt());
            ui->objectBox->addItem(show);
        }
    }
        break;

    default:
        break;
    }

    // payments type
    QVariantList paymenttypesList = Operations::selectAllPaymentTypes();
    ui->paymentTypeBox->addItem("-");
    foreach (QVariant pType, paymenttypesList) {
        QVariantList data = pType.toList();
        QString show = data[1].toString();
        paymentsType.insert(show, data[0].toInt());
        ui->paymentTypeBox->addItem(show);
    }
}

void AddUpdateEvents::on_clientBox_currentIndexChanged(int index)
{
    switch (this->eventType) {
    case EventsE::Payments:
    {
        qDebug() << "what";
        ui->objectBox->clear();
        ui->apartmentBox->clear();
        objects.clear();
        apartments.clear();

        QString selectedClient = ui->clientBox->itemText(index);
        int clientId = clients.value(selectedClient);

        QVariantList objectsList = Operations::selectObjectsWithInstallmentByClient(clientId);
        ui->objectBox->addItem("-");

        foreach (QVariant object, objectsList)
        {
            QVariantList data = object.toList();
            QString show = data[1].toString();
            objects.insert(show, data[0].toInt());
            ui->objectBox->addItem(show);
        }
    }
        break;
    default:
        break;
    }
}

void AddUpdateEvents::on_objectBox_currentIndexChanged(int index)
{
    ui->apartmentBox->clear();
    apartments.clear();

    switch (this->eventType) {
    case EventsE::Payments:
    {
        QString selectedObject = ui->objectBox->itemText(index);
        int objectId = objects.value(selectedObject);
        int clientId = clients.value(ui->clientBox->currentText()); // Получаем ID клиента

        QVariantList apartmentsList = Operations::selectInstallmentApartments(clientId, objectId);

        ui->apartmentBox->addItem("-");
        foreach (QVariant apartment, apartmentsList) {
            QVariantList data = apartment.toList();
            QString show = data[2].toString();
            apartments.insert(show, data[0].toInt());
            ui->apartmentBox->addItem(show);
        }
    }
        break;
    case EventsE::Sales:
    {
        QString selectedObject = ui->objectBox->itemText(index);
        int objectId = objects.value(selectedObject);

        QVariantList apartmentsList = Operations::selectAvailableApartments(objectId, this->id);

        qDebug() << "what888";
        qDebug() << this->id;
        qDebug() << apartmentsList;
        ui->apartmentBox->addItem("-");
        foreach (QVariant apartment, apartmentsList) {
            QVariantList data = apartment.toList();
            QString show = data[2].toString();
            apartments.insert(show, data[0].toInt());
            ui->apartmentBox->addItem(show);
        }
    }
        break;

    default:
        break;
    }
}

// void AddUpdateEvents::on_apartmentBox_currentIndexChanged(int index)
// {
//     switch (this->eventType) {
//     case EventsE::Sales:
//         if (index == 0) {
//             ui->costEdit->clear();
//             currentApartmentPrice = 0.0;
//         } else {
//             QString selectedApartment = ui->apartmentBox->itemText(index);
//             double price = getPriceForApartment(selectedApartment);
//             currentApartmentPrice = price;
//             // Обновляем виджет с ценой
//             ui->costEdit->setText(QString::number(price, 'f', 2));
//         }
//         break;
//     default:
//         break;
//     }
// }
void AddUpdateEvents::on_apartmentBox_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->costEdit->clear();
        ui->currencyEdit->clear();
        currentApartmentPrice = 0.0;
        ui->initialPaymentEdit->clear();
        ui->monthQuantityEdit->clear();
        ui->discountEdit->clear();
    } else {
        // Считываем название выбранной квартиры
        QString aptName = ui->apartmentBox->itemText(index);
        // Берём её ID из мапы apartments
        int aptId = apartments.value(aptName);

        // Запрашиваем цену и валюту по ID
        QVariantList aptData = getPriceForApartment(aptId);
        if (!aptData.isEmpty()) {
            currentApartmentPrice = aptData[0].toDouble();
            ui->costEdit->setText(QString::number(currentApartmentPrice, 'f', 2));
            ui->currencyEdit->setText(aptData[1].toString());
            ui->initialPaymentEdit->clear();
            ui->monthQuantityEdit->clear();
            ui->discountEdit->clear();
        }
    }
}



bool AddUpdateEvents::checkEventFill(int clientId, int objectId, int apartmentId)
{
    bool result = true;

    if (ui->objectBox->currentText() == "-")
    {
        ui->objectBox->setStyleSheet("background-color: red;");
        ui->objectLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->clientBox->currentText() == "-")
    {
        ui->clientBox->setStyleSheet("background-color: red;");
        ui->clientLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->paymentTypeBox->currentText() == "-")
    {
        // qDebug() << ui->totalAmountEdit->text();
        ui->paymentTypeBox->setStyleSheet("background-color: red;");
        ui->paymentTypeLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->apartmentBox->currentText() == "-")
    {
        // qDebug() << ui->totalAmountEdit->text();
        ui->apartmentBox->setStyleSheet("background-color: red;");
        ui->apartmentLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->totalEdit->text().isEmpty())
    {
        ui->totalEdit->setStyleSheet("background-color: red;");
        ui->totalLabel->setStyleSheet("color: red;");
        result = false;
    }
    if ((ui->installmentCheckBox->isChecked() && ui->monthQuantityEdit->text().isEmpty()) || ui->installmentCheckBox->isChecked() && ui->monthQuantityEdit->text().replace("0", "").isEmpty())
    {
        ui->monthQuantityEdit->setStyleSheet("background-color: red;");
        ui->monthQuantityLabel->setStyleSheet("color: red;");
        result = false;
    }

    if(this->eventType == EventsE::Payments) {
        if (ui->clientBox->currentText() != "-" && ui->objectBox->currentText() != "-" && ui->apartmentBox->currentText() != "-") {
            // Получаем дату продажи квартиры
            QDate saleDate = Operations::getSaleDate(clientId, objectId, apartmentId);

            // Если дата продажи пустая или дата оплаты меньше даты продажи, возвращаем false
            if (saleDate.isNull() || date < saleDate) {
                ui->dateButton->setStyleSheet("background-color: red;");
                result = false;
            }
        }
    }


    return result;
}

void AddUpdateEvents::loadSalesData() {
    QVariantList sales = Operations::getSale(id)[0].toList();

    if (!sales.isEmpty()) {
        date = sales[0].toDate();
        ui->dateButton->setText(date.toString("dd.MM.yyyy"));
        ui->clientBox->setCurrentText(sales[1].toString());  // Клиент
        ui->objectBox->setCurrentText(sales[2].toString());  // Объект
        ui->apartmentBox->setCurrentText(sales[3].toString());  // Квартира

        ui->costEdit->setText(QString::number(sales[4].toDouble(), 'f', 2));  // Стоимость
        ui->discountEdit->setText(QString::number(sales[5].toDouble(), 'f', 2));  // Скидка
        ui->totalEdit->setText(QString::number(sales[6].toDouble(), 'f', 2));  // Итоговая сумма

        ui->paymentTypeBox->setCurrentText(sales[7].toString());  // Тип оплаты
        ui->installmentCheckBox->setChecked(sales[8].toBool());  // Рассрочка (Да/Нет)

        ui->initialPaymentEdit->setText(QString::number(sales[9].toDouble(), 'f', 2));  // Первоначальный взнос
        ui->monthQuantityEdit->setText(QString::number(sales[10].toInt()));  // Кол-во месяцев
        ui->monthlyPaymentEdit->setText(QString::number(sales[11].toDouble(), 'f', 2));  // Ежемесячный платеж

        ui->descriptionEdit->setText(sales[12].toString());  // Описание
        ui->currencyEdit->setText(sales[13].toString());
    }
}


void AddUpdateEvents::loadPaymentData() {
    QVariantList payment = Operations::getPayment(id)[0].toList();

    if (!payment.isEmpty()) {
        date = payment[0].toDate();
        ui->dateButton->setText(date.toString("dd.MM.yyyy"));
        ui->clientBox->setCurrentText(payment[1].toString());  // Клиент
        ui->objectBox->setCurrentText(payment[2].toString());  // Объект
        ui->apartmentBox->setCurrentText(payment[3].toString());  // Квартира
        ui->paymentTypeBox->setCurrentText(payment[4].toString());  // Тип оплаты

        ui->totalEdit->setText(QString::number(payment[5].toDouble(), 'f', 2));  // Оплатил

        ui->descriptionEdit->setText(payment[6].toString());  // Описание
        ui->currencyEdit->setText(payment[7].toString());

        ui->costEdit->hide();
        ui->costLabel->hide();
        ui->discountEdit->hide();
        ui->discountLabel->hide();
        ui->installmentCheckBox->hide();
        ui->installmentLabel->hide();
        ui->initialPaymentEdit->hide();
        ui->initialPaymentLabel->hide();
        ui->monthlyPaymentEdit->hide();
        ui->monthlyPaymentLabel->hide();
        ui->monthQuantityEdit->hide();
        ui->monthQuantityLabel->hide();
        ui->totalEdit->setReadOnly(false);

    }
}

void AddUpdateEvents::on_addButton_clicked() {
    if (checkEventFill(clients.value(ui->clientBox->currentText()), objects.value(ui->objectBox->currentText()), apartments.value(ui->apartmentBox->currentText()))) {
        switch (this->eventType) {
        case EventsE::Sales: {
            // Получаем значения из полей формы
            int clientId = clients.value(ui->clientBox->currentText(), 0);
            int objectId = objects.value(ui->objectBox->currentText(), 0);
            int apartmentId = apartments.value(ui->apartmentBox->currentText(), 0);
            QDate saleDate = date;
            double apartmentCost = ui->costEdit->text().replace(',', '.').toDouble();
            float discount = ui->discountEdit->text().replace(',', '.').toFloat();
            double totalAmount = ui->totalEdit->text().replace(',', '.').toDouble();
            int paymentTypeId = paymentsType.value(ui->paymentTypeBox->currentText(), 0);
            QString currency = ui->currencyEdit->text();
            bool isInstallmentPlan = ui->installmentCheckBox->isChecked();
            double installment = ui->initialPaymentEdit->text().replace(',', '.').toDouble();
            int monthQuantity = ui->monthQuantityEdit->text().toInt();
            double monthlyPayment = ui->monthlyPaymentEdit->text().replace(',', '.').toDouble();
            QString description = ui->descriptionEdit->toPlainText();
            ui->totalEdit->setValidator(new QDoubleValidator());

            if(!ui->installmentCheckBox->isChecked()) {
                installment = 0;
                monthQuantity = 0;
                monthlyPayment = 0;
            }

            // Вызываем функцию обновления
            bool success = Operations::updateSale(
                id, saleDate, clientId, objectId, apartmentId,
                apartmentCost, discount, totalAmount,
                paymentTypeId, currency, isInstallmentPlan,
                installment, monthQuantity, monthlyPayment,
                description
                );

            emit closed();
            this->close();
            break;
        }
        break;

        case EventsE::Payments: {
            int clientId = clients.value(ui->clientBox->currentText(), 0);
            int objectId = objects.value(ui->objectBox->currentText(), 0);
            int apartmentId = apartments.value(ui->apartmentBox->currentText(), 0);
            QDate paymentDate = date;
            int paymentTypeId = paymentsType.value(ui->paymentTypeBox->currentText(), 0);
            QString currency = ui->currencyEdit->text();
            double payment = ui->totalEdit->text().replace(',', '.').toDouble();
            QString description = ui->descriptionEdit->toPlainText();

            bool success = Operations::updatePayment(
                id, paymentDate, clientId, objectId, apartmentId,
                paymentTypeId, currency, payment, description
                );

            emit closed();
            this->close();
            break;
        }

        default:
            break;
        }
    }

    // Сброс цвета полей ввода через 200 мс
    QTimer::singleShot(200, this, &AddUpdateEvents::resetInputColor);
}


void AddUpdateEvents::on_dateButton_clicked() {
    CalendarPage *calendar = new CalendarPage(date);
    connect(calendar, &CalendarPage::changeDate, this, &AddUpdateEvents::onChangeDate);
    calendar->show();
}

void AddUpdateEvents::onChangeDate(QDate date) {
    this->date = date;
    ui->dateButton->setText(this->date.toString("dd.MM.yyyy"));
}

void AddUpdateEvents::updateMonthlyPayment() {
    double total = ui->totalEdit->text().toDouble();
    double initialPayment = ui->initialPaymentEdit->text().toDouble();
    int months = ui->monthQuantityEdit->text().toInt();

    if (months > 0) {
        double monthlyPayment = (total - initialPayment) / months;
        ui->monthlyPaymentEdit->setText(QString::number(monthlyPayment, 'f', 2));
    } else {
        ui->monthlyPaymentEdit->clear();
    }
}


// int AddUpdateEvents::getPriceForApartment(const QString &apartmentName) {
//     QVariantList apartmentsList = Operations::selectAllApartmentsWithoutId();
//     foreach (QVariant apartment, apartmentsList) {
//         QVariantList data = apartment.toList();
//         if (data[2].toString() == apartmentName) {
//             return data[8].toInt();
//         }
//     }
//     return 0;
// }

// Теперь метод принимает ID
QVariantList AddUpdateEvents::getPriceForApartment(int apartmentId) {
    QVariantList apartmentsList = Operations::selectAllApartmentsWithoutId();
    foreach (QVariant apartment, apartmentsList) {
        QVariantList data = apartment.toList();
        // Предположим, что ID квартиры в базе лежит в data[0]
        if (data[0].toInt() == apartmentId) {
            QVariantList result;
            // Предположим, что цена в data[8], а валюта в data[9]
            result.append(data[8].toDouble());  // Цена
            result.append(data[9].toString());  // Валюта
            return result;
        }
    }
    return QVariantList();
}



void AddUpdateEvents::on_costEdit_textChanged(const QString &arg1)
{
    currentApartmentPrice = arg1.toDouble();
    updateTotalPrice();
}

void AddUpdateEvents::on_discountEdit_textChanged(const QString &arg1)
{
    updateTotalPrice();

    if (ui->installmentCheckBox->isChecked()) {
        updateMonthlyPayment();
    }
}


void AddUpdateEvents::updateTotalPrice() {
    // Получаем значение из поля
    QString costText = ui->costEdit->text();
    QString discountText = ui->discountEdit->text();

    // Преобразуем их в нужные типы
    double cost = costText.toDouble();
    double discount = discountText.toDouble();

    double result = cost - discount;
    ui->totalEdit->setText(QString::number(result, 'f', 2));
}

void AddUpdateEvents::resetInputColor()
{
    ui->totalEdit->setStyleSheet("");
    ui->totalLabel->setStyleSheet("color: white;");
    ui->clientLabel->setStyleSheet("color: white;");
    ui->clientBox->setStyleSheet("background-color: white;");
    ui->paymentTypeLabel->setStyleSheet("color: white;");
    ui->paymentTypeBox->setStyleSheet("background-color: white;");
    ui->objectLabel->setStyleSheet("color: white;");
    ui->objectBox->setStyleSheet("background-color: white;");
    ui->apartmentBox->setStyleSheet("background-color: white;");
    ui->apartmentLabel->setStyleSheet("color: white;");
    ui->monthQuantityEdit->setStyleSheet("");
    ui->monthQuantityLabel->setStyleSheet("color: white;");
    if (this->eventType == EventsE::Payments) {
        ui->dateButton->setStyleSheet("");
    }
}
