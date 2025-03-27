#include "addpaymentspage.h"
#include "pages/ui_addpaymentspage.h"

AddPaymentsPage::AddPaymentsPage(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPaymentsPage)
{
    ui->setupUi(this);
    this->nav = nav;

    ui->frame->setProperty("color", "green");

    ui->dateButton->setProperty("color", "gray");

    ui->clientLabel->setProperty("color", "white");
    ui->objectLabel->setProperty("color", "white");
    ui->apartmentLabel->setProperty("color", "white");
    ui->paymentTypeLabel->setProperty("color", "white");
    ui->currencyLabel->setProperty("color", "white");
    ui->paymentLabel->setProperty("color", "white");
    ui->remainLabel->setProperty("color", "white");
    ui->monthlyPaymentLabel->setProperty("color", "white");
    ui->descriptionLabel->setProperty("color", "white");

    ui->paymentEdit->setValidator(new QDoubleValidator());

    date = QDate::currentDate();
    ui->dateButton->setText(date.toString("dd.MM.yyyy"));

    ui->vskButton->setStyleSheet("background: transparent; border: none;");
    ui->vskButton->setText("");
    ui->vskButton->setEnabled(false);

    connect(ui->clientBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_clientBox_currentIndexChanged(int)));
    connect(ui->objectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_objectBox_currentIndexChanged(int)));

    openWindow();
}

AddPaymentsPage::~AddPaymentsPage()
{
    delete ui;
}

void AddPaymentsPage::openWindow()
{
    setComboBoxesData();
}

void AddPaymentsPage::on_oplatyButton_clicked()
{
    nav->openEvents(0);
}


void AddPaymentsPage::on_backButton_clicked()
{
    nav->changeWindow(0);
}

bool AddPaymentsPage::checkEventFill(int clientId, int objectId, int apartmentId)
{
    bool result = true;

    if (ui->clientBox->currentText() != "-" && ui->objectBox->currentText() != "-" && ui->apartmentBox->currentText() != "-") {
        // Получаем дату продажи квартиры
        QDate saleDate = Operations::getSaleDate(clientId, objectId, apartmentId);

        // Если дата продажи пустая или дата оплаты меньше даты продажи, возвращаем false
        if (saleDate.isNull() || date < saleDate) {
            ui->dateButton->setStyleSheet("background-color: red;");
            result = false;
        }
    }

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
    if (ui->paymentEdit->text().isEmpty())
    {
        ui->paymentEdit->setStyleSheet("background-color: red;");
        ui->paymentLabel->setStyleSheet("color: red;");
        result = false;
    }
    return result;
}

