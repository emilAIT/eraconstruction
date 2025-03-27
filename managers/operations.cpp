#include "operations.h"


Operations::Operations() {}

QVariantList Operations::selectClientsWithDebt()
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT c.id, c.name "
        "FROM eraconstruction_sales s "
        "JOIN eraconstruction_clients c ON s.clientId = c.id "
        "LEFT JOIN ( "
        "    SELECT clientId, apartmentId, objectId, SUM(payment) AS totalPaid "
        "    FROM eraconstruction_payments "
        "    GROUP BY clientId, apartmentId, objectId "
        ") p ON s.clientId = p.clientId AND s.apartmentId = p.apartmentId AND s.objectId = p.objectId "
        "WHERE s.isInstallmentPlan = true "
        "AND (s.installment + IFNULL(p.totalPaid, 0)) < s.totalAmount"
        );

    return db.executeGet(query);
}

QVariantList Operations::selectClientsWithPurchase()
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT c.id, c.name "
        "FROM eraconstruction_sales s "
        "JOIN eraconstruction_clients c ON s.clientId = c.id "
        "WHERE s.isInstallmentPlan = 1"
        );

    return db.executeGet(query);
}

QVariantList Operations::selectAllClients() {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT * FROM eraconstruction_clients"
        );
    return db.executeGet(query);
}

QVariantList Operations::getClient(int clientId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT * FROM eraconstruction_clients WHERE id = %1")
        .arg(clientId);
    return db.executeGet(query);
}


bool Operations::addClient(QString name, QString phoneNumber,QString authority,QString dateOfBirth,QString dateOfIssue,QString passportId,QString placeOfRessidence, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
                        "INSERT INTO eraconstruction_clients (name, phoneNumber, authority, dateOfBirth, dateOfIssue, "
                        "passportId, placeOfResidence, description) "
                        "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')")
        .arg(name)
        .arg(phoneNumber)
        .arg(authority)
        .arg(dateOfBirth)
        .arg(dateOfIssue)
        .arg(passportId)
        .arg(placeOfRessidence)
        .arg(description);
    return db.executeSet(query);
}

bool Operations::updateClient(int clientId, QString name, QString phoneNumber,QString authority,QString dateOfBirth,QString dateOfIssue,QString passportId,QString placeOfResidence, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
                        "UPDATE eraconstruction_clients SET name = '%1', phoneNumber = '%2', authority = '%3', "
                        "dateOfBirth = '%4', dateOfIssue = '%5', passportId = '%6', placeOfResidence = '%7', "
                        "description = '%8' WHERE id = %9")
                        .arg(name)
                        .arg(phoneNumber)
                        .arg(authority)
                        .arg(dateOfBirth)
                        .arg(dateOfIssue)
                        .arg(passportId)
                        .arg(placeOfResidence)
                        .arg(description)
                        .arg(clientId);
    return db.executeSet(query);
}


bool Operations::deleteClient(int clientId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "DELETE FROM eraconstruction_clients WHERE id = %1")
        .arg(clientId);
    return db.executeSet(query);
}


bool Operations::clientExists(QString name) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_clients "
                        "WHERE LOWER(name) = LOWER('%1')"
                        ).arg(name);
    QVariant result = db.executeGet(query);
    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();
        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();
            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();
                return count > 0;  // Если count > 0, клиент существует
            }
        }
    }
    return false;
}








QVariantList Operations::selectAllPaymentTypes() {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT * FROM eraconstruction_paymentTypes"
        );
    return db.executeGet(query);
}


QVariantList Operations::getPaymentType(int paymentTypeId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT * FROM eraconstruction_paymentTypes WHERE id = %1")
        .arg(paymentTypeId);
    return db.executeGet(query);
}


bool Operations::addPaymentType(QString name) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "INSERT INTO eraconstruction_paymentTypes (name) VALUES ('%1')")
        .arg(name);
    return db.executeSet(query);
}


bool Operations::paymentTypeExists(const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString("SELECT COUNT(*) FROM eraconstruction_paymentTypes WHERE LOWER(name) = LOWER('%1')").arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();
                return count > 0;  // Если count > 0, тип платежа существует
            }
        }
    }

    return false;
}


bool Operations::paymentTypeExistsForEdit(int paymentTypeId, const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT COUNT(*) FROM eraconstruction_paymentTypes "
        "WHERE LOWER(name) = LOWER('%1')"
        ).arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();

                if (count > 0) {
                    QString idQuery = QString(
                        "SELECT id FROM eraconstruction_paymentTypes WHERE LOWER(name) = LOWER('%1')"
                        ).arg(name);

                    QVariant idResult = db.executeGet(idQuery);

                    if (idResult.isValid() && idResult.canConvert<QList<QVariant>>()) {
                        QList<QVariant> idOuterList = idResult.toList();

                        if (!idOuterList.isEmpty()) {
                            QVariantList idInnerList = idOuterList.first().toList();

                            if (!idInnerList.isEmpty()) {
                                int dbPaymentTypeId = idInnerList.first().toInt();
                                if (dbPaymentTypeId == paymentTypeId) {
                                    return false; // Можно редактировать
                                } else {
                                    return true;  // Тип платежа существует, но не для этого редактируемого
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;  // Тип платежа не существует
}


bool Operations::updatePaymentType(int paymentTypeId, QString name) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "UPDATE eraconstruction_paymentTypes SET name = '%1' WHERE id = %2")
        .arg(name)
        .arg(paymentTypeId);
    return db.executeSet(query);
}


bool Operations::deletePaymentType(int paymentTypeId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "DELETE FROM eraconstruction_paymentTypes WHERE id = %1")
        .arg(paymentTypeId);
    return db.executeSet(query);
}


QVariantList Operations::selectAllUsers() {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT * FROM eraconstruction_users"
        );
    return db.executeGet(query);
}


QVariantList Operations::getUser(int userId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT * FROM eraconstruction_users WHERE id = %1")
        .arg(userId);
    return db.executeGet(query);
}


bool Operations::addUser(QString name, QString password, bool isAdmin, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query;
    if (password == "ffc68d3e87de7154c716e486f7b1dedb42dd3d7782e8dfef44498bd30b24aa67") {
        query = "INSERT INTO eraconstruction_users (name, isAdmin, description) VALUES ('" + name + "', " + QString::number(isAdmin) + ", '" + description + "')";
    } else {
        query = "INSERT INTO eraconstruction_users (name, password, isAdmin, description) VALUES ('" + name + "', '" + password + "', " + QString::number(isAdmin) + ", '" + description + "')";
    }
    return db.executeSet(query);
}


