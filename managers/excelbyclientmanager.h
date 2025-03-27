#ifndef EXCELMANAGER_H
#define EXCELMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QAbstractItemModel>
#include <QAxObject>
#include <QMessageBox>

class ExcelByClientManager
{
public:
    ExcelByClientManager();
    void static exportToExcel(QString title, QList<QAbstractItemModel *> models, int start = 0);

private:
    QString static getDesktopDir();
};

#endif // EXCELMANAGER_H
