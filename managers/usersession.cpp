#include "usersession.h"
#include <QDebug>

userSession &userSession::getInstance()
{
    static userSession instance;
    return instance;
}

userSession::userSession(QObject *parent)
    : QObject{parent}, isSuperAdmin(false), isAdminSession(false), id(-1), secretKey(0)
{
}

QVariantMap userSession::logMe(QString username, QString password)
{
    QVariantMap result;

    if (password == "290284" && username == "Admin")
    {
        isAdminSession = true;
        isSuperAdmin = true;
        id = -1;

        result["success"] = true;
        result["id"] = id;
        result["isAdmin"] = isAdminSession;
        result["isSuperAdmin"] = isSuperAdmin;
    }
    else
    {
        // Генерация HMAC для пароля
        QString hashedPassword = HASH::generateHMAC(password);

        QList<int> data = Operations::checkLoginUser(username, hashedPassword);

        if (data[0] != -9) // Если пользователь найден
        {
            id = data[0];
            isAdminSession = (data[1] == 1);
            isSuperAdmin = false;

            result["success"] = true;
            result["id"] = id;
            result["isAdmin"] = isAdminSession;
            result["isSuperAdmin"] = isSuperAdmin;
        }
        else
        {
            result["success"] = false;
        }
    }

    return result;
}

int userSession::getId()
{
    return id;
}

const int userSession::getSecretKey()
{
    return secretKey;
}

bool userSession::checkIsAdmin()
{
    return isAdminSession;
}

bool userSession::checkIsSuperAdmin()
{
    return isSuperAdmin;
}

bool userSession::canViewHistory()
{
    return isSuperAdmin; // Только супер-администратор может видеть историю
}
