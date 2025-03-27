#include "reportoperations.h"

ReportOperations::ReportOperations() {}

QVariantList ReportOperations::getPaymentTypesReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    COALESCE(pt.id, 0) AS id,\n"
        "    COALESCE(pt.name, '-') AS payment_type,\n"
        "    SUM(t.amount) AS amount, \n"
        "    t.currency AS currency \n"
        "FROM (\n"
        "    -- Данные из продаж (только фактически полученные деньги)\n"
        "    SELECT\n"
        "        s.id AS id,\n"
        "        s.date AS date,\n"
        "        s.paymentTypeId AS paymentTypeId,\n"
        "        s.currency AS currency,\n"
        "        CASE\n"
        "            WHEN s.isInstallmentPlan = 1 THEN s.installment -- Рассрочка, учитываем только первоначальный взнос\n"
        "            ELSE s.totalAmount -- Полная оплата, учитываем всю сумму\n"
        "        END AS amount\n"
        "    FROM eraconstruction_sales s\n"
        "    WHERE s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'\n"
        "\n"
        "    -- Данные из оплат\n"
        "    UNION ALL\n"
        "    SELECT\n"
        "        p.id AS id,\n"
        "        p.date AS date,\n"
        "        p.paymentTypeId AS paymentTypeId,\n"
        "        p.currency AS currency,\n"
        "        p.payment AS amount\n"
        "    FROM eraconstruction_payments p\n"
        "    WHERE p.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'\n"
        "\n"
        "    UNION ALL\n"
        "    -- Данные из расходов (отрицательные суммы)\n"
        "    SELECT\n"
        "        e.id AS id,\n"
        "        e.date AS date,\n"
        "        e.paymentTypeId AS paymentTypeId,\n"
        "        e.currency AS currency,\n"
        "        -e.amount AS amount\n"
        "    FROM eraconstruction_expenses e\n"
        "    WHERE e.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'\n"
        ") t\n"
        "LEFT JOIN eraconstruction_paymentTypes pt ON t.paymentTypeId = pt.id\n"
        "WHERE t.amount <> 0\n"
        "GROUP BY pt.id, pt.name, t.currency\n"
        "ORDER BY payment_type ASC";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllPaymentTypesReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    t.currency,\n"
        "    SUM(t.amount) AS total_amount\n"
        "FROM (\n"
        "    -- Данные из продаж (только фактически полученные деньги)\n"
        "    SELECT\n"
        "        CASE\n"
        "            WHEN s.isInstallmentPlan = 1 THEN s.installment -- Рассрочка: только первоначальный взнос\n"
        "            ELSE s.totalAmount -- Полная оплата: вся сумма\n"
        "        END AS amount,\n"
        "        s.currency\n"
        "    FROM eraconstruction_sales s\n"
        "    WHERE s.date BETWEEN :fromDate AND :toDate\n"
        "\n"
        "    UNION ALL\n"
        "\n"
        "    -- Данные из оплат\n"
        "    SELECT p.payment AS amount, p.currency\n"
        "    FROM eraconstruction_payments p\n"
        "    WHERE p.date BETWEEN :fromDate AND :toDate\n"
        "\n"
        "    UNION ALL\n"
        "\n"
        "    -- Данные из расходов (отрицательные суммы)\n"
        "    SELECT -e.amount AS amount, e.currency\n"
        "    FROM eraconstruction_expenses e\n"
        "    WHERE e.date BETWEEN :fromDate AND :toDate\n"
        ") t\n"
        "GROUP BY t.currency";  // Группируем по валюте

    QSqlQuery sqlQuery;
    sqlQuery.prepare(query);
    sqlQuery.bindValue(":fromDate", from.toString("yyyy-MM-dd"));
    sqlQuery.bindValue(":toDate", to.toString("yyyy-MM-dd"));

    if (!sqlQuery.exec()) {
        qWarning() << "Ошибка выполнения запроса:" << sqlQuery.lastError().text();
        return result; // Возвращаем пустой список в случае ошибки
    }

    // Обрабатываем результат запроса
    while (sqlQuery.next()) {
        QString currency = sqlQuery.value("currency").toString(); // Получаем валюту
        double totalAmount = sqlQuery.value("total_amount").toDouble(); // Получаем сумму

        QVariantMap row;
        row["currency"] = currency;
        row["total_amount"] = totalAmount;
        result.append(row); // Добавляем в результат
    }

    return result;
}



QVariantList ReportOperations::getObjectsReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "  o.id AS ID, "
        "  o.name AS Object, "
        "  (SELECT COUNT(*) "
        "   FROM eraconstruction_sales s2 "
        "   WHERE s2.objectId = o.id "
        "   AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "') AS Sold, "
        "  (SELECT COUNT(*) "
        "   FROM eraconstruction_apartments a "
        "   WHERE a.objectId = o.id) - "
        "  (SELECT COUNT(*) "
        "   FROM eraconstruction_sales s2 "
        "   WHERE s2.objectId = o.id "
        "   AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "') AS Not_Sold, "
        "  (SELECT COUNT(*) "
        "   FROM eraconstruction_sales s2 "
        "   WHERE s2.objectId = o.id "
        "   AND s2.isInstallmentPlan = 1 "
        "   AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "') AS Installment, "
        "  (SELECT COUNT(*) "
        "   FROM eraconstruction_sales s2 "
        "   WHERE s2.objectId = o.id "
        "   AND s2.isInstallmentPlan = 0 "
        "   AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "') AS Full_Payment, "
        "  COALESCE(SUM(CASE WHEN s.currency = 'KGS' THEN s.totalAmount ELSE 0 END), 0) AS Amount_KGS, "
        "  COALESCE(SUM(CASE WHEN s.currency = 'USD' THEN s.totalAmount ELSE 0 END), 0) AS Amount_USD, "
        "  COALESCE(SUM(CASE WHEN s.currency = 'KGS' AND s.isInstallmentPlan = 1 THEN s.totalAmount - s.installment ELSE 0 END), 0) "
        "  - COALESCE((SELECT SUM(p.payment) "
        "             FROM eraconstruction_payments p "
        "             WHERE p.currency = 'KGS' "
        "             AND p.apartmentId IN (SELECT apartmentId FROM eraconstruction_sales s2 WHERE s2.objectId = o.id AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "')), 0) AS Debt_KGS, "
        "  COALESCE(SUM(CASE WHEN s.currency = 'USD' AND s.isInstallmentPlan = 1 THEN s.totalAmount - s.installment ELSE 0 END), 0) "
        "  - COALESCE((SELECT SUM(p.payment) "
        "             FROM eraconstruction_payments p "
        "             WHERE p.currency = 'USD' "
        "             AND p.apartmentId IN (SELECT apartmentId FROM eraconstruction_sales s2 WHERE s2.objectId = o.id AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "')), 0) AS Debt_USD, "
        "  COALESCE(SUM(CASE WHEN s.currency = 'KGS' AND s.isInstallmentPlan = 1 THEN s.installment WHEN s.currency = 'KGS' AND s.isInstallmentPlan = 0 THEN s.totalAmount ELSE 0 END), 0) "
        "  + COALESCE((SELECT SUM(p.payment) "
        "             FROM eraconstruction_payments p "
        "             WHERE p.currency = 'KGS' "
        "             AND p.apartmentId IN (SELECT apartmentId FROM eraconstruction_sales s2 WHERE s2.objectId = o.id AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "')), 0) AS INCOME_KGS, "
        "  COALESCE(SUM(CASE WHEN s.currency = 'USD' AND s.isInstallmentPlan = 1 THEN s.installment WHEN s.currency = 'USD' AND s.isInstallmentPlan = 0 THEN s.totalAmount ELSE 0 END), 0) "
        "  + COALESCE((SELECT SUM(p.payment) "
        "             FROM eraconstruction_payments p "
        "             WHERE p.currency = 'USD' "
        "             AND p.apartmentId IN (SELECT apartmentId FROM eraconstruction_sales s2 WHERE s2.objectId = o.id AND s2.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "')), 0) AS INCOME_USD "
        "FROM eraconstruction_objects o "
        "LEFT JOIN eraconstruction_sales s ON o.id = s.objectId "
        "WHERE s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "' "
        "GROUP BY o.id, o.name;";

    result = db.executeGet(query);
    return result;
}



