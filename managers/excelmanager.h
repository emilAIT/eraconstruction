#ifndef EXCELMANAGER_H
#define EXCELMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QAbstractItemModel>
#include <QAxObject>
#include <QMessageBox>

class ExcelManager
{
public:
    ExcelManager();
    void exportToExcel(QString title, QString dates, QList<QAbstractItemModel *> models, int start = 0);

private:
    QString getDesktopDir();
};

#endif // EXCELMANAGER_H
