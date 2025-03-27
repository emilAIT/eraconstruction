#include "addupdateotherevents.h"
#include "ui_addupdateotherevents.h"

#include "qtimer.h"

AddUpdateOtherEvents::AddUpdateOtherEvents(EventsE eventType, int typeOperation, int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddUpdateOtherEvents)
{
    ui->setupUi(this);

    this->eventType = eventType;
    this->id = id;
    this->typeOperation = typeOperation;

    //для оперативной памяти удаляет с рем когда закрываю окно
    this->setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setProperty("color", "green");

    ui->amountEdit->setValidator(new QDoubleValidator());

    ui->descriptionLabel->setProperty("color", "white");
    ui->dateButton->setProperty("color", "gray");

    date = QDate::currentDate();
    ui->dateButton->setText(date.toString("dd.MM.yyyy"));

    ui->dateLabel->setProperty("color", "white");
    ui->typeLabel->setProperty("color", "white");
    ui->whoWhatLabel->setProperty("color", "white");
    ui->paymentTypeLabel->setProperty("color", "white");
    ui->currencyLabel->setProperty("color", "white");
    ui->amountLabel->setProperty("color", "white");
    ui->descriptionLabel->setProperty("color", "white");

    QVariantList typesList = Operations::selectAllTypes();
    ui->typeBox->addItem("-");
    foreach (QVariant type, typesList) {
        QVariantList data = type.toList();
        QString show = data[1].toString();
        types.insert(show, data[0].toInt());
        ui->typeBox->addItem(show);
    }

    QVariantList paymentTypesList = Operations::selectAllPaymentTypes();
    ui->paymentTypeBox->addItem("-");
    foreach (QVariant pType, paymentTypesList) {
        QVariantList data = pType.toList();
        QString show = data[1].toString();
        paymentTypes.insert(show, data[0].toInt());
        ui->paymentTypeBox->addItem(show);
    }

    switch (this->typeOperation) {
    case 1:
        ui->Header->setText("Добавить расход");
        break;

    case -1:
        ui->Header->setText("Редактировать расход");
        loadExpensesData();
        break;

    default:
        ui->Header->setText("Расходы");
        break;
    }
}

AddUpdateOtherEvents::~AddUpdateOtherEvents()
{
    delete ui;
}

void AddUpdateOtherEvents::on_dateButton_clicked() {
    CalendarPage *calendar = new CalendarPage(date);
    connect(calendar, &CalendarPage::changeDate, this, &AddUpdateOtherEvents::onChangeDate);
    calendar->show();
}

void AddUpdateOtherEvents::onChangeDate(QDate date) {
    this->date = date;
    ui->dateButton->setText(this->date.toString("dd.MM.yyyy"));
}

void AddUpdateOtherEvents::on_typeBox_currentIndexChanged(int index)
{
    ui->whoWhatBox->clear();
    whoWhats.clear();

    QString selectedType = ui->typeBox->itemText(index);
    int typeId = types.value(selectedType);

    QVariantList whoWhatList = Operations::selectAllWhoWhats(typeId);

    ui->whoWhatBox->addItem("-");
    qDebug() << whoWhatList;
    foreach (QVariant whoWhat, whoWhatList) {
        QVariantList data = whoWhat.toList();
        QString show = data[2].toString();
        whoWhats.insert(show, data[0].toInt());
        ui->whoWhatBox->addItem(show);
    }
}



void AddUpdateOtherEvents::on_addButton_clicked()
{
    if(checkFill()){
    switch(this->typeOperation) {
        case 1: {
            int typeId = types.value(ui->typeBox->currentText(), 0);
            int whoWhatId = whoWhats.value(ui->whoWhatBox->currentText(), 0);
            int paymentTypeId = paymentTypes.value(ui->paymentTypeBox->currentText(), 0);
            double amount = ui->amountEdit->text().replace(',','.').toDouble();
            QDate expenseDate = date;
            QString description = ui->descriptionEdit->toPlainText();
            QString currency = ui->currencyBox->currentText();

            bool success = Operations::addExpense(
                expenseDate, typeId, whoWhatId, paymentTypeId, currency,
                amount, description
                );
            break;
        }

        case -1:
            int typeId = types.value(ui->typeBox->currentText(), 0);
            int whoWhatId = whoWhats.value(ui->whoWhatBox->currentText(), 0);
            int paymentTypeId = paymentTypes.value(ui->paymentTypeBox->currentText(), 0);
            double amount = ui->amountEdit->text().replace(',', '.').toDouble();
            QDate expenseDate = date;
            QString description = ui->descriptionEdit->toPlainText();
            QString currency = ui->currencyBox->currentText();

            bool success = Operations::updateExpense(
                id, expenseDate, typeId, whoWhatId, paymentTypeId, currency,
                amount, description
                );
            break;
        }
    emit closed();
        this->close();}
    else{
        QTimer::singleShot(200, this, &AddUpdateOtherEvents::resetInputColor);
    }

}


void AddUpdateOtherEvents::loadExpensesData() {
    QVariantList expenses = Operations::getExpense(id);
    if (!expenses.isEmpty()) {
        QVariantList expenseData = expenses[0].toList();
        date = expenseData[0].toDate();
        ui->dateButton->setText(date.toString("dd.MM.yyyy"));
        ui->typeBox->setCurrentText(expenseData[1].toString());
        ui->whoWhatBox->setCurrentText(expenseData[2].toString());
        ui->paymentTypeBox->setCurrentText(expenseData[3].toString());
        ui->amountEdit->setText(QString::number(expenseData[4].toDouble(), 'f', 2));
        ui->descriptionEdit->setText(expenseData[5].toString());
        ui->currencyBox->setCurrentText(expenseData[6].toString());
    }
}

bool AddUpdateOtherEvents::checkFill()
{
    bool result = true;
    if (ui->typeBox->currentText() == "-") {
        ui->typeBox->setStyleSheet("background-color: red;");
        ui->typeLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->whoWhatBox->currentText() == "-") {
        ui->whoWhatBox->setStyleSheet("background-color: red;");
        ui->whoWhatLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->paymentTypeBox->currentText() == "-") {
        ui->paymentTypeBox->setStyleSheet("background-color: red;");
        ui->paymentTypeLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->currencyBox->currentText() == "-") {
        ui->currencyBox->setStyleSheet("background-color: red;");
        ui->currencyLabel->setStyleSheet("color: red;");
        result = false;
    }
    if (ui->amountEdit->text().isEmpty())
    {
        ui->amountEdit->setStyleSheet("background-color: red;");
        ui->amountLabel->setStyleSheet("color: red;");
        result = false;
    }
    return result;
}

void AddUpdateOtherEvents::resetInputColor(){

    ui->typeBox->setStyleSheet("background-color: white;");
    ui->typeLabel->setStyleSheet("color: white;");
    ui->whoWhatBox->setStyleSheet("background-color: white;");
    ui->whoWhatLabel->setStyleSheet("color: white;");
    ui->paymentTypeLabel->setStyleSheet("color: white;");
    ui->paymentTypeBox->setStyleSheet("background-color: white;");
    ui->currencyLabel->setStyleSheet("color: white;");
    ui->currencyBox->setStyleSheet("background-color: white;");
    ui->amountLabel->setStyleSheet("color: white;");
    ui->amountEdit->setStyleSheet("");
}