QVariantList ReportOperations::getAllObjectsReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT "
        "   SUM(CASE WHEN s.apartmentId IS NOT NULL THEN 1 ELSE 0 END) AS Продано, "
        "    (SELECT COUNT(*) FROM eraconstruction_apartments a "
        "     WHERE a.objectId IN (SELECT DISTINCT objectId FROM eraconstruction_sales)) - COUNT(s.id) AS Не_Продано,\n"
        "   SUM(CASE WHEN s.isInstallmentPlan = 1 THEN 1 ELSE 0 END) AS Рассрочка, "
        "   SUM(CASE WHEN s.isInstallmentPlan = 0 THEN 1 ELSE 0 END) AS Полный_расчет, "
        "   COALESCE(SUM(CASE WHEN s.currency = 'KGS' THEN s.totalAmount ELSE 0 END), 0) AS Сумма_KGS, "
        "   COALESCE(SUM(CASE WHEN s.currency = 'USD' THEN s.totalAmount ELSE 0 END), 0) AS Сумма_USD, "
        "   COALESCE(SUM(CASE WHEN s.currency = 'KGS' THEN "
        "       (CASE "
        "           WHEN s.isInstallmentPlan = 0 THEN 0 "
        "           ELSE (s.totalAmount - s.installment - IFNULL((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "                WHERE p.objectId = s.objectId AND p.apartmentId = s.apartmentId AND p.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "' "
        "                AND p.date BETWEEN '%1' AND '%2'), 0)) "
        "       END) "
        "    ELSE 0 END), 0) AS Долг_KGS, "
        "   COALESCE(SUM(CASE WHEN s.currency = 'USD' THEN "
        "       (CASE "
        "           WHEN s.isInstallmentPlan = 0 THEN 0 "
        "           ELSE (s.totalAmount - s.installment - IFNULL((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "                WHERE p.objectId = s.objectId AND p.apartmentId = s.apartmentId AND p.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'"
        "                AND p.date BETWEEN '%1' AND '%2' AND p.currency = 'USD'), 0)) "
        "       END) "
        "    ELSE 0 END), 0) AS Долг_USD, "
        "   COALESCE(SUM(CASE WHEN s.currency = 'KGS' THEN "
        "       (CASE "
        "           WHEN s.isInstallmentPlan = 0 THEN s.totalAmount "
        "           ELSE s.installment + IFNULL((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "        WHERE p.objectId = s.objectId AND p.apartmentId = s.apartmentId "
        "        AND p.date BETWEEN '%1' AND '%2'), 0) "
        "       END) "
        "    ELSE 0 END), 0) AS Приход_KGS, "
        "   COALESCE(SUM(CASE WHEN s.currency = 'USD' THEN "
        "       (CASE "
        "           WHEN s.isInstallmentPlan = 0 THEN s.totalAmount "
        "           ELSE s.installment + IFNULL((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "        WHERE p.objectId = s.objectId AND p.apartmentId = s.apartmentId "
        "        AND p.date BETWEEN '%1' AND '%2' AND p.currency = 'USD'), 0) "
        "       END) "
        "    ELSE 0 END), 0) AS Приход_USD "
        "FROM eraconstruction_objects o "
        "JOIN eraconstruction_apartments a ON o.id = a.objectId "
        "LEFT JOIN eraconstruction_sales s ON a.id = s.apartmentId AND a.objectId = s.objectId "
        "WHERE (s.date BETWEEN '%1' AND '%2' OR s.apartmentId IS NULL);"
        ).arg(from.toString("yyyy-MM-dd"), to.toString("yyyy-MM-dd"));


    result = db.executeGet(query);
    return result;
}



QVariantList ReportOperations::getExpensesReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "    t.id AS ID, "
        "    CASE "
        "        WHEN t.name IS NULL THEN '-' "
        "        ELSE t.name "
        "    END AS TypeName, "
        "    SUM(CASE WHEN e.currency = 'KGS' THEN e.amount ELSE 0 END) AS AmountKGS, "
        "    SUM(CASE WHEN e.currency = 'USD' THEN e.amount ELSE 0 END) AS AmountUSD "
        "FROM eraconstruction_expenses e "
        "LEFT JOIN eraconstruction_types t ON e.typeId = t.id "
        "WHERE e.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "' "
                                                                                "GROUP BY e.typeId";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllExpensesReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "    SUM(CASE WHEN e.currency = 'KGS' THEN e.amount ELSE 0 END) AS TotalAmountKGS, "
        "    SUM(CASE WHEN e.currency = 'USD' THEN e.amount ELSE 0 END) AS TotalAmountUSD "
        "FROM eraconstruction_expenses e "
        "WHERE e.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'";

    result = db.executeGet(query);
    return result;
}


QVariantList ReportOperations::getClientsReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    c.id AS id,\n"
        "    COALESCE(c.name, '-') AS client,\n"
        "    s.currency AS currency,\n"
        "    SUM(s.totalAmount) AS total_sum,\n"
        "    SUM(\n"
        "        CASE\n"
        "            WHEN s.isInstallmentPlan = 1 THEN (s.installment + COALESCE((\n"
        "                SELECT SUM(p.payment) FROM eraconstruction_payments p\n"
        "                WHERE p.clientId = s.clientId AND p.apartmentId = s.apartmentId AND p.objectId = s.objectId\n"
        "            ), 0))\n"
        "            ELSE s.totalAmount\n"
        "        END\n"
        "    ) AS paid,\n"
        "    SUM(s.totalAmount) - SUM(\n"
        "        CASE\n"
        "            WHEN s.isInstallmentPlan = 1 THEN (s.installment + COALESCE((\n"
        "                SELECT SUM(p.payment) FROM eraconstruction_payments p\n"
        "                WHERE p.clientId = s.clientId AND p.apartmentId = s.apartmentId AND p.objectId = s.objectId\n"
        "            ), 0))\n"
        "            ELSE s.totalAmount\n"
        "        END\n"
        "    ) AS remaining\n"
        "FROM eraconstruction_sales s\n"
        "LEFT JOIN eraconstruction_clients c ON s.clientId = c.id\n"
        "WHERE s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'\n"
        "GROUP BY c.id, c.name, s.currency\n"
        "ORDER BY c.name ASC";

    result = db.executeGet(query);
    return result;
}



