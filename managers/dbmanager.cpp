#include "dbmanager.h"
#include "qapplication.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

dbManager &dbManager::getInstance()
{
    static dbManager instance;
    return instance;
}

dbManager::dbManager()
{
    ni = new NoInternet();
    connect(ni, &NoInternet::reload, this, &dbManager::onReload);
}

dbManager::~dbManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool dbManager::connectDB()
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    QFile file("conn.data");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading.";
        openError();
        return false;
    }

    QTextStream in(&file);
    EncryptionManager& manager = EncryptionManager::getInstance();

    QString hostData = manager.decrypt(in.readLine());
    QString portData = manager.decrypt(in.readLine());
    QString dbName = manager.decrypt(in.readLine());
    QString username = manager.decrypt(in.readLine());
    QString password = manager.decrypt(in.readLine());

    db.setHostName(hostData);
    db.setPort(portData.toInt());
    db.setDatabaseName(dbName);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open())
    {
        qDebug() << "database connection error: " << db.lastError().text();
        // Logger::instance()->logInfo("database connection error: " + db.lastError().text());
        openError();
        return false;
    }
    else
    {
        qDebug() << "database connected successfully!";
        // Logger::instance()->logInfo("database connected successfully!");
        createTables();
        return true;
    }
}




bool dbManager::isConnected() const
{
    return db.isOpen();
}

bool dbManager::onReload()
{
    if (connectDB()) {
        emit reload();
        return true;
    }
    return false;
}

void dbManager::openError()
{
    if (ni->isHidden())
        ni->show();
}

bool dbManager::executeSet(const QString query)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qDebug() << "set query execution: " << query;
    QSqlQuery q;
    if (!q.exec(query))
    {
        qDebug() << "set query error: " << q.lastError();
        openError();
        QApplication::restoreOverrideCursor();
        return false;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

QVariantList dbManager::executeGet(const QString query)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qDebug() << "get query execution: " << query;
    QSqlQuery q;
    QVariantList out;
    if (q.exec(query))
    {
        while (q.next())
        {
            out.push_back(getValuesFromRecord(q.record()));
        }
    }
    else
    {
        qDebug() << "get query error: " << q.lastError();
        openError();
    }
    QApplication::restoreOverrideCursor();
    return out;
}

QVariantList dbManager::getValuesFromRecord(const QSqlRecord &record)
{
    QVariantList values;
    for (int i = 0; i < record.count(); ++i)
    {
        values.append(record.value(i));
    }
    return values;
}



void dbManager::createTables() {
    createClientsTable();
    createPaymentTypesTable();
    createUsersTable();
    createTypesTable();
    createWhoWhatTable();
    createObjectsTable();
    createApartmentsTable();
    createSalesTable();
    createPaymentTable();

    createExpensesTable();
}




void dbManager::createClientsTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_clients (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            phoneNumber TEXT NOT NULL,
            authority TEXT NOT NULL,
            dateOfBirth TEXT NOT NULL,
            dateOfIssue TEXT NOT NULL,
            passportId TEXT NOT NULL,
            placeOfResidence TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}


void dbManager::createPaymentTypesTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_paymentTypes (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createUsersTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_users (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            password TEXT NOT NULL,
            isAdmin BOOLEAN NOT NULL DEFAULT false,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}


void dbManager::createTypesTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_types (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            isAdmin BOOLEAN NOT NULL DEFAULT false
        )
    )Q";
    this->executeSet(createTableQuery);
}


void dbManager::createWhoWhatTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_whoWhat (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            typeId INTEGER NOT NULL,
            name TEXT NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}


void dbManager::createObjectsTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_objects (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            address TEXT NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}


void dbManager::createApartmentsTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_apartments (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            objectId INTEGER NOT NULL,
            apartmentNumber TEXT NOT NULL,
            ploshad DECIMAL(12,2) NOT NULL,
            roomsQuantity INTEGER NOT NULL,
            sanuzelsQuantity INTEGER NOT NULL,
            floor INTEGER NOT NULL,
            priceSquare DECIMAL(12,2) NOT NULL,
            totalAmount DECIMAL(12,2) NOT NULL,
            currency TEXT NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}




void dbManager::createExpensesTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_expenses (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            typeId INTEGER NOT NULL,
            whoWhatId INTEGER NOT NULL,
            paymentTypeId INTEGER NOT NULL,
            currency TEXT NOT NULL,
            amount DECIMAL(12,2) NOT NULL,
            date DATE DEFAULT (CURRENT_DATE),
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}




void dbManager::createSalesTable() {
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_sales (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            clientId INTEGER NOT NULL,
            objectId INTEGER NOT NULL,
            apartmentId INTEGER NOT NULL,
            apartmentCost DECIMAL(12,2) NOT NULL,
            discount FLOAT NOT NULL,
            totalAmount DECIMAL(12,2) NOT NULL,
            paymentTypeId INTEGER NOT NULL,
            currency TEXT NOT NULL,
            isInstallmentPlan BOOLEAN NOT NULL DEFAULT false,
            installment DECIMAL(12,2) NOT NULL,
            monthQuantity INTEGER NOT NULL,
            monthlyPayment DECIMAL(15,5) NOT NULL,
            description TEXT,
            date DATE DEFAULT (CURRENT_DATE)
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createPaymentTable(){
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS eraconstruction_payments (
            id  INTEGER PRIMARY KEY AUTO_INCREMENT,
            clientId INTEGER NOT NULL,
            objectId INTEGER NOT NULL,
            apartmentId INTEGER NOT NULL,
            paymentTypeId INTEGER NOT NULL,
            currency TEXT NOT NULL,
            payment DECIMAL(12,2) NOT NULL,
            description TEXT,
            date DATE DEFAULT (CURRENT_DATE)
        )
    )Q";
    this->executeSet(createTableQuery);
}