bool Operations::userExists(const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = "SELECT COUNT(*) FROM eraconstruction_users WHERE name = '" + name + "'";

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();
                return count > 0;  // Если count > 0, пользователь существует
            }
        }
    }

    return false;
}


bool Operations::userExistsForEdit(int userId, const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_users "
                        "WHERE name = '%1'"
                        ).arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();

                if (count > 0) {
                    QString idQuery = QString(
                                          "SELECT id FROM eraconstruction_users WHERE name = '%1'"
                                          ).arg(name);

                    QVariant idResult = db.executeGet(idQuery);

                    int dbUserId = -1;
                    if (idResult.isValid() && idResult.canConvert<QList<QVariant>>()) {
                        QList<QVariant> idOuterList = idResult.toList();
                        if (!idOuterList.isEmpty()) {
                            QVariantList idInnerList = idOuterList.first().toList();
                            if (!idInnerList.isEmpty()) {
                                dbUserId = idInnerList.first().toInt();
                            }
                        }
                    }
                    if (dbUserId == userId) {
                        return false;  // Можно редактировать
                    }
                    else {
                        return true;
                    }
                }
                else {
                    return false;
                }
            }
        }
    }

    return true;  // Блокируем добавление, если такой пользователь уже существует
}


bool Operations::updateUser(int userId, QString name, QString password, bool isAdmin, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query;
    if (password == "ffc68d3e87de7154c716e486f7b1dedb42dd3d7782e8dfef44498bd30b24aa67") {
        query = "UPDATE eraconstruction_users SET name = '" + name + "', isAdmin = " + QString::number(isAdmin) + ", description = '" + description + "' WHERE id = " + QString::number(userId);
    } else {
        query = "UPDATE eraconstruction_users SET name = '" + name + "', password = '" + password + "', isAdmin = " + QString::number(isAdmin) + ", description = '" + description + "' WHERE id = " + QString::number(userId);
    }
    return db.executeSet(query);
}


bool Operations::deleteUser(int userId) {
    dbManager &db = dbManager::getInstance();
    QString query = "DELETE FROM eraconstruction_users WHERE id = " + QString::number(userId);
    return db.executeSet(query);
}


QList<int> Operations::checkLoginUser(QString username, QString password) {
    dbManager &db = dbManager::getInstance();

    // Выполняем запрос к базе данных для проверки пароля
    QVariantList data = db.executeGet("SELECT id, isAdmin FROM eraconstruction_users WHERE name = '" + username + "' AND password = '" + password + "'");
    if (!data.isEmpty()) // Если пользователь найден
    {
        QVariantList userData = data[0].toList(); // Берем первую строку результата
        return QList<int>({userData[0].toInt(), userData[1].toInt()}); // Возвращаем ID и флаг администратора
    }

    // Если пользователь не найден, возвращаем код ошибки
    return QList<int>({-9, 0});
}





QVariantList Operations::selectAllTypes() {
    dbManager &db = dbManager::getInstance();
    userSession &us = userSession::getInstance();
    QVariantList data;
    if (us.checkIsAdmin()) {
        data = db.executeGet("SELECT * FROM eraconstruction_types ORDER BY name");
    } else {
        data = db.executeGet("SELECT * FROM eraconstruction_types WHERE isAdmin = false ORDER BY name");
    }
    return data;
}

bool Operations::addType(QString name, bool isAdmin) {
    dbManager &db = dbManager::getInstance();
    QString query = "INSERT INTO eraconstruction_types (name, isAdmin) VALUES ('" + name + "', " + QString::number(isAdmin) + ")";
    return db.executeSet(query);
}

bool Operations::typeExists(const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_types "
                        "WHERE LOWER(name) = LOWER('%1')"
    ).arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();
                return count > 0;  // Если count > 0, значит тип существует
            }
        }
    } else {
        qDebug() << "Query execution failed or returned invalid result!";
    }

    return false;
}