QVariantList ReportOperations::getAllClientsReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    SUM(CASE WHEN s.currency = 'KGS' THEN s.totalAmount ELSE 0 END) AS total_sum_KGS,\n"
        "    SUM(CASE WHEN s.currency = 'USD' THEN s.totalAmount ELSE 0 END) AS total_sum_USD,\n"
        "    SUM(CASE WHEN s.currency = 'KGS' THEN (CASE WHEN s.isInstallmentPlan = 1 THEN (s.installment + COALESCE((\n"
        "        SELECT SUM(p.payment) FROM eraconstruction_payments p\n"
        "        WHERE p.clientId = s.clientId AND p.apartmentId = s.apartmentId AND p.objectId = s.objectId\n"
        "    ), 0)) ELSE s.totalAmount END) ELSE 0 END) AS paid_KGS,\n"
        "    SUM(CASE WHEN s.currency = 'USD' THEN (CASE WHEN s.isInstallmentPlan = 1 THEN (s.installment + COALESCE((\n"
        "        SELECT SUM(p.payment) FROM eraconstruction_payments p\n"
        "        WHERE p.clientId = s.clientId AND p.apartmentId = s.apartmentId AND p.objectId = s.objectId\n"
        "    ), 0)) ELSE s.totalAmount END) ELSE 0 END) AS paid_USD,\n"
        "    SUM(CASE WHEN s.currency = 'KGS' THEN s.totalAmount ELSE 0 END) - SUM(CASE WHEN s.currency = 'KGS' THEN (CASE WHEN s.isInstallmentPlan = 1 THEN (s.installment + COALESCE((\n"
        "        SELECT SUM(p.payment) FROM eraconstruction_payments p\n"
        "        WHERE p.clientId = s.clientId AND p.apartmentId = s.apartmentId AND p.objectId = s.objectId\n"
        "    ), 0)) ELSE s.totalAmount END) ELSE 0 END) AS remaining_KGS,\n"
        "    SUM(CASE WHEN s.currency = 'USD' THEN s.totalAmount ELSE 0 END) - SUM(CASE WHEN s.currency = 'USD' THEN (CASE WHEN s.isInstallmentPlan = 1 THEN (s.installment + COALESCE((\n"
        "        SELECT SUM(p.payment) FROM eraconstruction_payments p\n"
        "        WHERE p.clientId = s.clientId AND p.apartmentId = s.apartmentId AND p.objectId = s.objectId\n"
        "    ), 0)) ELSE s.totalAmount END) ELSE 0 END) AS remaining_USD\n"
        "FROM eraconstruction_sales s\n"
        "WHERE s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'";

    result = db.executeGet(query);
    return result;
}





QVariantList ReportOperations::getApartmentsReport(int objectId, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT s.id, c.name AS client, o.name AS object, "
        "CASE WHEN a.apartmentNumber IS NULL THEN '-' ELSE a.apartmentNumber END AS apartment, "
        "s.apartmentCost, s.discount, s.totalAmount, s.currency, pt.name AS payment_type, "
        "CASE WHEN s.isInstallmentPlan = 1 THEN 'Рассрочка' ELSE 'Полный расчет' END AS status, "
        "(s.installment + COALESCE((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "WHERE p.clientId = s.clientId AND p.objectId = s.objectId AND p.apartmentId = s.apartmentId), 0)) AS paid, "
        "(s.totalAmount - (s.installment + COALESCE((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "WHERE p.clientId = s.clientId AND p.objectId = s.objectId AND p.apartmentId = s.apartmentId), 0))) AS remaining, "
        "s.monthlyPayment, "
        "(COALESCE((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "WHERE p.clientId = s.clientId AND p.objectId = s.objectId AND p.apartmentId = s.apartmentId), 0)) AS total_paid, "
        "LEAST(TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY), CURDATE()), s.monthQuantity) AS months_since_purchase "
        "FROM eraconstruction_sales s "
        "LEFT JOIN eraconstruction_clients c ON s.clientId = c.id "
        "LEFT JOIN eraconstruction_objects o ON s.objectId = o.id "
        "LEFT JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
        "LEFT JOIN eraconstruction_paymentTypes pt ON s.paymentTypeId = pt.id ";

    if (objectId > 0) {
        query += "WHERE s.objectId = " + QString::number(objectId) +
                 " AND s.date BETWEEN DATE('" + from.toString("yyyy-MM-dd") +
                 "') AND DATE('" + to.toString("yyyy-MM-dd") + "') ";
    } else {
        query += "WHERE s.date BETWEEN DATE('" + from.toString("yyyy-MM-dd") +
                 "') AND DATE('" + to.toString("yyyy-MM-dd") + "') ";
    }

    query += "ORDER BY c.name, o.name, a.apartmentNumber";
    result = db.executeGet(query);
    return result;
}


QVariantMap ReportOperations::getAllApartmentsReport(int objectId, QDate from, QDate to) {
    QVariantMap result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "    SUM(CASE WHEN s.currency = 'KGS' THEN s.discount ELSE 0 END) AS totalDiscountKGS, "
        "    SUM(CASE WHEN s.currency = 'USD' THEN s.discount ELSE 0 END) AS totalDiscountUSD, "
        "    SUM(CASE WHEN s.currency = 'KGS' THEN s.totalAmount ELSE 0 END) AS totalAmountKGS, "
        "    SUM(CASE WHEN s.currency = 'USD' THEN s.totalAmount ELSE 0 END) AS totalAmountUSD, "
        "    SUM(CASE WHEN s.currency = 'KGS' AND s.isInstallmentPlan = true THEN (COALESCE(p.totalPayment, 0) + COALESCE(s.installment, 0)) "
        "             WHEN s.currency = 'KGS' AND s.isInstallmentPlan = false THEN s.totalAmount ELSE 0 END) AS totalPaidKGS, "
        "    SUM(CASE WHEN s.currency = 'USD' AND s.isInstallmentPlan = true THEN (COALESCE(p.totalPayment, 0) + COALESCE(s.installment, 0)) "
        "             WHEN s.currency = 'USD' AND s.isInstallmentPlan = false THEN s.totalAmount ELSE 0 END) AS totalPaidUSD, "
        "    SUM(CASE WHEN s.isInstallmentPlan = true AND s.currency = 'KGS' THEN s.totalAmount ELSE 0 END) "
        "      - SUM(CASE WHEN s.isInstallmentPlan = true AND s.currency = 'KGS' THEN (COALESCE(p.totalPayment, 0) + COALESCE(s.installment, 0)) ELSE 0 END) "
        "      AS totalRemainingKGS, "
        "    SUM(CASE WHEN s.isInstallmentPlan = true AND s.currency = 'USD' THEN s.totalAmount ELSE 0 END) "
        "      - SUM(CASE WHEN s.isInstallmentPlan = true AND s.currency = 'USD' THEN (COALESCE(p.totalPayment, 0) + COALESCE(s.installment, 0)) ELSE 0 END) "
        "      AS totalRemainingUSD "
        "FROM eraconstruction_sales s "
        "LEFT JOIN ( "
        "    SELECT clientId, apartmentId, currency, SUM(payment) AS totalPayment "
        "    FROM eraconstruction_payments "
        "    GROUP BY clientId, apartmentId, currency "
        ") p ON s.clientId = p.clientId AND s.apartmentId = p.apartmentId AND s.currency = p.currency "
        "WHERE s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'";

    // Добавляем фильтрацию по объекту, если он выбран
    if (objectId > 0) {
        query += " AND s.objectId = " + QString::number(objectId);
    }

    QVariantList report = db.executeGet(query);

    if (report.isEmpty()) {
        return result;
    }

    QVariantList row = report.first().toList();

    result["totalDiscount_KGS"] = row[0].toDouble();
    result["totalDiscount_USD"] = row[1].toDouble();
    result["totalAmount_KGS"] = row[2].toDouble();
    result["totalAmount_USD"] = row[3].toDouble();
    result["totalPaid_KGS"] = row[4].toDouble();
    result["totalPaid_USD"] = row[5].toDouble();
    result["totalRemaining_KGS"] = row[6].toDouble();
    result["totalRemaining_USD"] = row[7].toDouble();

    return result;
}





