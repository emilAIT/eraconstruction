#ifndef CONTRACTWORDMANAGER_H
#define CONTRACTWORDMANAGER_H

#include <QString>

class ContractWordManager
{
public:
    ContractWordManager();

    // Метод для создания договора с подстановкой динамических данных
    QString createContract(
        const QString &client,
        const QString &roomsQuantity,
        const QString &ploshad,
        const QString &apartment,
        const QString &pricepPerMetr,
        const QString &apartmentCost,
        const QString &totalAmount,
        const QString &floor,
        const QString &date,
        const QString &initialPayment,
        const QString &monthlyPayment,
        const QString &debt,
        const QString &res,
        const bool &isInstallment,
        const QString &dateOfBirth,
        const QString &pasportId,
        const QString &authority,
        const QString &dateOfIssue,
        const QString &placeOfResidence,
        const QString &objectLocation,
        const QString &currency
        );

    // Метод для формирования HTML-содержимого договора по шаблону
    QString buildHtml(
        const QString &client,
        const QString &roomsQuantity,
        const QString &ploshad,
        const QString &apartment,
        const QString &pricePerMetr,
        const QString &apartmentCost,
        const QString &totalAmount,
        const QString &floor,
        const QString &date,
        const QString &initialPayment,
        const QString &monthlyPayment,
        const QString &debt,
        const QString &res,
        const bool &isInstallment,
        const QString &dateOfBirth,
        const QString &pasportId,
        const QString &authority,
        const QString &dateOfIssue,
        const QString &placeOfResidence,
        const QString &objectLocation,
        const QString &currency
        );

    // Метод для предпросмотра договора с кнопкой "Печать"
    void showWordViewer(const QString &filePath);
};

#endif // CONTRACTWORDMANAGER_H