bool Operations::typeExistsForEdit(int typeId, const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_types "
                        "WHERE LOWER(name) = LOWER('%1')"
                        ).arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();

                if (count > 0) {
                    QString idQuery = QString(
                                          "SELECT id FROM eraconstruction_types WHERE LOWER(name) = LOWER('%1')"
                                          ).arg(name);

                    QVariant idResult = db.executeGet(idQuery);

                    if (idResult.isValid() && idResult.canConvert<QList<QVariant>>()) {
                        QList<QVariant> idOuterList = idResult.toList();

                        if (!idOuterList.isEmpty()) {
                            QVariantList idInnerList = idOuterList.first().toList();

                            if (!idInnerList.isEmpty()) {
                                int dbTypeId = idInnerList.first().toInt();
                                return dbTypeId != typeId;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

QVariantList Operations::getType(int typeId) {
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM eraconstruction_types WHERE id = " + QString::number(typeId));
    return data;
}

bool Operations::deleteType(int typeId) {
    dbManager &db = dbManager::getInstance();
    QString query1 = "DELETE FROM eraconstruction_whoWhat WHERE typeId = " + QString::number(typeId);
    QString query2 = "DELETE FROM eraconstruction_types WHERE id = " + QString::number(typeId);
    return db.executeSet(query1) && db.executeSet(query2);
}


bool Operations::updateType(int typeId, QString name, bool isAdmin) {
    dbManager &db = dbManager::getInstance();
    QString query = "UPDATE eraconstruction_types SET name = '" + name + "', isAdmin = " + QString::number(isAdmin) + " WHERE id = " + QString::number(typeId);
    return db.executeSet(query);
}



QVariantList Operations::selectAllWhoWhats(int typeId) {
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM eraconstruction_whoWhat WHERE typeId = " + QString::number(typeId) + " ORDER BY name");
    return data;
}


bool Operations::addWhoWhat(int typeId, QString whoWhat) {
    dbManager &db = dbManager::getInstance();
    QString query = "INSERT INTO eraconstruction_whoWhat (typeId, name) VALUES (" + QString::number(typeId) + ", '" + whoWhat + "')";
    return db.executeSet(query);
}

bool Operations::whoWhatExists(int typeId, const QString& name) {
    dbManager &db = dbManager::getInstance();

    if (!db.isConnected()) {
        return true;
    }

    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_whoWhat "
                        "WHERE typeId = %1 AND LOWER(name) = LOWER('%2')"
                        ).arg(typeId).arg(name);

    QVariant result = db.executeGet(query);

    if (!result.isValid()) {
        return true;
    }

    qDebug() << "Result from DB:" << result;

    if (result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty() && outerList.first().canConvert<QList<QVariant>>()) {
            QList<QVariant> innerList = outerList.first().toList();

            if (!innerList.isEmpty() && innerList.first().canConvert<int>()) {
                int count = innerList.first().toInt();
                qDebug() << "Correct count:" << count;
                return count > 0;
            }
        }
    }

    return true;
}

bool Operations::whoWhatExistsForEdit(int typeId, int whoWhatId, const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT COUNT(*) FROM eraconstruction_whoWhat "
        "WHERE typeId = %1 AND LOWER(name) = LOWER('%2')"
        ).arg(typeId).arg(name);

    QVariant result = db.executeGet(query);

    int count = 0;
    if (result.canConvert<QVariantList>()) {
        QVariantList outerList = result.toList();
        if (!outerList.isEmpty() && outerList.first().canConvert<QVariantList>()) {
            QVariantList innerList = outerList.first().toList();
            if (!innerList.isEmpty() && innerList.first().canConvert<int>()) {
                count = innerList.first().toInt();
            }
        }
    }

    if (count == 0) {
        return false; // Такой объект не существует, можно добавлять
    }

    // Если такой объект существует, получаем его ID
    QString idQuery = QString(
                          "SELECT id FROM eraconstruction_whoWhat "
                          "WHERE typeId = %1 AND LOWER(name) = LOWER('%2')"
                          ).arg(typeId).arg(name);

    QVariant idResult = db.executeGet(idQuery);

    int dbWhoWhatId = -1;
    if (idResult.canConvert<QVariantList>()) {
        QVariantList idOuterList = idResult.toList();
        if (!idOuterList.isEmpty() && idOuterList.first().canConvert<QVariantList>()) {
            QVariantList idInnerList = idOuterList.first().toList();
            if (!idInnerList.isEmpty() && idInnerList.first().canConvert<int>()) {
                dbWhoWhatId = idInnerList.first().toInt();
            }
        }
    }

    if (dbWhoWhatId == whoWhatId) {
        return false; // Можно редактировать
    }

    return true; // Блокируем редактирование
}

QVariantList Operations::getWhoWhat(int whoWhatId) {
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT t.name, w.name AS typeName FROM eraconstruction_whoWhat w JOIN eraconstruction_types t ON w.typeId = t.id WHERE w.id = " + QString::number(whoWhatId));
    return data;
}

bool Operations::deleteWhoWhat(int whoWhatId) {
    dbManager &db = dbManager::getInstance();
    QString query = "DELETE FROM eraconstruction_whoWhat WHERE id = " + QString::number(whoWhatId);
    return db.executeSet(query);
}

bool Operations::updateWhoWhat(int whoWhatId, int typeId, QString whoWhat) {
    dbManager &db = dbManager::getInstance();
    QString query = "UPDATE eraconstruction_whoWhat SET typeId = " + QString::number(typeId) + ", name = '" + whoWhat + "' WHERE id = " + QString::number(whoWhatId);
    return db.executeSet(query);
}




QVariantList Operations::selectObjectsByClient(int clientId)
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT o.id, o.name "
        "FROM eraconstruction_sales s "
        "JOIN eraconstruction_objects o ON s.objectId = o.id "
        "LEFT JOIN ( "
        "    SELECT apartmentId, objectId, SUM(payment) AS totalPaid "
        "    FROM eraconstruction_payments "
        "    WHERE clientId = %1 "
        "    GROUP BY apartmentId, objectId "
        ") p ON s.apartmentId = p.apartmentId AND s.objectId = p.objectId "
        "WHERE s.clientId = %1 "
        "AND s.isInstallmentPlan = 1 "
        "AND (s.totalAmount - s.installment) > COALESCE(p.totalPaid, 0)"
        ).arg(clientId);

    return db.executeGet(query);
}

QVariantList Operations::selectObjectsWithInstallmentByClient(int clientId)
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT o.id, o.name "
        "FROM eraconstruction_sales s "
        "JOIN eraconstruction_objects o ON s.objectId = o.id "
        "WHERE s.clientId = %1 "
        "AND s.isInstallmentPlan = 1"
        ).arg(clientId);

    return db.executeGet(query);
}


QVariantList Operations::selectAllObjects() {
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM eraconstruction_objects ORDER BY name");
    return data;
}

QVariantList Operations::getObject(int objectId) {
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM eraconstruction_objects WHERE id = " + QString::number(objectId));
    return data;
}

QVariantList Operations::getApartmentsInSales(int objectId) {
    dbManager &db = dbManager::getInstance();

    // Запрос на получение всех квартир для объекта, которые находятся в продаже
    QString query = "SELECT a.id, a.apartmentNumber FROM eraconstruction_apartments a "
                    "JOIN eraconstruction_sales s ON a.id = s.apartmentId "
                    "WHERE s.objectId = " + QString::number(objectId);

    return db.executeGet(query);
}


bool Operations::addObject(QString name, QString address) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
                        "INSERT INTO eraconstruction_objects (name, address) VALUES ('%1', '%2')")
                        .arg(name)
                        .arg(address);
    return db.executeSet(query);
}

bool Operations::objectExists(const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT COUNT(*) FROM eraconstruction_objects "
        "WHERE LOWER(name) = LOWER('%1')"
        ).arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();
                return count > 0;  // Если count > 0, объект существует
            }
        }
    } else {
        qDebug() << "Query execution failed or returned invalid result!";
    }
    return false;
}