QVariantList ReportOperations::getDebtsReport() {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT "
        "    sale_id, "
        "    client, "
        "    apartment, "
        "    object, "
        "    currency, "
        "    GREATEST( "
        "        LEAST( "
        "            monthsPassed, "
        "            monthQuantity "
        "        ) - LEAST( "
        "            monthQuantity, "
        "            CASE "
        "                WHEN isInstallmentPlan = 0 THEN monthsPassed "
        "                WHEN monthlyPayment > 0 THEN "
        "                    CASE "
        "                        WHEN LEAST(monthsPassed, monthQuantity) = monthQuantity THEN "
        "                            CASE "
        "                                WHEN COALESCE(total_paid, 0) / monthlyPayment - FLOOR(COALESCE(total_paid, 0) / monthlyPayment) >= 0.999 "
        "                                     AND (total_paid = totalAmount - installment OR total_paid = 0) "
        "                                    THEN CEIL(COALESCE(total_paid, 0) / monthlyPayment) "
        "                                ELSE FLOOR(COALESCE(total_paid, 0) / monthlyPayment) "
        "                            END "
        "                        ELSE FLOOR(COALESCE(total_paid, 0) / monthlyPayment) "
        "                    END "
        "                ELSE 0 "
        "            END "
        "        ), "
        "        0 "
        "    ) AS totalUnpaidMonths, "
        "    GREATEST( "
        "        CASE "
        "            WHEN LEAST(monthsPassed, monthQuantity) = monthQuantity THEN "
        "                totalAmount - (COALESCE(total_paid, 0) + installment) "
        "            ELSE "
        "                LEAST(monthsPassed, monthQuantity) * monthlyPayment - COALESCE(total_paid, 0) "
        "        END, "
        "        0 "
        "    ) AS debt "
        "FROM ( "
        "    SELECT "
        "        s.id AS sale_id, "
        "        COALESCE(c.name, '-') AS client, "
        "        a.apartmentNumber AS apartment, "
        "        o.name AS object, "
        "        s.currency AS currency, "
        "        COALESCE(p.total_paid, 0) AS total_paid, " // заменяем NULL на 0
        "        s.monthQuantity, "
        "        s.totalAmount, "
        "        s.monthlyPayment, "
        "        s.installment, "
        "        s.isInstallmentPlan, "
        "        TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY), CURDATE()) AS monthsPassed "
        "    FROM eraconstruction_sales s "
        "    LEFT JOIN eraconstruction_clients c ON s.clientId = c.id "
        "    LEFT JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
        "    LEFT JOIN eraconstruction_objects o ON s.objectId = o.id "
        "    LEFT JOIN ( "
        "        SELECT "
        "            clientId, "
        "            apartmentId, "
        "            objectId, "
        "            COALESCE(SUM(payment), 0) AS total_paid "
        "        FROM eraconstruction_payments "
        "        GROUP BY clientId, apartmentId, objectId "
        "    ) p ON s.clientId = p.clientId "
        "       AND s.apartmentId = p.apartmentId "
        "       AND s.objectId = p.objectId "
        "    WHERE s.isInstallmentPlan = 1 "
        ") AS subquery "
        "ORDER BY totalUnpaidMonths DESC;"
        );

    result = db.executeGet(query);
    qDebug() << result;
    return result;
}




QVariantList ReportOperations::getAllDebtsReport() {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    // Берём всю логику из getDebtsReport (убираем только ORDER BY),
    // и в верхнем SELECT суммируем поле debt по валюте
    QString query = QString(
        "SELECT "
        "    SUM(CASE WHEN sub.currency = 'KGS' THEN sub.debt ELSE 0 END) AS total_debt_KGS, "
        "    SUM(CASE WHEN sub.currency = 'USD' THEN sub.debt ELSE 0 END) AS total_debt_USD "
        "FROM ( "
        "    SELECT "
        "        s.id AS sale_id, "
        "        c.name AS client, "
        "        a.apartmentNumber AS apartment, "
        "        o.name AS object, "
        "        s.currency AS currency, "
        "        COALESCE(p.total_paid, 0) AS total_paid, " // заменяем NULL на 0
        "        s.monthQuantity, "
        "        s.totalAmount, "
        "        s.monthlyPayment, "
        "        s.installment, "
        "        s.isInstallmentPlan, "
        "        TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY), CURDATE()) AS monthsPassed, "
        "        GREATEST( "
        "            CASE "
        "                WHEN LEAST(TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY), CURDATE()), s.monthQuantity) = s.monthQuantity THEN "
        "                    s.totalAmount - (COALESCE(p.total_paid, 0) + s.installment) "
        "                ELSE "
        "                    LEAST(TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY), CURDATE()), s.monthQuantity) * s.monthlyPayment "
        "                    - COALESCE(p.total_paid, 0) "
        "            END, "
        "            0 "
        "        ) AS debt "
        "    FROM eraconstruction_sales s "
        "    LEFT JOIN eraconstruction_clients c ON s.clientId = c.id "
        "    LEFT JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
        "    LEFT JOIN eraconstruction_objects o ON s.objectId = o.id "
        "    LEFT JOIN ( "
        "        SELECT "
        "            clientId, "
        "            apartmentId, "
        "            objectId, "
        "            COALESCE(SUM(payment), 0) AS total_paid "
        "        FROM eraconstruction_payments "
        "        GROUP BY clientId, apartmentId, objectId "
        "    ) p ON s.clientId = p.clientId "
        "       AND s.apartmentId = p.apartmentId "
        "       AND s.objectId = p.objectId "
        "    WHERE s.isInstallmentPlan = 1 "
        ") AS sub "
        );

    result = db.executeGet(query);
    return result;
}









