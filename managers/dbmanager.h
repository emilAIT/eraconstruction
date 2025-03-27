#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "../pages/nointernet.h"
#include "encryptionmanager.h"

#include "qobject.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariantList>
#include <QSqlRecord>
#include <QDir>
#include <QDebug>

class dbManager : public QObject
{
    Q_OBJECT
public:
    static dbManager &getInstance();

    bool connectDB();
    bool isConnected() const;
    bool executeSet(const QString query);
    QVariantList executeGet(const QString query);
    void openError();

private:
    dbManager();
    ~dbManager();

    QVariantList getValuesFromRecord(const QSqlRecord &record);

    void createTables();

    void createClientsTable();
    void createPaymentTypesTable();
    void createUsersTable();
    void createTypesTable();
    void createWhoWhatTable();
    void createObjectsTable();
    void createApartmentsTable();
    void createSalesTable();
    void createPaymentTable();


    void createExpensesTable();


    NoInternet *ni;
    QString dbName;
    QSqlDatabase db;

signals:
    void reload();

private slots:
    bool onReload();
};

#endif // DBMANAGER_H