bool Operations::objectExistsForEdit(int objectId, const QString& name) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT COUNT(*) FROM eraconstruction_objects "
        "WHERE LOWER(name) = LOWER('%1')"
        ).arg(name);

    QVariant result = db.executeGet(query);

    if (result.isValid() && result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty()) {
            QVariantList innerList = outerList.first().toList();

            if (!innerList.isEmpty()) {
                int count = innerList.first().toInt();

                if (count > 0) {
                    QString idQuery = QString(
                          "SELECT id FROM eraconstruction_objects "
                          "WHERE LOWER(name) = LOWER('%1')"
                          ).arg(name);

                    QVariant idResult = db.executeGet(idQuery);

                    if (idResult.isValid() && idResult.canConvert<QList<QVariant>>()) {
                        QList<QVariant> idOuterList = idResult.toList();

                        if (!idOuterList.isEmpty()) {
                            QVariantList idInnerList = idOuterList.first().toList();

                            if (!idInnerList.isEmpty()) {
                                int dbObjectId = idInnerList.first().toInt();
                                if (dbObjectId == objectId) {
                                    return false;
                                } else {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool Operations::deleteObject(int objectId) {
    dbManager &db = dbManager::getInstance();

    // Удаление продаж квартир объекта
    QString querySales = "DELETE FROM eraconstruction_sales WHERE apartmentId IN ("
                         "SELECT id FROM eraconstruction_apartments WHERE objectId = " + QString::number(objectId) + ")";

    // Удаление оплат квартир объекта
    QString queryPayments = "DELETE FROM eraconstruction_payments WHERE apartmentId IN ("
                            "SELECT id FROM eraconstruction_apartments WHERE objectId = " + QString::number(objectId) + ")";

    // Удаление самих квартир объекта
    QString queryApartments = "DELETE FROM eraconstruction_apartments WHERE objectId = " + QString::number(objectId);

    // Удаление самого объекта
    QString queryObject = "DELETE FROM eraconstruction_objects WHERE id = " + QString::number(objectId);

    return db.executeSet(querySales) &&
           db.executeSet(queryPayments) &&
           db.executeSet(queryApartments) &&
           db.executeSet(queryObject);
}

bool Operations::updateObject(int objectId, QString name, QString address) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
                        "UPDATE eraconstruction_objects SET name = '%1', address = '%2' WHERE id = %3")
                        .arg(name)
                        .arg(address)
                        .arg(objectId);
    return db.executeSet(query);
}


QVariantList Operations::selectApartmentsByClient(int clientId, int objectId)
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT a.id, a.objectId, a.apartmentNumber "
        "FROM eraconstruction_sales s "
        "JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
        "LEFT JOIN ( "
        "    SELECT apartmentId, objectId, clientId, SUM(payment) AS totalPaid "
        "    FROM eraconstruction_payments "
        "    WHERE clientId = %1 AND objectId = %2 "
        "    GROUP BY apartmentId, objectId, clientId "
        ") p ON s.apartmentId = p.apartmentId AND s.objectId = p.objectId AND s.clientId = p.clientId "
        "WHERE s.clientId = %1 AND s.objectId = %2 "
        "AND s.isInstallmentPlan = 1 "
        "AND (s.totalAmount - s.installment) > COALESCE(p.totalPaid, 0)"
        ).arg(clientId).arg(objectId);

    return db.executeGet(query);
}

QVariantList Operations::selectInstallmentApartments(int clientId, int objectId)
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT a.id, a.objectId, a.apartmentNumber "
        "FROM eraconstruction_sales s "
        "JOIN eraconstruction_apartments a ON s.apartmentId = a.id "
        "WHERE s.clientId = %1 AND s.objectId = %2 "
        "AND s.isInstallmentPlan = 1"
        ).arg(clientId).arg(objectId);

    return db.executeGet(query);
}

QVariantList Operations::selectAvailableApartments(int objectId, int saleId)
{
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT DISTINCT a.id, a.objectId, a.apartmentNumber "
        "FROM eraconstruction_apartments a "
        "LEFT JOIN eraconstruction_sales s ON a.id = s.apartmentId "
        "WHERE a.objectId = %1 "
        "AND (s.id IS NULL OR s.id = %2)"
        "ORDER BY a.apartmentNumber"
        ).arg(objectId).arg(saleId);

    return db.executeGet(query);
}

QVariantList Operations::selectAllApartmentsWithoutId() {
    dbManager &db = dbManager::getInstance();
    QString query = "SELECT * FROM eraconstruction_apartments";
    return db.executeGet(query);
}


QVariantList Operations::selectAllApartments(int objectId) {
    dbManager &db = dbManager::getInstance();
    QString query = "SELECT * FROM eraconstruction_apartments WHERE objectId = " + QString::number(objectId);
    return db.executeGet(query);
}

QVariantList Operations::getApartment(int apartmentId) {
    dbManager &db = dbManager::getInstance();
    QVariantList data = db.executeGet("SELECT * FROM eraconstruction_apartments WHERE id = " + QString::number(apartmentId));
    return data;
}

bool Operations::isApartmentInSales(int apartmentId) {
    dbManager &db = dbManager::getInstance();

    QString query = "SELECT COUNT(*) FROM eraconstruction_sales WHERE apartmentId = " + QString::number(apartmentId);
    QVariantList result = db.executeGet(query);

    if (!result.isEmpty()) {
        bool ok;
        int count = result.first().toList().first().toInt(&ok);
        return ok && count > 0; // Если найдено хотя бы одно совпадение, значит квартира в продаже
    }
    return false;
}

bool Operations::addApartment(int objectId, QString apartmentNumber, double ploshad, int roomsQuantity, int sanuzelsQuantity, int floor, double priceSquare, double totalAmount, QString currency) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "INSERT INTO eraconstruction_apartments (objectId, apartmentNumber, ploshad, roomsQuantity, sanuzelsQuantity, floor, priceSquare, totalAmount, currency) VALUES (%1, '%2', %3, %4, %5, %6, %7, %8, '%9')")
        .arg(objectId)
        .arg(apartmentNumber)
        .arg(ploshad)
        .arg(roomsQuantity)
        .arg(sanuzelsQuantity)
        .arg(floor)
        .arg(priceSquare, 0, 'f', 2)
        .arg(totalAmount, 0, 'f', 2)
        .arg(currency);
    return db.executeSet(query);
}

bool Operations::apartmentExists(int objectId, const QString& apartmentNumber) {
    dbManager &db = dbManager::getInstance();

    if (!db.isConnected()) {
        return true; // Блокируем добавление, возвращая "квартира уже есть"
    }

    QString query = QString(
        "SELECT COUNT(*) FROM eraconstruction_apartments "
        "WHERE objectId = %1 AND LOWER(apartmentNumber) = LOWER('%2')"
        ).arg(objectId).arg(apartmentNumber);

    QVariant result = db.executeGet(query);

    if (!result.isValid()) {
        return true; // Блокируем добавление
    }

    qDebug() << "Result from DB:" << result;

    if (result.canConvert<QList<QVariant>>()) {
        QList<QVariant> outerList = result.toList();

        if (!outerList.isEmpty() && outerList.first().canConvert<QList<QVariant>>()) {
            QList<QVariant> innerList = outerList.first().toList();

            if (!innerList.isEmpty() && innerList.first().canConvert<int>()) {
                int count = innerList.first().toInt();
                qDebug() << "Correct count:" << count;
                return count > 0; // Если count > 0, квартира уже есть
            }
        }
    }

    return true; // Блокируем добавление
}