QVariantList ReportOperations::getTotalReport(QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    const QString dateFrom = from.toString("yyyy-MM-dd");
    const QString dateTo   = to.toString("yyyy-MM-dd");

    QString query =
        // 1. Типы оплат по paymentType
        // "SELECT\n"
        // "    pt.name AS Description,\n"
        // "    COALESCE(SUM(t.amount), 0) AS Amount,\n"
        // "    1 AS SortOrder\n"
        // "FROM (\n"
        // "    SELECT\n"
        // "        p.paymentTypeId AS paymentTypeId,\n"
        // "        p.payment AS amount\n"
        // "    FROM eraconstruction_payments p\n"
        // "    WHERE p.date <= '" + dateTo + "'\n"
        // "    UNION ALL\n"
        // "    SELECT\n"
        // "        e.paymentTypeId AS paymentTypeId,\n"
        // "        CASE\n"
        // "            WHEN e.isInstallmentPlan = false THEN e.totalAmount\n"
        // "            ELSE e.installment\n"
        // "        END AS amount\n"
        // "    FROM eraconstruction_sales e\n"
        // "    WHERE e.date <= '" + dateTo + "'\n"
        // "    UNION ALL\n"
        // "    SELECT\n"
        // "        ex.paymentTypeId AS paymentTypeId,\n"
        // "        -ex.amount AS amount\n"
        // "    FROM eraconstruction_expenses ex\n"
        // "    WHERE ex.date <= '" + dateTo + "'\n"
        // ") t\n"
        // "JOIN eraconstruction_paymentTypes pt ON t.paymentTypeId = pt.id\n"
        // "WHERE t.paymentTypeId IS NOT NULL\n"
        // "GROUP BY pt.name\n"



        // 1. Типы оплат по paymentType для KGS
        "SELECT\n"
        "    CONCAT(pt.name, ' KGS:') AS Description,\n"
        "    COALESCE(SUM(t.amount), 0) AS Amount,\n"
        "    1 AS SortOrder\n"
        "FROM (\n"
        "    SELECT\n"
        "        p.paymentTypeId AS paymentTypeId,\n"
        "        p.payment AS amount,\n"
        "        p.currency AS currency\n"
        "    FROM eraconstruction_payments p\n"
        "    WHERE p.date <= '" + dateTo + "' AND p.currency = 'KGS'\n"
        "    UNION ALL\n"
        "    SELECT\n"
        "        e.paymentTypeId AS paymentTypeId,\n"
        "        CASE WHEN e.isInstallmentPlan = false THEN e.totalAmount ELSE e.installment END AS amount,\n"
        "        e.currency AS currency\n"
        "    FROM eraconstruction_sales e\n"
        "    WHERE e.date <= '" + dateTo + "' AND e.currency = 'KGS'\n"
        "    UNION ALL\n"
        "    SELECT\n"
        "        ex.paymentTypeId AS paymentTypeId,\n"
        "        -ex.amount AS amount,\n"
        "        ex.currency AS currency\n"
        "    FROM eraconstruction_expenses ex\n"
        "    WHERE ex.date <= '" + dateTo + "' AND ex.currency = 'KGS'\n"
        ") t\n"
        "JOIN eraconstruction_paymentTypes pt ON t.paymentTypeId = pt.id\n"
        "WHERE t.paymentTypeId IS NOT NULL\n"
        "GROUP BY pt.name\n"

        "UNION ALL\n"

        // 1. Типы оплат по paymentType для USD
        "SELECT\n"
        "    CONCAT(pt.name, ' USD:') AS Description,\n"
        "    COALESCE(SUM(t.amount), 0) AS Amount,\n"
        "    1 AS SortOrder\n"
        "FROM (\n"
        "    SELECT\n"
        "        p.paymentTypeId AS paymentTypeId,\n"
        "        p.payment AS amount,\n"
        "        p.currency AS currency\n"
        "    FROM eraconstruction_payments p\n"
        "    WHERE p.date <= '" + dateTo + "' AND p.currency = 'USD'\n"
        "    UNION ALL\n"
        "    SELECT\n"
        "        e.paymentTypeId AS paymentTypeId,\n"
        "        CASE WHEN e.isInstallmentPlan = false THEN e.totalAmount ELSE e.installment END AS amount,\n"
        "        e.currency AS currency\n"
        "    FROM eraconstruction_sales e\n"
        "    WHERE e.date <= '" + dateTo + "' AND e.currency = 'USD'\n"
        "    UNION ALL\n"
        "    SELECT\n"
        "        ex.paymentTypeId AS paymentTypeId,\n"
        "        -ex.amount AS amount,\n"
        "        ex.currency AS currency\n"
        "    FROM eraconstruction_expenses ex\n"
        "    WHERE ex.date <= '" + dateTo + "' AND ex.currency = 'USD'\n"
        ") t\n"
        "JOIN eraconstruction_paymentTypes pt ON t.paymentTypeId = pt.id\n"
        "WHERE t.paymentTypeId IS NOT NULL\n"
        "GROUP BY pt.name\n"



        "UNION ALL\n"
        // 2. Разделитель (пустая строка)
        "SELECT '' AS Description, '' AS Amount, 2 AS SortOrder\n"

        "UNION ALL\n"
        // 3. Общий приход KGS
        "SELECT\n"
        "    'Общий приход KGS:' AS Description,\n"
        "    (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN s.currency = 'KGS' THEN\n"
        "                CASE WHEN s.isInstallmentPlan = false THEN s.totalAmount ELSE s.installment END\n"
        "            ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_sales s\n"
        "        WHERE s.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    )\n"
        "    + (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN p.currency = 'KGS' THEN p.payment ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_payments p\n"
        "        WHERE p.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    ) AS Amount,\n"
        "    3 AS SortOrder\n"

        "UNION ALL\n"
        // 4. Общий расход KGS
        "SELECT\n"
        "    'Общий расход KGS:' AS Description,\n"
        "    -COALESCE((\n"
        "        SELECT SUM(\n"
        "            CASE WHEN ex.currency = 'KGS' THEN ex.amount ELSE 0 END\n"
        "        )\n"
        "        FROM eraconstruction_expenses ex\n"
        "        WHERE ex.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    ), 0) AS Amount,\n"
        "    4 AS SortOrder\n"

        "UNION ALL\n"
        // 5. ИТОГО KGS
        "SELECT\n"
        "    'Итого KGS:' AS Description,\n"
        "    (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN s.currency = 'KGS' THEN\n"
        "                CASE WHEN s.isInstallmentPlan = false THEN s.totalAmount ELSE s.installment END\n"
        "            ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_sales s\n"
        "        WHERE s.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    )\n"
        "    + (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN p.currency = 'KGS' THEN p.payment ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_payments p\n"
        "        WHERE p.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    )\n"
        "    - (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN ex.currency = 'KGS' THEN ex.amount ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_expenses ex\n"
        "        WHERE ex.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    ) AS Amount,\n"
        "    5 AS SortOrder\n"

        "UNION ALL\n"
        // 6. Разделитель (пустая строка)
        "SELECT '' AS Description, '' AS Amount, 6 AS SortOrder\n"

        "UNION ALL\n"
        // 7. Общий приход USD
        "SELECT\n"
        "    'Общий приход USD:' AS Description,\n"
        "    (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN s.currency = 'USD' THEN\n"
        "                CASE WHEN s.isInstallmentPlan = false THEN s.totalAmount ELSE s.installment END\n"
        "            ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_sales s\n"
        "        WHERE s.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    )\n"
        "    + (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN p.currency = 'USD' THEN p.payment ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_payments p\n"
        "        WHERE p.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    ) AS Amount,\n"
        "    7 AS SortOrder\n"

        "UNION ALL\n"
        // 8. Общий расход USD
        "SELECT\n"
        "    'Общий расход USD:' AS Description,\n"
        "    -COALESCE((\n"
        "        SELECT SUM(\n"
        "            CASE WHEN ex.currency = 'USD' THEN ex.amount ELSE 0 END\n"
        "        )\n"
        "        FROM eraconstruction_expenses ex\n"
        "        WHERE ex.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    ), 0) AS Amount,\n"
        "    8 AS SortOrder\n"

        "UNION ALL\n"
        // 9. ИТОГО USD
        "SELECT\n"
        "    'Итого USD:' AS Description,\n"
        "    (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN s.currency = 'USD' THEN\n"
        "                CASE WHEN s.isInstallmentPlan = false THEN s.totalAmount ELSE s.installment END\n"
        "            ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_sales s\n"
        "        WHERE s.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    )\n"
        "    + (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN p.currency = 'USD' THEN p.payment ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_payments p\n"
        "        WHERE p.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    )\n"
        "    - (\n"
        "        SELECT COALESCE(SUM(\n"
        "            CASE WHEN ex.currency = 'USD' THEN ex.amount ELSE 0 END\n"
        "        ), 0)\n"
        "        FROM eraconstruction_expenses ex\n"
        "        WHERE ex.date BETWEEN '" + dateFrom + "' AND '" + dateTo + "'\n"
        "    ) AS Amount,\n"
        "    9 AS SortOrder\n"

        "ORDER BY SortOrder;";

    result = db.executeGet(query);
    return result;
}










