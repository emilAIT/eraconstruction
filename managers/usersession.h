#ifndef USERSESSION_H
#define USERSESSION_H

#include <QObject>
#include <QString>
#include <QList>
#include "operations.h"
#include "../scripts/hash.h"

class userSession : public QObject
{
    Q_OBJECT

public:
    static userSession &getInstance(); // Метод для получения единственного экземпляра userSession

    QVariantMap logMe(QString username,QString password);      // Метод для входа пользователя
    int getId();                       // Получить ID пользователя
    const int getSecretKey();          // Получить секретный ключ
    bool checkIsAdmin();
    bool checkIsSuperAdmin();          // Проверка, является ли пользователь супер-администратором
    bool canViewHistory();
        // Проверка, является ли пользователь администратором

private:
    explicit userSession(QObject *parent = nullptr);
    userSession(const userSession &) = delete;
    userSession &operator=(const userSession &) = delete;

    int id = -1;                       // ID пользователя
    int secretKey = 123456;            // Пример секретного ключа
    bool isAdminSession = false;
    bool isSuperAdmin;        // Флаг, является ли пользователь администратором
};

#endif // USERSESSION_H