bool Operations::apartmentExistsForEdit(int objectId, int apartmentId, const QString& apartmentNumber) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_apartments "
                        "WHERE objectId = %1 AND LOWER(apartmentNumber) = LOWER('%2')"
                        ).arg(objectId).arg(apartmentNumber);

    QVariant result = db.executeGet(query);

    int count = 0;
    if (result.canConvert<QVariantList>()) {
        QVariantList outerList = result.toList();
        if (!outerList.isEmpty() && outerList.first().canConvert<QVariantList>()) {
            QVariantList innerList = outerList.first().toList();
            if (!innerList.isEmpty() && innerList.first().canConvert<int>()) {
                count = innerList.first().toInt();
            }
        }
    }

    if (count == 0) {
        return false; // Квартиры нет, можно добавить
    }

    // Если такая квартира существует, получаем её ID
    QString idQuery = QString(
        "SELECT id FROM eraconstruction_apartments "
        "WHERE objectId = %1 AND LOWER(apartmentNumber) = LOWER('%2')"
        ).arg(objectId).arg(apartmentNumber);

    QVariant idResult = db.executeGet(idQuery);

    int dbApartmentId = -1;
    if (idResult.canConvert<QVariantList>()) {
        QVariantList idOuterList = idResult.toList();
        if (!idOuterList.isEmpty() && idOuterList.first().canConvert<QVariantList>()) {
            QVariantList idInnerList = idOuterList.first().toList();
            if (!idInnerList.isEmpty() && idInnerList.first().canConvert<int>()) {
                dbApartmentId = idInnerList.first().toInt();
            }
        }
    }

    if (dbApartmentId == apartmentId) {
        return false; // Можно редактировать
    }

    return true; // Блокируем добавление
}

bool Operations::updateApartment(int apartmentId,int objectId, QString apartmentNumber, double ploshad, int roomsQuantity, int sanuzelsQuantity, int floor, double priceSquare, double totalAmount, QString currency) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "UPDATE eraconstruction_apartments SET objectId = %1, apartmentNumber = '%2', ploshad = %3, roomsQuantity = %4, sanuzelsQuantity = %5, floor = %6, priceSquare = %7, totalAmount = %8, currency = '%10' WHERE id = %9")
        .arg(objectId)
        .arg(apartmentNumber)
        .arg(ploshad)
        .arg(roomsQuantity)
        .arg(sanuzelsQuantity)
        .arg(floor)
        .arg(priceSquare, 0, 'f', 2)
        .arg(totalAmount, 0, 'f', 2)
        .arg(apartmentId)
        .arg(currency);
    return db.executeSet(query);
}



bool Operations::deleteApartment(int apartmentId) {
    dbManager &db = dbManager::getInstance();

    // Удаляем из продаж
    QString deleteSalesQuery = "DELETE FROM eraconstruction_sales WHERE apartmentId = " + QString::number(apartmentId);

    // Удаляем из оплат
    QString deletePaymentsQuery = "DELETE FROM eraconstruction_payments WHERE apartmentId = " + QString::number(apartmentId);

    // Удаляем из таблицы квартир
    QString deleteApartmentQuery = "DELETE FROM eraconstruction_apartments WHERE id = " + QString::number(apartmentId);

    // Выполняем все запросы
    return db.executeSet(deleteSalesQuery) &&
           db.executeSet(deletePaymentsQuery) &&
           db.executeSet(deleteApartmentQuery);
}



bool Operations::isApartmentSold(int apartmentId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
                        "SELECT COUNT(*) FROM eraconstruction_sales WHERE apartmentId = %1"
                        ).arg(apartmentId);

    QVariantList result = db.executeGet(query);
    if (!result.isEmpty() && result.first().toList().first().toInt() > 0) {
        return true;
    }
    return false;
}



QVariantList Operations::selectAllSales(QDate fromdate, QDate todate) {
    dbManager &db = dbManager::getInstance();
    QString fromdateString = fromdate.toString("yyyy-MM-dd");
    QString todateString = todate.toString("yyyy-MM-dd");

    QVariantList data = db.executeGet(
        "SELECT eraconstruction_sales.id, eraconstruction_sales.date, "
        "CASE "
        "    WHEN eraconstruction_clients.id IS NULL THEN '-' "
        "    WHEN eraconstruction_clients.id = 0 THEN '-' "
        "    ELSE eraconstruction_clients.name "
        "END AS client, "
        "CASE "
        "    WHEN eraconstruction_objects.id IS NULL THEN '-' "
        "    WHEN eraconstruction_objects.id = 0 THEN '-' "
        "    ELSE eraconstruction_objects.name "
        "END AS object, "
        "CASE "
        "    WHEN eraconstruction_apartments.id IS NULL THEN '-' "
        "    WHEN eraconstruction_apartments.id = 0 THEN '-' "
        "    ELSE eraconstruction_apartments.apartmentNumber "
        "END AS apartment, "
        "eraconstruction_sales.apartmentCost, eraconstruction_sales.discount, "
        "eraconstruction_sales.totalAmount, "

        "CASE "
        "    WHEN eraconstruction_paymentTypes.id IS NULL THEN '-' "
        "    WHEN eraconstruction_paymentTypes.id = 0 THEN '-' "
        "    ELSE eraconstruction_paymentTypes.name "
        "END AS paymentType, "
        "eraconstruction_sales.isInstallmentPlan, eraconstruction_sales.installment, "
        "eraconstruction_sales.monthQuantity, eraconstruction_sales.monthlyPayment, "
        "eraconstruction_sales.description, eraconstruction_sales.currency "
        "FROM eraconstruction_sales "
        "LEFT JOIN eraconstruction_clients ON eraconstruction_sales.clientId = eraconstruction_clients.id "
        "LEFT JOIN eraconstruction_objects ON eraconstruction_sales.objectId = eraconstruction_objects.id "
        "LEFT JOIN eraconstruction_apartments ON eraconstruction_sales.apartmentId = eraconstruction_apartments.id "
        "LEFT JOIN eraconstruction_paymentTypes ON eraconstruction_sales.paymentTypeId = eraconstruction_paymentTypes.id "
        "WHERE DATE(eraconstruction_sales.date) BETWEEN '" + fromdateString + "' AND '" + todateString + "' "
        "ORDER BY eraconstruction_clients.name"
    );
    return data;
}

QVariantList Operations::getSale(int saleId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT eraconstruction_sales.date, "
        "COALESCE(eraconstruction_clients.name, '-') AS client, "
        "COALESCE(eraconstruction_objects.name, '-') AS object, "
        "COALESCE(eraconstruction_apartments.apartmentNumber, '-') AS apartment, "
        "eraconstruction_sales.apartmentCost, eraconstruction_sales.discount, "
        "eraconstruction_sales.totalAmount, "
        "COALESCE(eraconstruction_paymentTypes.name, '-') AS paymentType, "
        "eraconstruction_sales.isInstallmentPlan, eraconstruction_sales.installment, "
        "eraconstruction_sales.monthQuantity, eraconstruction_sales.monthlyPayment, "
        "eraconstruction_sales.description, "
        "eraconstruction_sales.currency "
        "FROM eraconstruction_sales "
        "LEFT JOIN eraconstruction_clients ON eraconstruction_sales.clientId = eraconstruction_clients.id "
        "LEFT JOIN eraconstruction_objects ON eraconstruction_sales.objectId = eraconstruction_objects.id "
        "LEFT JOIN eraconstruction_apartments ON eraconstruction_sales.apartmentId = eraconstruction_apartments.id "
        "LEFT JOIN eraconstruction_paymentTypes ON eraconstruction_sales.paymentTypeId = eraconstruction_paymentTypes.id "
        "WHERE eraconstruction_sales.id = %1")
        .arg(saleId);
    return db.executeGet(query);
}