// ================= reports by item =================
QVariantList ReportOperations::getPaymentTypesReportByItem(int paymentTypeId, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT\n"
        "    t.id AS id,\n"
        "    t.date AS date,\n"
        "    pt.name AS payment_type,\n"
        "    t.amount AS amount,\n"
        "    t.source AS source,\n"
        "    t.description AS description,\n"
        "    t.currency AS currency\n"
        "FROM (\n"
        "    -- Платежи\n"
        "    SELECT\n"
        "        p.id AS id,\n"
        "        p.date AS date,\n"
        "        p.paymentTypeId AS paymentTypeId,\n"
        "        p.payment AS amount,\n"
        "        CONCAT(c.name, ' (еж. платеж)') AS source,\n"
        "        p.description AS description,\n"
        "        p.currency AS currency\n"
        "    FROM eraconstruction_payments p\n"
        "    LEFT JOIN eraconstruction_clients c ON p.clientId = c.id\n"
        "\n"
        "    UNION ALL\n"
        "\n"
        "    -- Продажи\n"
        "    SELECT\n"
        "        s.id AS id,\n"
        "        s.date AS date,\n"
        "        s.paymentTypeId AS paymentTypeId,\n"
        "        CASE\n"
        "            WHEN s.isInstallmentPlan = 1 THEN s.installment  -- Рассрочка: берем первоначальный взнос\n"
        "            ELSE s.totalAmount  -- Без рассрочки: берем полную сумму\n"
        "        END AS amount,\n"
        "        CONCAT(c.name, ' (', CASE WHEN s.isInstallmentPlan = 1 THEN 'первоначальный взнос' ELSE 'полный расчет' END, ')') AS source,\n"
        "        s.description AS description,\n"
        "        s.currency AS currency\n"
        "    FROM eraconstruction_sales s\n"
        "    LEFT JOIN eraconstruction_clients c ON s.clientId = c.id\n"
        "\n"
        "    UNION ALL\n"
        "\n"
        "    -- Расходы\n"
        "    SELECT\n"
        "        e.id AS id,\n"
        "        e.date AS date,\n"
        "        e.paymentTypeId AS paymentTypeId,\n"
        "        -e.amount AS amount,\n"
        "        t.name AS source,\n"
        "        e.description AS description,\n"
        "        e.currency AS currency\n"
        "    FROM eraconstruction_expenses e\n"
        "    LEFT JOIN eraconstruction_types t ON e.typeId = t.id\n"
        ") t\n"
        "LEFT JOIN eraconstruction_paymentTypes pt ON t.paymentTypeId = pt.id\n"
        "WHERE t.paymentTypeId = " + QString::number(paymentTypeId) + "\n"
        "AND t.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'\n"
        "ORDER BY t.date";

    result = db.executeGet(query);
    qDebug() << result;
    return result;
}




QVariantList ReportOperations::getAllPaymentTypesReportByItem(int paymentTypeId, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query = QString(
                        "SELECT "
                        "   -- Сумма по KGS\n"
                        "   (SELECT SUM(t.amount) AS total_amount\n"
                        "    FROM (\n"
                        "        -- Платежи (прямые платежи от клиентов)\n"
                        "        SELECT p.payment AS amount\n"
                        "        FROM eraconstruction_payments p\n"
                        "        WHERE p.paymentTypeId = %1\n"
                        "        AND p.date BETWEEN '%2' AND '%3'\n"
                        "        AND p.currency = 'KGS'\n"
                        "        UNION ALL\n"
                        "        -- Продажи (берем всю сумму, если нет рассрочки, и только первый взнос, если есть)\n"
                        "        SELECT\n"
                        "            CASE\n"
                        "                WHEN s.isInstallmentPlan = 1 THEN s.installment  -- Рассрочка: только первоначальный взнос\n"
                        "                ELSE s.totalAmount  -- Без рассрочки: вся сумма\n"
                        "            END AS amount\n"
                        "        FROM eraconstruction_sales s\n"
                        "        WHERE s.paymentTypeId = %1\n"
                        "        AND s.date BETWEEN '%2' AND '%3'\n"
                        "        AND s.currency = 'KGS'\n"
                        "        UNION ALL\n"
                        "        -- Расходы (отрицательная сумма)\n"
                        "        SELECT -e.amount AS amount\n"
                        "        FROM eraconstruction_expenses e\n"
                        "        WHERE e.paymentTypeId = %1\n"
                        "        AND e.date BETWEEN '%2' AND '%3'\n"
                        "        AND e.currency = 'KGS'\n"
                        "    ) t) AS total_KGS,\n"

                        "   -- Сумма по USD\n"
                        "   (SELECT SUM(t.amount) AS total_amount\n"
                        "    FROM (\n"
                        "        -- Платежи (прямые платежи от клиентов)\n"
                        "        SELECT p.payment AS amount\n"
                        "        FROM eraconstruction_payments p\n"
                        "        WHERE p.paymentTypeId = %1\n"
                        "        AND p.date BETWEEN '%2' AND '%3'\n"
                        "        AND p.currency = 'USD'\n"
                        "        UNION ALL\n"
                        "        -- Продажи (берем всю сумму, если нет рассрочки, и только первый взнос, если есть)\n"
                        "        SELECT\n"
                        "            CASE\n"
                        "                WHEN s.isInstallmentPlan = 1 THEN s.installment  -- Рассрочка: только первоначальный взнос\n"
                        "                ELSE s.totalAmount  -- Без рассрочки: вся сумма\n"
                        "            END AS amount\n"
                        "        FROM eraconstruction_sales s\n"
                        "        WHERE s.paymentTypeId = %1\n"
                        "        AND s.date BETWEEN '%2' AND '%3'\n"
                        "        AND s.currency = 'USD'\n"
                        "        UNION ALL\n"
                        "        -- Расходы (отрицательная сумма)\n"
                        "        SELECT -e.amount AS amount\n"
                        "        FROM eraconstruction_expenses e\n"
                        "        WHERE e.paymentTypeId = %1\n"
                        "        AND e.date BETWEEN '%2' AND '%3'\n"
                        "        AND e.currency = 'USD'\n"
                        "    ) t) AS total_USD"
                        ).arg(paymentTypeId).arg(from.toString("yyyy-MM-dd")).arg(to.toString("yyyy-MM-dd"));

    result = db.executeGet(query);
    qDebug() << result;
    return result;
}






