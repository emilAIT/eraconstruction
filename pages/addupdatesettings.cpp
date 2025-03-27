#include "addupdatesettings.h"
#include "pages/ui_addupdatesettings.h"
#include "qtimer.h"
#include "../pages/calendarpage.h"

AddUpdateSettings::AddUpdateSettings(AddUpdateSettingsE mode, int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddUpdateSettings)
{
    ui->setupUi(this);
    //для оперативной памяти удаляет с рем когда закрываю окно
    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setProperty("color", "green");

    this->mode = mode;
    this->id = id;

    // блокирует другие окна
    this->setWindowModality(Qt::ApplicationModal);

    Bdate = QDate::currentDate();
    ui->DateOfBirthButton->setText(Bdate.toString("dd.MM.yyyy"));

    Issuedate = QDate::currentDate();
    ui->dateOfIssueButton->setText(Issuedate.toString("dd.MM.yyyy"));


    ui->roomsQuantityEdit->setValidator(new QIntValidator());
    ui->sanuzelsQuantityEdit->setValidator(new QIntValidator());
    ui->floorEdit->setValidator(new QIntValidator());
    ui->priceSquareEdit->setValidator(new QDoubleValidator());

    ui->ploshadEdit->setValidator(new QDoubleValidator());
    ui->totalAmountEdit->setValidator(new QDoubleValidator());

    connect(ui->ploshadEdit, &QLineEdit::textChanged, this, &AddUpdateSettings::calculateTotalAmount);
    connect(ui->priceSquareEdit, &QLineEdit::textChanged, this, &AddUpdateSettings::calculateTotalAmount);

    if (id > 0)
    {
        ui->addButton->setText("Сохранить");
    }
    else
    {
        ui->addButton->setText("Добавить");
    }



    switch (this->mode) {
    case AddUpdateSettingsE::Objects:
        if (true) {
            ui->currencyFrame->hide();
            ui->authorityFrame->hide();
            ui->dateOfBirthFrame->hide();
            ui->dateOfIssueFrame->hide();
            ui->passportIdFrame->hide();
            ui->placeOfResidenceFrame->hide();
            ui->phoneFrame->hide();
            ui->apartmentFrame->hide();
            ui->ploshadFrame->hide();
            ui->roomsQuanityFrame->hide();
            ui->paymentTypeFrame->hide();
            ui->typeFrame->hide();
            ui->typesFrame->hide();
            ui->whoWhatFrame->hide();
            ui->clientFrame->hide();
            ui->userFrame->hide();
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityFrame->hide();
            ui->objectLabel->setProperty("color", "white");
            ui->addressLabel->setProperty("color", "white");
            ui->objectsFrame->hide();
            ui->floorFrame->hide();
            ui->PasswordFrame->hide();
            ui->priceSquareFrame->hide();
            ui->totalAmountFrame->hide();
            ui->isAdminFrame->hide();
            ui->descriptionFrame->hide();

            if (id > 0) {
                ui->Header->setText("Редактировать объект");
                QVariantList data = Operations::getObject(id)[0].toList();
                if (!data.isEmpty())
                {
                    ui->objectEdit->setText(data[1].toString());
                    ui->addressEdit->setText(data[2].toString());
                }
            } else {
                ui->Header->setText("Добавить объект");
            }
        }
        break;
    case AddUpdateSettingsE::Apartments:
        if (true) {
            ui->addressFrame->hide();
            ui->authorityFrame->hide();
            ui->dateOfBirthFrame->hide();
            ui->dateOfIssueFrame->hide();
            ui->passportIdFrame->hide();
            ui->placeOfResidenceFrame->hide();
            ui->phoneFrame->hide();
            ui->apartmentLabel->setProperty("color", "white");
            ui->ploshadLabel->setProperty("color", "white");
            ui->roomsQuantityLabel->setProperty("color", "white");
            ui->currencyLabel->setProperty("color", "white");
            ui->paymentTypeFrame->hide();
            ui->typeFrame->hide();
            ui->typesFrame->hide();
            ui->whoWhatFrame->hide();
            ui->clientFrame->hide();
            ui->userFrame->hide();
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityLabel->setProperty("color", "white");
            ui->objectFrame->hide();
            ui->objectsLabel->setProperty("color", "white");
            ui->floorLabel->setProperty("color", "white");
            ui->PasswordFrame->hide();
            ui->priceSquareLabel->setProperty("color", "white");
            ui->totalAmountLabel->setProperty("color", "white");
            ui->isAdminFrame->hide();
            ui->descriptionFrame->hide();

            QVariantList objectList = Operations::selectAllObjects();
            ui->objectsBox->addItem("-");
            foreach (QVariant object, objectList)
            {
                QVariantList data = object.toList();
                QString show = data[1].toString();
                objects.insert(show, data[0].toInt());
                ui->objectsBox->addItem(show);
            }

            if (id > 0) {
                ui->Header->setText("Редактировать квартиру");
                QVariantList apartment = Operations::getApartment(id)[0].toList();
                if (!apartment.isEmpty())
                {
                    int objId = apartment[1].toInt();
                    QString objName = "-";
                    QMapIterator<QString, int> it(objects);
                    while (it.hasNext()) {
                        it.next();
                        if (it.value() == objId) {
                            objName = it.key();
                            break;
                        }
                    }

                    ui->objectsBox->setCurrentText(objName);
                    ui->apartmentEdit->setText(apartment[2].toString());
                    ui->ploshadEdit->setText(QString::number(apartment[3].toDouble(), 'f', 2));
                    ui->roomsQuantityEdit->setText(apartment[4].toString());
                    ui->sanuzelsQuantityEdit->setText(apartment[5].toString());
                    ui->floorEdit->setText(apartment[6].toString());
                    ui->priceSquareEdit->setText(QString::number(apartment[7].toDouble(), 'f', 2));
                    ui->totalAmountEdit->setText(QString::number(apartment[8].toDouble(), 'f', 2));
                    ui->currencyBox->setCurrentText(apartment[9].toString());
                }
            } else {
              ui->Header->setText("Добавить квартиру");
            }
        }
        break;
    case AddUpdateSettingsE::Type:
        if (true) {

            ui->currencyFrame->hide();
            ui->addressFrame->hide();
            ui->authorityFrame->hide();
            ui->dateOfBirthFrame->hide();
            ui->dateOfIssueFrame->hide();
            ui->passportIdFrame->hide();
            ui->placeOfResidenceFrame->hide();
            ui->phoneFrame->hide();
            ui->apartmentFrame->hide();
            ui->ploshadFrame->hide();
            ui->roomsQuanityFrame->hide();
            ui->paymentTypeFrame->hide();
            ui->typeLabel->setProperty("color", "white");
            ui->typesFrame->hide();
            ui->whoWhatFrame->hide();
            ui->clientFrame->hide();
            ui->userFrame->hide();
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityFrame->hide();
            ui->objectFrame->hide();
            ui->objectsFrame->hide();
            ui->floorFrame->hide();
            ui->PasswordFrame->hide();
            ui->priceSquareFrame->hide();
            ui->totalAmountFrame->hide();
            ui->isAdminCheckBox->setProperty("color", "white");
            ui->descriptionFrame->hide();

            if (id > 0) {
                ui->Header->setText("Редактировать тип");
                QVariantList type = Operations::getType(id)[0].toList();
                if (!type.isEmpty())
                {
                    ui->typeEdit->setText(type[1].toString());
                    ui->isAdminCheckBox->setChecked(type[2].toBool());
                }
            } else {
                ui->Header->setText("Добавить тип");
            }
        }
        break;
    case AddUpdateSettingsE::WhoWhat:
        if (true) {

            ui->currencyFrame->hide();
            ui->addressFrame->hide();
            ui->authorityFrame->hide();
            ui->dateOfBirthFrame->hide();
            ui->dateOfIssueFrame->hide();
            ui->passportIdFrame->hide();
            ui->placeOfResidenceFrame->hide();
            ui->phoneFrame->hide();
            ui->apartmentFrame->hide();
            ui->ploshadFrame->hide();
            ui->roomsQuanityFrame->hide();
            ui->paymentTypeFrame->hide();
            ui->typeFrame->hide();
            ui->typesLabel->setProperty("color", "white");
            ui->whoWhatLabel->setProperty("color", "white");
            ui->clientFrame->hide();
            ui->userFrame->hide();
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityFrame->hide();
            ui->objectFrame->hide();
            ui->objectsFrame->hide();
            ui->floorFrame->hide();
            ui->PasswordFrame->hide();
            ui->priceSquareFrame->hide();
            ui->totalAmountFrame->hide();
            ui->isAdminFrame->hide();
            ui->descriptionFrame->hide();

            QVariantList typesList = Operations::selectAllTypes();
            ui->typesBox->addItem("-");
            foreach (QVariant type, typesList)
            {
                QVariantList data = type.toList();
                QString show = data[1].toString();
                types.insert(show, data[0].toInt());
                ui->typesBox->addItem(show);
            }

            if (id > 0) {
                ui->Header->setText("Редактировать Кто/Что");
                QVariantList whoWhat = Operations::getWhoWhat(id)[0].toList();
                if (!whoWhat.isEmpty())
                {
                    ui->typesBox->setCurrentText(whoWhat[0].toString());
                    ui->whoWhatEdit->setText(whoWhat[1].toString());
                }
            }
            else
            {
                ui->Header->setText("Добавить Кто/Что");
            }
        }
        break;

    case AddUpdateSettingsE::Clients:
        if (true) {

            ui->currencyFrame->hide();
            ui->addressFrame->hide();
            ui->phoneLabel->setProperty("color", "white");
            ui->apartmentFrame->hide();
            ui->ploshadFrame->hide();
            ui->roomsQuanityFrame->hide();
            ui->paymentTypeFrame->hide();
            ui->typeFrame->hide();
            ui->typesFrame->hide();
            ui->whoWhatFrame->hide();
            ui->clientLabel->setProperty("color", "white");
            ui->authorityLabel->setProperty("color", "white");
            ui->dateOfBirthLabel->setProperty("color", "white");
            ui->dateOfIssueLabel->setProperty("color", "white");

            ui->DateOfBirthButton->setProperty("color", "gray");
            ui->dateOfIssueButton->setProperty("color", "gray");

            ui->dateOfIssueLabel->setProperty("color", "white");
            ui->passportIdLabel->setProperty("color", "white");
            ui->placeOfResidenceLabel->setProperty("color", "white");
            ui->userFrame->hide();
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityFrame->hide();
            ui->objectFrame->hide();
            ui->objectsFrame->hide();
            ui->floorFrame->hide();
            ui->PasswordFrame->hide();
            ui->priceSquareFrame->hide();
            ui->totalAmountFrame->hide();
            ui->isAdminFrame->hide();

            ui->descriptionLabel->setProperty("color", "white");


            if (id > 0) {
                ui->Header->setText("Редактировать клиента");
                QVariantList client = Operations::getClient(id)[0].toList();
                if (!client.isEmpty())
                {
                    ui->clientEdit->setText(client[1].toString());
                    ui->phoneEdit->setText(client[2].toString());
                    ui->authorityEdit->setText(client[3].toString());
                    QString birthDateStr = client[4].toString();
                    ui->DateOfBirthButton->setText(Bdate.toString("dd.MM.yyyy"));  // Укажи правильный формат!
                    if (Bdate.isValid()) {
                        ui->DateOfBirthButton->setText(Bdate.toString("dd.MM.yyyy"));
                    } else {
                        qDebug() << "Ошибка: Некорректная дата рождения из БД ->" << birthDateStr;
                        ui->DateOfBirthButton->setText("Выберите дату");
                    }

                    // Обработка даты выдачи паспорта
                    QString issueDateStr = client[5].toString();
                    ui->dateOfIssueButton->setText(Issuedate.toString("dd.MM.yyyy"));
                    if (Issuedate.isValid()) {
                        ui->dateOfIssueButton->setText(Issuedate.toString("dd.MM.yyyy"));
                    } else {
                        qDebug() << "Ошибка: Некорректная дата выдачи из БД ->" << issueDateStr;
                        ui->dateOfIssueButton->setText("Выберите дату");
                    }

                    ui->passportIdEdit->setText(client[6].toString());
                    ui->placeOfResidenceEdit->setText(client[7].toString());
                    ui->descriptionEdit->setText(client[8].toString());
                }
            } else {
                ui->Header->setText("Добавить клиента");
            }
        }
        break;
    case AddUpdateSettingsE::PaymentTypes:
        if (true) {

            ui->currencyFrame->hide();
            ui->addressFrame->hide();
            ui->authorityFrame->hide();
            ui->dateOfBirthFrame->hide();
            ui->dateOfIssueFrame->hide();
            ui->passportIdFrame->hide();
            ui->placeOfResidenceFrame->hide();
            ui->phoneFrame->hide();
            ui->apartmentFrame->hide();
            ui->ploshadFrame->hide();
            ui->roomsQuanityFrame->hide();
            ui->paymentTypeLabel->setProperty("color", "white");
            ui->typeFrame->hide();
            ui->typesFrame->hide();
            ui->whoWhatFrame->hide();
            ui->clientFrame->hide();
            ui->userFrame->hide();
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityFrame->hide();
            ui->objectFrame->hide();
            ui->objectsFrame->hide();
            ui->floorFrame->hide();
            ui->PasswordFrame->hide();
            ui->priceSquareFrame->hide();
            ui->totalAmountFrame->hide();
            ui->isAdminFrame->hide();
            ui->descriptionFrame->hide();

            if (id > 0) {
                ui->Header->setText("Редактировать тип оплаты");
                QVariantList paymentType = Operations::getPaymentType(id)[0].toList();
                if (!paymentType.isEmpty())
                {
                    ui->paymentTypeEdit->setText(paymentType[1].toString());
                }
            } else {
                ui->Header->setText("Добавить тип оплаты");
            }
        }
        break;

    case AddUpdateSettingsE::Users:
        if (true) {

            ui->currencyFrame->hide();
            ui->addressFrame->hide();
            ui->authorityFrame->hide();
            ui->dateOfBirthFrame->hide();
            ui->dateOfIssueFrame->hide();
            ui->passportIdFrame->hide();
            ui->placeOfResidenceFrame->hide();
            ui->phoneFrame->hide();
            ui->apartmentFrame->hide();
            ui->ploshadFrame->hide();
            ui->roomsQuanityFrame->hide();
            ui->paymentTypeFrame->hide();
            ui->typeFrame->hide();
            ui->typesFrame->hide();
            ui->whoWhatFrame->hide();
            ui->clientFrame->hide();
            ui->userLabel->setProperty("color", "white");
            ui->clientsFrame->hide();
            ui->sanuzelsQuantityFrame->hide();
            ui->objectFrame->hide();
            ui->objectsFrame->hide();
            ui->floorFrame->hide();
            ui->PasswordLabel->setProperty("color", "white");
            ui->priceSquareFrame->hide();
            ui->totalAmountFrame->hide();
            ui->isAdminCheckBox->setProperty("color", "white");
            ui->descriptionLabel->setProperty("color", "white");

            if (id > 0) {
                ui->Header->setText("Редактировать пользователя");
                QVariantList user = Operations::getUser(id)[0].toList();
                if (!user.isEmpty())
                {
                    ui->userEdit->setText(user[1].toString());
                    ui->isAdminCheckBox->setChecked(user[3].toBool());
                    ui->descriptionEdit->setText(user[4].toString());
                } else {
                    ui->Header->setText("Добавить пользователя");
                }
            }
        }
        break;


    default:
        break;
    }
}

