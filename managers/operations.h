#ifndef OPERATIONS_H
#define OPERATIONS_H



#include <QVariantList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "dbmanager.h"
#include "usersession.h"

using namespace std;

class Operations
{
public:
    Operations();

    QVariantList static selectClientsWithDebt();
    QVariantList static selectClientsWithPurchase();
    QVariantList static selectAllClients();
    QVariantList static getClient(int clientId);
    bool static addClient(QString name, QString phoneNumber,QString authority,QString dateOfBirth,QString dateOfIssue,QString passportId,QString placeOfRessidence, QString description);
    bool static updateClient(int clientsId,QString name, QString phoneNumber,QString authority,QString dateOfBirth,QString dateOfIssue,QString passportId,QString placeOfRessidence, QString description);
    bool static deleteClient(int clientId);
    bool static clientExists(QString name);

    QVariantList static selectAllPaymentTypes();
    QVariantList static getPaymentType(int paymentTypeId);
    bool static addPaymentType(QString name);
    bool static paymentTypeExists(const QString& name);
    bool static paymentTypeExistsForEdit(int paymentTypeId, const QString& name);
    bool static updatePaymentType(int paymentTypeId, QString name);
    bool static deletePaymentType(int paymentTypeId);

    QVariantList static selectAllUsers();
    QVariantList static getUser(int userId);
    bool static addUser(QString name, QString password, bool isAdmin, QString description);
    bool static userExists(const QString& name);
    bool static userExistsForEdit(int userId, const QString& name);
    bool static updateUser(int userId, QString name, QString password, bool isAdmin, QString description);
    bool static deleteUser(int userId);
    QList<int> static checkLoginUser(QString username,QString password);

    QVariantList static selectAllTypes();
    QVariantList static getType(int typeId);
    bool static addType(QString name, bool isAdmin);
    bool static typeExists(const QString& name);
    bool static typeExistsForEdit(int typeId, const QString& name);
    bool static updateType(int typeId, QString name, bool isAdmin);
    bool static deleteType(int typeId);

    QVariantList static selectAllWhoWhats(int typeId);
    QVariantList static getWhoWhat(int whoWhatId);
    bool static addWhoWhat(int typeId, QString whoWhat);
    bool static whoWhatExists(int typeId, const QString& name);
    bool static whoWhatExistsForEdit(int typeId, int whoWhatId, const QString& name);
    bool static updateWhoWhat(int whoWhatId, int typeId, QString name);
    bool static deleteWhoWhat(int whoWhatId);

    QVariantList static selectObjectsByClient(int clientId);
    QVariantList static selectObjectsWithInstallmentByClient(int clientId);
    QVariantList static selectAllObjects();
    QVariantList static selectAllObjectsForMainWindow();
    QVariantList static selectAvailableObjects(int saleId);
    QVariantList static getObject(int objectId);
    QVariantList static getApartmentsInSales(int objectId);
    bool static addObject(QString name, QString address);
    bool static objectExists(const QString& name);
    bool static objectExistsForEdit(int objectId, const QString& name);
    bool static updateObject(int objectId, QString name, QString address);
    bool static deleteObject(int objectId);

    QVariantList static selectApartmentsByClient(int clientId, int objectId);
    QVariantList static selectInstallmentApartments(int clientId, int objectId);
    QVariantList static selectAvailableApartments(int objectId, int saleId);
    QVariantList static selectAllApartments(int objectId);
    QVariantList static selectAllApartmentsForMainWindow(int objectId);
    QVariantList static selectAllApartmentsWithoutId();
    QVariantList static getApartment(int apartmentId);
    bool static isApartmentInSales(int apartmentId);
    bool static addApartment(int objectId, QString apartmentNumber, double ploshad, int roomsQuantity, int sanuzelsQuantity, int floor, double priceSquare, double totalAmount, QString currency);
    bool static apartmentExists(int objectId, const QString& apartmentNumber);
    bool static apartmentExistsForEdit(int objectId, int apartmentId, const QString& apartmentNumber);
    bool static updateApartment(int apartmentId, int objectId, QString apartmentNumber, double ploshad, int roomsQuantity, int sanuzelsQuantity, int floor, double priceSquare, double totalAmount, QString currency);
    bool static deleteApartment(int apartmentId);
    bool static isApartmentSold(int apartmentId);

    QVariantList static selectAllSales(QDate fromdate, QDate todate);
    QVariantList static getSale(int saleId);
    bool static addSale(QDate date, int clientId, int objectId, int apartmentId, float apartmentCost, float discount, float totalAmount, int paymentTypeId, QString currency, bool isInstallmentPlan, double installment, int monthQuantity, double monthlyPayment, QString description);
    bool static updateSale(int saleId, QDate date, int clientId, int objectId, int apartmentId, double apartmentCost, float discount, double totalAmount, int paymentTypeId, QString currency, bool isInstallmentPlan, double installment, int monthQuantity, double monthlyPayment, QString description);
    bool static deleteSale(int saleId);

    QDate static getSaleDate(int clientId, int objectId, int apartmentId);
    bool static addPayment(QDate date, int clientId, int objectId, int apartmentId, int paymentTypeId, QString currency, double payment, QString description);
    QVariantList static getMonthlyPayments(int clientId, int objectId, int apartmentId);
    QVariantList static getPaymentDetails(int clientId, int objectId, int apartmentId);
    QVariantList static selectAllPayments(QDate fromdate, QDate todate);
    QVariantList static getPayment(int paymentId);
    bool static updatePayment(int paymentId, QDate date, int clientId, int objectId, int apartmentId, int paymentTypeId, QString currency, double payment, QString description);
    bool static deletePayment(int paymentId);

    QVariantList static selectAllExpenses(QDate fromdate, QDate todate);
    bool static addExpense(QDate date, int typeId, int whoWhatId, int paymentTypeId, QString currency, double amount, QString description);
    QVariantList static getExpense(int expenseId);
    bool static updateExpense(int expenseId, QDate date, int typeId, int whoWhatId, int paymentTypeId, QString currency, double amount, QString description);
    bool static deleteExpense(int expenseId);

    bool static checkHaveDebts();
};

#endif // OPERATIONS_H