bool Operations::addSale(QDate date, int clientId, int objectId, int apartmentId, float apartmentCost, float discount, float totalAmount, int paymentTypeId, QString currency, bool isInstallmentPlan, double installment, int monthQuantity, double monthlyPayment, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "INSERT INTO eraconstruction_sales (date, clientId, objectId, apartmentId, apartmentCost, discount, totalAmount, paymentTypeId, currency, isInstallmentPlan, installment, monthQuantity, monthlyPayment, description) "
        "VALUES ('%1', %2, %3, %4, %5, %6, %7, %8, '%9', %10, %11, %12, %13, '%14')")
        .arg(date.toString("yyyy-MM-dd"))
        .arg(clientId)
        .arg(objectId)
        .arg(apartmentId)
        .arg(apartmentCost, 0, 'f', 2)
        .arg(discount, 0, 'f', 2)
        .arg(totalAmount, 0, 'f', 2)
        .arg(paymentTypeId)
        .arg(currency)
        .arg(isInstallmentPlan)
        .arg(installment, 0, 'f', 2)
        .arg(monthQuantity)
        .arg(monthlyPayment, 0, 'f', 5)
        .arg(description);
    return db.executeSet(query);
}


bool Operations::updateSale(int saleId, QDate date, int clientId, int objectId, int apartmentId, double apartmentCost, float discount, double totalAmount, int paymentTypeId, QString currency, bool isInstallmentPlan, double installment, int monthQuantity, double monthlyPayment, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "UPDATE eraconstruction_sales SET date = '%1', clientId = %2, objectId = %3, apartmentId = %4, apartmentCost = %5, "
        "discount = %6, totalAmount = %7, paymentTypeId = %8, currency = '%9', isInstallmentPlan = %10, installment = %11, monthQuantity = %12, "
        "monthlyPayment = %13, description = '%14' WHERE id = %15")
        .arg(date.toString("yyyy-MM-dd"))
        .arg(clientId)
        .arg(objectId)
        .arg(apartmentId)
        .arg(apartmentCost, 0, 'f', 2)
        .arg(discount, 0, 'f', 2)
        .arg(totalAmount, 0, 'f', 2)
        .arg(paymentTypeId)
        .arg(currency)
        .arg(isInstallmentPlan)
        .arg(installment, 0, 'f', 2)
        .arg(monthQuantity)
        .arg(monthlyPayment, 0, 'f', 5)
        .arg(description)
        .arg(saleId);
    return db.executeSet(query);
}

bool Operations::deleteSale(int saleId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString("DELETE FROM eraconstruction_sales WHERE id = %1")
        .arg(saleId);
    return db.executeSet(query);
}

QDate Operations::getSaleDate(int clientId, int objectId, int apartmentId) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT date "
        "FROM eraconstruction_sales "
        "WHERE clientId = %1 AND objectId = %2 AND apartmentId = %3")
        .arg(clientId)
        .arg(objectId)
        .arg(apartmentId);

    QVariantList result = db.executeGet(query);

    if (!result.isEmpty() && result[0].canConvert<QVariantList>()) {
        QVariantList row = result[0].toList(); // Получаем первую строку из списка
        if (!row.isEmpty()) {
            QVariant saleDateVar = row[0]; // Берем дату
            QDate saleDate = saleDateVar.toDate();
            qDebug() << "Дата продажи:" << saleDate;
            return saleDate;
        }
    }

    return QDate();  // Возвращаем пустую дату, если не нашли сделку
}

bool Operations::addPayment(QDate date, int clientId, int objectId, int apartmentId, int paymentTypeId, QString currency, double payment, QString description) {
    qDebug() << payment;
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "INSERT INTO eraconstruction_payments (date, clientId, objectId, apartmentId, paymentTypeId, currency, payment, description) "
        "VALUES ('%1', %2, %3, %4, %5, '%6', %7, '%8')")
        .arg(date.toString("yyyy-MM-dd"))
        .arg(clientId)
        .arg(objectId)
        .arg(apartmentId)
        .arg(paymentTypeId)
        .arg(currency)
        .arg(payment, 0, 'f', 2)
        .arg(description);
    return db.executeSet(query);
}

QVariantList Operations::getMonthlyPayments(int clientId, int objectId, int apartmentId) {
    dbManager &db = dbManager::getInstance();
    QString query;

    if (clientId > 0 && objectId > 0 && apartmentId > 0) {
        query = QString(
        "SELECT eraconstruction_sales.monthlyPayment "
        "FROM eraconstruction_sales "
        "LEFT JOIN eraconstruction_apartments ON eraconstruction_sales.apartmentId = eraconstruction_apartments.id "
        "LEFT JOIN eraconstruction_objects ON eraconstruction_apartments.objectId = eraconstruction_objects.id "
        "WHERE eraconstruction_sales.clientId = %1 "
        "AND eraconstruction_sales.apartmentId = %2 "
        "AND eraconstruction_objects.id = %3"
        ).arg(clientId).arg(apartmentId).arg(objectId);
    } else {
        return QVariantList();
    }

    return db.executeGet(query);
}

QVariantList Operations::getPaymentDetails(int clientId, int objectId, int apartmentId) {
    dbManager &db = dbManager::getInstance();
    QString query;

    if (clientId > 0 && objectId > 0 && apartmentId > 0) {
        query = QString(
        "SELECT "
        "    eraconstruction_sales.totalAmount, "
        "    eraconstruction_sales.installment, "
        "    SUM(eraconstruction_payments.payment) AS totalPaid, "
        "    eraconstruction_sales.currency "
        "FROM "
        "    eraconstruction_sales "
        "LEFT JOIN eraconstruction_apartments ON eraconstruction_sales.apartmentId = eraconstruction_apartments.id "
        "LEFT JOIN eraconstruction_objects ON eraconstruction_apartments.objectId = eraconstruction_objects.id "
        "LEFT JOIN eraconstruction_payments ON eraconstruction_sales.clientId = eraconstruction_payments.clientId "
        "AND eraconstruction_sales.objectId = eraconstruction_payments.objectId "
        "AND eraconstruction_sales.apartmentId = eraconstruction_payments.apartmentId "
        "WHERE "
        "eraconstruction_sales.clientId = %1 "
        "AND eraconstruction_sales.apartmentId = %2 "
        "AND eraconstruction_objects.id = %3 "
        "GROUP BY "
        "eraconstruction_sales.id"
        ).arg(clientId).arg(apartmentId).arg(objectId);

    } else {
        return QVariantList();
    }

    return db.executeGet(query);
}