AddUpdateSettings::~AddUpdateSettings() {
    delete ui;
}


void AddUpdateSettings::on_addButton_clicked() {
    addRecord();
}


void AddUpdateSettings::addRecord() {
    QDate fromDate = this->from;
    QDate toDate = this->to;
    if (checkFill()){
    if (id > 0) {
        switch (this->mode) {
        case AddUpdateSettingsE::Clients:
            Operations::updateClient(id,ui->clientEdit->text().trimmed(), ui->phoneEdit->text().trimmed(), ui->authorityEdit->text().trimmed(), ui->DateOfBirthButton->text().trimmed(), ui->dateOfIssueButton->text().trimmed(), ui->passportIdEdit->text().trimmed(), ui->placeOfResidenceEdit->text().trimmed(), ui->descriptionEdit->toPlainText().trimmed());
            break;
        case AddUpdateSettingsE::PaymentTypes:
            Operations::updatePaymentType(id, ui->paymentTypeEdit->text().trimmed());
            break;
        case AddUpdateSettingsE::Users:
            Operations::updateUser(id, ui->userEdit->text().trimmed(), HASH::generateHMAC(ui->PasswordEdit->text().trimmed()), ui->isAdminCheckBox->isChecked(), ui->descriptionEdit->toPlainText().trimmed());
            break;
        case AddUpdateSettingsE::Type:
            Operations::updateType(id, ui->typeEdit->text().trimmed(), ui->isAdminCheckBox->isChecked());
            break;
        case AddUpdateSettingsE::WhoWhat:
            Operations::updateWhoWhat(id, types.value(ui->typesBox->currentText()), ui->whoWhatEdit->text().trimmed());
            emit objectAdded(types.value(ui->typesBox->currentText()));
            break;
        case AddUpdateSettingsE::Objects:
            Operations::updateObject(id, ui->objectEdit->text().trimmed(),ui->addressEdit->text().trimmed());
            break;
        case AddUpdateSettingsE::Apartments:
            Operations::updateApartment(id,
                                        objects.value(ui->objectsBox->currentText()),
                                        ui->apartmentEdit->text().trimmed(),
                                        ui->ploshadEdit->text().replace(',', '.').toDouble(),
                                        ui->roomsQuantityEdit->text().toInt(),
                                        ui->sanuzelsQuantityEdit->text().toInt(),
                                        ui->floorEdit->text().toInt(),
                                        ui->priceSquareEdit->text().replace(',', '.').toDouble(),
                                        ui->totalAmountEdit->text().replace(',', '.').toDouble(),
                                        ui->currencyBox->currentText());
            emit objectAdded(objects.value(ui->objectsBox->currentText()));
            break;
        }
    } else {
        switch (this->mode) {
        case AddUpdateSettingsE::Clients:
            Operations::addClient(ui->clientEdit->text().trimmed(), ui->phoneEdit->text().trimmed(), ui->authorityEdit->text().trimmed(), ui->DateOfBirthButton->text().trimmed(), ui->dateOfIssueButton->text().trimmed(), ui->passportIdEdit->text().trimmed(), ui->placeOfResidenceEdit->text().trimmed(), ui->descriptionEdit->toPlainText().trimmed());
            break;
        case AddUpdateSettingsE::PaymentTypes:
            Operations::addPaymentType(ui->paymentTypeEdit->text().trimmed());
            break;
        case AddUpdateSettingsE::Users:
            Operations::addUser(ui->userEdit->text().trimmed(), HASH::generateHMAC(ui->PasswordEdit->text().trimmed()), ui->isAdminCheckBox->isChecked(), ui->descriptionEdit->toPlainText().trimmed());
            break;
        case AddUpdateSettingsE::Type:
            Operations::addType(ui->typeEdit->text().trimmed(), ui->isAdminCheckBox->isChecked());
            break;
        case AddUpdateSettingsE::WhoWhat:
            Operations::addWhoWhat(types.value(ui->typesBox->currentText()), ui->whoWhatEdit->text().trimmed());
            emit objectAdded(types.value(ui->typesBox->currentText()));
            break;
        case AddUpdateSettingsE::Objects:
        {
            QString objectName = ui->objectEdit->text().trimmed();
            QString objectAddress = ui->addressEdit->text().trimmed();
            if (!objectName.isEmpty()) {
                Operations::addObject(objectName,objectAddress);
            }
        }
            break;
        case AddUpdateSettingsE::Apartments:
            QString currency = ui->currencyBox->currentText();
            Operations::addApartment(
                                    objects.value(ui->objectsBox->currentText()),
                                    ui->apartmentEdit->text().trimmed(),
                                    ui->ploshadEdit->text().replace(',', '.').toDouble(),
                                    ui->roomsQuantityEdit->text().toInt(),
                                    ui->sanuzelsQuantityEdit->text().toInt(),
                                    ui->floorEdit->text().toInt(),
                                    ui->priceSquareEdit->text().replace(',', '.').toDouble(),
                                    ui->totalAmountEdit->text().replace(',', '.').toDouble(),
                                    currency);
            emit objectAdded(objects.value(ui->objectsBox->currentText()));
            break;
        }
    }
    emit close();
    this->closed();}
    else{
        QTimer::singleShot(200, this, &AddUpdateSettings::resetInputColor);
    }
}