void AddPaymentsPage::setComboBoxesData() {
    ui->clientBox->clear();
    ui->objectBox->clear();
    ui->apartmentBox->clear();
    ui->paymentTypeBox->clear();
    ui->currencyEdit->clear();
    ui->paymentEdit->clear();
    clients.clear();
    objects.clear();
    apartments.clear();
    paymentsType.clear();

    QVariantList clientsList = Operations::selectClientsWithDebt();
    ui->clientBox->addItem("-");

    foreach (QVariant client, clientsList)
    {
        QVariantList data = client.toList();
        QString show = data[1].toString();
        clients.insert(show, data[0].toInt());
        ui->clientBox->addItem(show);
    }

    // Теперь объекты и квартиры не загружаем здесь

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

void AddPaymentsPage::on_clientBox_currentIndexChanged(int index)
{
     qDebug() << "what";
    ui->objectBox->clear();
    ui->apartmentBox->clear();
    objects.clear();
    apartments.clear();

    QString selectedClient = ui->clientBox->itemText(index);
    int clientId = clients.value(selectedClient);

    QVariantList objectsList = Operations::selectObjectsByClient(clientId);
    qDebug() << "what";
    qDebug() << objectsList;
    ui->objectBox->addItem("-");

    foreach (QVariant object, objectsList)
    {
        QVariantList data = object.toList();
        QString show = data[1].toString();
        objects.insert(show, data[0].toInt());
        ui->objectBox->addItem(show);
    }
}

void AddPaymentsPage::on_objectBox_currentIndexChanged(int index)
{
    ui->apartmentBox->clear();
    apartments.clear();

    QString selectedObject = ui->objectBox->itemText(index);
    int objectId = objects.value(selectedObject);
    int clientId = clients.value(ui->clientBox->currentText()); // Получаем ID клиента

    QVariantList apartmentsList = Operations::selectApartmentsByClient(clientId, objectId);

    ui->apartmentBox->addItem("-");
    foreach (QVariant apartment, apartmentsList) {
        QVariantList data = apartment.toList();
        QString show = data[2].toString();
        apartments.insert(show, data[0].toInt());
        ui->apartmentBox->addItem(show);
    }
}

void AddPaymentsPage::on_apartmentBox_currentIndexChanged(int index)
{
    updateMonthlyPayment();
    updatePaymentDetails();
}

void AddPaymentsPage::on_addEventButton_clicked()
{
    if (checkEventFill(clients.value(ui->clientBox->currentText()), objects.value(ui->objectBox->currentText()), apartments.value(ui->apartmentBox->currentText()))) {
        QDate datetime = this->date;
        QString currency = ui->currencyEdit->text();

        Operations::addPayment(
            datetime,
            clients.value(ui->clientBox->currentText()),       // ID клиента
            objects.value(ui->objectBox->currentText()),       // ID объекта
            apartments.value(ui->apartmentBox->currentText()), // ID квартиры
            paymentsType.value(ui->paymentTypeBox->currentText()),  // ID типа оплаты
            currency,
            ui->paymentEdit->text().replace(',', '.').toDouble(),    // Оплата
            ui->descriptionEdit->toPlainText()     // Описание
            );

        clearAllInputsEvent();
        setComboBoxesData();
    }

    // Сброс цвета полей ввода через 200 мс
    QTimer::singleShot(200, this, &AddPaymentsPage::resetInputColor);
}

void AddPaymentsPage::updateMonthlyPayment() {
    QLocale locale(QLocale::Russian);

    QVariantList monthlyPayments = Operations::getMonthlyPayments(
        clients.value(ui->clientBox->currentText()),
        objects.value(ui->objectBox->currentText()),
        apartments.value(ui->apartmentBox->currentText())
        );

    if (monthlyPayments.isEmpty()) {
        ui->monthlyPaymentEdit->setText("");
    } else {
        QVariantList totalPaymentList = monthlyPayments.first().toList();
        if (!totalPaymentList.isEmpty()) {
            QVariant totalPayment = totalPaymentList.first();
            double amount = totalPayment.toDouble();
            QString amountStr = locale.toString(amount, 'f', 2);
            ui->monthlyPaymentEdit->setText(amountStr);
        }
    }
}

void AddPaymentsPage::updatePaymentDetails() {
    QLocale locale(QLocale::Russian);

    QVariantList paymentDetails = Operations::getPaymentDetails(
        clients.value(ui->clientBox->currentText()),
        objects.value(ui->objectBox->currentText()),
        apartments.value(ui->apartmentBox->currentText())
        );

    if (paymentDetails.isEmpty()) {
        ui->remainEdit->setText("");
    } else {
        QVariantList detailsList = paymentDetails.first().toList();

        if (detailsList.size() >= 3) {
            double totalAmount = detailsList[0].toDouble();
            double installment = detailsList[1].toDouble();
            double totalPaid = detailsList[2].toDouble();
            QString currency = detailsList[3].toString();

            double remainingAmount = totalAmount - installment - totalPaid;

            QString amountStr = locale.toString(remainingAmount, 'f', 2);

            ui->remainEdit->setText(amountStr);
            ui->currencyEdit->setText(currency);
        }
    }
}



void AddPaymentsPage::clearAllInputsEvent() {
    ui->clientBox->setCurrentIndex(0);
    ui->objectBox->setCurrentIndex(0);
    ui->apartmentBox->setCurrentIndex(0);
    ui->paymentTypeBox->setCurrentIndex(0);
    ui->currencyEdit->clear();
    ui->paymentEdit->clear();
    ui->descriptionEdit->clear();
}

void AddPaymentsPage::on_dateButton_clicked()
{
    CalendarPage *calendar = new CalendarPage(date);

    connect(calendar, &CalendarPage::changeDate, this, &AddPaymentsPage::onChangeDate);

    calendar->show();
}

void AddPaymentsPage::onChangeDate(QDate date)
{
    this->date = date;
    ui->dateButton->setText(this->date.toString("dd.MM.yyyy"));

    clearAllInputsEvent();
}

void AddPaymentsPage::resetInputColor()
{
    ui->dateButton->setStyleSheet("");
    ui->paymentEdit->setStyleSheet("");
    ui->paymentLabel->setStyleSheet("color: white;");
    ui->clientLabel->setStyleSheet("color: white;");
    ui->clientBox->setStyleSheet("background-color: white;");
    ui->paymentTypeLabel->setStyleSheet("color: white;");
    ui->paymentTypeBox->setStyleSheet("background-color: white;");
    ui->objectLabel->setStyleSheet("color: white;");
    ui->objectBox->setStyleSheet("background-color: white;");
    ui->apartmentBox->setStyleSheet("background-color: white;");
    ui->apartmentLabel->setStyleSheet("color: white;");
}