QVariantList Operations::selectAllPayments(QDate fromdate, QDate todate) {
    dbManager &db = dbManager::getInstance();
    QString fromdateString = fromdate.toString("yyyy-MM-dd");
    QString todateString = todate.toString("yyyy-MM-dd");

    QVariantList data = db.executeGet(
        "SELECT eraconstruction_payments.id, eraconstruction_payments.date, "
        "CASE "
        "    WHEN eraconstruction_clients.id IS NULL THEN '-' "
        "    WHEN eraconstruction_clients.id = 0 THEN '-' "
        "    ELSE eraconstruction_clients.name "
        "END AS client, "
        "CASE "
        "    WHEN eraconstruction_objects.id IS NULL THEN '-' "
        "    WHEN eraconstruction_objects.id = 0 THEN '-' "
        "    ELSE eraconstruction_objects.name "
        "END AS object, "
        "CASE "
        "    WHEN eraconstruction_apartments.id IS NULL THEN '-' "
        "    WHEN eraconstruction_apartments.id = 0 THEN '-' "
        "    ELSE eraconstruction_apartments.apartmentNumber "
        "END AS apartment, "
        "CASE "
        "    WHEN eraconstruction_paymentTypes.id IS NULL THEN '-' "
        "    WHEN eraconstruction_paymentTypes.id = 0 THEN '-' "
        "    ELSE eraconstruction_paymentTypes.name "
        "END AS paymentType, "
        "eraconstruction_payments.currency, "
        "eraconstruction_payments.payment, "
        "eraconstruction_payments.description,"
        "eraconstruction_payments.apartmentId "
        "FROM eraconstruction_payments "
        "LEFT JOIN eraconstruction_clients ON eraconstruction_payments.clientId = eraconstruction_clients.id "
        "LEFT JOIN eraconstruction_objects ON eraconstruction_payments.objectId = eraconstruction_objects.id "
        "LEFT JOIN eraconstruction_apartments ON eraconstruction_payments.apartmentId = eraconstruction_apartments.id "
        "LEFT JOIN eraconstruction_paymentTypes ON eraconstruction_payments.paymentTypeId = eraconstruction_paymentTypes.id "
        "WHERE DATE(eraconstruction_payments.date) BETWEEN '" + fromdateString + "' AND '" + todateString + "' "
        "ORDER BY eraconstruction_clients.name"
        );

    return data;
}

QVariantList Operations::getPayment(int paymentId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT eraconstruction_payments.date, "
        "COALESCE(eraconstruction_clients.name, '-') AS client, "
        "COALESCE(eraconstruction_objects.name, '-') AS object, "
        "COALESCE(eraconstruction_apartments.apartmentNumber, '-') AS apartment, "
        "COALESCE(eraconstruction_paymentTypes.name, '-') AS paymentType, "
        "eraconstruction_payments.payment, "
        "eraconstruction_payments.description, "
        "eraconstruction_payments.currency "
        "FROM eraconstruction_payments "
        "LEFT JOIN eraconstruction_clients ON eraconstruction_payments.clientId = eraconstruction_clients.id "
        "LEFT JOIN eraconstruction_objects ON eraconstruction_payments.objectId = eraconstruction_objects.id "
        "LEFT JOIN eraconstruction_apartments ON eraconstruction_payments.apartmentId = eraconstruction_apartments.id "
        "LEFT JOIN eraconstruction_paymentTypes ON eraconstruction_payments.paymentTypeId = eraconstruction_paymentTypes.id "
        "WHERE eraconstruction_payments.id = %1")
        .arg(paymentId);
    return db.executeGet(query);
}

bool Operations::updatePayment(int paymentId, QDate date, int clientId, int objectId, int apartmentId,
                               int paymentTypeId, QString currency, double payment, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "UPDATE eraconstruction_payments SET date = '%1', clientId = %2, objectId = %3, apartmentId = %4, "
        "paymentTypeId = %5, currency = '%6', payment = %7, description = '%8' WHERE id = %9")
        .arg(date.toString("yyyy-MM-dd"))
        .arg(clientId)
        .arg(objectId)
        .arg(apartmentId)
        .arg(paymentTypeId)
        .arg(currency)
        .arg(payment, 0, 'f', 2)
        .arg(description)
        .arg(paymentId);

    return db.executeSet(query);
}

bool Operations::deletePayment(int paymentId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString("DELETE FROM eraconstruction_payments WHERE id = %1")
        .arg(paymentId);
    return db.executeSet(query);
}



QVariantList Operations::selectAllExpenses(QDate fromdate, QDate todate) {
    dbManager &db = dbManager::getInstance();
    QString fromdateString = fromdate.toString("yyyy-MM-dd");
    QString todateString = todate.toString("yyyy-MM-dd");

    QVariantList data = db.executeGet(
        "SELECT eraconstruction_expenses.id, eraconstruction_expenses.date, "
        "CASE "
        "    WHEN eraconstruction_types.id IS NULL THEN '-' "
        "    WHEN eraconstruction_types.id = 0 THEN '-' "
        "    ELSE eraconstruction_types.name "
        "END AS type, "
        "CASE "
        "    WHEN eraconstruction_whoWhat.id IS NULL THEN '-' "
        "    WHEN eraconstruction_whoWhat.id = 0 THEN '-' "
        "    ELSE eraconstruction_whoWhat.name "
        "END AS whoWhat, "
        "CASE "
        "    WHEN eraconstruction_paymentTypes.id IS NULL THEN '-' "
        "    WHEN eraconstruction_paymentTypes.id = 0 THEN '-' "
        "    ELSE eraconstruction_paymentTypes.name "
        "END AS paymentType, "
        "eraconstruction_expenses.amount, "
        "eraconstruction_expenses.description, "
        "eraconstruction_expenses.currency "
        "FROM eraconstruction_expenses "
        "LEFT JOIN eraconstruction_types ON eraconstruction_expenses.typeId = eraconstruction_types.id "
        "LEFT JOIN eraconstruction_whoWhat ON eraconstruction_expenses.whoWhatId = eraconstruction_whoWhat.id "
        "LEFT JOIN eraconstruction_paymentTypes ON eraconstruction_expenses.paymentTypeId = eraconstruction_paymentTypes.id "
        "WHERE DATE(eraconstruction_expenses.date) BETWEEN '" + fromdateString + "' AND '" + todateString + "' "
        "ORDER BY eraconstruction_expenses.date"
        );

    return data;
}

