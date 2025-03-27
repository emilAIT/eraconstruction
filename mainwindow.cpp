#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qstandarditemmodel.h"
#include <QPainter>

MainWindow::MainWindow(nm *nav, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->nav = nav;
    this->setWindowTitle("Главное окно | ERA CONSTRUCTION");

    // чтобы добавил цвет
    ui->frame->setProperty("color", "green");

    // что бы менял кнопки на настройки когда входим
    ui->settingsButton->setCurrentIndex(-1);
    ui->reportsButton->setCurrentIndex(-1);


    ui->settingsButton->setProperty("type", "nav");
    ui->reportsButton->setProperty("type", "nav");

    ui->dateButton->setProperty("color", "gray");


    // что бы менял цвет текста
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
    ui->version->setProperty("color", "white");


    ui->initialPaymentLabel->setVisible(false);
    ui->initialPaymentEdit->setVisible(false);
    ui->monthQuantityLabel->setVisible(false);
    ui->monthQuantityEdit->setVisible(false);
    ui->monthlyPaymentLabel->setVisible(false);
    ui->monthlyPaymentEdit->setVisible(false);

    ui->costEdit->setValidator(new QDoubleValidator());
    ui->discountEdit->setValidator(new QIntValidator());
    ui->initialPaymentEdit->setValidator(new QDoubleValidator());
    ui->monthQuantityEdit->setValidator(new QIntValidator());
    ui->monthlyPaymentEdit->setValidator(new QDoubleValidator());

    date = QDate::currentDate();
    ui->dateButton->setText(date.toString("dd.MM.yyyy"));

    connect(ui->initialPaymentEdit, &QLineEdit::textChanged, this, &MainWindow::updateMonthlyPayment);
    connect(ui->monthQuantityEdit, &QLineEdit::textChanged, this, &MainWindow::updateMonthlyPayment);


    openWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openWindow()
{
    setComboBoxesData();
    checkDebts();

    if (!isAdmin)
        ui->reportsButton->setStyleSheet("QComboBox::drop-down { background-color: transparent; }");
}

void MainWindow::setReportsAvailability(bool isEnabled)
{
    ui->reportsButton->setStyleSheet("background: transparent; border: none;");
    ui->reportsButton->clear();
    ui->reportsButton->setEnabled(isEnabled);

    ui->reportsButton->setStyleSheet("QComboBox::drop-down { background-color: transparent; }");

    int index = ui->settingsButton->findText("ПОЛЬЗОВАТЕЛИ");
    if (index != -1)
    {
        ui->settingsButton->removeItem(index); // Скрываем пункт
    }

    this->isAdmin = false;
}

bool MainWindow::checkEventFill()
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
    if ((ui->installmentCheckBox->isChecked() && ui->monthQuantityEdit->text().isEmpty()) || ui->installmentCheckBox->isChecked() && ui->monthQuantityEdit->text().replace("0", "").isEmpty())
    {
        ui->monthQuantityEdit->setStyleSheet("background-color: red;");
        ui->monthQuantityLabel->setStyleSheet("color: red;");
        result = false;
    }
    return result;
}

void MainWindow::checkDebts() {
    bool hasDebts = Operations::checkHaveDebts();

    // Проверяем, есть ли долги
    if (hasDebts) {
        ui->reportsButton->setStyleSheet("QComboBox::drop-down { border-radius: 10px; width: 20px; height: 20px; background-color: red; }");

        QPixmap pixmap(16, 16); // Размер картинки для кружочка
        pixmap.fill(Qt::transparent); // Делаем фон прозрачным

        QPainter painter(&pixmap);
        painter.setBrush(QBrush(Qt::red)); // Устанавливаем красный цвет
        painter.setPen(Qt::NoPen); // Без обводки
        painter.drawEllipse(0, 0, 16, 16); // Рисуем кружок
        painter.end();

        ui->reportsButton->setItemData(5, pixmap, Qt::DecorationRole); // Устанавливаем кружочек
    } else {
        ui->reportsButton->setStyleSheet("QComboBox::drop-down { background-color: transparent; }");
        QIcon emptyIcon; // Создаем пустую иконку
        ui->reportsButton->setItemIcon(5, emptyIcon);
    }
}

