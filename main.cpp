#include "mainwindow.h"
#include "managers/dbmanager.h"
#include "managers/encryptionmanager.h"
#include "managers/stylemanager.h"
#include "pages/login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    styleManager s(&a);
    EncryptionManager &em = EncryptionManager::getInstance();

    em.setKey("kjafibsnkfnerougbervnmfejgrubdinsm");

    dbManager &db = dbManager::getInstance();

    db.connectDB();

    login *w = new login;

    w->show();
    return a.exec();
}
