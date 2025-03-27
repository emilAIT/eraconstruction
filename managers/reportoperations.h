#ifndef REPORTOPERATIONS_H
#define REPORTOPERATIONS_H


#include <QDate>
#include <QVariantList>

#include "../managers/dbmanager.h"

class ReportOperations
{
public:
    ReportOperations();

    // general reports
    QVariantList static getPaymentTypesReport(QDate from, QDate to);
    QVariantList static getAllPaymentTypesReport(QDate from, QDate to);

    QVariantList static getObjectsReport(QDate from, QDate to);
    QVariantList static getAllObjectsReport(QDate from, QDate to);

    QVariantList static getExpensesReport(QDate from, QDate to);
    QVariantList static getAllExpensesReport(QDate from, QDate to);

    QVariantList static getClientsReport(QDate from, QDate to);
    QVariantList static getAllClientsReport(QDate from, QDate to);

    QVariantList static getApartmentsReport(int objectId, QDate from, QDate to);
    QVariantMap static getAllApartmentsReport(int objectId, QDate from, QDate to);

    QVariantList static getDebtsReport();
    QVariantList static getAllDebtsReport();

    QVariantList static getTotalReport(QDate from, QDate to);

    // reports by item
    QVariantList static getPaymentTypesReportByItem(int id, QDate from, QDate to);
    QVariantList static getAllPaymentTypesReportByItem(int id, QDate from, QDate to);

    QVariantList static getObjectsReportByItem(int id, QDate from, QDate to);
    QVariantList static getAllObjectsReportByItem(int id, QDate from, QDate to);

    QVariantList static getExpensesReportByItem(int id, QDate from, QDate to);
    QVariantList static getAllExpensesReportByItem(int id, QDate from, QDate to);

    QVariantList static getClientsReportByItem(int id, QDate from, QDate to);
    QVariantList static getAllClientsReportByItem(int id, QDate from, QDate to);

    QVariantList static getApartmentPayments(int id);
    QVariantList static getApartmentInformation(int id);
    QVariantList static getApartmentAllPayments(int id);

    QVariantList static getClientInformation(int Salesid);
    QVariantList static getClientPayments(int saleId);

    QVariantList static getClientApartments(int clientId);
    QVariantList static getClients();
    QVariantList static getClientsPaymentsForReportByClient(int salesId);

    // functions for report by apartment, debtor and client is not fully ready
    // QVariantList static getPaymentHistoryReportByItem(int id, QDate from, QDate to); // second table in report by client page and second table by apartment page
    // QVariantList static getAllPaymentHistoryReportByItem(int id, QDate from, QDate to);




};

#endif // REPORTOPERATIONS_H