void MainWindow::setComboBoxesData() {
    ui->clientBox->clear();
    ui->objectBox->clear();
    ui->apartmentBox->clear();
    ui->paymentTypeBox->clear();
    ui->currencyEdit->clear();
    ui->discountEdit->clear();
    clients.clear();
    objects.clear();
    apartments.clear();
    paymentsType.clear();

    ui->initialPaymentEdit->clear();
    ui->monthQuantityEdit->clear();
    ui->monthlyPaymentEdit->clear();

    ui->initialPaymentLabel->setVisible(false);
    ui->initialPaymentEdit->setVisible(false);
    ui->monthQuantityLabel->setVisible(false);
    ui->monthQuantityEdit->setVisible(false);
    ui->monthlyPaymentLabel->setVisible(false);
    ui->monthlyPaymentEdit->setVisible(false);

    ui->installmentCheckBox->setChecked(false);

    QVariantList clientsList = Operations::selectAllClients();
    ui->clientBox->addItem("-");
    foreach (QVariant client, clientsList)
    {
        QVariantList data = client.toList();
        QString show = data[1].toString();
        clients.insert(show, data[0].toInt());
        ui->clientBox->addItem(show);
    }

    QVariantList objectsList = Operations::selectAllObjectsForMainWindow();
    ui->objectBox->addItem("-");
    foreach (QVariant object, objectsList)
    {
        QVariantList data = object.toList();
        QString show = data[1].toString();
        objects.insert(show, data[0].toInt());
        ui->objectBox->addItem(show);
    }

    // apartments
    on_objectBox_currentIndexChanged(0);

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

void MainWindow::on_objectBox_currentIndexChanged(int index)
{
    ui->apartmentBox->clear();
    apartments.clear();
    ui->discountEdit->clear();
    ui->monthQuantityEdit->clear();
    ui->initialPaymentEdit->clear();
    QString selectedObject = ui->objectBox->itemText(index);
    int objectId = objects.value(selectedObject);
    QVariantList apartmentsList = Operations::selectAllApartmentsForMainWindow(objectId);

    ui->apartmentBox->addItem("-");
    foreach (QVariant apartment, apartmentsList) {
        QVariantList data = apartment.toList();
        QString show = data[2].toString();
        apartments.insert(show, data[0].toInt());
        ui->apartmentBox->addItem(show);
        updateMonthlyPayment();
    }
}

void MainWindow::on_reportsButton_currentIndexChanged(int index)
{
    nav->openGeneralReports(index);

    ui->reportsButton->setCurrentIndex(-1);

    // // Проверяем, является ли пользователь супер-администратором
    // if (!userSession::getInstance().checkIsSuperAdmin())
    // {
    //     // Скрываем пункт с индексом 10, если он доступен
    //     if (ui->reportsButton->count() > 10)
    //     {
    //         ui->reportsButton->removeItem(10);
    //     }
    // }
}


void MainWindow::on_settingsButton_currentIndexChanged(int index)
{
    nav->openSettings(index);
    // что бы менял комбо бокс на "настройки" после
    ui->settingsButton->setCurrentIndex(-1);
}


void MainWindow::on_paymentsButton_clicked()
{
    nav->openEvents(3);
}


void MainWindow::on_expensesButton_clicked()
{
    nav->openEvents(1);
}


void MainWindow::on_salesButton_clicked()
{
    nav->openEvents(2);
}


void MainWindow::on_installmentCheckBox_stateChanged(int state)
{
    bool isVisible = (state == Qt::Checked);

    ui->initialPaymentLabel->setVisible(isVisible);
    ui->initialPaymentEdit->setVisible(isVisible);
    ui->monthQuantityLabel->setVisible(isVisible);
    ui->monthQuantityEdit->setVisible(isVisible);
    ui->monthlyPaymentLabel->setVisible(isVisible);
    ui->monthlyPaymentEdit->setVisible(isVisible);

    if (!isVisible) {
        ui->initialPaymentEdit->clear();
        ui->monthQuantityEdit->clear();
        ui->monthlyPaymentEdit->clear();
    }
}

void MainWindow::on_apartmentBox_currentIndexChanged(int index)
{
    if (index == 0) {
        ui->costEdit->clear();
        currentApartmentPrice = 0.0;
    } else {
        QString selectedApartment = ui->apartmentBox->itemText(index);
        QVariantList apartmentData = getPriceForApartment(selectedApartment);  // Получаем цену и валюту


        if (!apartmentData.isEmpty()) {
            currentApartmentPrice = apartmentData[0].toDouble();
            ui->costEdit->setText(QString::number(currentApartmentPrice, 'f', 2));
            ui->currencyEdit->setText(apartmentData[1].toString());  // Устанавливаем валюту
            ui->initialPaymentEdit->clear();
            ui->monthQuantityEdit->clear();
            ui->discountEdit->clear();
        }
    }
}


QVariantList MainWindow::getPriceForApartment(const QString &apartmentName) {
    QVariantList apartmentsList = Operations::selectAllApartmentsWithoutId();
    foreach (QVariant apartment, apartmentsList) {
        QVariantList data = apartment.toList();
        if (data[0].toInt() == apartments.value(apartmentName)) {
            QVariantList result;
            result.append(data[8].toDouble());  // Цена
            result.append(data[9].toString());  // Валюта
            return result;
        }
    }
    return QVariantList();
}

void MainWindow::on_costEdit_textChanged(const QString &arg1)
{
    currentApartmentPrice = arg1.toDouble();
    updateTotalPrice();
}

void MainWindow::on_discountEdit_textChanged(const QString &arg1)
{
    updateTotalPrice();

    if (ui->installmentCheckBox->isChecked()) {
        updateMonthlyPayment();
    }
}


void MainWindow::updateTotalPrice() {
    double cost = ui->costEdit->text().toDouble();
    int discount = ui->discountEdit->text().toInt();
    double result = cost - discount;
    ui->totalEdit->setText(QString::number(result, 'f', 2));
}


void MainWindow::on_dateButton_clicked()
{
    CalendarPage *calendar = new CalendarPage(date);

    connect(calendar, &CalendarPage::changeDate, this, &MainWindow::onChangeDate);

    calendar->show();
}

void MainWindow::onChangeDate(QDate date)
{
    this->date = date;
    ui->dateButton->setText(this->date.toString("dd.MM.yyyy"));

    clearAllInputsEvent();
}


void MainWindow::on_addEventButton_clicked()
{
    double total = ui->totalEdit->text().toDouble();
    double initialPayment = ui->initialPaymentEdit->text().isEmpty() ? 0 : ui->initialPaymentEdit->text().replace(',', '.').toDouble();
    int months = ui->monthQuantityEdit->text().isEmpty() ? 0 : ui->monthQuantityEdit->text().toInt();
    double discount = ui->discountEdit->text().isEmpty() ? 0 : ui->discountEdit->text().replace(',', '.').toDouble();

    double monthlyPayment = (months > 0) ? (total - initialPayment) / months : 0;

    QString currency = ui->currencyEdit->text();

    if (checkEventFill()) {
        QDate datetime = this->date;

        Operations::addSale(
            datetime,
            clients.value(ui->clientBox->currentText()),       // ID клиента
            objects.value(ui->objectBox->currentText()),       // ID объекта
            apartments.value(ui->apartmentBox->currentText()), // ID квартиры
            ui->costEdit->text().replace(',', '.').toFloat(),  // Стоимость квартиры
            discount,                                           // Скидка
            ui->totalEdit->text().replace(',', '.').toFloat(), // Итоговая сумма
            paymentsType.value(ui->paymentTypeBox->currentText()), // ID типа оплаты
            currency,
            ui->installmentCheckBox->isChecked(),                   // Рассрочка (да/нет)
            initialPayment,                                          // Первоначальный взнос
            months,                                                  // Количество месяцев рассрочки
            monthlyPayment,                                          // Ежемесячный платеж
            ui->descriptionEdit->toPlainText()                      // Описание
            );

        clearAllInputsEvent();
        openWindow();
    }

    // Сброс цвета полей ввода через 200 мс
    QTimer::singleShot(200, this, &MainWindow::resetInputColor);
}





void MainWindow::updateMonthlyPayment() {
    double total = ui->totalEdit->text().toDouble();
    double initialPayment = ui->initialPaymentEdit->text().toDouble();
    int months = ui->monthQuantityEdit->text().toInt();

    if (months > 0 && total > 0) {
        double monthlyPayment = (total - initialPayment) / months;
        ui->monthlyPaymentEdit->setText(QString::number(monthlyPayment, 'f', 2));
    } else {
        ui->monthlyPaymentEdit->clear();
    }
}

// general (change window)
void MainWindow::clearAllInputsEvent() {
    ui->clientBox->setCurrentIndex(0);
    ui->objectBox->setCurrentIndex(0);
    ui->apartmentBox->setCurrentIndex(0);
    ui->costEdit->clear();
    ui->discountEdit->clear();
    ui->totalEdit->clear();
    ui->paymentTypeBox->setCurrentIndex(0);
    ui->currencyEdit->clear();
    ui->initialPaymentEdit->clear();
    ui->monthQuantityEdit->clear();
    ui->monthlyPaymentEdit->clear();
    ui->descriptionEdit->clear();
    on_installmentCheckBox_stateChanged(0);
    ui->installmentCheckBox->setCheckState(Qt::Unchecked);

}

void MainWindow::resetInputColor()
{
    ui->monthQuantityEdit->setStyleSheet("");
    ui->monthQuantityLabel->setStyleSheet("color: white;");
    ui->clientLabel->setStyleSheet("color: white;");
    ui->clientBox->setStyleSheet("background-color: white;");
    ui->paymentTypeLabel->setStyleSheet("color: white;");
    ui->paymentTypeBox->setStyleSheet("background-color: white;");
    ui->objectLabel->setStyleSheet("color: white;");
    ui->objectBox->setStyleSheet("background-color: white;");
    ui->apartmentBox->setStyleSheet("background-color: white;");
    ui->apartmentLabel->setStyleSheet("color: white;");
}