QVariantList Operations::getExpense(int expenseId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "SELECT eraconstruction_expenses.date, "
        "COALESCE(eraconstruction_types.name, '-') AS type, "
        "COALESCE(eraconstruction_whoWhat.name, '-') AS whoWhat, "
        "COALESCE(eraconstruction_paymentTypes.name, '-') AS paymentType, "
        "eraconstruction_expenses.amount, "
        "eraconstruction_expenses.description,"
        "eraconstruction_expenses.currency "
        "FROM eraconstruction_expenses "
        "LEFT JOIN eraconstruction_types ON eraconstruction_expenses.typeId = eraconstruction_types.id "
        "LEFT JOIN eraconstruction_whoWhat ON eraconstruction_expenses.whoWhatId = eraconstruction_whoWhat.id "
        "LEFT JOIN eraconstruction_paymentTypes ON eraconstruction_expenses.paymentTypeId = eraconstruction_paymentTypes.id "
        "WHERE eraconstruction_expenses.id = %1")
        .arg(expenseId);
    return db.executeGet(query);
}

bool Operations::addExpense(QDate date, int typeId, int whoWhatId, int paymentTypeId, QString currency, double amount, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "INSERT INTO eraconstruction_expenses (date, typeId, whoWhatId, paymentTypeId, currency, amount, description) "
        "VALUES ('%1', %2, %3, %4, '%5', %6, '%7')")
        .arg(date.toString("yyyy-MM-dd"))
        .arg(typeId)
        .arg(whoWhatId)
        .arg(paymentTypeId)
        .arg(currency)
        .arg(amount, 0, 'f', 2)
        .arg(description);
    return db.executeSet(query);
}

bool Operations::updateExpense(int expenseId, QDate date, int typeId, int whoWhatId, int paymentTypeId, QString currency, double amount, QString description) {
    dbManager &db = dbManager::getInstance();
    QString query = QString(
        "UPDATE eraconstruction_expenses SET date = '%1', typeId = %2, whoWhatId = %3, paymentTypeId = %4, currency = '%5', "
        "amount = %6, description = '%7' WHERE id = %8")
        .arg(date.toString("yyyy-MM-dd"))
        .arg(typeId)
        .arg(whoWhatId)
        .arg(paymentTypeId)
        .arg(currency)
        .arg(amount, 0, 'f', 2)
        .arg(description)
        .arg(expenseId);

    return db.executeSet(query);
}

bool Operations::deleteExpense(int expenseId) {
    dbManager &db = dbManager::getInstance();
    QString query = QString("DELETE FROM eraconstruction_expenses WHERE id = %1")
        .arg(expenseId);
    return db.executeSet(query);
}

QVariantList Operations::selectAllObjectsForMainWindow() {
    dbManager &db = dbManager::getInstance();

    QString query = R"Q(
        SELECT o.* FROM eraconstruction_objects o
        WHERE EXISTS (
            SELECT 1 FROM eraconstruction_apartments a
            LEFT JOIN eraconstruction_sales s ON a.id = s.apartmentId
            WHERE a.objectId = o.id AND s.id IS NULL
        )
        ORDER BY o.name
    )Q";
    return db.executeGet(query);
}

QVariantList Operations::selectAvailableObjects(int saleId) {
    dbManager &db = dbManager::getInstance();

    QString query = QString(
        "SELECT o.* FROM eraconstruction_objects o "
        "WHERE EXISTS ( "
        "    SELECT 1 FROM eraconstruction_apartments a "
        "    LEFT JOIN eraconstruction_sales s ON a.id = s.apartmentId "
        "    WHERE a.objectId = o.id AND (s.id = %1 OR s.id IS NULL) "
        ") "
        "ORDER BY o.name"
        ).arg(saleId);

    return db.executeGet(query);
}

QVariantList Operations::selectAllApartmentsForMainWindow(int objectId) {
    dbManager &db = dbManager::getInstance();
    QString query = R"Q(
        SELECT a.* FROM eraconstruction_apartments a
        LEFT JOIN eraconstruction_sales s ON a.id = s.apartmentId
        WHERE a.objectId = )Q" + QString::number(objectId) + R"Q( AND s.id IS NULL
        ORDER BY a.apartmentNumber
    )Q";
    return db.executeGet(query);
}

bool Operations::checkHaveDebts() {
    dbManager &db = dbManager::getInstance();

    QString query = R"SQL(
        SELECT COUNT(*) > 0 AS hasDebt
        FROM (
            SELECT
                s.id AS saleId,
                s.clientId,
                s.apartmentId,
                s.objectId,
                s.date AS purchaseDate,
                s.monthlyPayment,

                -- Проверяем, полностью ли погашен долг
                (GREATEST(0, TIMESTAMPDIFF(MONTH,
                    CASE
                        WHEN DAY(s.date) = DAY(CURDATE()) THEN DATE_ADD(s.date, INTERVAL 1 DAY)  -- Если дата покупки сегодня, то долг начинается с завтрашнего дня.
                        ELSE s.date  -- Иначе долг начинается с даты покупки.
                    END, CURDATE()) * s.monthlyPayment)
                 - COALESCE((
                    SELECT SUM(p.payment)
                    FROM eraconstruction_payments p
                    WHERE p.clientId = s.clientId
                      AND p.apartmentId = s.apartmentId
                      AND p.objectId = s.objectId
                ), 0)) AS debtAmount,

                -- Проверяем, не оплачена ли квартира полностью
                (s.installment + COALESCE((
                    SELECT SUM(p.payment)
                    FROM eraconstruction_payments p
                    WHERE p.clientId = s.clientId
                      AND p.apartmentId = s.apartmentId
                      AND p.objectId = s.objectId
                ), 0)) >= s.totalAmount AS isFullyPaid

            FROM eraconstruction_sales s
            WHERE s.isInstallmentPlan = 1
        ) AS debt_check
        WHERE debtAmount > 0 AND isFullyPaid = 0
    )SQL";

    QVariantList result = db.executeGet(query);

    if (result.isEmpty()) {
        return false;
    }

    // Добавляем проверку на вложенные списки
    QVariantList innerResult = result[0].toList();
    if (innerResult.isEmpty()) {
        return false; // Нет данных – долгов нет
    }

    bool hasDebt = innerResult[0].toBool();
    qDebug() << "Has debt: " << hasDebt;

    return hasDebt;
}