bool AddUpdateSettings::checkFill(){
    bool result = true;
    if (id < 0) {
        switch (this->mode) {
        case AddUpdateSettingsE::Clients:
            if (Operations::clientExists(ui->clientEdit->text().trimmed())) {
                ui->clientEdit->setStyleSheet("background-color: red;");
                ui->clientLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->phoneEdit->text().trimmed().isEmpty()){
                ui->phoneEdit->setStyleSheet("background-color: red;");
                ui->phoneLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->authorityEdit->text().trimmed().isEmpty()){
                ui->authorityEdit->setStyleSheet("background-color: red;");
                ui->authorityLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->DateOfBirthButton->text().trimmed().isEmpty()){
                ui->DateOfBirthButton->setStyleSheet("background-color: red;");
                ui->dateOfBirthLabel->setStyleSheet("color: red;");
                result = false;
            }
            if (ui->dateOfIssueButton->text().trimmed().isEmpty()){
                ui->dateOfIssueButton->setStyleSheet("background-color: red;");
                ui->dateOfIssueLabel->setStyleSheet("color: red;");
                result = false;
            }
            if (ui->passportIdEdit->text().trimmed().isEmpty()){
                ui->passportIdEdit->setStyleSheet("background-color: red;");
                ui->passportIdLabel->setStyleSheet("color: red;");
                result = false;
            }
            if (ui->placeOfResidenceEdit->text().trimmed().isEmpty()){
                ui->placeOfResidenceEdit->setStyleSheet("background-color: red;");
                ui->placeOfResidenceLabel->setStyleSheet("color: red;");
                result = false;
            }
            break;
        case AddUpdateSettingsE::PaymentTypes:
            if (ui->paymentTypeEdit->text().trimmed().isEmpty()){
                ui->paymentTypeEdit->setStyleSheet("background-color: red;");
                ui->paymentTypeLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::paymentTypeExists(ui->paymentTypeEdit->text().trimmed())) {
                ui->paymentTypeEdit->setStyleSheet("background-color:red;");
                ui->paymentTypeLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Users:
            if (ui->userEdit->text().trimmed().isEmpty()){
                ui->userEdit->setStyleSheet("background-color: red;");
                ui->userLabel->setStyleSheet("color: red;");
                result = false;
            }
            if(ui->PasswordEdit->text().trimmed().isEmpty()){
                ui->PasswordEdit->setStyleSheet("background-color: red;");
                ui->PasswordLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::userExists(ui->userEdit->text().trimmed())) {
                ui->userEdit->setStyleSheet("background-color:red;");
                ui->userLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Type:
            if (ui->typeEdit->text().trimmed().isEmpty()){
                ui->typeEdit->setStyleSheet("background-color: red;");
                ui->typeLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::typeExists(ui->typeEdit->text().trimmed())) {
                ui->typeEdit->setStyleSheet("background-color:red;");
                ui->typeLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::WhoWhat:
            if (ui->typesBox->currentText() == "-") {
                ui->typesBox->setStyleSheet("background-color: red;");
                ui->typesLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->whoWhatEdit->text().trimmed().isEmpty()){
                ui->whoWhatEdit->setStyleSheet("background-color: red;");
                ui->whoWhatLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::whoWhatExists(types.value(ui->typesBox->currentText()), ui->whoWhatEdit->text().trimmed())) {
                ui->whoWhatEdit->setStyleSheet("background-color:red;");
                ui->whoWhatLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Objects:
            if (ui->objectEdit->text().trimmed().isEmpty()){
                ui->objectEdit->setStyleSheet("background-color: red;");
                ui->objectLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::objectExists(ui->objectEdit->text().trimmed())) {
                ui->objectEdit->setStyleSheet("background-color:red;");
                ui->objectLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->addressEdit->text().trimmed().isEmpty()){
                ui->addressEdit->setStyleSheet("background-color:red;");
                ui->addressLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Apartments:
            if (ui->apartmentEdit->text().trimmed().isEmpty()){
                ui->apartmentEdit->setStyleSheet("background-color: red;");
                ui->apartmentLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::apartmentExists(objects.value(ui->objectsBox->currentText()), ui->apartmentEdit->text().trimmed())) {
                ui->apartmentEdit->setStyleSheet("background-color:red;");
                ui->apartmentLabel->setStyleSheet("color:red;");
                result = false;
            }

            if (ui->objectsBox->currentText() == "-") {
                ui->objectsBox->setStyleSheet("background-color: red;");
                ui->objectsLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(ui->roomsQuantityEdit->text().isEmpty()){
                ui->roomsQuantityEdit->setStyleSheet("background-color:red;");
                ui->roomsQuantityLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->sanuzelsQuantityEdit->text().isEmpty()){
                ui->sanuzelsQuantityEdit->setStyleSheet("background-color:red;");
                ui->sanuzelsQuantityLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->floorEdit->text().isEmpty()){
                ui->floorEdit->setStyleSheet("background-color:red;");
                ui->floorLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->priceSquareEdit->text().isEmpty()){
                ui->priceSquareEdit->setStyleSheet("background-color:red;");
                ui->priceSquareLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->ploshadEdit->text().isEmpty()){
                ui->ploshadEdit->setStyleSheet("background-color:red;");
                ui->ploshadLabel->setStyleSheet("color:red;");
                result = false;
            }
            if (ui->currencyBox->currentText() == "-") {
                ui->currencyBox->setStyleSheet("background-color: red;");
                ui->currencyLabel->setStyleSheet("color: red;");
                result = false;
            }

            break;
        }
    }
    else {
        switch (this->mode) {
        case AddUpdateSettingsE::Clients:
            if (Operations::clientExists(ui->clientEdit->text().trimmed())){
                ui->clientEdit->setStyleSheet("background-color: red;");
                ui->clientLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->phoneEdit->text().trimmed().isEmpty()){
                ui->phoneEdit->setStyleSheet("background-color: red;");
                ui->phoneLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->authorityEdit->text().trimmed().isEmpty()){
                ui->authorityEdit->setStyleSheet("background-color: red;");
                ui->authorityLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->DateOfBirthButton->text().trimmed().isEmpty()){
                ui->DateOfBirthButton->setStyleSheet("background-color: red;");
                ui->dateOfBirthLabel->setStyleSheet("color: red;");
                result = false;
            }
            if (ui->dateOfIssueButton->text().trimmed().isEmpty()){
                ui->dateOfIssueButton->setStyleSheet("background-color: red;");
                ui->dateOfIssueLabel->setStyleSheet("color: red;");
                result = false;
            }
            if (ui->passportIdEdit->text().trimmed().isEmpty()){
                ui->passportIdEdit->setStyleSheet("background-color: red;");
                ui->passportIdLabel->setStyleSheet("color: red;");
                result = false;
            }
            if (ui->placeOfResidenceEdit->text().trimmed().isEmpty()){
                ui->placeOfResidenceEdit->setStyleSheet("background-color: red;");
                ui->placeOfResidenceLabel->setStyleSheet("color: red;");
                result = false;
            }
            break;
        case AddUpdateSettingsE::PaymentTypes:
            if (ui->paymentTypeEdit->text().trimmed().isEmpty()){
                ui->paymentTypeEdit->setStyleSheet("background-color: red;");
                ui->paymentTypeLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::paymentTypeExistsForEdit(id, ui->paymentTypeEdit->text().trimmed())) {
                ui->paymentTypeEdit->setStyleSheet("background-color:red;");
                ui->paymentTypeLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Users:
            if (ui->userEdit->text().trimmed().isEmpty()){
                ui->userEdit->setStyleSheet("background-color: red;");
                ui->userLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(ui->PasswordEdit->text().trimmed().isEmpty()){
                ui->PasswordEdit->setStyleSheet("background-color: red;");
                ui->PasswordLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::userExistsForEdit(id, ui->userEdit->text().trimmed())) {
                ui->userEdit->setStyleSheet("background-color:red;");
                ui->userLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Type:
            if (ui->typeEdit->text().trimmed().isEmpty()){
                ui->typeEdit->setStyleSheet("background-color: red;");
                ui->typeLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::typeExistsForEdit(id, ui->typeEdit->text().trimmed())) {
                ui->typeEdit->setStyleSheet("background-color:red;");
                ui->typeLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::WhoWhat:
            if (ui->typesBox->currentText() == "-") {
                ui->typesBox->setStyleSheet("background-color: red;");
                ui->typesLabel->setStyleSheet("color: red;");
                result = false;
            }

            if (ui->whoWhatEdit->text().trimmed().isEmpty()){
                ui->whoWhatEdit->setStyleSheet("background-color: red;");
                ui->whoWhatLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::whoWhatExistsForEdit(types.value(ui->typesBox->currentText()), id, ui->whoWhatEdit->text().trimmed())) {
                ui->whoWhatEdit->setStyleSheet("background-color:red;");
                ui->whoWhatLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Objects:
            if (ui->objectEdit->text().trimmed().isEmpty()){
                ui->objectEdit->setStyleSheet("background-color: red;");
                ui->objectLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::objectExistsForEdit(id, ui->objectEdit->text().trimmed())) {
                ui->objectEdit->setStyleSheet("background-color:red;");
                ui->objectLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->addressEdit->text().trimmed().isEmpty()){
                ui->addressEdit->setStyleSheet("background-color:red;");
                ui->addressLabel->setStyleSheet("color:red;");
                result = false;
            }

            break;
        case AddUpdateSettingsE::Apartments:
            if (ui->apartmentEdit->text().trimmed().isEmpty()){
                ui->apartmentEdit->setStyleSheet("background-color: red;");
                ui->apartmentLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(Operations::apartmentExistsForEdit(objects.value(ui->objectsBox->currentText()), id, ui->apartmentEdit->text().trimmed())) {
                ui->apartmentEdit->setStyleSheet("background-color:red;");
                ui->apartmentLabel->setStyleSheet("color:red;");
                result = false;
            }

            if (ui->objectsBox->currentText() == "-") {
                ui->objectsBox->setStyleSheet("background-color: red;");
                ui->objectsLabel->setStyleSheet("color: red;");
                result = false;
            }

            if(ui->roomsQuantityEdit->text().isEmpty()){
                ui->roomsQuantityEdit->setStyleSheet("background-color:red;");
                ui->roomsQuantityLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->sanuzelsQuantityEdit->text().isEmpty()){
                ui->sanuzelsQuantityEdit->setStyleSheet("background-color:red;");
                ui->sanuzelsQuantityLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->floorEdit->text().isEmpty()){
                ui->floorEdit->setStyleSheet("background-color:red;");
                ui->floorLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->priceSquareEdit->text().isEmpty()){
                ui->priceSquareEdit->setStyleSheet("background-color:red;");
                ui->priceSquareLabel->setStyleSheet("color:red;");
                result = false;
            }

            if(ui->ploshadEdit->text().isEmpty()){
                ui->ploshadEdit->setStyleSheet("background-color:red;");
                ui->ploshadLabel->setStyleSheet("color:red;");
                result = false;
            }

            if (ui->currencyBox->currentText() == "-" || Operations::isApartmentSold(id)) {
                ui->currencyBox->setStyleSheet("background-color: red;");
                ui->currencyLabel->setStyleSheet("color: red;");
                result = false;
            }

            break;
        }
    }
    return result;
}

void AddUpdateSettings::resetInputColor()
{
    ui->addressEdit->setStyleSheet("");
    ui->addressLabel->setStyleSheet("Color: white;");

    ui->authorityEdit->setStyleSheet("");
    ui->authorityLabel->setStyleSheet("Color: white;");

    ui->DateOfBirthButton->setStyleSheet("");
    ui->dateOfBirthLabel->setStyleSheet("Color: white;");

    ui->dateOfIssueButton->setStyleSheet("");
    ui->dateOfIssueLabel->setStyleSheet("Color: white;");

    ui->passportIdEdit->setStyleSheet("");
    ui->passportIdLabel->setStyleSheet("Color: white;");

    ui->placeOfResidenceEdit->setStyleSheet("");
    ui->placeOfResidenceLabel->setStyleSheet("Color: white;");



    ui->paymentTypeEdit->setStyleSheet("");
    ui->paymentTypeLabel->setStyleSheet("Color: white;");

    ui->typeEdit->setStyleSheet("");
    ui->typeLabel->setStyleSheet("Color: white;");

    ui->whoWhatEdit->setStyleSheet("");
    ui->whoWhatLabel->setStyleSheet("Color: white;");

    ui->clientEdit->setStyleSheet("");
    ui->clientLabel->setStyleSheet("Color: white;");

    ui->objectEdit->setStyleSheet("");
    ui->objectLabel->setStyleSheet("Color: white;");

    ui->PasswordEdit->setStyleSheet("");
    ui->PasswordLabel->setStyleSheet("Color: white;");

    ui->phoneEdit->setStyleSheet("");
    ui->phoneLabel->setStyleSheet("Color: white;");

    ui->clientsLabel->setStyleSheet("Color: white;");
    ui->clientsBox->setStyleSheet("background-color: white;");

    ui->typesLabel->setStyleSheet("Color: white");
    ui->typesBox->setStyleSheet("background-color: white;");

    ui->apartmentEdit->setStyleSheet("");
    ui->apartmentLabel->setStyleSheet("Color: white");

    ui->ploshadEdit->setStyleSheet("");
    ui->ploshadLabel->setStyleSheet("Color: white");

    ui->roomsQuantityEdit->setStyleSheet("");
    ui->roomsQuantityLabel->setStyleSheet("Color: white");

    ui->userEdit->setStyleSheet("");
    ui->userLabel->setStyleSheet("Color: white");

    ui->sanuzelsQuantityEdit->setStyleSheet("");
    ui->sanuzelsQuantityLabel->setStyleSheet("Color: white");

    ui->floorEdit->setStyleSheet("");
    ui->floorLabel->setStyleSheet("color:white;");

    ui->priceSquareEdit->setStyleSheet("");
    ui->priceSquareLabel->setStyleSheet("color:white;");

    ui->totalAmountEdit->setStyleSheet("");
    ui->totalAmountLabel->setStyleSheet("color:white;");

    ui->objectsBox->setStyleSheet("background-color: white;");
    ui->objectsLabel->setStyleSheet("color:white;");

    ui->currencyBox->setStyleSheet("background-color: white;");
    ui->currencyLabel->setStyleSheet("color:white;");
}


void AddUpdateSettings::calculateTotalAmount() {
    bool ok1, ok2;
    double ploshad = ui->ploshadEdit->text().replace(',', '.').toDouble(&ok1);
    double pricePerSquare = ui->priceSquareEdit->text().replace(',', '.').toDouble(&ok2);

    if (ok1 && ok2) {
        double total = ploshad * pricePerSquare;
        ui->totalAmountEdit->setText(QString::number(total, 'f', 2));
    } else {
        ui->totalAmountEdit->clear(); // Очищаем, если некорректные данные
    }
}

void AddUpdateSettings::on_DateOfBirthButton_clicked()
{
    CalendarPage *calendar = new CalendarPage(Bdate);

    connect(calendar, &CalendarPage::changeDate, this, &AddUpdateSettings::changeDateOfBirthDate);

    calendar->show();
}


void AddUpdateSettings::changeDateOfBirthDate(QDate date){
    this->Bdate = date;
    ui->DateOfBirthButton->setText(this->Bdate.toString("dd.MM.yyyy"));
}


void AddUpdateSettings::on_dateOfIssueButton_clicked()
{
    CalendarPage *calendars = new CalendarPage(Issuedate);

    connect(calendars, &CalendarPage::changeDate, this, &AddUpdateSettings::changedateOfIssueDate);

    calendars->show();
}

void AddUpdateSettings::changedateOfIssueDate(QDate date){
    this->Issuedate = date;
    ui->dateOfIssueButton->setText(this->Issuedate.toString("dd.MM.yyyy"));
}