QVariantList ReportOperations::getObjectsReportByItem(int id, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();
    QString query =
        "SELECT "
        "    COALESCE(c.name, '-') AS Клиент,\n"
        "    COALESCE(a.apartmentNumber, '-'), "
        "    s.apartmentCost, "
        "    s.discount, "
        "    (s.apartmentCost - s.discount) AS Итог, "
        "    CASE WHEN s.isInstallmentPlan = 1 THEN 'Рассрочка' ELSE 'Полный расчет' END AS Статус, "
        "    CASE "
        "        WHEN s.isInstallmentPlan = 0 THEN (s.apartmentCost - s.discount) "
        "        ELSE (s.installment + "
        "              COALESCE((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "                       WHERE p.apartmentId = s.apartmentId AND p.objectId = s.objectId "
        "                       AND p.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' "
                                        "                       AND '" + to.toString("yyyy-MM-dd") + "'), 0)) "
                                      "    END AS Приход, "
                                      "    a.ploshad, "
                                      "    a.priceSquare, "
                                      "    a.roomsQuantity, "
                                      "    a.sanuzelsQuantity, "
                                      "    a.floor, "
                                      "    a.currency "
                                      "FROM eraconstruction_sales s "
                                      "JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
                                      "JOIN eraconstruction_clients c ON s.clientId = c.id "
                                      "WHERE s.objectId = " + QString::number(id) + " "
                                "AND s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "';";

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getAllObjectsReportByItem(int id, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "    s.currency, "
        "    COALESCE(SUM(s.apartmentCost), 0) AS Общая_стоимость, "
        "    COALESCE(SUM(s.discount), 0) AS Общая_скидка, "
        "    COALESCE(SUM(s.apartmentCost - s.discount), 0) AS Итог, "
        "    COALESCE(SUM(CASE "
        "        WHEN s.isInstallmentPlan = 0 THEN (s.apartmentCost - s.discount) "
        "        ELSE (s.installment + "
        "              COALESCE((SELECT SUM(p.payment) FROM eraconstruction_payments p "
        "                       WHERE p.objectId = s.objectId "
        "                       AND p.apartmentId = s.apartmentId "
        "                       AND p.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' "
                                        "                       AND '" + to.toString("yyyy-MM-dd") + "'), 0)) "
                                      "    END), 0) AS Общий_приход "
                                      "FROM eraconstruction_sales s "
                                      "WHERE s.objectId = " + QString::number(id) + " "
                                "AND s.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "' "
                                                                                "GROUP BY s.currency;"; // Группируем по валюте

    result = db.executeGet(query);
    return result;
}



QVariantList ReportOperations::getExpensesReportByItem(int id, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "    e.id AS id, "
        "    e.date AS date, "
        "    COALESCE(t.name, '-') AS type, "
        "    COALESCE(w.name, '-') AS who_what, "
        "    pt.name AS payment_type, "
        "    SUM(CASE WHEN e.currency = 'KGS' THEN e.amount ELSE 0 END) AS amountKGS, "
        "    SUM(CASE WHEN e.currency = 'USD' THEN e.amount ELSE 0 END) AS amountUSD, "
        "    e.description AS description "
        "FROM eraconstruction_expenses e "
        "LEFT JOIN eraconstruction_types t ON e.typeId = t.id "
        "LEFT JOIN eraconstruction_whoWhat w ON e.whoWhatId = w.id "
        "LEFT JOIN eraconstruction_paymentTypes pt ON e.paymentTypeId = pt.id "
        "WHERE e.typeId = " + QString::number(id) + " "
                                "AND e.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "' "
                                                                                "GROUP BY e.id, e.date, t.name, w.name, pt.name, e.description "
                                                                                "ORDER BY e.date";


    result = db.executeGet(query);
    return result;

}

QVariantList ReportOperations::getAllExpensesReportByItem(int id, QDate from, QDate to) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query =
        "SELECT "
        "    SUM(CASE WHEN e.currency = 'KGS' THEN e.amount ELSE 0 END) AS totalAmountKGS, "
        "    SUM(CASE WHEN e.currency = 'USD' THEN e.amount ELSE 0 END) AS totalAmountUSD "
        "FROM eraconstruction_expenses e "
                    "WHERE e.typeId = " + QString::number(id) + " "
                                            "AND e.date BETWEEN '" + from.toString("yyyy-MM-dd") + "' AND '" + to.toString("yyyy-MM-dd") + "'";

    result = db.executeGet(query);
    return result;
}



QVariantList ReportOperations::getClientsReportByItem(int id, QDate from, QDate to) {

}

QVariantList ReportOperations::getAllClientsReportByItem(int id, QDate from, QDate to) {

}

QVariantList ReportOperations::getApartmentPayments(int id) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString salesQuery = QString(
                             "SELECT clientId, objectId, apartmentId FROM eraconstruction_sales WHERE id = %1"
                             ).arg(id);

    QVariantList salesData = db.executeGet(salesQuery);

    if (salesData.isEmpty()) {
        qDebug() << "No sales data found for id:" << id;
        return result;  // Возвращаем пустой список, если данных нет
    }

    QVariantList salesRecord = salesData.first().toList();
    int clientId = salesRecord[0].toInt();
    int objectId = salesRecord[1].toInt();
    int apartmentId = salesRecord[2].toInt();

    QString paymentQuery =
        "SELECT "
        "    p.date AS date, "
        "    pt.name AS paymentType, "
        "    p.payment AS amount, "
        "    p.clientId AS clientId, "
        "    p.description AS description "
        "FROM eraconstruction_payments p "
        "LEFT JOIN eraconstruction_paymentTypes pt ON p.paymentTypeId = pt.id "
        "WHERE p.clientId = " + QString::number(clientId) + " "
        "AND p.objectId = " + QString::number(objectId) + " "
        "AND p.apartmentId = " + QString::number(apartmentId) + " "
        "ORDER BY p.date ASC";

    result = db.executeGet(paymentQuery);
    return result;
}

QVariantList ReportOperations::getApartmentAllPayments(int id) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString salesQuery = QString(
        "SELECT clientId, objectId, apartmentId FROM eraconstruction_sales WHERE id = %1"
        ).arg(id);

    QVariantList salesData = db.executeGet(salesQuery);

    if (salesData.isEmpty()) {
        qDebug() << "No sales data found for id:" << id;
        return result;  // Возвращаем пустой список, если данных нет
    }

    QVariantList salesRecord = salesData.first().toList();
    int clientId = salesRecord[0].toInt();
    int objectId = salesRecord[1].toInt();
    int apartmentId = salesRecord[2].toInt();

    QString paymentSumQuery = QString(
        "SELECT SUM(p.payment) AS totalAmount "
        "FROM eraconstruction_payments p "
        "WHERE p.clientId = %1 AND p.objectId = %2 AND p.apartmentId = %3"
        ).arg(clientId).arg(objectId).arg(apartmentId);

    result = db.executeGet(paymentSumQuery);
    return result;
}

QVariantList ReportOperations::getApartmentInformation(int id) {
    QVariantList result;
    dbManager &db = dbManager::getInstance();

    QString query = QString(
                        "SELECT c.name AS client, o.name AS object, a.roomsQuantity, a.sanuzelsQuantity, "
                        "a.ploshad, a.floor, a.priceSquare, s.totalAmount, s.currency "
                        "FROM eraconstruction_sales s "
                        "LEFT JOIN eraconstruction_clients c ON s.clientId = c.id "
                        "LEFT JOIN eraconstruction_objects o ON s.objectId = o.id "
                        "LEFT JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
                        "WHERE s.id = %1"
                        ).arg(id);

    result = db.executeGet(query);
    return result;
}

QVariantList ReportOperations::getClientInformation(int salesId) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(R"(
    WITH PaymentSummary AS (
        SELECT
            s.id AS saleId,
            s.totalAmount,
            s.installment AS initialPayment,
            s.monthlyPayment,
            s.currency,
            LEAST(TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY),  CURDATE()) + IF(DAY(CURDATE()) >= DAY(s.date), 0, -1), s.monthQuantity) AS monthsPassed,
            LEAST(s.monthQuantity, TIMESTAMPDIFF(MONTH, DATE_ADD(s.date, INTERVAL 1 DAY),  CURDATE())) AS months,
            s.monthQuantity,  -- Добавляем monthQuantity сюда, чтобы использовать его в главном запросе
            COALESCE(SUM(p.payment), 0) AS totalPaid
        FROM eraconstruction_sales s
        LEFT JOIN eraconstruction_payments p
            ON s.clientId = p.clientId
            AND s.apartmentId = p.apartmentId
        WHERE s.id = %1
        GROUP BY s.id
    )
    SELECT
        totalAmount,
        (totalAmount - (totalPaid + initialPayment)) AS remaining,
        (totalPaid + initialPayment) AS paid,
        monthlyPayment,
    GREATEST(0,
    CASE
        WHEN totalPaid < totalAmount - initialPayment THEN
            -- Недоплаченные месяцы: рассчитываем, сколько еще нужно заплатить
            FLOOR(totalPaid / ROUND(monthlyPayment,5))
        ELSE
            0
    END,
    CASE
        WHEN totalAmount - initialPayment = totalPaid THEN
            GREATEST(0,monthQuantity)
        ELSE
            0
    END
) AS paidMonths,  -- Теперь считает только по платежам
GREATEST(0,
    CASE
        WHEN totalPaid < totalAmount - initialPayment THEN
            -- Недоплаченные месяцы: рассчитываем, сколько еще нужно заплатить
            CEIL(months - FLOOR(totalPaid / ROUND(monthlyPayment,5)))
        ELSE
            0
    END,
    CASE
        WHEN (totalAmount - (totalPaid + initialPayment)) > 0 AND (months * ROUND(monthlyPayment,5)) > totalPaid  AND months = monthsPassed THEN
            -- Когда полная сумма уплачена, но остаток еще не погашен
            1
        ELSE
            0
    END
) AS missedMonths,

GREATEST(0,CASE
        WHEN months = monthQuantity THEN
             (totalAmount - (totalPaid + initialPayment))  -- Остаток равен задолженности
        ELSE
             ROUND((months * monthlyPayment) - totalPaid,5)
    END )AS arrears,
GREATEST(0,CASE
        WHEN totalAmount <= (totalPaid +initialPayment) THEN 1
        ELSE 0
    END
) AS isEnd,
monthQuantity as count,
currency as cur



    FROM PaymentSummary
)").arg(salesId);


    return db.executeGet(query);
}

QVariantList ReportOperations::getClientPayments(int saleId) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(R"(
        SELECT s.date AS sale_month,
               s.monthlyPayment AS monthly_payment,
               p.date AS payment_date,
               IFNULL(p.payment, 0) AS payment
        FROM eraconstruction_sales s
        LEFT JOIN eraconstruction_payments p
            ON s.apartmentId = p.apartmentId
            AND s.objectId = p.objectId
            AND s.clientId = p.clientId  -- Добавлен фильтр по clientId
        WHERE s.id = %1
        ORDER BY sale_month, p.date;
    )").arg(saleId);
    return db.executeGet(query);
}

QVariantList ReportOperations::getClientApartments(int clientId) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(R"(
        SELECT DISTINCT
            s.id AS sales_id,
            s.date AS sale_date,
            o.name AS object_name,
            a.apartmentNumber AS apartment,
            s.currency,
            a.totalAmount AS cost,
            s.discount,
            (a.totalAmount - s.discount) AS total,
            CASE
                WHEN s.isInstallmentPlan = 1 THEN COALESCE(s.installment, 0) + COALESCE(SUM(p.payment), 0)
                ELSE a.totalAmount - s.discount
            END AS paid_amount,
            CASE
                WHEN s.isInstallmentPlan = 1 THEN 'Рассрочка'
                ELSE 'Полный расчет'
            END AS status,
            a.ploshad AS area,
            a.priceSquare AS price_per_m2,
            a.roomsQuantity AS rooms,
            a.sanuzelsQuantity AS bathrooms,
            a.floor,
            CASE
                WHEN s.isInstallmentPlan = 1 THEN (a.totalAmount - s.discount) - (COALESCE(s.installment, 0) + COALESCE(SUM(p.payment), 0))
                ELSE 0
            END AS debt
        FROM
            eraconstruction_sales s
        JOIN eraconstruction_apartments a ON a.id = s.apartmentId
        JOIN eraconstruction_objects o ON o.id = a.objectId
        LEFT JOIN eraconstruction_payments p ON p.apartmentId = a.id
        WHERE s.clientId = %1
        GROUP BY s.id, s.date, o.name, a.apartmentNumber, a.totalAmount, s.discount, s.installment, a.ploshad, a.priceSquare, a.roomsQuantity, a.sanuzelsQuantity, a.floor, s.isInstallmentPlan
        ORDER BY sale_date, o.name, a.apartmentNumber;
    )").arg(clientId);

    return db.executeGet(query);
}

QVariantList ReportOperations::getClients() {
    dbManager &db = dbManager::getInstance();

    QString query = R"(
        SELECT DISTINCT
            c.id AS client_id,
            c.name AS client_name
        FROM
            eraconstruction_clients c
        JOIN
            eraconstruction_sales s ON c.id = s.clientId
    )";

    return db.executeGet(query);
}

QVariantList ReportOperations::getClientsPaymentsForReportByClient(int salesId) {
    dbManager &db = dbManager::getInstance();

    QString getSaleQuery = QString(R"(
        SELECT clientId, objectId, apartmentId
        FROM eraconstruction_sales
        WHERE id = %1
    )").arg(salesId);

    QVariantList saleData = db.executeGet(getSaleQuery);

    if (saleData.isEmpty()) {
        return {};
    }

    QVariantList sale = saleData.first().toList();
    int clientId = sale[0].toInt();
    int objectId = sale[1].toInt();
    int apartmentId = sale[2].toInt();

    QString getPaymentsQuery = QString(R"(
        SELECT
            p.date AS payment_date,
            pt.name AS payment_type,
            p.payment AS paid_amount,
            p.description AS payment_description
        FROM
            eraconstruction_payments p
        JOIN
            eraconstruction_paymentTypes pt ON pt.id = p.paymentTypeId
        WHERE
            p.clientId = %1 AND p.objectId = %2 AND p.apartmentId = %3
    )").arg(clientId).arg(objectId).arg(apartmentId);

    return db.executeGet(getPaymentsQuery);
}

